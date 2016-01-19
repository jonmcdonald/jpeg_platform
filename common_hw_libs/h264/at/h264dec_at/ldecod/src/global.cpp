#ifdef ESLCPP
#include <stdlib.h>
#include "global.h"

pic_parameter_set_rbsp_t *active_pps;
seq_parameter_set_rbsp_t *active_sps;
int  **refFrArr;                                //!< Array for reference frames of each block
byte **imgY_ref;                                //!< reference frame find snr
byte ***imgUV_ref;
int  ReMapRef[20];
// B pictures
int  Bframe_ctr;
int  frame_no;
// For MB level frame/field coding
int  TopFieldForSkip_Y[16][16];
int  TopFieldForSkip_UV[2][16][16];


#define ET_SIZE 300      //!< size of error text buffer
char errortext[ET_SIZE]; //!< buffer for error message for exit with error()

int g_new_frame;
int tot_time;

int Frame_cnt = 0;
unsigned int FPindex = 0;
unsigned int Bitstream_leng = 0;
const unsigned int FPTable[61] = { 0, 18096, 22467, 27789, 33256, 38574, 44100, 49755, 55463, 61224, 66957, 72743, 78538, 84377, 90136, 
                                 95842, 101722, 107491, 113255, 119101, 124798, 130439, 136150, 141908, 147790, 153574, 159420, 165286,
                                 171211, 176982, 182715, 200744, 205434, 211004, 216515, 221996, 227632, 233334, 239056, 244743, 250469,
                                 256024, 261873, 267527, 273232, 278871, 284451, 289783, 294982, 300148, 305274, 310505, 315767, 321003,
                                 326247, 331502, 336834, 342246, 347644, 353088, 358625 };

FILE *p_out;                    //!< pointer to output YUV file
FILE *p_ref;                    //!< pointer to input original reference YUV file file
FILE *p_log;                    //!< SNR file

// SLICE function pointers
int  (*nal_startcode_follows) (struct img_par *, struct inp_par *, int);

// NAL functions TML/CABAC bitstream
int  uvlc_startcode_follows();
int  cabac_startcode_follows();
DataPartition *AllocPartition();

FILE *bits;

#endif
