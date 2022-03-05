/*---------------------------------------------------------------------
        Copyright (C) 1998, Nintendo.
        
        File            asci.c
        Coded    by     Tetsuyuki Ootsuka.      July, 1998.
        Comments        ASCII Character Font Data
   
        $Id: 
   ---------------------------------------------------------------------*/
unsigned char pat[][16] = {
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x10,0x38,0x10,0x00,},
{0x00,0x6c,0x6c,0x24,0x48,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x24,0x24,0x24,0x26,0x3c,0x64,0x24,0x24,0x26,0x3c,0x64,0x24,0x24,0x24,0x00,},
{0x00,0x08,0x1c,0x2a,0x49,0x48,0x28,0x1c,0x0a,0x09,0x09,0x49,0x2a,0x1c,0x08,0x00,},
{0x00,0x22,0x54,0x54,0x54,0x28,0x08,0x08,0x10,0x10,0x14,0x2a,0x2a,0x2a,0x44,0x00,},
{0x00,0x38,0x44,0x44,0x44,0x44,0x28,0x30,0x20,0x52,0x52,0x4a,0x4a,0x44,0x3a,0x00,},
{0x00,0x60,0x60,0x20,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x04,0x08,0x08,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x08,0x08,0x04,},
{0x20,0x10,0x10,0x08,0x08,0x04,0x04,0x04,0x04,0x04,0x04,0x08,0x08,0x10,0x10,0x20,},
{0x00,0x00,0x08,0x08,0x49,0x49,0x2a,0x1c,0x08,0x1c,0x2a,0x49,0x49,0x08,0x08,0x00,},
{0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x7f,0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x20,0x40,},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00,0x00,},
{0x00,0x02,0x04,0x04,0x04,0x08,0x08,0x08,0x10,0x10,0x10,0x20,0x20,0x20,0x40,0x00,},
{0x00,0x3c,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x3c,0x00,},
{0x00,0x08,0x18,0x28,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x1c,0x00,},
{0x00,0x18,0x24,0x42,0x42,0x42,0x02,0x04,0x08,0x10,0x20,0x20,0x40,0x40,0x7e,0x00,},
{0x00,0x18,0x24,0x42,0x42,0x02,0x04,0x18,0x04,0x02,0x42,0x42,0x42,0x24,0x18,0x00,},
{0x00,0x04,0x0c,0x0c,0x14,0x14,0x14,0x24,0x24,0x44,0x44,0x7e,0x04,0x04,0x04,0x00,},
{0x00,0x7e,0x40,0x40,0x40,0x58,0x64,0x42,0x02,0x02,0x02,0x42,0x42,0x24,0x18,0x00,},
{0x00,0x18,0x24,0x42,0x42,0x40,0x58,0x64,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,},
{0x00,0x7e,0x42,0x42,0x42,0x04,0x04,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,},
{0x00,0x18,0x24,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x42,0x24,0x18,0x00,},
{0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x26,0x1a,0x02,0x42,0x42,0x24,0x18,0x00,},
{0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x08,0x10,},
{0x00,0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x40,0x20,0x10,0x08,0x04,0x02,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x00,0x7e,0x00,0x00,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x40,0x20,0x10,0x08,0x04,0x02,0x02,0x04,0x08,0x10,0x20,0x40,0x00,0x00,},
{0x00,0x18,0x24,0x42,0x42,0x02,0x04,0x08,0x10,0x10,0x00,0x00,0x10,0x38,0x10,0x00,},
{0x00,0x1c,0x22,0x41,0x49,0x55,0x55,0x55,0x55,0x55,0x55,0x4a,0x40,0x21,0x1e,0x00,},
{0x00,0x18,0x18,0x24,0x24,0x24,0x24,0x24,0x3c,0x42,0x42,0x42,0x42,0x42,0x00,0x00,},
{0x00,0x78,0x44,0x42,0x42,0x42,0x44,0x78,0x44,0x42,0x42,0x42,0x44,0x78,0x00,0x00,},
{0x00,0x18,0x24,0x42,0x42,0x40,0x40,0x40,0x40,0x42,0x42,0x42,0x24,0x18,0x00,0x00,},
{0x00,0x78,0x44,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x44,0x78,0x00,0x00,},
{0x00,0x7e,0x40,0x40,0x40,0x40,0x40,0x7c,0x40,0x40,0x40,0x40,0x40,0x7e,0x00,0x00,},
{0x00,0x7e,0x40,0x40,0x40,0x40,0x40,0x7c,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,},
{0x00,0x18,0x24,0x42,0x40,0x40,0x40,0x4e,0x42,0x42,0x42,0x42,0x26,0x1a,0x00,0x00,},
{0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x7e,0x42,0x42,0x42,0x42,0x42,0x42,0x00,0x00,},
{0x00,0x1c,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x1c,0x00,0x00,},
{0x00,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x42,0x42,0x24,0x18,0x00,0x00,},
{0x00,0x42,0x44,0x44,0x48,0x48,0x50,0x60,0x50,0x48,0x48,0x44,0x44,0x42,0x00,0x00,},
{0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7e,0x00,0x00,},
{0x00,0x41,0x63,0x63,0x55,0x55,0x49,0x49,0x41,0x41,0x41,0x41,0x41,0x41,0x00,0x00,},
{0x00,0x62,0x62,0x52,0x52,0x52,0x52,0x4a,0x4a,0x4a,0x4a,0x4a,0x46,0x46,0x00,0x00,},
{0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,},
{0x00,0x78,0x44,0x42,0x42,0x42,0x44,0x78,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,},
{0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x52,0x4a,0x24,0x1a,0x00,0x00,},
{0x00,0x78,0x44,0x42,0x42,0x42,0x44,0x78,0x48,0x48,0x44,0x44,0x42,0x42,0x00,0x00,},
{0x00,0x18,0x24,0x42,0x40,0x40,0x20,0x18,0x04,0x02,0x42,0x42,0x24,0x18,0x00,0x00,},
{0x00,0x7f,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,},
{0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,},
{0x00,0x41,0x41,0x41,0x41,0x22,0x22,0x22,0x22,0x14,0x14,0x14,0x08,0x08,0x00,0x00,},
{0x00,0x41,0x41,0x41,0x41,0x49,0x49,0x49,0x55,0x55,0x55,0x55,0x22,0x22,0x00,0x00,},
{0x00,0x42,0x42,0x42,0x24,0x24,0x18,0x18,0x24,0x24,0x24,0x42,0x42,0x42,0x00,0x00,},
{0x00,0x41,0x41,0x41,0x22,0x22,0x14,0x14,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,},
{0x00,0x7e,0x02,0x04,0x04,0x08,0x08,0x10,0x10,0x20,0x20,0x40,0x40,0x7e,0x00,0x00,},
{0x0e,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0e,},
{0x00,0x41,0x41,0x22,0x22,0x14,0x14,0x7f,0x08,0x08,0x7f,0x08,0x08,0x08,0x08,0x00,},
{0x38,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x38,},
{0x18,0x24,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7e,0x00,0x00,},
{0x30,0x18,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x3c,0x42,0x02,0x1e,0x22,0x42,0x42,0x46,0x3a,0x00,0x00,},
{0x00,0x40,0x40,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x42,0x42,0x64,0x58,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x18,0x24,0x42,0x40,0x40,0x40,0x42,0x24,0x18,0x00,0x00,},
{0x00,0x02,0x02,0x02,0x02,0x1a,0x26,0x42,0x42,0x42,0x42,0x42,0x26,0x1a,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x7e,0x40,0x40,0x22,0x1c,0x00,0x00,},
{0x00,0x08,0x14,0x10,0x10,0x7c,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x1a,0x26,0x42,0x42,0x42,0x42,0x26,0x1a,0x02,0x44,0x38,},
{0x00,0x40,0x40,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x00,0x00,},
{0x00,0x00,0x10,0x10,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,},
{0x00,0x00,0x04,0x04,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x24,0x24,0x18,},
{0x00,0x40,0x40,0x40,0x40,0x42,0x44,0x48,0x50,0x60,0x50,0x48,0x44,0x42,0x00,0x00,},
{0x00,0x30,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x56,0x69,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x58,0x64,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x58,0x64,0x42,0x42,0x42,0x42,0x64,0x58,0x40,0x40,0x40,},
{0x00,0x00,0x00,0x00,0x00,0x1a,0x26,0x42,0x42,0x42,0x42,0x26,0x1a,0x02,0x02,0x02,},
{0x00,0x00,0x00,0x00,0x00,0x2c,0x32,0x22,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x3c,0x42,0x42,0x20,0x18,0x04,0x42,0x42,0x3c,0x00,0x00,},
{0x00,0x00,0x00,0x10,0x10,0x7c,0x10,0x10,0x10,0x10,0x10,0x12,0x12,0x0c,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x26,0x1a,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x42,0x42,0x42,0x42,0x24,0x24,0x24,0x18,0x18,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x41,0x49,0x49,0x55,0x55,0x55,0x55,0x22,0x22,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x42,0x42,0x24,0x18,0x18,0x24,0x42,0x42,0x42,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x42,0x42,0x42,0x24,0x24,0x14,0x18,0x08,0x10,0x10,0x20,},
{0x00,0x00,0x00,0x00,0x00,0x7e,0x02,0x04,0x08,0x08,0x10,0x20,0x40,0x7e,0x00,0x00,},
{0x06,0x08,0x08,0x08,0x08,0x08,0x10,0x30,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x06,},
{0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,},
{0x30,0x08,0x08,0x08,0x08,0x08,0x04,0x06,0x04,0x08,0x08,0x08,0x08,0x08,0x08,0x30,},
{0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},

/*
 *  arrow (No.127 -)
 */
{0x00,0x00,0x18,0x3c,0x5a,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,},
{0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x5a,0x3c,0x18,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x08,0x04,0xfe,0xfe,0x04,0x08,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x00,0x20,0x40,0xfe,0xfe,0x40,0x20,0x00,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0xf8,0xe0,0xf0,0xb8,0x9c,0x0e,0x07,0x02,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x1f,0x07,0x0f,0x1d,0x39,0x70,0xe0,0x40,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x40,0xe0,0x70,0x39,0x1d,0x0f,0x07,0x1f,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x00,0x02,0x07,0x0e,0x9c,0xb8,0xf0,0xe0,0xf8,0x00,0x00,0x00,0x00,},
/*
 *  C button unit (No.135 -)
 */
{0x00,0x00,0x00,0x00,0x10,0x10,0x38,0x38,0x38,0x7c,0x7c,0x7c,0xfe,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x20,0x30,0x38,0x3c,0x3e,0x3e,0x3c,0x38,0x30,0x20,0x00,0x00,0x00,},
{0x00,0x00,0x00,0xfe,0x7c,0x7c,0x7c,0x38,0x38,0x38,0x10,0x10,0x00,0x00,0x00,0x00,},
{0x00,0x00,0x00,0x04,0x0c,0x1c,0x3c,0x7c,0x7c,0x3c,0x1c,0x0c,0x04,0x00,0x00,0x00,},
/*
 *  Blocks (No.138 -)
 */
{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,},

};
