/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1995, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo. (Originated by SGI)
        
        $RCSfile: topgun.c,v $
        $Revision: 1.66 $
        $Date: 1998/12/24 15:04:23 $
 *---------------------------------------------------------------------*/

/*
 * File:	topgun.c
 *
 * Main program for topgun, the harrier flying game
 *
 */

#include <ultra64.h>
#include <ramrom.h>
#include "topgun.h"

Lights1 light = { 250, 250, 250, 0xff,		/* diffuse */ 
	            0,   0,-128,    0, 		/* direction */
	           20,  20,  20, 0xff 		/* ambient */
};

/*
 * Symbol genererated by "makerom" to indicate the end of the code segment
 * in virtual (and physical) memory
 */
extern char _codeSegmentBssEnd[];
extern char _zbufferSegmentEnd[];
extern char _cfbSegmentBssEnd[];

/* these are fixed sizes: */
#define SP_UCODE_SIZE		4096
#define SP_UCODE_DATA_SIZE	2048

/*
 * Symbols generated by "makerom" to tell us where the static segment is
 * in ROM.
 */
extern char _staticSegmentRomStart[], _staticSegmentRomEnd[];

/*
 * Stacks for the threads as well as message queues for synchronization
 */
u64	bootStack[STACKSIZE/8];

static void	idle(void *ptr);
static void	mainproc(void *ptr);

static OSThread	idleThread;
static u64	idleThreadStack[STACKSIZE/8];

static OSThread	mainThread;
static u64	mainThreadStack[STACKSIZE/8];

#define NUM_PI_MSGS     8

static OSMesg PiMessages[NUM_PI_MSGS];
static OSMesgQueue PiMessageQ;

OSMesgQueue	dmaMessageQ, rdpMessageQ, retraceMessageQ;
OSMesg		dmaMessageBuf, rspMessageBuf, rdpMessageBuf, retraceMessageBuf;
OSMesg		dummyMessage;
OSIoMesg	dmaIOMessageBuf;

/*
 * Dynamic segment in code space. Needs K0_TO_PHYS()...
 */
Dynamic dynamic;

/*
 * must be in BSS, not on the stack for this to work:
 */
OSTask		tlist;       /* globaltask lists */
Gfx		*glistp;	/* global for test case procs */
    
/*
 * global variables for arguments, to control test cases
 */
static void reset(void);
void sceneDraw (Dynamic *dynamicp);

static float	time=0.0;		/* Time				*/
static float	ax, ay, az;		/* Position of plane		*/
static float	cx, cy, cz;		/* Position of camera		*/
static float	wx, wy, wz;		/* Where camera wants to be	*/
static float	speed;			/* Speed of camera		*/

static int	keep_mem = 1;
static int	automated = 1;
static int	frame_count = 0;
static int	output_frame = 1;
static char	*output_prefix = "InData/topgun";
static int	rdp_flag = 1;    /* Defaults ucode , fifo_code=1,xbus=0 */
static int	rdp_mspanFlushFlag = 0;
static int      draw_buffer = 0;
static void     *cfb_ptrs[2];
static u64	ramrombuf[RAMROM_MSG_SIZE/8];

typedef union {
    u32		word[2];
    u64		force_alignment;
} scratch_t;
scratch_t scratch_buff[1024], *scrp;

#define PI_BASE	((u32)(0xb0700000))
static u32	*piAddr = (u32 *)(PI_BASE);
u32		dummy_val;



OSPiHandle	*handler;

void
boot(void)
{
    osInitialize();

    handler = osCartRomInit();

    osCreateThread(&idleThread, 1, idle, NULL, idleThreadStack+STACKSIZE/8, 10);
    osStartThread(&idleThread);

    /* never reached */
}

static void
idle(void *ptr)
{
    /* Initialize video */
    osCreateViManager(OS_PRIORITY_VIMGR);
    osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
    
    /*
     * Start PI Mgr for access to cartridge
     */
    osCreatePiManager((OSPri)OS_PRIORITY_PIMGR, &PiMessageQ, PiMessages, 
		      NUM_PI_MSGS);
    
    /*
     * Create main thread
     */
    osCreateThread(&mainThread, 3, mainproc, NULL,
		   mainThreadStack+STACKSIZE/8, 10);
    
    osStartThread(&mainThread);

    /*
     * Become the idle thread
     */
    osSetThreadPri( 0, 0 );
    for(;;);
}

static void
mainproc(void *ptr)
{
    int		keep_going = 1, i, j;
    OSTask	*tlistp;
    Dynamic	*dynamicp;
    char	*staticSegment;
    u32		dmemaddr, t1, t2, t3;
    u16		perspNorm;
    int		x, y, w, h;

#ifdef DEBUG
    char *ap;
    u32 *argp;
    u32 argbuf[16];

    argp = (u32 *)RAMROM_APP_WRITE_ADDR;
    for (i=0; i<sizeof(argbuf)/4; i++, argp++) {
	osEPiReadIo(handler, (u32)argp, &argbuf[i]); /* Assume no DMA */
    }
    /* Parse the options */
    ap = (char *)argbuf;
    while (*ap != '\0') 
    {
	while (*ap == ' ')
	  ap++;
	if ( *ap == '-' && *(ap+1) == 'r') 
	{
	    rdp_flag = 0; /* Change ucode to xbus */
	    ap += 2;
	}
	else   /* don't know what it is, but move past it. */
	    ap++;
    }
#endif
    
    cfb_ptrs[0] = cfb_16_a;
    cfb_ptrs[1] = cfb_16_b;

    /*
     * Setup the message queues
     */
    osCreateMesgQueue(&dmaMessageQ, &dmaMessageBuf, 1);
    
    osCreateMesgQueue(&rdpMessageQ, &rdpMessageBuf, 1);
    osSetEventMesg(OS_EVENT_DP, &rdpMessageQ, dummyMessage);
    
    osCreateMesgQueue(&retraceMessageQ, &retraceMessageBuf, 1);
    osViSetEvent(&retraceMessageQ, dummyMessage, 1);
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
    
    /*
     * Stick the static segment right after the frame buffer
     */
    staticSegment = _cfbSegmentBssEnd+8;

    dmaIOMessageBuf.hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIOMessageBuf.hdr.retQueue = &dmaMessageQ;
    dmaIOMessageBuf.dramAddr     = staticSegment;
    dmaIOMessageBuf.devAddr      = (u32)_staticSegmentRomStart;
    dmaIOMessageBuf.size         = _staticSegmentRomEnd-_staticSegmentRomStart;

    osEPiStartDma(handler, &dmaIOMessageBuf, OS_READ);
    
    /*
     * Wait for DMA to finish
     */
    (void)osRecvMesg(&dmaMessageQ, &dummyMessage, OS_MESG_BLOCK);

    reset ();
    
    /*
     * Main game loop
     */
    while (keep_going) {

	/*
	 * this is only needed for regman freaks....
	 */
	piAddr = (u32 *)(PI_BASE);

	/*
	 * pointers to build the display list.
	 */
	tlistp = &tlist;
	dynamicp = &dynamic;
	glistp = &(dynamicp->glist[0]);

	/*
	 * Tell RCP where each segment is
	 */
	gSPSegment(glistp++, 0, 0x0);	/* K0 (physical) address segment */
	gSPSegment(glistp++, STATIC_SEGMENT,
		   osVirtualToPhysical(staticSegment));

	if (rdp_mspanFlushFlag) {
	    /*
	     * Force the pipeline to empty after the rendering of each
	     * display list item, to avoid incurring span buffer cache 
	     * coherency problems with DRAM r-m-w cycles.
	     */
	    gDPPipelineMode(glistp++, G_PM_1PRIMITIVE);
	}

	gSPDisplayList(glistp++, rdpinit_dl);
	gSPDisplayList(glistp++, rspinit_dl);

	/*
	 * Clear framebuffer
	 * doing this as a static display list is a little messier, due
	 * to cfb pointer conversion...
	 */
	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_FILL);
	/*
	 * clear color, cvg = FULL or 1
	 */
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, 
			 OS_K0_TO_PHYSICAL(cfb_ptrs[draw_buffer]));
	gDPSetFillColor(glistp++, GPACK_RGBA5551(64,64,255,1) << 16 | 
			 GPACK_RGBA5551(64,64,255,1)); 
	gDPPipeSync(glistp++);
	gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_1CYCLE);

	/*
	 * Clear zbuffer
	 */
	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_FILL);
	gDPSetDepthImage(glistp++, TOPGUN_ZBUFFER);
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, 
				TOPGUN_ZBUFFER);
	gDPSetFillColor(glistp++, GPACK_ZDZ(G_MAXFBZ, 0) << 16 |
			GPACK_ZDZ(G_MAXFBZ, 0));
	gDPPipeSync(glistp++);
	gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_1CYCLE);
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, 
			 OS_K0_TO_PHYSICAL(cfb_ptrs[draw_buffer]));

	/* end of standard display list part. */

	sceneDraw(dynamicp);

	/* put in full sync for RSP processing */
	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);

	/* 
	 * Build graphics task:
	 *
	 */
	tlistp->t.type = M_GFXTASK;
	tlistp->t.flags = OS_TASK_DP_WAIT;
	tlistp->t.ucode_boot = (unsigned long long *) rspbootTextStart;
	tlistp->t.ucode_boot_size = ((int)rspbootTextEnd - 
				     (int)rspbootTextStart);
	/*
	 * choose which ucode to run:
	 */
	if (rdp_flag) {
	    /* RSP output over FIFO to RDP: */
		tlistp->t.ucode = (unsigned long long *)
		    gspF3DEX2_fifoTextStart;
		tlistp->t.ucode_data = (unsigned long long *)
		    gspF3DEX2_fifoDataStart; 
	} else {
	    /* RSP output over XBUS to RDP: */
		tlistp->t.ucode = (unsigned long long *)
		    gspF3DEX2_xbusTextStart;
		tlistp->t.ucode_data = (unsigned long long *)
		    gspF3DEX2_xbusDataStart;
	}
	tlistp->t.ucode_size = SP_UCODE_SIZE;
	tlistp->t.ucode_data_size = SP_UCODE_DATA_SIZE;

	tlistp->t.dram_stack = (unsigned long long *) &(dram_stack[0]);
	tlistp->t.dram_stack_size = SP_DRAM_STACK_SIZE8;

	if (rdp_flag) {
	    tlistp->t.output_buff = (unsigned long long *) &(rdp_output[0]);
	    tlistp->t.output_buff_size
	      = (unsigned long long *)(&rdp_output[0] + RDP_OUTPUT_LEN);
	} else {
	    tlistp->t.output_buff = (unsigned long long *) 0x0;
	    tlistp->t.output_buff_size = (unsigned long long *) 0x0;
	}

	/* initial display list: */
	tlistp->t.data_ptr = (unsigned long long *) dynamicp->glist;
	tlistp->t.data_size = ((int)(glistp - dynamicp->glist) *
			       sizeof (Gfx));
	tlistp->t.yield_data_ptr = (unsigned long long *) NULL;
	tlistp->t.yield_data_size = 0xDA0;
	
	/*
	 * Can just flush 16KB and forget about each individual pieces
	 * of data to flush.
	 */
	osWritebackDCacheAll();
	
	osSpTaskLoad(tlistp);
	osSpTaskStartGo(tlistp);

	/* ignore RDP completion */
	(void)osRecvMesg(&rdpMessageQ, &dummyMessage, OS_MESG_BLOCK);

	/* setup to swap buffers */
	osViSwapBuffer(cfb_ptrs[draw_buffer]);

	/* Make sure there isn't an old retrace in queue 
	 * (assumes queue has a depth of 1) 
	 */
	if (MQ_IS_FULL(&retraceMessageQ))
	    (void)osRecvMesg(&retraceMessageQ, &dummyMessage, OS_MESG_BLOCK);
	
	/* Wait for Vertical retrace to finish swap buffers */
	(void)osRecvMesg(&retraceMessageQ, &dummyMessage, OS_MESG_BLOCK);
	draw_buffer ^= 1;

	frame_count++;
	time += 0.25;
    }
}

static void reset(void)
{
	time = 0;

	cx = 1000;
	cy = 350;
	cz = 24900;

	speed = 0;
}

void sceneDraw(Dynamic *dynamicp)
{
	float theta;
	short fract;
    	u16   perspNorm;
	u32   addrProjection, addrProjectionBG, addrViewing, addrViewingBG;
	u32   addrTranslate, addrRotate0, addrRotate1, addrRotate2;
	float scaleDown;

	/* scaleDown must be coupled with ground.c. In this case
	 * the original ground and sky sizes are scaled down by
	 * a factor of 16
	 */
	scaleDown = 0.0625;

	/*
	 * Use either Frustum or Perspective.  The following two
	 * calls are equivalent.
	 */
	guPerspective(&dynamicp->projectionBG,&perspNorm,
		      33, 1, 10*scaleDown, 5000*scaleDown, 1.0);

	guPerspective(&dynamicp->projection,&perspNorm, 33, 1, 10, 5000, 1.0);

    	gSPPerspNormalize(glistp++, perspNorm);

	ax = sinf(time/3000*2*M_PI) * 25000;
	ay = 300;
	az = cosf(time/3000*2*M_PI) * 25000;

	wx = ax + cosf (-time/50) * 1000;
	wy = ay + sinf (time/700) * 100;
	wz = az + sinf (-time/50) * 1000;

		/* cx, cy, cz, wx, wy, wz, speed); */
	cx = (wx - cx) * speed + cx;
	cy = (wy - cy) * speed + cy;
	cz = (wz - cz) * speed + cz;

	speed += 0.00001;
	if (speed > 1) {
		speed = 1;
	}

	guLookAt (&dynamicp->viewingBG, cx*scaleDown, cy*scaleDown, cz*scaleDown, 
		  ax*scaleDown, ay*scaleDown, az*scaleDown, 0, 1, 0);

	guLookAt (&dynamicp->viewing, cx, cy, cz, ax, ay, az, 0, 1, 0);

	/* fix dynamic seg addresses: */
	addrProjection = K0_TO_PHYS((u32) &(dynamicp->projection));
	addrProjectionBG = K0_TO_PHYS((u32) &(dynamicp->projectionBG));
	addrViewing =    K0_TO_PHYS((u32) &(dynamicp->viewing));
	addrViewingBG =    K0_TO_PHYS((u32) &(dynamicp->viewingBG));

	gSPMatrix(glistp++, addrProjectionBG,
		G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPMatrix(glistp++, addrViewingBG,
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);

	gSPLight(glistp++, &light, 1);

	/*
	 * render sky and ground
	 */

	gSPClearGeometryMode (glistp++, G_ZBUFFER | G_CULL_BACK);

	gDPSetTexturePersp(glistp++, G_TP_PERSP);

	gSPDisplayList(glistp++, ground);
	gSPDisplayList(glistp++, sky);

	/*
	 * setup SP/DP state for rendering the av8b
	 */

	gSPSetGeometryMode (glistp++, G_SHADING_SMOOTH | G_ZBUFFER |
		/* G_LIGHTING | */ G_CULL_BACK | G_SHADE);

	gSPMatrix(glistp++, addrProjection,
		G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPMatrix(glistp++, addrViewing,
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);

	guTranslate(&dynamicp->translate, ax, ay, az);
	/* fix dynamic seg addresses: */
	addrTranslate = K0_TO_PHYS((u32) &(dynamicp->translate));
	gSPMatrix(glistp++, addrTranslate,
		G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);

	theta = time/3000 * 360 - 90;
	guRotate(&dynamicp->rotate0, theta, 0.0, 1.0, 0.0);
	/* fix dynamic seg addresses: */
	addrRotate0 = K0_TO_PHYS((u32) &(dynamicp->rotate0));
	gSPMatrix(glistp++, addrRotate0,
		G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);

	theta = -90;
	guRotate(&dynamicp->rotate1, theta, 1.0, 0.0, 0.0);
	/* fix dynamic seg addresses: */
	addrRotate1 = K0_TO_PHYS((u32) &(dynamicp->rotate1));
	gSPMatrix(glistp++, addrRotate1,
		G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);

	theta = sinf (time/10)*10;
	guRotate(&dynamicp->rotate2, theta, 0.0, 1.0, 0.0);
	/* fix dynamic seg addresses: */
	addrRotate2 = K0_TO_PHYS((u32) &(dynamicp->rotate2));
	gSPMatrix(glistp++, addrRotate2,
		G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);

	gDPPipeSync(glistp++);
	gDPSetRenderMode(glistp++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

	gSPDisplayList(glistp++, av8b);

}
