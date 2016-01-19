#ifndef __bitstream_h__
#define __bitstream_h__

#include "stdio.h"
#include "ac_int.h"

class bitstream {
	private:

	uint4 bytebitcounter; //count bits 0-8
//	FILE * fp;
	unsigned char * fp;
	uint8 outbyte;
	uint21 bytewritecounter; //must hold up to 3*MAX_ROWS*MAX_COLUMNS
	uint39 outbuffer;  //must hold up to 4 bytes + 7 bits
	uint6 outbuffercount;  //count 0-39

	void pushbit(unsigned char c);
	void writebits();

	void flush();
	void writebuffer();
	void flushbuffer();
	// write bytes into the output stream
	inline void writebytes(unsigned char ptr) {
			fp[bytewritecounter] = ptr;
			bytewritecounter++;
	}

	public:

	// write numbits of bits onto the output stream
	void write(uint6 numbits, unsigned int bits);

	// write a jpeg file's preable before encoded bits
	void writepreamble();
	// complete the jpeg file
	void writepostscript();

	// return number of bytes_written
	uint21 bytes_written();
	

	inline  bitstream(unsigned char * ptr) : bytebitcounter(0), outbyte(0), bytewritecounter(0), outbuffer(0), outbuffercount(0) { fp = ptr; }
	inline ~bitstream() { }
};

#endif
