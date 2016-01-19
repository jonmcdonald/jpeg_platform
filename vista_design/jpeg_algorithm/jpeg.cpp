
#include "jpeg.h"
//#include "math/mgc_ac_math.h" // MBMB remove

// ----------------------------------------------------------------------------------------------

// ---------------------------------------------------
// Convert a 8x8 RGB block to a 8x8 YCbCr block
// ---------------------------------------------------
//void rgb2ycbcr( ac_channel<uint2> &blocktype,
bool rgb2ycbcr( ac_channel<uint2> &blocktype,
                ac_channel<rgb_t> &rgb,
				ac_channel<char_array_t> &ycbcr,
				ac_channel<uint2> &blocktype_out )
{
	ac_fixed<16,1,true> coeffs[3][3] = {
	 { 0.299   ,  0.587   ,  0.114    },
	 {-0.168736, -0.331264,  0.5      },
	 { 0.5     , -0.418688, -0.081312 }	};

	ac_fixed<8,8,false> tmp;
	// normalize values by subtracting 128 and convert to YCbCr
	unsigned char r, g, b;

	if (blocktype.available(1))
	{
		uint2	k = blocktype.read();
		blocktype_out.write(k);
		char_array_t ycbcr_tmp;

		convert2ycbcr:
		for (unsigned int i=0; i<8; i++) {
			for (unsigned int j=0; j<8; j++) {
				rgb_t rgb_temp = rgb.read();
				r = rgb_temp.r;
				g = rgb_temp.g;
				b = rgb_temp.b;
				tmp = coeffs[k][0]*r + coeffs[k][1]*g + coeffs[k][2]*b + ((k)?128:0);
				tmp -= 128;
				ycbcr_tmp.grid[i*8+j] = tmp.to_uint();
			}
		}
		ycbcr.write(ycbcr_tmp);
		return(true);
	}
	else
		return(false);
}

#define USE_OPTIMIZED_LIBS
// ---------------------------------------------------
// DCT 2D 8x8
// ---------------------------------------------------
// DCT Horizontal Processing
//static void dct_h(
static bool dct_h(
	  			  ac_channel<uint2> &blocktype,
				  ac_channel<char_array_t> &x_c,
				  ac_channel<short_array_t> &y_c,
				  ac_channel<uint2> &blocktype_out)
{
  if (blocktype.available(1))
  {
	  uint2 temp = blocktype.read();
	  blocktype_out.write(temp);

	  char_array_t x = x_c.read();
	  short_array_t y;
	  
	  for (int i=0; i<8; i++ ) {
		  #pragma unroll yes
		for (int j=0; j<8; j++ ) {
		  int tmp = 0;
	#ifndef USE_OPTIMIZED_LIBS
		  tmp += ((j&0x1)?(x[i*8+0]-x[i*8+7]):(x[i*8+0]+x[i*8+7])) * dctcoeff[j][0];
		  tmp += ((j&0x1)?(x[i*8+1]-x[i*8+6]):(x[i*8+1]+x[i*8+6])) * dctcoeff[j][1];
		  tmp += ((j&0x1)?(x[i*8+2]-x[i*8+5]):(x[i*8+2]+x[i*8+5])) * dctcoeff[j][2];
		  tmp += ((j&0x1)?(x[i*8+3]-x[i*8+4]):(x[i*8+3]+x[i*8+4])) * dctcoeff[j][3];
	#else
				tmp = Alt_4mult_add<0,1,1,9,true,10,true>(
		      		 ((j&0x1)?(x.grid[i*8+0]-x.grid[i*8+7]):(x.grid[i*8+0]+x.grid[i*8+7])) , dctcoeff[j][0] ,
		      		 ((j&0x1)?(x.grid[i*8+1]-x.grid[i*8+6]):(x.grid[i*8+1]+x.grid[i*8+6])) , dctcoeff[j][1] ,
		      		 ((j&0x1)?(x.grid[i*8+2]-x.grid[i*8+5]):(x.grid[i*8+2]+x.grid[i*8+5])) , dctcoeff[j][2] ,
		      		 ((j&0x1)?(x.grid[i*8+3]-x.grid[i*8+4]):(x.grid[i*8+3]+x.grid[i*8+4])) , dctcoeff[j][3]
							);
	#endif
		  y.grid[i*8+j] = (short)(tmp>>5);
		}
	  }
	  y_c.write(y);
	  return(true);
  }
  else
	  return(false);
}
// ---------------------------------------------------
// DCT Vertical Processing
//static void dct_v(
static bool dct_v(
	  			  ac_channel<uint2> &blocktype,
				  ac_channel<short_array_t> &x_c,
 				  ac_channel<short_array_t> &y_c,
				  ac_channel<uint2> &blocktype_out)
{
  if (blocktype.available(1))
  {
	  uint2 temp = blocktype.read();
	  blocktype_out.write(temp);

	  short_array_t x = x_c.read();
	  short_array_t y;

	  #pragma unroll yes
	  for (int i=0; i<8; i++ ) {
		for (int j=0; j<8; j++ ) {
		  int tmp = 0;
	#ifndef USE_OPTIMIZED_LIBS
		  tmp += ((i&0x1)?(x[ 0+j]-x[56+j]):(x[ 0+j]+x[56+j])) * dctcoeff[i][0];
		  tmp += ((i&0x1)?(x[ 8+j]-x[48+j]):(x[ 8+j]+x[48+j])) * dctcoeff[i][1];
		  tmp += ((i&0x1)?(x[16+j]-x[40+j]):(x[16+j]+x[40+j])) * dctcoeff[i][2];
		  tmp += ((i&0x1)?(x[24+j]-x[32+j]):(x[24+j]+x[32+j])) * dctcoeff[i][3];
	#else
				tmp = Alt_4mult_add<0,1,1,17,true,10,true>(
      				 ((i&0x1)?(x.grid[ 0+j]-x.grid[56+j]):(x.grid[ 0+j]+x.grid[56+j])) , dctcoeff[i][0] ,
      				 ((i&0x1)?(x.grid[ 8+j]-x.grid[48+j]):(x.grid[ 8+j]+x.grid[48+j])) , dctcoeff[i][1] ,
      				 ((i&0x1)?(x.grid[16+j]-x.grid[40+j]):(x.grid[16+j]+x.grid[40+j])) , dctcoeff[i][2] ,
      				 ((i&0x1)?(x.grid[24+j]-x.grid[32+j]):(x.grid[24+j]+x.grid[32+j])) , dctcoeff[i][3]
      				);
	#endif
		  y.grid[i*8+j] = tmp >> 15;
		}
	  }
	  y_c.write(y);
	  return(true);
  }
  else
	  return(false);
}

/*
// ---------------------------------------------------
// DCT 2D 8x8 Top
void dct8x8(
			ac_channel<uint2> &blocktype,
			char datai[64], 
			short datao[64],
			ac_channel<uint2> &blocktype_out)
{

	uint2 temp = blocktype.read();
	blocktype_out.write(temp);
	
	// transpose buffer
    short datat[64];

    // horizontal processing
    dct_h(datai, datat);
    // vertical processing
    dct_v(datat, datao);
}
*/

// ---------------------------------------------------
// Vectorize (Zig-Zag encoding)
void vectorize(short datai[64], int16 datao[64]) {
	const unsigned zigzagpath_vector[64] = {
		 0,  1,  8, 16,  9,  2,  3, 10,
		17, 24, 32, 25, 18, 11,  4,  5,
		12, 19, 26, 33, 40, 48, 41, 34,
		27, 20, 13,  6,  7, 14, 21, 28,
		35, 42, 49, 56, 57, 50, 43, 36,
		29, 22, 15, 23, 30, 37, 44, 51,
		58, 59, 52, 45, 38, 31, 39, 46,
		53, 60, 61, 54, 47, 55, 62, 63
	};

	vectorization:
	for (unsigned short i = 0; i < 64; i++) {
		datao[i] = datai[zigzagpath_vector[i]];
	}
};

// ---------------------------------------------------
// Quantize
void quantize(uint2 &type, int16 datai[64], int16 datao[64]) {

	uint1 LC = (type==LUMA)?LUMA:CHROMA;
  quantization:
	for (unsigned short i = 0; i < 64; i++) {
 		datao[i] = (short)(datai[i] / qvector[LC][i]);
//  		div(datai[i], (int8)qvector[LC][i], datao[i]);
	}

}

// ---------------------------------------------------
// Scan a 64 value vector and return the index of the last non-zero value
void findlastnonzero(
			int16  data[64],                  // input   block - quantize output
			uint6 &last_non_zero_index)       // output  index of the last non-zero value
{
  uint6 idx = 0;

	// ---------------------------------------------
  // Find the last non-zero coefficient
  find_last_non_zero:
	for (unsigned short i=0; i<64; i++) {
		if (data[i]) {
			idx = i;
		}
	}
	last_non_zero_index = idx;
}

//void reorder_and_quantize(
bool reorder_and_quantize(
			ac_channel<uint2>		    &blocktype,          // input   block type, Y, Cr, Cb
			ac_channel<short_array_t> &datai_c,
//			short 		   datai[64],			       // input   block - dct output, vectorize input
			ac_channel<short_array_t> &datao_c,
//			short		     datao[64],		         // output  block - quantize output, huffman input
			ac_channel<uint6> &last_non_zero_index_c,
//			uint6  			&last_non_zero_index,    // output  index of the last non-zero value in the output block
			ac_channel<uint2>		    &blocktype_out )

{
	int16 vectorOut[64];
	int16 quantizeOut[64];

	if (blocktype.available(1))
	{
		uint2 type = blocktype.read();
		blocktype_out.write(type);

		short_array_t datai = datai_c.read();
		short_array_t datao;
		uint6 last_non_zero_index;

		// zig-zag vectorize
		vectorize(datai.grid, vectorOut);

		// quantize the results (argument says which table to use)
		quantize(type, vectorOut, quantizeOut);

		// find the last non-zero coefficient
		findlastnonzero(quantizeOut, last_non_zero_index);

		write_out:
		for (int i=0; i<64; i++) {
			datao.grid[i]= quantizeOut[i];
		}
		last_non_zero_index_c.write(last_non_zero_index);
		datao_c.write(datao);
		return(true);
	}
	else
		return(false);
}

// ---------------------------------------------------
// Huffman encode
//void huffmanize(
bool huffmanize(
			ac_channel<uint2>         &blocktype,                    // input  block type, Y, Cr, Cb
			ac_channel<short_array_t> &datai_c,
//			short          datai[64],               // input  block - quantize output, huffman input
			ac_channel<uint6> &last_non_zero_index_c,
//			uint6         &last_non_zero_index,     // input  index of the last non-zero value in the input block
//			codes_t        codes[3*MAX_ROWS*MAX_COLUMNS] )               // output huffman codes
			ac_channel<codes_t> &codes)
{
	if (blocktype.available(1))
  {


      static uint21 code_length=0;  //count to 3*MAX_ROWS*MAX_COLUMNS

	  uint2 type = blocktype.read();
		// Luma/Chroma block
	  uint1 LC = (type==LUMA)?LUMA:CHROMA;

	  short_array_t datai = datai_c.read();

	  // previous dc coefficient values for the 3 picture elements from last block
	  static int dc[3] = {0, 0, 0};

	  uint6	 num_zeros = 0;
	  bool	 new_code;
	  uint6  huf_size;
	  uint32 huf_code;
	  uint6  huf_lnzi = last_non_zero_index_c.read();
		// ---------------------------------------------
		// Huffman encoding
		huffmanization:
		for (unsigned short i=0; i<64; i++) {
			int value = datai.grid[i];
			if (i==0) {
				// ---------------------------------------------
				// Huffman encode the DC Coefficient
				int dcval         = value;
				int diff          = dcval - dc[type];
				dc[type]   = dcval;

				huf_code = huffencode(huffcodes[LC][DC],diff);
				huf_size = huffencode(huffsizes[LC][DC],diff);
			} else {
				// ---------------------------------------------
				// Huffman encode the AC Coefficient
				if (value) {
					// if data is not 0...
					// ...insert the (num_zeros,block[i]) code
			  new_code   = true;
				} else {
					// if data is 0...
					if (num_zeros==15 && i<huf_lnzi) {
						// if we found 16 consecutive zeros
						// and if we are not yet in the last block of zero's
						// ...insert the special (15,0) code
				new_code   = true;
					} else if (i==63) {
						// if we reached the end of the last block of zero's
						// ...insert the special (0,0) code
				new_code   = true;
					} else {
						// else keep counting zero's !
				new_code   = false;
					}
				}
			huf_code  = (new_code) ? huffencode(huffcodes[LC][AC],num_zeros,value) : 0;
			huf_size  = (new_code) ? huffencode(huffsizes[LC][AC],num_zeros,value) : 0;
			num_zeros = (new_code || (i>=huf_lnzi)) ? 0 : (int)num_zeros+1;
			}
			codes_t temp_codes;
			temp_codes.code = huf_code; // the code        (the bits)
			temp_codes.size = huf_size; // the code length (the number of bits)
			code_length++;

			//check for end of image
			if (code_length==MAX_ROWS*MAX_COLUMNS*3) {
				temp_codes.done = true;
			    code_length=0; //reset counter for next image
				for (int j=0; j<=2; j++)
					dc[j] = 0; //reset dc static for next image
			}
			else
				temp_codes.done = false;

			if (temp_codes.size > 0)
				codes.write(temp_codes);
		}
		return(true);
  }
  else
	  return(false);
}

void extract(
				  ac_channel<bool> &start_jpeg,
			      rgb_t full_rgb[3*MAX_ROWS*MAX_COLUMNS], 
				  ac_channel<rgb_t> &rgb,
				  ac_channel<uint2> &block_type)
{
	if (start_jpeg.available(1)) {
		bool start_flag = start_jpeg.read();
		if (start_flag) {
			uint6 block_count=0;
			uint2 block=0;
			for (int i=0; i<3*MAX_ROWS*MAX_COLUMNS; i++) {
				rgb_t rgb_temp;
				rgb_temp.r = full_rgb[i].r;
				rgb_temp.g = full_rgb[i].g;
				rgb_temp.b = full_rgb[i].b;
				rgb.write(rgb_temp);
				if (block_count == 63) {
					block_type.write(block);
				    if (block<2)
						block++;
					else
						block=0;
				}
				block_count++;
			}
		}
	}
}

void write_out(
				 ac_channel<codes_t> &codes,
 				 ac_channel<bool> &done_jpeg,
 				 unsigned char jpeg_data[3*MAX_ROWS*MAX_COLUMNS],
				 uint21 &jpeg_data_len)
{
		done_jpeg.write(false);
		// Construct a Jpeg output object
		bitstream fp(jpeg_data);
		// write out all the header info for the jpeg
		fp.writepreamble();

		// write out the bitstream
//		for (int j=0; j<3*MAX_ROWS*MAX_COLUMNS; j++)
		codes_t huff_code;
		do
		{
			huff_code = codes.read();
//			if (huff_code.size > 0)
			fp.write(huff_code.size,huff_code.code);
		} while (!huff_code.done);
		// write out trailing info for the jpeg
		fp.writepostscript();
		jpeg_data_len=fp.bytes_written();
		done_jpeg.write(true);
//	}
}


void jpeg_encoder(
				  ac_channel<bool> &start_jpeg,
				  rgb_t full_rgb[3*MAX_ROWS*MAX_COLUMNS],
				  ac_channel<bool> &done_jpeg,
				  unsigned char jpeg_data[3*MAX_ROWS*MAX_COLUMNS],
				  uint21 &jpeg_data_len)
{

	static ac_channel<rgb_t> rgb;			// 8x8 RGB block (R:0, G:1, B:2)
	static ac_channel<codes_t> codes;		// huffman codes of compressed image for each 8x8
	static ac_channel<uint2> block_type;	// Type (R:0, G:1, B:2)

	static ac_channel<uint2> type_yuv2dct; // passing block_type to each hierarchy
	static ac_channel<uint2> type_dct_v2h;
	static ac_channel<uint2> type_dct2reorder;  // passing block_type to each hierarchy
	static ac_channel<uint2> type_reorder2huff;  // passing block_type to each hierarchy

	static ac_channel<char_array_t> ycbcrOut;
	//char 	ycbcrOut[64];
	static ac_channel<short_array_t> datat;
	//short datat[64]; // transpose buffer
	static ac_channel<short_array_t> dctOut;
//	short dctOut[64];
	static ac_channel<short_array_t> quantizeOut;
//	short quantizeOut[64];
	static ac_channel<uint6> last_non_zero_index;
//	uint6 last_non_zero_index;

	extract(start_jpeg, full_rgb, rgb, block_type); //generate 8x8 streams with corresponding block_type
	while (
			// convert 8x8 RGB block to 8x8 YCbCr
			rgb2ycbcr(block_type, rgb, ycbcrOut, type_yuv2dct) ||

			// run 2D 8x8 DCT on the block
//			dct8x8(type_yuv2dct, ycbcrOut, dctOut, type_dct2reorder)
			// horizontal processing
			dct_h(type_yuv2dct, ycbcrOut, datat, type_dct_v2h) ||
			// vertical processing
			dct_v(type_dct_v2h, datat, dctOut, type_dct2reorder) ||

			// zig-zag and quantize the results
			reorder_and_quantize(type_dct2reorder, dctOut, quantizeOut, last_non_zero_index, type_reorder2huff) ||

			// run-length and Huffman encode
			huffmanize(type_reorder2huff, quantizeOut, last_non_zero_index, codes)
			
			);
		
	write_out(codes, done_jpeg, jpeg_data, jpeg_data_len);
}
