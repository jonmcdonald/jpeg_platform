
#ifndef _JPEG_PROGRAM_H_
#define _JPEG_PROGRAM_H_

/*
** JPEG Program Header file.
 */

#define MAX_ROWS 600
#define MAX_COLUMNS 800
#define SRAM0_BASE_ADDRESS 0x04000000
#define JPEG_BASE_ADDRESS  0x10000000
#define IO_BASE_ADDRESS    0x10000020
#define INPUTADDR    0x0
#define INPUTLENGTH  0x4
#define OUTPUTADDR   0x8
#define OUTPUTLENGTH 0xC
#define STATUS       0x10
#define START        0x14
#define UPLOAD       0xC
#define DOWNLOAD     0x10

extern void jpeg_init();
extern void jpeg_main (unsigned int clk);

#endif

