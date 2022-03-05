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
        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
        
        $RCSfile: fill.c,v $
        $Revision: 1.21 $
        $Date: 1999/05/21 06:07:55 $
 *---------------------------------------------------------------------*/

/*
 * File:	fill.c
 *
 */

#include <ultra64.h>
#include <PR/ramrom.h>	/* needed for argument passing into the app */
#include <assert.h>
#include <os.h>

#include "controller.h"
#include "fill.h"
#include "static.h"
#include "timer.h"



/*
 * Symbol genererated by "makerom" to indicate the end of the code segment
 * in virtual (and physical) memory
 */
extern char _codeSegmentBssEnd[];

/*
 * Symbols generated by "makerom" to tell us where the static segment is
 * in ROM.
 */

/*
 * Stacks for the threads as well as message queues for synchronization
 * This stack is ridiculously large, and could also be reclaimed once
 * the main thread is started.
 */
u64	bootStack[STACKSIZE/sizeof(u64)];

static void	idle(void *);
static void	mainproc(void *);

static OSThread	idleThread;
static u64	idleThreadStack[STACKSIZE/sizeof(u64)];

static OSThread	mainThread;
static u64	mainThreadStack[STACKSIZE/sizeof(u64)];

/* this number (the depth of the message queue) needs to be equal
 * to the maximum number of possible overlapping PI requests.
 * For this app, 1 or 2 is probably plenty, other apps might
 * require a lot more.
 */
#define NUM_PI_MSGS     8

static OSMesg PiMessages[NUM_PI_MSGS];
static OSMesgQueue PiMessageQ;

OSMesgQueue     dmaMessageQ, rdpMessageQ, retraceMessageQ;
OSMesg          dmaMessageBuf, rdpMessageBuf, retraceMessageBuf;
OSMesg          dummyMessage;
OSIoMesg        dmaIOMessageBuf;

OSMesg		dummyMesg;
OSTask		*tlistp;
Dynamic		*dynamicp;

enum	BufferMode {SPLIT_Z_CFB, TENEMENT, ATHERTON};

/*
 *  Command line options
 */
int		BlankVideo	=  0;
int             Do8bit          =  0;
int		rdp_flag 	=  0; /* 0:xbus , 1:fifo */

enum BufferMode BufferPlacement =  SPLIT_Z_CFB;

int             ActiveController;
int             ControllerInput     =  1;
int             UseAAMode           =  0;
int             UseZMode            =  0;
int             DoClrClr            =  1;
int             SelfScaleTimer      =  1;
int             ClipRatio           =  1;
int             TimerTicks          =  7;

/* for special test only */
int             UseTextureMode      =  0;
int             Use2CycleMode       =  1;


int				TriSortOrder;

float           TimePerFrame        =  0;
float           TicksPerFrame       =  0;
float           TicksPerSec;

OSTime          t1, t2;


/*
 * Dynamic data.
 */
Dynamic dynamic;

/*
 * Task descriptor.
 */
OSTask	tlist =
{
    M_GFXTASK,			/* task type */
    OS_TASK_DP_WAIT,		/* task flags */
    NULL,			/* boot ucode pointer (fill in later) */
    0,				/* boot ucode size (fill in later) */
    NULL,			/* task ucode pointer (fill in later) */
    SP_UCODE_SIZE,		/* task ucode size */
    NULL,			/* task ucode data pointer (fill in later) */
    SP_UCODE_DATA_SIZE,		/* task ucode data size */
    &dram_stack[0],		/* task dram stack pointer */
    SP_DRAM_STACK_SIZE8,	/* task dram stack size */
    &rdp_output[0], 		/* task fifo buffer start ptr */
    &rdp_output[0]+RDPLIST_LEN,	/* task fifo buffer end ptr */
    NULL,			/* task data pointer (fill in later) */
    0,				/* task data size (fill in later) */
    NULL,			/* task yield buffer ptr (not used here) */
    0				/* task yield buffer size (not used here) */
};

Gfx		*glistp;	/* global for test case procs */
    
/*
 * global variables
 */

static int      draw_buffer        = 0;

static void    *cfb_ptrs[2];

OSPiHandle	*handler;

#ifdef DEBUG
/*
 *  To do memory alignment test, gload -a "-x -w" and translate tri across
 *  memory, noting time as you go.
 */
void parse_args(char *argstring)
{
  int		argc = 1;
  char	*arglist[32], **argv = arglist;	/* max 32 args */
  char	*ptr;

  if (argstring == NULL || argstring[0] == '\0')
    return;

  /* re-organize argstring to be like main(argv,argc) */

  ptr = argstring;
  while (*ptr != '\0') {
    while (*ptr != '\0' && (*ptr == ' ')) 
      {
	*ptr = '\0';
	ptr++;
      }
    if (*ptr != '\0')
      arglist[argc++] = ptr;
    while (*ptr != '\0' && (*ptr != ' ')) 
      {
	ptr++;
      }
  }

  /* process the arguments: */
  while ((argc > 1) && (argv[1][0] == '-')) 
    {
      switch(argv[1][1]) 
	{
	case '1':
	  Use2CycleMode = 0;
	  break;

	case '8':
	  Do8bit = 1;
	  break;

	case 'a':
	  UseAAMode = 1;
	  break;

	case 'b':
	  BlankVideo = 1;
	  break;

	case 'c':
	  DoClrClr = 0;
	  break;

	case 'm':
	  BufferPlacement = ATHERTON;
	  break;

	case 'r':
	  rdp_flag = 1;
	  break;

	case 's':
	  SelfScaleTimer = 0;
	  break;

	case 't':
	  BufferPlacement = TENEMENT;
	  break;

	case 'z':
	  UseZMode = 1;
	  break;

	default:
	  break;
	}
      
      argc--;
      argv++;
    }
}
#endif

void boot(void)
{
    /* notice that you can't call osSyncPrintf() until you set
     * up an idle thread.
     */
    
    osInitialize();

    handler = osCartRomInit();

    osCreateThread(&idleThread, 1, idle, (void *)0,
		   idleThreadStack+STACKSIZE/sizeof(u64), 10);
    osStartThread(&idleThread);

    /* never reached */
}

static void idle(void *arg)
{


  /* Initialize video */
  osCreateViManager(OS_PRIORITY_VIMGR);
  
  if (UseAAMode) {
    if(BlankVideo) {
	  osViModeNtscLan1.fldRegs[0].vStart = 0;
	  osViModeNtscLan1.fldRegs[1].vStart = 0;
    }
    osViSetMode(&osViModeNtscLan1);
  }
  else {
    if(BlankVideo) {
	  osViModeNtscLpn1.fldRegs[0].vStart = 0;
	  osViModeNtscLpn1.fldRegs[1].vStart = 0;
    }
    osViSetMode(&osViModeNtscLpn1);
  }
 
  /*
   * Start PI Mgr for access to cartridge
   */
  osCreatePiManager((OSPri)OS_PRIORITY_PIMGR, &PiMessageQ, PiMessages, 
		    NUM_PI_MSGS);
  

  /*
   * Create main thread
   */
  osCreateThread(&mainThread, 3, mainproc, NULL,
		 mainThreadStack+STACKSIZE/sizeof(u64), 10);
  
  osStartThread(&mainThread);
  
  /*
   * Become the idle thread
   */
  osSetThreadPri(0, 0);
  
  for (;;);
}

void CreateMessageQueues(void)
{
  osCreateMesgQueue(&dmaMessageQ, &dmaMessageBuf, 1);
  
  osCreateMesgQueue(&rdpMessageQ, &rdpMessageBuf, 1);
  osSetEventMesg(OS_EVENT_DP, &rdpMessageQ, dummyMessage);
  
  osCreateMesgQueue(&retraceMessageQ, &retraceMessageBuf, 1);
  osViSetEvent(&retraceMessageQ, dummyMessage, 1);  
}

void SetupSegments(void)
{
  /* Tell RCP where each segment is */

  gSPSegment(glistp++, 0, 0x0);   
}

void CreateTaskStructure(void)
{
  /* Build graphics task */
  tlistp->t.ucode_boot = (u64 *) rspbootTextStart;
  tlistp->t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;

  /* Choose ucode */
  if( rdp_flag )
    {
      tlistp->t.ucode      = (u64 *) gspF3DEX2_fifoTextStart;
      tlistp->t.ucode_data = (u64 *) gspF3DEX2_fifoDataStart;
    }
  else
    {
      tlistp->t.ucode      = (u64 *) gspF3DEX2_xbusTextStart;
      tlistp->t.ucode_data = (u64 *) gspF3DEX2_xbusDataStart;
    }
  
  /* initial display list: */
  tlistp->t.data_ptr = (u64 *) dynamicp->glist;
}

static void SwapViBuffer(void)
{
  /* setup to swap buffers */
  osViSwapBuffer(cfb_ptrs[draw_buffer]); 
  
  /* Make sure there isn't an old retrace in queue */
  if (MQ_IS_FULL(&retraceMessageQ))
    (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
  
  /* Wait for Vertical retrace to finish swap buffers */
  (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
  draw_buffer ^= 1;
}


static void SetupViewing(void)
{
    guOrtho(&dynamicp->projection,
        0, SCREEN_WD,
        0, SCREEN_HT, -1, -1000, 1.0);

    gSPMatrix(glistp++, K0_TO_PHYS(&(dynamicp->projection)),
          G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);

    guRotate(&dynamicp->identity, 0.0, 0.0, 0.0, 1.0);

    gSPMatrix(glistp++, K0_TO_PHYS(&(dynamicp->identity)),
          G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
}


static void SetRenderMode(void)
{
  if (UseAAMode && UseZMode)
    {
      gSPSetGeometryMode(glistp++, G_ZBUFFER);
      gDPSetRenderMode(glistp++, 
		       G_RM_AA_ZB_OPA_SURF, 
		       G_RM_AA_ZB_OPA_SURF2);      
    }
  else if (!UseAAMode && UseZMode)
    {
      gSPSetGeometryMode(glistp++, G_ZBUFFER);
      gDPSetRenderMode(glistp++, 
		       G_RM_ZB_OPA_SURF , 
		       G_RM_ZB_OPA_SURF2);      
    }
  else if (UseAAMode && !UseZMode)
    {
      gSPClearGeometryMode(glistp++, G_ZBUFFER);
      gDPSetRenderMode(glistp++, 
		       G_RM_AA_OPA_SURF, 
		       G_RM_AA_OPA_SURF2);      
    }
  else
    {
      gSPClearGeometryMode(glistp++, G_ZBUFFER);
      gDPSetRenderMode(glistp++, 
		       G_RM_OPA_SURF, 
		       G_RM_OPA_SURF2);      
    }  
}

static void SetClipRation(void)
{
  switch (ClipRatio)    
    {
    case 1:
      gSPClipRatio(glistp++, FRUSTRATIO_1);
      break;
    case 2:
      gSPClipRatio(glistp++, FRUSTRATIO_2);
      break;
    case 3:
      gSPClipRatio(glistp++, FRUSTRATIO_3);
      break;
    case 4:
      gSPClipRatio(glistp++, FRUSTRATIO_4);
      break;
    case 5:
      gSPClipRatio(glistp++, FRUSTRATIO_5);
      break;
    case 6:
      gSPClipRatio(glistp++, FRUSTRATIO_6);
      break;
    }    
}

static void doPolyFuncs(int ntris, float area, float aspect)
{
	int i;
	float tz, startz;
  	void makeTriList(int, float, float);

  	/* Setup display modes antialiased in 1 cycle */
  	SetRenderMode();
  	SetupViewing();
	makeTriList(ntris, area, aspect);
	gSPDisplayList(glistp++, dynamicp->trilist);
}


static void InitDisplayLists(void)
{
  /* pointers to build the display list */
  glistp = dynamicp->glist;
  
  SetupSegments();
  
  if(!Do8bit) {
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, 
		   OS_K0_TO_PHYSICAL(cfb_ptrs[draw_buffer]));
  } else {
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_8b, SCREEN_WD, 
		   OS_K0_TO_PHYSICAL(cfb_ptrs[draw_buffer]));
  }
  /* Initialize RDP state */
  gSPDisplayList(glistp++, rdpinit_dl);
  
  /* Initialize RSP state */
  gSPDisplayList(glistp++, rspinit_dl);  

  gDPSetCombineMode(glistp++, G_CC_MYNOISE, G_CC_MYNOISE);
  gDPSetPrimColor(glistp++, 0, 0, 255, 255, 0, 255);
}


static void ClearFrameBuffer(void)
{
  /* Clear color framebuffer */

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++, G_CYC_FILL);

  if(!Do8bit) {
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, 
		   OS_K0_TO_PHYSICAL(cfb_ptrs[draw_buffer]));
  } else {
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_8b, SCREEN_WD, 
		   OS_K0_TO_PHYSICAL(cfb_ptrs[draw_buffer]));
  }

  if(DoClrClr) {
    gDPSetFillColor(glistp++, GPACK_RGBA5551(255,0,0,1) << 16 | 
		  GPACK_RGBA5551(255,0,0,1));
  
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
  }
 
  gDPPipeSync(glistp++);
  if(Use2CycleMode) {
  	gDPSetCycleType(glistp++, G_CYC_2CYCLE);  
  } else {
  	gDPSetCycleType(glistp++, G_CYC_1CYCLE);  
  }
}

static void ClearZBuffer (void)
{
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 
		   SCREEN_WD, OS_K0_TO_PHYSICAL(zbuffer));

  gDPPipeSync(glistp++);

  gDPSetCycleType(glistp++, G_CYC_FILL);

  gDPSetFillColor(glistp++, 
		  GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0));

  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);

  gDPPipeSync(glistp++);
}

static void CleanupAndSendDisplayList(int dumpflag)
{
  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

#ifdef DEBUG
#ifndef __MWERKS__
  assert((glistp-dynamicp->glist) < GLIST_LEN);
#endif
#endif
	
  tlistp->t.data_size = (u32)((glistp - dynamicp->glist) * sizeof(Gfx));
	
  /* Write back dirty cache lines that need to be read by the RCP */
  osWritebackDCache(&dynamic, sizeof(dynamic));
	
  /* start up the RSP task */
  CreateTaskStructure();

  osRecvMesg(&retraceMessageQ, 0, OS_MESG_BLOCK);
  t1 = osGetTime(); 
  osSpTaskStart(tlistp);

  (void)osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);
  t2 = osGetTime();
}


/*
 *  Main game loop
 */
static void doPoly(void *arg)
{
  int done = 0;
  int i, ntri, tri_inc = 250, tri_div = 10;
  int remaining, ptri;
  float cur_area, cur_aspect;

  /* Main game loop */
 
  ActiveController = initControllers();
  cur_area   = AREA_LO;
  cur_aspect = ASPECT_LO;
  ntri = tri_inc;

#ifdef DEBUG
  osSyncPrintf("# aa=%d zb=%d cf=%d bl=%d 2c=%d ", 
	UseAAMode, UseZMode, DoClrClr, BlankVideo, Use2CycleMode);
  if (BufferPlacement == SPLIT_Z_CFB)
    osSyncPrintf("split z & cfb ");
  else if (BufferPlacement == TENEMENT)
    osSyncPrintf("z, cfb on same Meg ");
  else
    osSyncPrintf("Atherton buffers " );
  osSyncPrintf("produced by autofill\n"); 

  osSyncPrintf("# area, aspect, ntri per frame, time per tri (usec), fill (pix/sec)\n");
#endif

  while (!done) 
    {
      osContStartReadData(&controllerMsgQ);
      ReadController();
      InitDisplayLists();
      if (UseZMode) ClearZBuffer(); 
      ClearFrameBuffer();      
      CleanupAndSendDisplayList(0);	

	  /* init for rdp loop */
	  remaining = ntri;
	  TimePerFrame = 0;

	  /*
	   *  In order to limit rdp buffer size, make multiple
	   *  passes if number of triangles exceeds rdp buffer size.
	   *  Ignore cost of extra attributes.
	   */
	  do {
      	InitDisplayLists();
		ptri = MIN(remaining, RDPTRIS);
	     /* osSyncPrintf("remaining %d  ptri %d\n", remaining, ptri);  */
      	doPolyFuncs(ptri, cur_area, cur_aspect); 
      	CleanupAndSendDisplayList(1);	
      	TimePerFrame += (float) (t2 - t1);
		remaining -= ptri;
	  } while (remaining > 0);

      TimePerFrame = 1000.0 * TimePerFrame / ((OSTime) (TicksPerSec));

      /* check time, advance parameters if necessary */
      if(TimePerFrame > 16.667 && tri_inc == 1) {

#ifdef DEBUG
		osSyncPrintf("  %f %f %f %f %f\n", 
				cur_area, cur_aspect, (float)ntri, 
				1000.0 * TimePerFrame / (float)ntri, 
				(float)(ntri-1)*cur_area*60);	
#endif

		cur_area += AREA_STEP;
		if(cur_area > AREA_HI) {

#ifdef DEBUG
	  		osSyncPrintf("\n");
#endif
	  		cur_area = AREA_LO;
	  		cur_aspect += ASPECT_STEP;
	  		if(cur_aspect > ASPECT_HI) {
	   			 done = 1;
	  		}
		}
		tri_inc = 250;
		ntri = tri_inc;
      } else if (TimePerFrame < 16.667 && tri_inc > 0) {
		ntri += tri_inc;
	  } else if (TimePerFrame > 16.667 && tri_inc > 0) {
		tri_inc = -tri_inc / tri_div;
		if(tri_inc == 0)
			tri_inc = -1;
	    ntri += tri_inc;
	  } else if (TimePerFrame > 16.667 && tri_inc < 0) {
		ntri += tri_inc;
	  } else {
		tri_inc = -tri_inc / tri_div;
		if(tri_inc == 0)
			tri_inc = 1;
		ntri += tri_inc;
      }

      /* Now write out timer bar */

      InitDisplayLists();
      WriteTimerBar();
      CleanupAndSendDisplayList(0);		

      SwapViBuffer();
    }

#ifdef DEBUG
  osExit();
#endif

}

static void ComputeClockSpeed(void)
{
  u32 count0, count1;

  osRecvMesg(&retraceMessageQ, 0, OS_MESG_BLOCK);
  count0 = osGetCount();
  osRecvMesg(&retraceMessageQ, 0, OS_MESG_BLOCK);
  count1 = osGetCount();

  TicksPerSec = ((osTvType == 0) ? 50.0 : 60.0) * (count1 - count0);
}


static void mainproc(void *arg)
{
#ifdef DEBUG
    int i;
    u32 *argp;
    u32 argbuf[16];

    argp = (u32 *)RAMROM_APP_WRITE_ADDR;
    for (i=0; i<sizeof(argbuf)/4; i++, argp++) 
      {
	osEPiReadIo(handler, (u32)argp, &argbuf[i]); /* Assume no DMA */
      }

    parse_args((char *) argbuf);
#endif

  tlistp = &tlist;
  dynamicp = &dynamic;

  switch (BufferPlacement) {

    case SPLIT_Z_CFB:
      cfb_ptrs[0] = cfb_16_a; 
      cfb_ptrs[1] = cfb_16_b; 
      break;

    case TENEMENT:
      cfb_ptrs[0] = cfb_16_a_cozy; 
      cfb_ptrs[1] = cfb_16_b_cozy; 
      break;

    case ATHERTON:
      cfb_ptrs[0] = cfb_16_a; 
      cfb_ptrs[1] = cfb_16_b_other; 
      break;
  }

  CreateMessageQueues();

  ComputeClockSpeed();

  doPoly(arg);
}

