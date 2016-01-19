#include "assert.h"
#include "bitstream.h"
#include "tables.h"
#include "jpeg_header.h"

void bitstream::writebits() {
	unsigned char o = outbyte;
	writebytes(o);
}

void bitstream::flush() {
	if (outbuffercount == 0) return;
	bytebitcounter = outbuffercount;
//	for (int i=0; i<7; i++) {
//		pushbit(1);
//		if (bytebitcounter == 8) break;
//	}
	uint3 flush_shift=8-bytebitcounter;
	outbyte << flush_shift;
	outbyte |= (0xFF >> bytebitcounter);
	writebits();
	if (outbyte == 0xFF) { 
		outbyte = 0; writebits(); 
	}
	bytebitcounter = 0;
}

void bitstream::flushbuffer() {
	uint39 bits = outbuffer;

	//shift to MSB of long long
//	bits <<= (64 - outbuffercount);
//	outbyte = (bits>>56);
	bits <<= (39 - outbuffercount);
	outbyte = (bits>>31);
	for (unsigned char i=0; i<8; i++)
	{
		writebits();
		if (outbyte == 0xFF) { 
			outbyte = 0; 
		}
		else {
			bits<<=8;
//			outbyte = (bits>>56);
			outbyte = (bits>>31);
	        outbuffercount = outbuffercount-8;
		}
		if (outbuffercount<8)
			break;
	}
}

void bitstream::pushbit(unsigned char c) { // 1 or 0
	// first bit pushed will end up at MSB position
	outbyte <<= 1;
	outbyte |= c;
	bytebitcounter++;
}

// writes numbits of bits out
void bitstream::write(uint6 numbits, unsigned int bits) {

#ifndef __SYNTHESIS__
	if (numbits>32)
		assert(numbits <= 32);
#endif

/*	// move msb up to int boundary
	bits <<= (32 - numbits);
	for (unsigned char i = 0; i < 32; i++) {
		outbuffer <<= 1;
		outbuffer |= (bits >> 31);
		outbuffercount++;

		bits <<= 1;
		if (i==numbits-1) break;
	}
*/
	outbuffer <<= numbits;
	outbuffer |= bits;
	outbuffercount += numbits;
	if (outbuffercount > 7)
		flushbuffer();
}

// write a jpeg file upto the actual scan data
void bitstream::writepreamble() {
	for (int i=0; i<607; i++)
		writebytes(jpeg_preamble[i]);
}

// complete the jpeg file
void bitstream::writepostscript() {
	// make sure we are byte aligned
	flush();
//	write_EOI();
	for (int i=0; i<2; i++)
		writebytes(jpeg_postscript[i]);
}

uint21 bitstream::bytes_written() {
	return bytewritecounter;
}
