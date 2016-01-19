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
unsigned int FPindex = 0;
unsigned int Bitstream_leng = 0;

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
