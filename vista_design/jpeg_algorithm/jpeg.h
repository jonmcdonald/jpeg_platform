#ifndef _jpeg_h_
#define _jpeg_h_

#include "bitstream.h"
#include "huff.h"
#include "tables.h"
#include "bmp.h"

#ifndef SC_INCLUDE_MTI_AC
  #define SC_INCLUDE_MTI_AC
#endif
#include "ac_fixed.h"
#include <ac_channel.h>

//#include "altera/accel.h"
#include "accel.h" //map to operator causes problems in 2009a

// ----------------------------------------------------------------------------------------------
//  Data types
// ----------------------------------------------------------------------------------------------

// Huffman Codes
// DC code is index 0, AC codes 1..63
//typedef struct codes_t {
typedef struct {
  uint6   size;
  uint32  code;
  bool    done;
} codes_t;

//typedef struct rgb_t {
typedef struct {
	uint8	r;
	uint8	g;
	uint8	b;
} rgb_t;

typedef struct {
	char 	grid[64];
} char_array_t;

typedef struct {
	short grid[64]; // transpose buffer
} short_array_t;


// ----------------------------------------------------------------------------------------------
//  Forward definitions (local functions)
// ----------------------------------------------------------------------------------------------

//for peppers.bmp
//#define MAX_ROWS 384
//#define MAX_COLUMNS 512

//for factal_800x600.bmp
#define MAX_ROWS 600
#define MAX_COLUMNS 800


void jpeg_encoder(
		  ac_channel<bool> &start_jpeg,
		  rgb_t full_rgb[3*MAX_ROWS*MAX_COLUMNS],
		  ac_channel<bool> &done_jpeg,
		  unsigned char jpeg_data[3*MAX_ROWS*MAX_COLUMNS],
		  uint21 &jpeg_data_len);

#endif
