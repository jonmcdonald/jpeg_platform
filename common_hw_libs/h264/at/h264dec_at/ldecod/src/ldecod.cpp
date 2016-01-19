
/*!
 ***********************************************************************
 *  \mainpage
 *     This is the H.264/AVC decoder reference software. For detailed documentation
 *     see the comments in each file.
 *
 *  \author
 *     The main contributors are listed in contributors.h
 *
 *  \version
 *     JM 8.2
 *
 *  \note
 *     tags are used for document system "doxygen"
 *     available at http://www.doxygen.org
 */
/*!
 *  \file
 *     ldecod.c
 *  \brief
 *     H.264/AVC reference decoder project main()
 *  \author
 *     Main contributors (see contributors.h for copyright, address and affiliation details)
 *     - Inge Lille-Lang�y       <inge.lille-langoy@telenor.com>
 *     - Rickard Sjoberg         <rickard.sjoberg@era.ericsson.se>
 *     - Stephan Wenger          <stewe@cs.tu-berlin.de>
 *     - Jani Lainema            <jani.lainema@nokia.com>
 *     - Sebastian Purreiter     <sebastian.purreiter@mch.siemens.de>
 *     - Byeong-Moon Jeon        <jeonbm@lge.com>
 *     - Gabi Blaettermann       <blaetter@hhi.de>
 *     - Ye-Kui Wang             <wyk@ieee.org>
 *     - Karsten Suehring        <suehring@hhi.de>
 *
 ***********************************************************************
 */

#include "contributors.h"
#ifdef ESLSC
#include "systemc.h"
#include "tb_hls_cavld.h"
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>

#if defined WIN32
  #include <conio.h>
#endif

#include <assert.h>

#include "global.h"
#include "rtp.h"
#include "memalloc.h"
#include "mbuffer.h"
#include "leaky_bucket.h"
#include "fmo.h"
#include "annexb.h"
#include "output.h"
#include "cabac.h"

#include "erc_api.h"

//CYLin : initiator wrapper & H264 handshaking variable 
#include "global_sc.h"
sc_event event_dump_one_frame_req;
sc_event event_dump_one_frame_end;
bool     flag_start_dump_frame;
unsigned int  num_pending_frame;
//-------------------------------------

#define JM          "8"
#define VERSION     "8.2"

#define LOGFILE     "log.dec"
#define DATADECFILE "dataDec.txt"
#define TRACEFILE   "trace_dec.txt"
#define FRAME_BUFFER_SIZE 4


extern objectBuffer_t *erc_object_list;
extern ercVariables_t *erc_errorVar;

#ifdef ESLSC
tb_hls_cavld *tb_hls_cavld_p = NULL;
#endif

#ifdef LT_MODEL
int flag_1bit_frame_done;
int flag_3bit_frame_type;
int flag_1bit_bitstream_buff_low;
int flag_interrupt;
#endif


// I have started to move the inp and img structures into global variables.
// They are declared in the following lines.  Since inp is defined in conio.h
// and cannot be overridden globally, it is defined here as input
//
// Everywhere, input-> and img-> can now be used either globally or with
// the local override through the formal parameter mechanism

extern FILE* bits;
extern StorablePicture* dec_picture;

struct inp_par    *input;       //!< input parameters from input configuration file
struct snr_par    *snr;         //!< statistics
struct img_par    *img;         //!< image parameters

int global_init_done = 0;

/*!
 ***********************************************************************
 * \brief
 *    main function for TML decoder
 ***********************************************************************
 */
#ifdef ESLSC
int ldecode(int argc, char **argv, void *tb)
#else
//int ldecod(/*int argc, char **argv*/)
//int ldecod(char * cfg_file)
//int ldecod(char * cfg_file, unsigned char * bitstream)
//int ldecod(char * cfg_file, unsigned char * bitstream, unsigned int length)
int ldecod(unsigned char * bitstream, unsigned int length)
#endif
{
  // allocate memory for the structures
if (Frame_cnt == 0)
{
  if ((input =  (struct inp_par *)calloc(1, sizeof(struct inp_par)))==NULL) no_mem_exit("main: input");
  if ((snr =  (struct snr_par *)calloc(1, sizeof(struct snr_par)))==NULL) no_mem_exit("main: snr");
  if ((img =  (struct img_par *)calloc(1, sizeof(struct img_par)))==NULL) no_mem_exit("main: img");
}
  // Read Configuration File
/*
  if (argc != 2)
  {
    snprintf(errortext, ET_SIZE, "Usage: %s <config.dat> \n\t<config.dat> defines decoder parameters",argv[0]);
    error(errortext, 300);
  }
*/
  Bitstream_leng = length;
 //init_conf(input, cfg_file);// read cfg & init
  init_conf2(input);// read cfg & init

  g_new_frame=1;

  switch (input->FileFormat)
  {
  case 0:
    //OpenBitstreamFile (input->infile);//Load bitsteam: master_read()
    OpenBitstreamFile2 (bitstream);//Load bitsteam: master_read()
    break;
  case 1:
    OpenRTPFile (input->infile);
    break;
  default:
    printf ("Unsupported file format %d, exit\n", input->FileFormat);
  }

  // Allocate Slice data struct
  malloc_slice(input,img);

  init(img);

  dec_picture = NULL;

  dpb.init_done = 0;

  //  init_dpb(input);
  init_out_buffer();

  img->idr_psnr_number=0;
  img->psnr_number=0;

  img->number=0;
  img->type = I_SLICE;
  img->tr_old = -1; // WYK: Oct. 8, 2001, for detection of a new frame
  img->dec_ref_pic_marking_buffer = NULL;

  // B pictures
  Bframe_ctr=0;

  // time for total decoding session
  tot_time = 0;
#ifdef ESLSC
  tb_hls_cavld_p = (tb_hls_cavld*)tb;
#endif
  // one clock cycle for one frame
  //printf("[%d]",Frame_cnt);
  //cout<< "-->start_time:"<<sc_time_stamp() << endl;
  while (decode_one_frame(img, input, snr) != EOS)
  {
    //flush_one_dpb();//cylin  
    
    num_pending_frame++;
    if(!flag_start_dump_frame){             
      //cout<< sc_time_stamp() <<"\t"<<" ldecode().event_dump_one_frame_req.notify"<<endl;
      flag_start_dump_frame = 1;
      event_dump_one_frame_req.notify(5*CLK_CYCLES, CLK_UNIT);//CYLin:define in "global.h"
    }
    
  }
  //if(flag_start_dump_frame && num_pending_frame >= FRAME_BUFFER_SIZE)
  if(num_pending_frame >= FRAME_BUFFER_SIZE)
    wait(event_dump_one_frame_end);//CYLin:waiting pre-frame

  __report(input, img, snr);

if(Frame_cnt == 60)
{
  //if(flag_start_dump_frame)
  while(num_pending_frame){
      wait(event_dump_one_frame_end);//CYLin:waiting pre-frame
  }
  free_slice(input,img);
  FmoFinit();
  free_global_buffers(input, img);
  //printf("********  flush_dpb()  ***********\n");//cylin.debug
  flush_dpb();

  //CloseBitstreamFile();
  CloseBitstreamFile2();

//  fclose(p_out);
//  fclose(p_out2);
  if (p_ref)
    fclose(p_ref);
#if TRACE
  fclose(p_trace);
#endif

  ercClose(erc_errorVar);

  free_dpb();

  uninit_out_buffer();

  free (input);
  free (snr);
  free (img);
}  
  //while( !kbhit() ); 
  return 0;
}


/*!
 ***********************************************************************
 * \brief
 *    Initilize some arrays
 ***********************************************************************
 */
void init(struct img_par *img)  //!< image parameters
{
  int i;

  // initilize quad matrix used in snr routine
  for (i=0; i <  256; i++)
  {
    img->quad[i]=i*i; // fix from TML 1, truncation removed
  }
}


/*!
 ************************************************************************
 * \brief
 *    Read input from configuration file
 *
 * \par Input:
 *    Name of configuration filename
 *
 * \par Output
 *    none
 ************************************************************************
 */
void init_conf(struct inp_par *inp,
               char *config_filename)
{
  FILE *fd;
  int NAL_mode;

  // read the decoder configuration file
  if((fd=fopen(config_filename,"r")) == NULL)
  {
    snprintf(errortext, ET_SIZE, "Error: Control file %s not found\n",config_filename);
    error(errortext, 300);
  }

  fscanf(fd,"%s",inp->infile);                // H.26L compressed input bitsream
  fscanf(fd,"%*[^\n]");

  fscanf(fd,"%s",inp->outfile);               // YUV 4:2:2 input format
  fscanf(fd,"%*[^\n]");

  fscanf(fd,"%s",inp->reffile);               // reference file
  fscanf(fd,"%*[^\n]");


  // Frame buffer size
  fscanf(fd,"%d,",&inp->dpb_size);   // may be overwritten in case of RTP NAL //default: 10
  fscanf(fd,"%*[^\n]");
  if (inp->dpb_size < 1)
  {
    snprintf(errortext, ET_SIZE, "Decoded Picture Buffer Size is %d. It has to be at least 1",inp->dpb_size);
    error(errortext,1);
  }

  fscanf(fd,"%d",&(NAL_mode));                // NAL mode// default: 0
  fscanf(fd,"%*[^\n]");

  switch(NAL_mode)
  {
  case 0:
    inp->FileFormat = PAR_OF_ANNEXB;
    break;
  case 1:
    inp->FileFormat = PAR_OF_RTP;
    break;
  default:
    snprintf(errortext, ET_SIZE, "NAL mode %i is not supported", NAL_mode);
    error(errortext,400);
  }

#ifdef _LEAKYBUCKET_
  fscanf(fd,"%ld,",&inp->R_decoder);             // Decoder rate
  fscanf(fd, "%*[^\n]");
  fscanf(fd,"%ld,",&inp->B_decoder);             // Decoder buffer size
  fscanf(fd, "%*[^\n]");
  fscanf(fd,"%ld,",&inp->F_decoder);             // Decoder initial delay
  fscanf(fd, "%*[^\n]"); 
  fscanf(fd,"%s",inp->LeakyBucketParamFile);    // file where Leaky Bucket params (computed by encoder) are stored
  fscanf(fd,"%*[^\n]");
#endif

  fclose (fd);


#if TRACE
  if ((p_trace=fopen(TRACEFILE,"w"))==0)             // append new statistic at the end
  {
    snprintf(errortext, ET_SIZE, "Error open file %s!",TRACEFILE);
    error(errortext,500);
  }
#endif


  if ((p_out=fopen(inp->outfile,"wb"))==0)
  {
    snprintf(errortext, ET_SIZE, "Error open file %s ",inp->outfile);
    error(errortext,500);
  }
/*  if ((p_out2=fopen("out.yuv","wb"))==0)
  {
    snprintf(errortext, ET_SIZE, "Error open file %s ",inp->outfile);
    error(errortext,500);
  }*/



  fprintf(stdout,"--------------------------------------------------------------------------\n");
  fprintf(stdout," Decoder config file                    : %s \n",config_filename);
  fprintf(stdout,"--------------------------------------------------------------------------\n");
  fprintf(stdout," Input H.26L bitstream                  : %s \n",inp->infile);
  fprintf(stdout," Output decoded YUV 4:2:0               : %s \n",inp->outfile);
  fprintf(stdout," Output status file                     : %s \n",LOGFILE);
  if ((p_ref=fopen(inp->reffile,"rb"))==0)
  {
    fprintf(stdout," Input reference file                   : %s does not exist \n",inp->reffile);
    fprintf(stdout,"                                          SNR values are not available\n");
  }
  else
    fprintf(stdout," Input reference file                   : %s \n",inp->reffile);

  fprintf(stdout,"--------------------------------------------------------------------------\n");
#ifdef _LEAKYBUCKET_
  fprintf(stdout," Rate_decoder        : %8ld \n",inp->R_decoder);
  fprintf(stdout," B_decoder           : %8ld \n",inp->B_decoder);
  fprintf(stdout," F_decoder           : %8ld \n",inp->F_decoder);
  fprintf(stdout," LeakyBucketParamFile: %s \n",inp->LeakyBucketParamFile); // Leaky Bucket Param file
  calc_buffer(inp);
  fprintf(stdout,"--------------------------------------------------------------------------\n");
#endif
  fprintf(stdout,"POC must = frame# or field# for SNRs to be correct\n");
  fprintf(stdout,"Frame    POC   QP  SnrY    SnrU    SnrV   Time(ms)\n");

}

/*!
 ************************************************************************
 * \brief
 *    Read input from configuration file
 *
 * \par Input:
 *    Name of configuration filename
 *
 * \par Output
 *    none
 ************************************************************************
 */
void init_conf2(struct inp_par *inp)
{
  int NAL_mode;

  inp->infile   = "mobile_q20_60.yuv";
  inp->outfile  = "data/test_dec.yuv";
  inp->reffile  = "data/test_rec.yuv";
  inp->dpb_size = 10;
  NAL_mode = 0;// NAL mode// default: 0

  if (inp->dpb_size < 1)
  {
    snprintf(errortext, ET_SIZE, "Decoded Picture Buffer Size is %d. It has to be at least 1",inp->dpb_size);
    error(errortext,1);
  }

  switch(NAL_mode)
  {
  case 0:
    inp->FileFormat = PAR_OF_ANNEXB;
    break;
  case 1:
    inp->FileFormat = PAR_OF_RTP;
    break;
  default:
    snprintf(errortext, ET_SIZE, "NAL mode %i is not supported", NAL_mode);
    error(errortext,400);
  }

#ifdef _LEAKYBUCKET_
  inp->R_decoder = 500000;
  inp->B_decoder = 104000;
  inp->F_decoder = 73000;
  inp->LeakyBucketParamFile = "leakybucketparam.cfg";
#endif

#if TRACE
  if ((p_trace=fopen(TRACEFILE,"w"))==0)             // append new statistic at the end
  {
    snprintf(errortext, ET_SIZE, "Error open file %s!",TRACEFILE);
    error(errortext,500);
  }
#endif
if (Frame_cnt == 0)
{
  fprintf(stdout,"--------------------------------------------------------------------------\n");
  fprintf(stdout,"                          H.264 Decoder Configuration                     \n");
  fprintf(stdout,"--------------------------------------------------------------------------\n");
  fprintf(stdout," Input H.26L bitstream                  : %s \n",inp->infile);
  fprintf(stdout," Output decoded YUV 4:2:0               : %s \n",inp->outfile);
  fprintf(stdout," Output status file                     : %s \n",LOGFILE);
  fprintf(stdout," Input reference file                   : %s \n",inp->reffile);
  fprintf(stdout,"--------------------------------------------------------------------------\n");

#ifdef _LEAKYBUCKET_
  fprintf(stdout," Rate_decoder        : %8ld \n",inp->R_decoder);
  fprintf(stdout," B_decoder           : %8ld \n",inp->B_decoder);
  fprintf(stdout," F_decoder           : %8ld \n",inp->F_decoder);
  fprintf(stdout," LeakyBucketParamFile: %s \n",inp->LeakyBucketParamFile); // Leaky Bucket Param file
  calc_buffer(inp);
  fprintf(stdout,"--------------------------------------------------------------------------\n");
#endif
  fprintf(stdout,"POC must = frame# or field# for SNRs to be correct\n");
  fprintf(stdout,"Frame    POC   QP  SnrY    SnrU    SnrV   Time(ms)\n");
}
    printf("**************************************************************************\n");    
    printf("  Frame No.%d\n", Frame_cnt);
    printf("**************************************************************************\n");    
}


/*!
 ************************************************************************
 * \brief
 *    Reports the gathered information to appropriate outputs
 *
 * \par Input:
 *    struct inp_par *inp,
 *    struct img_par *img,
 *    struct snr_par *stat
 *
 * \par Output:
 *    None
 ************************************************************************
 */
void __report(struct inp_par *inp, struct img_par *img, struct snr_par *snr)
{
  #define OUTSTRING_SIZE 255
  char string[OUTSTRING_SIZE];
  FILE *p_log;

#ifndef WIN32
  time_t  now;
  struct tm *l_time;
#else
  char timebuf[128];
#endif

if (Frame_cnt == 0)
{
  fprintf(stdout,"-------------------- Average SNR all frames ------------------------------\n");
  fprintf(stdout," SNR Y(dB)           : %5.2f\n",snr->snr_ya);
  fprintf(stdout," SNR U(dB)           : %5.2f\n",snr->snr_ua);
  fprintf(stdout," SNR V(dB)           : %5.2f\n",snr->snr_va);
  fprintf(stdout," Total decoding time : %.3f sec \n",tot_time*0.001);
  fprintf(stdout,"--------------------------------------------------------------------------\n");
  fprintf(stdout," Exit JM %s decoder, ver %s ",JM,VERSION);
  fprintf(stdout,"\n");
}
  // write to log file

  snprintf(string, OUTSTRING_SIZE, "%s", LOGFILE);
  if ((p_log=fopen(string,"r"))==0)                    // check if file exist
  {
    if ((p_log=fopen(string,"a"))==0)
    {
      snprintf(errortext, ET_SIZE, "Error open file %s for appending",string);
      error(errortext, 500);
    }
    else                                              // Create header to new file
    {
      fprintf(p_log," ------------------------------------------------------------------------------------------\n");
      fprintf(p_log,"|  Decoder statistics. This file is made first time, later runs are appended               |\n");
      fprintf(p_log," ------------------------------------------------------------------------------------------ \n");
      fprintf(p_log,"| Date  | Time  |    Sequence        |#Img|Format|SNRY 1|SNRU 1|SNRV 1|SNRY N|SNRU N|SNRV N|\n");
      fprintf(p_log," ------------------------------------------------------------------------------------------\n");
    }
  }
  else
  { 
    fclose(p_log);
    p_log=fopen(string,"a");                    // File exist,just open for appending
  }

#ifdef WIN32
  _strdate( timebuf );
  fprintf(p_log,"| %1.5s |",timebuf );

  _strtime( timebuf);
  fprintf(p_log," % 1.5s |",timebuf);
#else
  now = time ((time_t *) NULL); // Get the system time and put it into 'now' as 'calender time'
  time (&now);
  l_time = localtime (&now);
  strftime (string, sizeof string, "%d-%b-%Y", l_time);
  fprintf(p_log,"| %1.5s |",string );

  strftime (string, sizeof string, "%H:%M:%S", l_time);
  fprintf(p_log,"| %1.5s |",string );
#endif

  fprintf(p_log,"%20.20s|",inp->infile);

  fprintf(p_log,"%3d |",img->number);

  fprintf(p_log,"%6.3f|",snr->snr_y1);
  fprintf(p_log,"%6.3f|",snr->snr_u1);
  fprintf(p_log,"%6.3f|",snr->snr_v1);
  fprintf(p_log,"%6.3f|",snr->snr_ya);
  fprintf(p_log,"%6.3f|",snr->snr_ua);
  fprintf(p_log,"%6.3f|\n",snr->snr_va);

  fclose(p_log);

  snprintf(string, OUTSTRING_SIZE,"%s", DATADECFILE);
  p_log=fopen(string,"a");

  if(Bframe_ctr != 0) // B picture used
  {
    fprintf(p_log, "%3d %2d %2d %2.2f %2.2f %2.2f %5d "
      "%2.2f %2.2f %2.2f %5d "
      "%2.2f %2.2f %2.2f %5d %.3f\n",
      img->number, 0, img->qp,
      snr->snr_y1,
      snr->snr_u1,
      snr->snr_v1,
      0,
      0.0,
      0.0,
      0.0,
      0,
      snr->snr_ya,
      snr->snr_ua,
      snr->snr_va,
      0,
      (double)0.001*tot_time/(img->number+Bframe_ctr-1));
  }
  else
  {
    fprintf(p_log, "%3d %2d %2d %2.2f %2.2f %2.2f %5d "
      "%2.2f %2.2f %2.2f %5d "
      "%2.2f %2.2f %2.2f %5d %.3f\n",
      img->number, 0, img->qp,
      snr->snr_y1,
      snr->snr_u1,
      snr->snr_v1,
      0,
      0.0,
      0.0,
      0.0,
      0,
      snr->snr_ya,
      snr->snr_ua,
      snr->snr_va,
      0,
      (double)0.001*tot_time/img->number);
  }
  fclose(p_log);
}

/*!
 ************************************************************************
 * \brief
 *    Allocates a stand-alone partition structure.  Structure should
 *    be freed by FreePartition();
 *    data structures
 *
 * \par Input:
 *    n: number of partitions in the array
 * \par return
 *    pointer to DataPartition Structure, zero-initialized
 ************************************************************************
 */

DataPartition *AllocPartition(int n)
{
  DataPartition *partArr, *dataPart;
  int i;

  partArr = (DataPartition *) calloc(n, sizeof(DataPartition));
  if (partArr == NULL)
  {
    snprintf(errortext, ET_SIZE, "AllocPartition: Memory allocation for Data Partition failed");
    error(errortext, 100);
  }

  for (i=0; i<n; i++) // loop over all data partitions
  {
    dataPart = &(partArr[i]);
    dataPart->bitstream = (Bitstream *) calloc(1, sizeof(Bitstream));
    if (dataPart->bitstream == NULL)
    {
      snprintf(errortext, ET_SIZE, "AllocPartition: Memory allocation for Bitstream failed");
      error(errortext, 100);
    }
    dataPart->bitstream->streamBuffer = (byte *) calloc(MAX_CODED_FRAME_SIZE, sizeof(byte));
    if (dataPart->bitstream->streamBuffer == NULL)
    {
      snprintf(errortext, ET_SIZE, "AllocPartition: Memory allocation for streamBuffer failed");
      error(errortext, 100);
    }
  }
  return partArr;
}




/*!
 ************************************************************************
 * \brief
 *    Frees a partition structure (array).  
 *
 * \par Input:
 *    Partition to be freed, size of partition Array (Number of Partitions)
 *
 * \par return
 *    None
 *
 * \note
 *    n must be the same as for the corresponding call of AllocPartition
 ************************************************************************
 */


void FreePartition (DataPartition *dp, int n)
{
  int i;

  assert (dp != NULL);
  assert (dp->bitstream != NULL);
  assert (dp->bitstream->streamBuffer != NULL);
  for (i=0; i<n; i++)
  {
    free (dp[i].bitstream->streamBuffer);
    free (dp[i].bitstream);
  }
  free (dp);
}


/*!
 ************************************************************************
 * \brief
 *    Allocates the slice structure along with its dependent
 *    data structures
 *
 * \par Input:
 *    Input Parameters struct inp_par *inp,  struct img_par *img
 ************************************************************************
 */
void malloc_slice(struct inp_par *inp, struct img_par *img)
{
  Slice *currSlice;

  img->currentSlice = (Slice *) calloc(1, sizeof(Slice));
  if ( (currSlice = img->currentSlice) == NULL)
  {
    snprintf(errortext, ET_SIZE, "Memory allocation for Slice datastruct in NAL-mode %d failed", inp->FileFormat);
    error(errortext,100);
  }
//  img->currentSlice->rmpni_buffer=NULL;
  //! you don't know whether we do CABAC hre, hence initialize CABAC anyway
  // if (inp->symbol_mode == CABAC)
  if (1)
  {
    // create all context models
    currSlice->mot_ctx = create_contexts_MotionInfo();
    currSlice->tex_ctx = create_contexts_TextureInfo();
  }
  currSlice->max_part_nr = 3;  //! assume data partitioning (worst case) for the following mallocs()
  currSlice->partArr = AllocPartition(currSlice->max_part_nr);
}


/*!
 ************************************************************************
 * \brief
 *    Memory frees of the Slice structure and of its dependent
 *    data structures
 *
 * \par Input:
 *    Input Parameters struct inp_par *inp,  struct img_par *img
 ************************************************************************
 */
void free_slice(struct inp_par *inp, struct img_par *img)
{
  Slice *currSlice = img->currentSlice;

  FreePartition (currSlice->partArr, 3);
//      if (inp->symbol_mode == CABAC)
  if (1)
  {
    // delete all context models
    delete_contexts_MotionInfo(currSlice->mot_ctx);
    delete_contexts_TextureInfo(currSlice->tex_ctx);
  }
  free(img->currentSlice);

  currSlice = NULL;
}

/*!
 ************************************************************************
 * \brief
 *    Dynamic memory allocation of frame size related global buffers
 *    buffers are defined in global.h, allocated memory must be freed in
 *    void free_global_buffers()
 *
 *  \par Input:
 *    Input Parameters struct inp_par *inp, Image Parameters struct img_par *img
 *
 *  \par Output:
 *     Number of allocated bytes
 ***********************************************************************
 */
int init_global_buffers(struct inp_par *inp, struct img_par *img)
{
  int memory_size=0;

  if (global_init_done)
  {
    free_global_buffers(inp, img);
  }

  if (img->structure != FRAME)
  {
    img->height *= 2;         // set height to frame (twice of field) for normal variables
    img->height_cr *= 2;      // set height to frame (twice of field) for normal variables
  }

  // allocate memory for reference frame in find_snr
  memory_size += get_mem2D(&imgY_ref, img->height, img->width);
  memory_size += get_mem3D(&imgUV_ref, 2, img->height_cr, img->width_cr);

  // allocate memory in structure img

#ifndef STATIC_ALLOC
  if(((img->mb_data) = (Macroblock *) calloc((img->width/MB_BLOCK_SIZE) * (img->height/MB_BLOCK_SIZE),sizeof(Macroblock))) == NULL)
    no_mem_exit("init_global_buffers: img->mb_data");

  if(((img->intra_block) = (int*)calloc((img->width/MB_BLOCK_SIZE) * (img->height/MB_BLOCK_SIZE),sizeof(int))) == NULL)
    no_mem_exit("init_global_buffers: img->intra_block");

  memory_size += get_mem2Dint(&(img->ipredmode),img->width/BLOCK_SIZE , img->height/BLOCK_SIZE);

  // CAVLC mem
  memory_size += get_mem3Dint(&(img->nz_coeff), img->FrameSizeInMbs, 4, 6);
  memory_size += get_mem2Dint(&(img->siblock),img->width/MB_BLOCK_SIZE  , img->height/MB_BLOCK_SIZE);
#else
  // ipredmode
  memory_size += (1920/BLOCK_SIZE) * (1088/BLOCK_SIZE) * sizeof(int);
  
  // CAVLC mem
  // nz_coeff
  memory_size += (1920/MB_BLOCK_SIZE) * (1088/MB_BLOCK_SIZE) * 4 * 6 * sizeof(int);
  // siblock
  memory_size += (176/MB_BLOCK_SIZE) * (144/MB_BLOCK_SIZE) * sizeof(int);
#endif


 
  memory_size += get_mem2Dint(&(img->field_anchor),img->height/BLOCK_SIZE,img->width/BLOCK_SIZE);

  memory_size += get_mem3Dint(&(img->wp_weight), 2, MAX_REFERENCE_PICTURES, 3);
  memory_size += get_mem3Dint(&(img->wp_offset), 2, MAX_REFERENCE_PICTURES, 3);
  memory_size += get_mem4Dint(&(img->wbp_weight), 2, MAX_REFERENCE_PICTURES, MAX_REFERENCE_PICTURES, 3);





  if (img->structure != FRAME)
  {
    img->height /= 2;      // reset height for normal variables
    img->height_cr /= 2;   // reset height for normal variables
  }
  
  global_init_done = 1;

  return (memory_size);
}

/*!
 ************************************************************************
 * \brief
 *    Free allocated memory of frame size related global buffers
 *    buffers are defined in global.h, allocated memory is allocated in
 *    int init_global_buffers()
 *
 * \par Input:
 *    Input Parameters struct inp_par *inp, Image Parameters struct img_par *img
 *
 * \par Output:
 *    none
 *
 ************************************************************************
 */
void free_global_buffers(struct inp_par *inp, struct img_par *img)
{
  free_mem2D (imgY_ref);
  free_mem3D (imgUV_ref,2);

#ifndef STATIC_ALLOC
  // CAVLC free mem
  free_mem3Dint(img->nz_coeff, img->FrameSizeInMbs);
  free_mem2Dint(img->siblock);

  // free mem, allocated for structure img
  if (img->mb_data       != NULL) free(img->mb_data);

  free (img->intra_block);
  free_mem2Dint (img->ipredmode);
#endif

  free_mem2Dint(img->field_anchor);

  free_mem3Dint(img->wp_weight, 2);
  free_mem3Dint(img->wp_offset, 2);
  free_mem4Dint(img->wbp_weight, 2, MAX_REFERENCE_PICTURES);

  global_init_done = 0;

}



