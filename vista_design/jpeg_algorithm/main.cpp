
#include "jpeg.h"
#include "bmp.h"


#ifdef CCS_SCVERIFY
#include "mc_testbench.h"
#define CCS_DESIGN(a) testbench::exec_##a
//#define CCS_MAIN(a,b) void testbench::main()
#define CCS_MAIN(a,b) int testbench::main()
#define CCS_RETURN(a) return (a)
#else
#define CCS_DESIGN(a) a
#define CCS_MAIN(a,b) int main(a,b)
#define CCS_RETURN(a) return (a)
#endif

#include <iostream>

using std::cout;
using std::endl;

void jpgencoder(const char * bmpfilename, const char * jpgfilename)
{
	cout << " Reading BMP file " << bmpfilename << endl;

	bmp * bitmap = new bmp(bmpfilename);

	cout << " Compressing to JPG file " << jpgfilename << endl;

	assert(bitmap != NULL);

	// rows and columns must be divisible by 8, so make bigger if necessary
	if (bitmap->rows() % 8) {
		printf("  ERROR: number of rows (image width) must be divisible by 8. please resize original bitmap image\n");
		exit(-1);
	}
	if (bitmap->columns() % 8) {
		printf("  ERROR: number of columns (image height) must be divisible by 8. please resize original bitmap image\n");
		exit(-1);
	}

	unsigned int img_rows = bitmap->rows();
	unsigned int img_columns = bitmap->columns();
	static ac_channel<bool> start_jpeg;
	static ac_channel<bool> done_jpeg;
	static unsigned char jpeg_data[3*MAX_ROWS*MAX_COLUMNS];  	// compress JPEG stream in bytes
	uint21 jpeg_data_len=0; //number of bytes in stream
	static rgb_t rgb[MAX_ROWS*MAX_COLUMNS];  	// full RGB image struct
	static rgb_t rgb_stream[3*MAX_ROWS*MAX_COLUMNS]; //reordered and tripled RGB to allow streaming into JPEG compressor
	
	// extract RGB arrays from BMP and re-order from bottom left to top left
	for (unsigned i = 0; i < img_rows; i++) {
		for (unsigned j = 0; j < img_columns; j++) {
			rgb[i*img_columns+j].r = bitmap->red(i, j);
			rgb[i*img_columns+j].g = bitmap->green(i, j);
			rgb[i*img_columns+j].b = bitmap->blue(i, j);
		}
	}

	unsigned int incr=0;
	for (int row = 0; row < MAX_ROWS; row += 8)	{
		for (int column = 0; column < MAX_COLUMNS; column += 8)	{
			for (uint2 block = 0; block <= 2; block++) {
				for (int i = 0; i < 8; i++)	{
					for (int j = 0; j < 8; j++) {
						int full_rgb_idx = (row+i)*MAX_COLUMNS+(column+j);
						rgb_stream[incr].r = rgb[full_rgb_idx].r;
						rgb_stream[incr].g = rgb[full_rgb_idx].g;
						rgb_stream[incr].b = rgb[full_rgb_idx].b;
						incr++;
					}
				}
			}
		}
	}

	start_jpeg.write(true);

	CCS_DESIGN( jpeg_encoder(start_jpeg, rgb_stream, done_jpeg, jpeg_data, jpeg_data_len) );

	FILE * fp;
	//add check for done
	if (done_jpeg.available(2)) {
		done_jpeg.read(); //throw away initialize value
		if (done_jpeg.read()) {
			fp = fopen(jpgfilename, "wb");
			for(int k=0; k<jpeg_data_len; k++) {
				fwrite(&jpeg_data[k], 1, 1, fp);
				if (k<610) {
					printf("0x%02X, ", jpeg_data[k]);
					if ((k+1)%12 == 0) printf("\n");
				}
			}
			fclose(fp);
		}
		else
			cout << "Done response false from JPEG encoder" << endl;
	}
	else
		cout << "No done response from JPEG encoder" << endl;

	cout << " ...Done" << endl << endl;

}


CCS_MAIN(int argc, const char *argv[])
{
	char *output;

	switch (argc) {
		case 3:
			jpgencoder(argv[1], argv[2]);
			break;
		case 2:
			output = (char *)malloc(strlen(argv[1])+4);
			strcpy(output, argv[1]);
			strcat(output, ".jpg");
			jpgencoder(argv[1], output);
			break;
		default:
			jpgencoder("default.bmp", "default.bmp.jpg");
			break;
	}
	CCS_RETURN(0);
}
