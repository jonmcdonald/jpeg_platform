
/*!
 ***********************************************************************
 * \file image.c
 *
 * \brief
 *    Decode a Slice
 *
 * \author
 *    Main contributors (see contributors.h for copyright, address and affiliation details)
 *    - Inge Lille-Lang�y               <inge.lille-langoy@telenor.com>
 *    - Rickard Sjoberg                 <rickard.sjoberg@era.ericsson.se>
 *    - Jani Lainema                    <jani.lainema@nokia.com>
 *    - Sebastian Purreiter             <sebastian.purreiter@mch.siemens.de>
 *    - Byeong-Moon Jeon                <jeonbm@lge.com>
 *    - Thomas Wedi                     <wedi@tnt.uni-hannover.de>
 *    - Gabi Blaettermann               <blaetter@hhi.de>
 *    - Ye-Kui Wang                     <wyk@ieee.org>
 *    - Antti Hallapuro                 <antti.hallapuro@nokia.com>
 *    - Alexis Tourapis                 <alexismt@ieee.org>
 ***********************************************************************
 */

#include "contributors.h"
#include <iostream>

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <string.h>
#include <assert.h>
#include <systemc.h>

#include "global.h"
#include "errorconcealment.h"
#include "image.h"
#include "mbuffer.h"
#include "fmo.h"
#include "nalu.h"
#include "parsetcommon.h"
#include "parset.h"
#include "header.h"
#include "rtp.h"
#include "sei.h"
#include "output.h"
#include "biaridecod.h"
#include "mb_access.h"

#include "context_ini.h"
#include "cabac.h"

#include "vlc.h"

#include "erc_api.h"

#ifdef ESLSC
#include "tb_hls_cavld.h"
extern tb_hls_cavld *tb_hls_cavld_p;
#endif

extern objectBuffer_t *erc_object_list;
extern ercVariables_t *erc_errorVar;
extern frame erc_recfr;
extern int erc_mvperMB;
extern struct img_par *erc_img;
//extern int FPindex;
//extern FILE *p_out2;

extern StorablePicture **listX[6];

StorablePicture *dec_picture;

void MbAffPostProc()
{
  byte temp[16][32];

  byte ** imgY  = dec_picture->imgY;
  byte ***imgUV = dec_picture->imgUV;

  int i, x, y, x0, y0, uv;
  for (i=0; i<(int)img->PicSizeInMbs; i+=2)
  {
    if (dec_picture->mb_field[i])
    {
      get_mb_pos(i, &x0, &y0);
      for (y=0; y<(2*MB_BLOCK_SIZE);y++)
        for (x=0; x<MB_BLOCK_SIZE; x++)
          temp[x][y] = imgY[y0+y][x0+x];

      for (y=0; y<MB_BLOCK_SIZE;y++)
        for (x=0; x<MB_BLOCK_SIZE; x++)
        {
          imgY[y0+(2*y)][x0+x]   = temp[x][y];
          imgY[y0+(2*y+1)][x0+x] = temp[x][y+MB_BLOCK_SIZE];
        }

      x0 = x0/2;
      y0 = y0/2;

      for (uv=0; uv<2; uv++)
      {
        for (y=0; y<(2*MB_BLOCK_SIZE/2);y++)
          for (x=0; x<MB_BLOCK_SIZE/2; x++)
            temp[x][y] = imgUV[uv][y0+y][x0+x];
          
        for (y=0; y<MB_BLOCK_SIZE/2;y++)
          for (x=0; x<MB_BLOCK_SIZE/2; x++)
          {
            imgUV[uv][y0+(2*y)][x0+x]   = temp[x][y];
            imgUV[uv][y0+(2*y+1)][x0+x] = temp[x][y+MB_BLOCK_SIZE/2];
          }
      }
    }
  }
}

/*!
 ***********************************************************************
 * \brief
 *    decodes one I- or P-frame
 *
 ***********************************************************************
 */

int decode_one_frame(struct img_par *img,struct inp_par *inp, struct snr_par *snr)
{
  int current_header;
  Slice *currSlice = img->currentSlice;
  int ercStartMB;
  int ercSegment;
//  frame recfr;

  time_t ltime1;                  // for time measurement
  time_t ltime2;

#ifdef WIN32
  struct _timeb tstruct1;
  struct _timeb tstruct2;
#else
  struct timeb tstruct1;
  struct timeb tstruct2;
#endif

  int tmp_time;                   // time used by decoding the last frame

#ifdef ESLSC
  int filesize, result;
  byte *buffer;
#endif

#ifdef WIN32
  _ftime (&tstruct1);             // start time ms
#else
  ftime (&tstruct1);              // start time ms
#endif
  time( &ltime1 );                // start time s

  img->current_slice_nr = 0;
#ifdef ESLCPP
  img->current_mb_nr = 0xFFFFED99;     // initialized to an impossible value for debugging -- correct value is taken from slice header
#else
  img->current_mb_nr = -4711;     // initialized to an impossible value for debugging -- correct value is taken from slice header
#endif
  currSlice->next_header = -8888; // initialized to an impossible value for debugging -- correct value is taken from slice header
  img->num_dec_mb = 0;

  while ((currSlice->next_header != EOS && currSlice->next_header != SOP))
  {
#ifdef ESLSC
    //20111110 JiffKuo, need to revise the hankshaking
    if(tb_hls_cavld_p->stream_req.read()){
      printf("3. Enter Testbench...\n");
      tb_hls_cavld_p->stream_read_enb.write(1);
      // read bitstream into buffer
/*
      fseek(bits, 0, SEEK_END);
      filesize = ftell(bits);
      fseek(bits, 0, SEEK_SET);
      if((buffer = (byte*)calloc(filesize, sizeof(byte))) == NULL)
        printf("[Error]: Cannnot create buffer!\n");
      result=fread(buffer, 1, filesize, bits);
      if(result != filesize)
        printf("[Error]: buffer load bitstream fail!!\n");
      fseek(bits, 0, SEEK_SET);

      for(int i=0; i<filesize; i++){
        tb_hls_cavld_p->stream_data.write((sc_uint<8>)buffer[i]);
        wait();
      }
*/
      tb_hls_cavld_p->stream_read_enb.write(0);
      free(buffer);
      //wait(2); //1->send read_enb, 2->receive stream_req
      wait(2*CLK_CYCLES,CLK_UNIT);

    }else{
      tb_hls_cavld_p->stream_read_enb.write(0);
    }
#endif
    //printf("FP index: \n", FPindex);
    if (FPindex == Bitstream_leng+1) current_header = EOS;
    else current_header = read_new_slice();

    if (current_header == EOS)
      return EOS;

    if (img->structure == FRAME)
      decode_frame_slice(img, inp, current_header);
    else 
      decode_field_slice(img, inp, current_header);

    img->current_slice_nr++;

#ifdef LT_MODEL
    flag_1bit_frame_done = 1;
    //wait();
    wait(CLK_CYCLES,CLK_UNIT);
    //std::cout<<sc_time_stamp()<<"[H264] Image.cpp"<<std::endl;
    flag_1bit_frame_done = 0;
    //wait();
    wait(CLK_CYCLES,CLK_UNIT);
#endif
  }

  //deblocking for frame or first field
  DeblockFrame( img, dec_picture->imgY, dec_picture->imgUV ) ;
  if (img->MbaffFrameFlag)
    MbAffPostProc();

  //printf("Achim: After deblocking\n");
  store_picture_in_dpb(dec_picture);
  dec_picture=NULL;

  if(img->structure != FRAME)       //if the previous pict is top or bottom field, 
  {
    img->current_slice_nr = 0;
    currSlice->next_header = -8889;
    img->num_dec_mb = 0;
    while ((currSlice->next_header != EOS && currSlice->next_header != SOP))
    {   
      current_header = read_new_slice();
      
      if (current_header == EOS)
        return EOS;
      
      decode_field_slice(img, inp, current_header);
      
      img->current_slice_nr++;  
    }

    //deblocking second field
    DeblockFrame( img, dec_picture->imgY, dec_picture->imgUV ) ;
    store_picture_in_dpb(dec_picture);
    dec_picture=NULL;
  }

  g_new_frame=1;

/*  recfr.yptr = &imgY[0][0];
  recfr.uptr = &imgUV[0][0][0];
  recfr.vptr = &imgUV[1][0][0];
*/
  //! this is always true at the beginning of a frame
  ercStartMB = 0;
  ercSegment = 0;

/* !KS: This needs to be fixed for multiple slices
  
  //! mark the start of the first segment
  ercStartSegment(0, ercSegment, 0 , erc_errorVar);
  //! generate the segments according to the macroblock map
  for(i = 1; i<img->PicSizeInMbs; i++)
  {
    if(img->mb_data[i].ei_flag != img->mb_data[i-1].ei_flag)
    {
      ercStopSegment(i-1, ercSegment, 0, erc_errorVar); //! stop current segment
      
      //! mark current segment as lost or OK
      if(img->mb_data[i-1].ei_flag)
        ercMarkCurrSegmentLost(img->width, erc_errorVar);
      else
        ercMarkCurrSegmentOK(img->width, erc_errorVar);
      
      ercSegment++;  //! next segment
      ercStartSegment(i, ercSegment, 0 , erc_errorVar); //! start new segment
      ercStartMB = i;//! save start MB for this segment 
    }
  }
  //! mark end of the last segent
  ercStopSegment(img->PicSizeInMbs-1, ercSegment, 0, erc_errorVar);
  if(img->mb_data[i-1].ei_flag)
    ercMarkCurrSegmentLost(img->width, erc_errorVar);
  else
    ercMarkCurrSegmentOK(img->width, erc_errorVar);

  //! call the right error concealment function depending on the frame type.
  erc_mvperMB /= img->PicSizeInMbs;

  erc_img = img;
  if(img->type == I_SLICE || img->type == SI_SLICE) // I-frame
    ercConcealIntraFrame(&recfr, img->width, img->height, erc_errorVar);
  else
    ercConcealInterFrame(&recfr, erc_object_list, img->width, img->height, erc_errorVar);
*/
  

  if (img->structure == FRAME)         // buffer mgt. for frame mode
    frame_postprocessing(img, inp);
  else
    field_postprocessing(img, inp);   // reset all interlaced variables

  post_poc( img );                    // POC200301


#ifdef WIN32
  _ftime (&tstruct2);   // end time ms
#else
  ftime (&tstruct2);    // end time ms
#endif
  time( &ltime2 );                                // end time sec
  tmp_time=(ltime2*1000+tstruct2.millitm) - (ltime1*1000+tstruct1.millitm);
  tot_time=tot_time + tmp_time;

#if 0
  if(img->type == I_SLICE) // I picture
    fprintf(stdout,"%3d(I)  %3d %5d %7.4f %7.4f %7.4f %5d\n",
        frame_no, img->ThisPOC, img->qp,snr->snr_y,snr->snr_u,snr->snr_v,tmp_time);
  else if(img->type == P_SLICE) // P pictures
    fprintf(stdout,"%3d(P)  %3d %5d %7.4f %7.4f %7.4f %5d\n",
    frame_no, img->ThisPOC, img->qp,snr->snr_y,snr->snr_u,snr->snr_v,tmp_time);
  else if(img->type == SP_SLICE) // SP pictures
    fprintf(stdout,"%3d(SP) %3d %5d %7.4f %7.4f %7.4f %5d\n",
    frame_no, img->ThisPOC, img->qp,snr->snr_y,snr->snr_u,snr->snr_v,tmp_time);
  else if (img->type == SI_SLICE)
    fprintf(stdout,"%3d(SI) %3d %5d %7.4f %7.4f %7.4f %5d\n",
    frame_no, img->ThisPOC, img->qp,snr->snr_y,snr->snr_u,snr->snr_v,tmp_time);
  else if(!img->disposable_flag) // stored B pictures
    fprintf(stdout,"%3d(BS) %3d %5d %7.4f %7.4f %7.4f %5d\n",
        frame_no, img->ThisPOC, img->qp,snr->snr_y,snr->snr_u,snr->snr_v,tmp_time);
  else // B pictures
    fprintf(stdout,"%3d(B)  %3d %5d %7.4f %7.4f %7.4f %5d\n",
        frame_no, img->ThisPOC, img->qp,snr->snr_y,snr->snr_u,snr->snr_v,tmp_time);
  fflush(stdout);
#endif

  //! TO 19.11.2001 Known Problem: for init_frame we have to know the picture type of the actual frame
  //! in case the first slice of the P-Frame following the I-Frame was lost we decode this P-Frame but 
  //! do not write it because it was assumed to be an I-Frame in init_frame. So we force the decoder to
  //! guess the right picture type. This is a hack a should be removed by the time there is a clean
  //! solution where we do not have to know the picture type for the function init_frame.
  if(img->type == I_SLICE)
    img->type = P_SLICE;
  //! End TO 19.11.2001

  if(img->type == I_SLICE || img->type == SI_SLICE || img->type == P_SLICE || !img->disposable_flag)   // I or P pictures
    img->number++;
  else
    Bframe_ctr++;    // B pictures

  exit_frame(img, inp);

  if (img->structure != FRAME)
  {
    img->height /= 2;
    img->height_cr /= 2;
  }
#ifdef ESLCPP
  img->current_mb_nr = 0xFFFFED99;   // impossible value for debugging, StW
#else
  img->current_mb_nr = -4712;   // impossible value for debugging, StW
#endif
  img->current_slice_nr = 0;

  return (SOP);
}


/*!
 ************************************************************************
 * \brief
 *    Find PSNR for all three components.Compare decoded frame with
 *    the original sequence. Read inp->jumpd frames to reflect frame skipping.
 ************************************************************************
 */
void find_snr(
  struct snr_par  *snr,   //!< pointer to snr parameters
  StorablePicture *p,     //!< picture to be compared
  FILE *p_ref)            //!< filestream to reference YUV file
{
  int i,j;
  int diff_y,diff_u,diff_v;
  int uv;
  int  status;

  // calculate frame number
  // KS: This works for the way, the HHI encoder sets POC
//  frame_no = img->ThisPOC/2;
  
  int  psnrPOC = active_sps->mb_adaptive_frame_field_flag ? (img->toppoc!=img->bottompoc)? img->framepoc /2 :img->framepoc  : img->framepoc/2;
  // calculate frame number
  // KS: This works for the way, the HHI encoder sets POC
  //frame_no = img->ThisPOC/2;

  if (psnrPOC==0 && img->psnr_number)
    img->idr_psnr_number=img->psnr_number + 1;
  img->psnr_number=max(img->psnr_number,img->idr_psnr_number+psnrPOC);

  frame_no = img->idr_psnr_number+psnrPOC;

  rewind(p_ref);

  for (i=0; i<frame_no; i++)
  {
    status = fseek (p_ref, (long) p->size_y* (long) (p->size_x*3/2), SEEK_CUR);
    if (status != 0)
    {
      snprintf(errortext, ET_SIZE, "Error in seeking frame number: %d", frame_no);
      error(errortext, 500);
    }
  }

  for (j=0; j < p->size_y; j++)
    for (i=0; i < p->size_x; i++)
      imgY_ref[j][i]=fgetc(p_ref);

  for (uv=0; uv < 2; uv++)
    for (j=0; j < p->size_y_cr ; j++)
      for (i=0; i < p->size_x_cr; i++)
        imgUV_ref[uv][j][i]=fgetc(p_ref);

  img->quad[0]=0;
  diff_y=0;
  for (j=0; j < p->size_y; ++j)
  {
    for (i=0; i < p->size_x; ++i)
    {
      diff_y += img->quad[abs(p->imgY[j][i]-imgY_ref[j][i])];
    }
  }

  // Chroma
  diff_u=0;
  diff_v=0;

  for (j=0; j < p->size_y_cr; ++j)
  {
    for (i=0; i < p->size_x_cr; ++i)
    {
      diff_u += img->quad[abs(imgUV_ref[0][j][i]-p->imgUV[0][j][i])];
      diff_v += img->quad[abs(imgUV_ref[1][j][i]-p->imgUV[1][j][i])];
    }
  }

/*  if (diff_y == 0)
      diff_y = 1;
  if (diff_u == 0)
      diff_u = 1;
  if (diff_v == 0)
      diff_v = 1; */

  // Collecting SNR statistics
  if (diff_y != 0)
    snr->snr_y=(float)(10*log10(65025*(float)(p->size_x)*(p->size_y)/(float)diff_y));        // luma snr for current frame
  else
    snr->snr_y=0;
  if (diff_u != 0)
    snr->snr_u=(float)(10*log10(65025*(float)(p->size_x)*(p->size_y)/(float)(4*diff_u)));    //  chroma snr for current frame
  else
    snr->snr_u=0;
  if (diff_v != 0)
    snr->snr_v=(float)(10*log10(65025*(float)(p->size_x)*(p->size_y)/(float)(4*diff_v)));    //  chroma snr for current frame
  else
    snr->snr_v=0;

  if (img->number == 0) // first
  {
    snr->snr_y1=(float)(10*log10(65025*(float)(p->size_x)*(p->size_y)/(float)diff_y));       // keep luma snr for first frame
    snr->snr_u1=(float)(10*log10(65025*(float)(p->size_x)*(p->size_y)/(float)(4*diff_u)));   // keep chroma snr for first frame
    snr->snr_v1=(float)(10*log10(65025*(float)(p->size_x)*(p->size_y)/(float)(4*diff_v)));   // keep chroma snr for first frame
    snr->snr_ya=snr->snr_y1;
    snr->snr_ua=snr->snr_u1;
    snr->snr_va=snr->snr_v1;
    
    if (diff_y == 0)   
      snr->snr_ya=50; // need to assign a reasonable large number so avg snr of entire sequece isn't infinite
    if (diff_u == 0)
      snr->snr_ua=50;
    if (diff_v == 0)
      snr->snr_va=50;

  }
/*  else
  {
    snr->snr_ya=(float)(snr->snr_ya*(img->number+Bframe_ctr)+snr->snr_y)/(img->number+Bframe_ctr+1); // average snr chroma for all frames
    snr->snr_ua=(float)(snr->snr_ua*(img->number+Bframe_ctr)+snr->snr_u)/(img->number+Bframe_ctr+1); // average snr luma for all frames
    snr->snr_va=(float)(snr->snr_va*(img->number+Bframe_ctr)+snr->snr_v)/(img->number+Bframe_ctr+1); // average snr luma for all frames
  } */
 else
  {
    snr->snr_ya=(float)(snr->snr_ya*(img->number - 1 + Bframe_ctr)+snr->snr_y)/(img->number+Bframe_ctr); // average snr luma for all frames
    snr->snr_ua=(float)(snr->snr_ua*(img->number - 1 + Bframe_ctr)+snr->snr_u)/(img->number+Bframe_ctr); // average snr chroma for all frames
    snr->snr_va=(float)(snr->snr_va*(img->number - 1 + Bframe_ctr)+snr->snr_v)/(img->number+Bframe_ctr); // average snr chromafor all frames
  }
}


/*!
 ************************************************************************
 * \brief
 *    Interpolation of 1/4 subpixel
 ************************************************************************
 */
void get_block(int ref_frame, StorablePicture **list, int x_pos, int y_pos, struct img_par *img, int block[BLOCK_SIZE][BLOCK_SIZE])
{

  int dx, dy;
  int x, y;
  int i, j;
  int maxold_x,maxold_y;
  int result;
  int pres_x;
  int pres_y; 
  int tmp_res[4][9];
  static const int COEF[6] = {
    1, -5, 20, 20, -5, 1
  };

  dx = x_pos&3;
  dy = y_pos&3;
  x_pos = (x_pos-dx)/4;
  y_pos = (y_pos-dy)/4;

  maxold_x = img->width-1;
  maxold_y = img->height-1;

  if (dec_picture->mb_field[img->current_mb_nr])
    maxold_y = img->height/2 - 1;

  if (dx == 0 && dy == 0) {  /* fullpel position */
    for (j = 0; j < BLOCK_SIZE; j++)
      for (i = 0; i < BLOCK_SIZE; i++)
        block[i][j] = list[ref_frame]->imgY[max(0,min(maxold_y,y_pos+j))][max(0,min(maxold_x,x_pos+i))];
  }
  else { /* other positions */

    if (dy == 0) { /* No vertical interpolation */

      for (j = 0; j < BLOCK_SIZE; j++) {
        for (i = 0; i < BLOCK_SIZE; i++) {
          for (result = 0, x = -2; x < 4; x++)
            result += list[ref_frame]->imgY[max(0,min(maxold_y,y_pos+j))][max(0,min(maxold_x,x_pos+i+x))]*COEF[x+2];
          block[i][j] = max(0, min(255, (result+16)/32));
        }
      }

      if ((dx&1) == 1) {
        for (j = 0; j < BLOCK_SIZE; j++)
          for (i = 0; i < BLOCK_SIZE; i++)
            block[i][j] = (block[i][j] + list[ref_frame]->imgY[max(0,min(maxold_y,y_pos+j))][max(0,min(maxold_x,x_pos+i+dx/2))] +1 )/2;
      }
    }
    else if (dx == 0) {  /* No horizontal interpolation */

      for (j = 0; j < BLOCK_SIZE; j++) {
        for (i = 0; i < BLOCK_SIZE; i++) {
          for (result = 0, y = -2; y < 4; y++)
            result += list[ref_frame]->imgY[max(0,min(maxold_y,y_pos+j+y))][max(0,min(maxold_x,x_pos+i))]*COEF[y+2];
          block[i][j] = max(0, min(255, (result+16)/32));
        }
      }

      if ((dy&1) == 1) {
        for (j = 0; j < BLOCK_SIZE; j++)
          for (i = 0; i < BLOCK_SIZE; i++)
           block[i][j] = (block[i][j] + list[ref_frame]->imgY[max(0,min(maxold_y,y_pos+j+dy/2))][max(0,min(maxold_x,x_pos+i))] +1 )/2;
      }
    }
    else if (dx == 2) {  /* Vertical & horizontal interpolation */

      for (j = -2; j < BLOCK_SIZE+3; j++) {
        for (i = 0; i < BLOCK_SIZE; i++)
          for (tmp_res[i][j+2] = 0, x = -2; x < 4; x++)
            tmp_res[i][j+2] += list[ref_frame]->imgY[max(0,min(maxold_y,y_pos+j))][max(0,min(maxold_x,x_pos+i+x))]*COEF[x+2];
      }

      for (j = 0; j < BLOCK_SIZE; j++) {
        for (i = 0; i < BLOCK_SIZE; i++) {
          for (result = 0, y = -2; y < 4; y++)
            result += tmp_res[i][j+y+2]*COEF[y+2];
          block[i][j] = max(0, min(255, (result+512)/1024));
        } 
      }

      if ((dy&1) == 1) {
        for (j = 0; j < BLOCK_SIZE; j++)
          for (i = 0; i < BLOCK_SIZE; i++)
            block[i][j] = (block[i][j] + max(0, min(255, (tmp_res[i][j+2+dy/2]+16)/32)) +1 )/2;
      }
    }
    else if (dy == 2) {  /* Horizontal & vertical interpolation */

      for (j = 0; j < BLOCK_SIZE; j++) {
        for (i = -2; i < BLOCK_SIZE+3; i++)
          for (tmp_res[j][i+2] = 0, y = -2; y < 4; y++)
            tmp_res[j][i+2] += list[ref_frame]->imgY[max(0,min(maxold_y,y_pos+j+y))][max(0,min(maxold_x,x_pos+i))]*COEF[y+2];
      }

      for (j = 0; j < BLOCK_SIZE; j++) {
        for (i = 0; i < BLOCK_SIZE; i++) {
          for (result = 0, x = -2; x < 4; x++)
            result += tmp_res[j][i+x+2]*COEF[x+2];
          block[i][j] = max(0, min(255, (result+512)/1024));
        }
      }

      if ((dx&1) == 1) {
        for (j = 0; j < BLOCK_SIZE; j++)
          for (i = 0; i < BLOCK_SIZE; i++)
            block[i][j] = (block[i][j] + max(0, min(255, (tmp_res[j][i+2+dx/2]+16)/32))+1)/2;
      }
    }
    else {  /* Diagonal interpolation */

      for (j = 0; j < BLOCK_SIZE; j++) {
        for (i = 0; i < BLOCK_SIZE; i++) {
          pres_y = dy == 1 ? y_pos+j : y_pos+j+1;
          pres_y = max(0,min(maxold_y,pres_y));
          for (result = 0, x = -2; x < 4; x++)
            result += list[ref_frame]->imgY[pres_y][max(0,min(maxold_x,x_pos+i+x))]*COEF[x+2];
          block[i][j] = max(0, min(255, (result+16)/32));
        }
      }

      for (j = 0; j < BLOCK_SIZE; j++) {
        for (i = 0; i < BLOCK_SIZE; i++) {
          pres_x = dx == 1 ? x_pos+i : x_pos+i+1;
          pres_x = max(0,min(maxold_x,pres_x));
          for (result = 0, y = -2; y < 4; y++)
            result += list[ref_frame]->imgY[max(0,min(maxold_y,y_pos+j+y))][pres_x]*COEF[y+2];
          block[i][j] = (block[i][j] + max(0, min(255, (result+16)/32)) +1 ) / 2;
        }
      }

    }
  }

}


static void reorder_lists(int currSliceType, Slice * currSlice)
{

  if ((currSliceType != I_SLICE)&&(currSliceType != SI_SLICE))
  {
    if (currSlice->ref_pic_list_reordering_flag_l0)
    {
      reorder_ref_pic_list(listX[0], &listXsize[0], 
                           img->num_ref_idx_l0_active - 1, 
                           currSlice->remapping_of_pic_nums_idc_l0, 
                           currSlice->abs_diff_pic_num_minus1_l0, 
                           currSlice->long_term_pic_idx_l0);
    }
    if (NULL == listX[0][img->num_ref_idx_l0_active-1])
    {
      error("number of entries in list 0 smaller than num_ref_idx_l0_active_minus1",500);
    }
    // that's a definition
    listXsize[0] = img->num_ref_idx_l0_active;
  }
  if (currSliceType == B_SLICE)
  {
    if (currSlice->ref_pic_list_reordering_flag_l1)
    {
      reorder_ref_pic_list(listX[1], &listXsize[1], 
                           img->num_ref_idx_l1_active - 1, 
                           currSlice->remapping_of_pic_nums_idc_l1, 
                           currSlice->abs_diff_pic_num_minus1_l1, 
                           currSlice->long_term_pic_idx_l1);
    }
    if (NULL == listX[1][img->num_ref_idx_l1_active-1])
    {
      error("number of entries in list 1 smaller than num_ref_idx_l1_active_minus1",500);
    }
    // that's a definition
    listXsize[1] = img->num_ref_idx_l1_active;
  }

  free_ref_pic_list_reordering_buffer(currSlice);
}


/*!
 ************************************************************************
 * \brief
 *    initialize ref_pic_num array
 ************************************************************************
 */
void set_ref_pic_num()
{
  int i,j;

  for (i=0;i<listXsize[LIST_0];i++)
  {
    dec_picture->ref_pic_num[LIST_0][i]=listX[LIST_0][i]->poc * 2 + ((listX[LIST_0][i]->structure==BOTTOM_FIELD)?1:0) ; 
  }

  for (i=0;i<listXsize[LIST_1];i++)
  {
    dec_picture->ref_pic_num[LIST_1][i]=listX[LIST_1][i]->poc  *2 + ((listX[LIST_1][i]->structure==BOTTOM_FIELD)?1:0);
  }

  if (img->structure==FRAME)
    for (j=2;j<6;j++)
      for (i=0;i<listXsize[j];i++)
      {
        dec_picture->ref_pic_num[j][i] = listX[j][i]->poc * 2 + ((listX[j][i]->structure==BOTTOM_FIELD)?1:0);
      }
}


/*!
 ************************************************************************
 * \brief
 *    Reads new slice from bit_stream
 ************************************************************************
 */
int read_new_slice()
{
  NALU_t *nalu = AllocNALU(MAX_CODED_FRAME_SIZE);
  int current_header;
  int ret;
  int BitsUsedByHeader;
  Slice *currSlice = img->currentSlice;
  Bitstream *currStream;
  int newframe;

  int slice_id_a, slice_id_b, slice_id_c;
  int redundant_pic_cnt_b, redundant_pic_cnt_c;
  long ftell_position, expected_slice_type;
  
//  int i;
  expected_slice_type = NALU_TYPE_DPA;

  while (1)
  {
    //ftell_position = ftell(bits);//boy 

    if (input->FileFormat == PAR_OF_ANNEXB)
      ret=GetAnnexbNALU (nalu);
    else
      ret=GetRTPNALU (nalu);

    NALUtoRBSP(nalu);
//    printf ("nalu->len %d\n", nalu->len);
    
    if (ret < 0)
      printf ("Error while getting the NALU in file format %s, exit\n", input->FileFormat==PAR_OF_ANNEXB?"Annex B":"RTP");
    if (ret == 0)
    {
//      printf ("read_new_slice: returning %s\n", "EOS");
      if(expected_slice_type != NALU_TYPE_DPA)
      {
        /* oops... we found the next slice, go back! */
        //fseek(bits, ftell_position, SEEK_SET);//boy
        FreeNALU(nalu);
        return current_header;
      }
      else
        return EOS;
    }
//    cout<< "XXXXXXXXXXXXXXXXXXX" <<endl;
    // Got a NALU
    if (nalu->forbidden_bit)
    {
      printf ("Found NALU w/ forbidden_bit set, bit error?  Let's try...\n");
    }

    //    cout<< "XXXXXXXXXXXXXXXXXXX   "<< nalu->nal_unit_type <<endl;
    switch (nalu->nal_unit_type)
    {
      case NALU_TYPE_SLICE:
      case NALU_TYPE_IDR:
        
#ifdef LT_MODEL
        if(nalu->nal_unit_type == NALU_TYPE_SLICE)     
          flag_3bit_frame_type = 1;
        else if (nalu->nal_unit_type == NALU_TYPE_IDR)
          flag_3bit_frame_type = 0;
#endif 
        img->idr_flag = (nalu->nal_unit_type == NALU_TYPE_IDR);
        img->nal_reference_idc = nalu->nal_reference_idc;
        img->disposable_flag = (nalu->nal_reference_idc == NALU_PRIORITY_DISPOSABLE);
        currSlice->dp_mode = PAR_DP_1;
        currSlice->max_part_nr = 1;
        currSlice->ei_flag = 0;
        currStream = currSlice->partArr[0].bitstream;
        currStream->ei_flag = 0;
        currStream->frame_bitoffset = currStream->read_len = 0;
        memcpy (currStream->streamBuffer, &nalu->buf[1], nalu->len-1);
        currStream->code_len = currStream->bitstream_length = RBSPtoSODB(currStream->streamBuffer, nalu->len-1);

        // Some syntax of the Slice Header depends on the parameter set, which depends on
        // the parameter set ID of the SLice header.  Hence, read the pic_parameter_set_id
        // of the slice header first, then setup the active parameter sets, and then read
        // the rest of the slice header
        BitsUsedByHeader = FirstPartOfSliceHeader();
        UseParameterSet (currSlice->pic_parameter_set_id);
        BitsUsedByHeader+= RestOfSliceHeader ();

        FmoInit (active_pps, active_sps);

        init_lists(img->type, img->currentSlice->structure);
        reorder_lists (img->type, img->currentSlice);

/*        if (img->frame_num==1) // write a reference list
        {
          count ++;
          if (count==1)
            for (i=0; i<listXsize[0]; i++)
              write_picture(listX[0][i], p_out2);
        }
*/
        //if (img->MbaffFrameFlag)
        if (img->structure==FRAME)
        {
          init_mbaff_lists();
        }

        if (img->currentSlice->structure!=0)
        {
          img->height /=2 ;
          img->height_cr /=2;
        }

        // From here on, active_sps, active_pps and the slice header are valid
        if (img->MbaffFrameFlag)
          img->current_mb_nr = currSlice->start_mb_nr << 1;
        else
          img->current_mb_nr = currSlice->start_mb_nr;

        if (img->tr_old != img->ThisPOC)
        {
          newframe=1;
          img->tr_old = img->ThisPOC;
        }
        else
          newframe = 0;
        if (newframe)
          current_header = SOP;
        else
          current_header = SOS;

        if(img->structure != img->structure_old)        
          newframe |= 1;

        img->structure_old = img->structure; 
//! new stuff StW
        if(newframe || g_new_frame)
        {
          current_header = SOP;
          g_new_frame=0;
        }
        else
          current_header = SOS;

        if (active_pps->entropy_coding_mode_flag)
        {
          int ByteStartPosition = currStream->frame_bitoffset/8;
          if (currStream->frame_bitoffset%8 != 0) 
          {
            ByteStartPosition++;
          }
          arideco_start_decoding (&currSlice->partArr[0].de_cabac, currStream->streamBuffer, ByteStartPosition, &currStream->read_len, img->type);
        }
// printf ("read_new_slice: returning %s\n", current_header == SOP?"SOP":"SOS");
        FreeNALU(nalu);
        return current_header;
        break;
      case NALU_TYPE_DPA:
        //! The state machine here should follow the same ideas as the old readSliceRTP()
        //! basically:
        //! work on DPA (as above)
        //! read and process all following SEI/SPS/PPS/PD/Filler NALUs
        //! if next video NALU is dpB, 
        //!   then read and check whether it belongs to DPA, if yes, use it
        //! else
        //!   ;   // nothing
        //! read and process all following SEI/SPS/PPS/PD/Filler NALUs
        //! if next video NALU is dpC
        //!   then read and check whether it belongs to DPA (and DPB, if present), if yes, use it, done
        //! else
        //!   use the DPA (and the DPB if present)

        /* 
            LC: inserting the code related to DP processing, mainly copying some of the parts
            related to NALU_TYPE_SLICE, NALU_TYPE_IDR.
        */

        if(expected_slice_type != NALU_TYPE_DPA)
        {
          /* oops... we found the next slice, go back! */
          //fseek(bits, ftell_position, SEEK_SET);//boy
          FreeNALU(nalu);
          return current_header;
        }

        img->idr_flag          = (nalu->nal_unit_type == NALU_TYPE_IDR);
        img->nal_reference_idc = nalu->nal_reference_idc;
        img->disposable_flag   = (nalu->nal_reference_idc == NALU_PRIORITY_DISPOSABLE);
        currSlice->dp_mode     = PAR_DP_3;
        currSlice->max_part_nr = 3;
        currSlice->ei_flag     = 0;
        currStream             = currSlice->partArr[0].bitstream;
        currStream->ei_flag    = 0;
        currStream->frame_bitoffset = currStream->read_len = 0;
        memcpy (currStream->streamBuffer, &nalu->buf[1], nalu->len-1);
        currStream->code_len = currStream->bitstream_length = RBSPtoSODB(currStream->streamBuffer, nalu->len-1);
        
        BitsUsedByHeader     = FirstPartOfSliceHeader();
        UseParameterSet (currSlice->pic_parameter_set_id);
        BitsUsedByHeader    += RestOfSliceHeader ();
        
        FmoInit (active_pps, active_sps);
        
        init_lists(img->type, img->currentSlice->structure);
        reorder_lists (img->type, img->currentSlice);
        
        if (img->MbaffFrameFlag)
        {
          init_mbaff_lists();
        }

        if (img->currentSlice->structure!=0)
        {
          img->height /=2 ;
          img->height_cr /=2;
        }

        // From here on, active_sps, active_pps and the slice header are valid
        if (img->MbaffFrameFlag)
          img->current_mb_nr = currSlice->start_mb_nr << 1;
        else
          img->current_mb_nr = currSlice->start_mb_nr;

        if (img->tr_old != img->ThisPOC)
        {
          newframe=1;
          img->tr_old = img->ThisPOC;
        }
        else
          newframe = 0;
        if (newframe)
          current_header = SOP;
        else
          current_header = SOS;

        if(img->structure != img->structure_old)        
          newframe |= 1;

        img->structure_old = img->structure; 
//! new stuff StW
        if(newframe || g_new_frame)
        {
          current_header = SOP;
          g_new_frame=0;
        }
        else
          current_header = SOS;

        /* 
           LC:
              Now I need to read the slice ID, which depends on the value of 
              redundant_pic_cnt_present_flag (pag.49). 
        */

        slice_id_a  = ue_v("NALU:SLICE_A slice_idr", currStream);
        if (active_pps->entropy_coding_mode_flag)
        {
          int ByteStartPosition = currStream->frame_bitoffset/8;
          if (currStream->frame_bitoffset%8 != 0) 
          {
            ByteStartPosition++;
          }
          arideco_start_decoding (&currSlice->partArr[0].de_cabac, currStream->streamBuffer, ByteStartPosition, &currStream->read_len, img->type);
        }
// printf ("read_new_slice: returning %s\n", current_header == SOP?"SOP":"SOS");
        break;
      case NALU_TYPE_DPB:
        /* LC: inserting the code related to DP processing */

        currStream             = currSlice->partArr[1].bitstream;
        currStream->ei_flag    = 0;
        currStream->frame_bitoffset = currStream->read_len = 0;
        memcpy (currStream->streamBuffer, &nalu->buf[1], nalu->len-1);
        currStream->code_len = currStream->bitstream_length = RBSPtoSODB(currStream->streamBuffer, nalu->len-1);

        slice_id_b  = ue_v("NALU:SLICE_B slice_idr", currStream);
        if (active_pps->redundant_pic_cnt_present_flag)
          redundant_pic_cnt_b = ue_v("NALU:SLICE_B redudand_pic_cnt", currStream);
        else
          redundant_pic_cnt_b = 0;
        
        /*  LC: Initializing CABAC for the current data stream. */

        if (active_pps->entropy_coding_mode_flag)
        {
          int ByteStartPosition = currStream->frame_bitoffset/8;
          if (currStream->frame_bitoffset % 8 != 0) 
            ByteStartPosition++;
          
          arideco_start_decoding (&currSlice->partArr[1].de_cabac, currStream->streamBuffer, 
            ByteStartPosition, &currStream->read_len, img->type);
          
        }

        /* LC: resilience code to be inserted */
        /*         FreeNALU(nalu); */
        /*         return current_header; */

        break;
      case NALU_TYPE_DPC:
        /* LC: inserting the code related to DP processing */
        currStream             = currSlice->partArr[2].bitstream;
        currStream->ei_flag    = 0;
        currStream->frame_bitoffset = currStream->read_len = 0;
        memcpy (currStream->streamBuffer, &nalu->buf[1], nalu->len-1);
        currStream->code_len = currStream->bitstream_length = RBSPtoSODB(currStream->streamBuffer, nalu->len-1);
        
        slice_id_c  = ue_v("NALU:SLICE_C slice_idr", currStream);
        if (active_pps->redundant_pic_cnt_present_flag)
          redundant_pic_cnt_c = ue_v("NALU:SLICE_C redudand_pic_cnt", currStream);
        else
          redundant_pic_cnt_c = 0;
        
        /* LC: Initializing CABAC for the current data stream. */

        if (active_pps->entropy_coding_mode_flag)
        {
          int ByteStartPosition = currStream->frame_bitoffset/8;
          if (currStream->frame_bitoffset % 8 != 0) 
            ByteStartPosition++;
          
          arideco_start_decoding (&currSlice->partArr[2].de_cabac, currStream->streamBuffer, 
            ByteStartPosition, &currStream->read_len, img->type);
        }

        /* LC: resilience code to be inserted */

        FreeNALU(nalu);
        return current_header;

        break;
      case NALU_TYPE_SEI:
        printf ("read_new_slice: Found NALU_TYPE_SEI, len %d\n", nalu->len);
        InterpretSEIMessage(nalu->buf,nalu->len,img);
        break;
      case NALU_TYPE_PPS:
        ProcessPPS(nalu);
        break;
      case NALU_TYPE_SPS:
        ProcessSPS(nalu);
        break;
      case NALU_TYPE_PD:
//        printf ("read_new_slice: Found 'Access Unit Delimiter' NAL unit, len %d, ignored\n", nalu->len);
        break;
      case NALU_TYPE_EOSEQ:
//        printf ("read_new_slice: Found 'End of Sequence' NAL unit, len %d, ignored\n", nalu->len);
        break;
      case NALU_TYPE_EOSTREAM:
//        printf ("read_new_slice: Found 'End of Stream' NAL unit, len %d, ignored\n", nalu->len);
        break;
      case NALU_TYPE_FILL:
        printf ("read_new_slice: Found NALU_TYPE_FILL, len %d\n", nalu->len);
        printf ("Skipping these filling bits, proceeding w/ next NALU\n");
        break;
      default:
        printf ("Found NALU type %d, len %d undefined, ignore NALU, moving on\n", nalu->nal_unit_type, nalu->len);
    }
  }

  FreeNALU(nalu);

  return  current_header;
}


/*!
 ************************************************************************
 * \brief
 *    Initializes the parameters for a new frame
 ************************************************************************
 */
void init_frame(struct img_par *img, struct inp_par *inp)
{
  int i,k,l;

  if (dec_picture)
  {
    // this may only happen on slice loss
    store_picture_in_dpb(dec_picture);
  }

  dec_picture = alloc_storable_picture (FRAME, img->width, img->height, img->width_cr, img->height_cr);
  dec_picture->poc=img->framepoc;
  dec_picture->top_poc=img->toppoc;
  dec_picture->bottom_poc=img->bottompoc;
  dec_picture->pic_num = img->frame_num;
  dec_picture->coded_frame = (img->structure==FRAME);
//  dec_picture->mb_adaptive_frame_field_flag = img->MbaffFrameFlag;

   
  img->current_slice_nr=0;

  if (img->type > SI_SLICE)
  {
    set_ec_flag(SE_PTYPE);
    img->type = P_SLICE;  // concealed element
  }

  // allocate memory for frame buffers
  if (img->idr_flag) 
  {
    init_global_buffers(inp, img); 
  }

  // CAVLC init
  for (i=0;i < (int)img->PicSizeInMbs; i++)
    for (k=0;k<4;k++)
      for (l=0;l<6;l++)
        img->nz_coeff[i][k][l]=-1;  // CAVLC

  if(img->constrained_intra_pred_flag)
  {
    for (i=0; i<(int)img->PicSizeInMbs; i++)
    {
      img->intra_block[i] = 1;
    }
  }

  // WYK: Oct. 8, 2001. Set the slice_nr member of each MB to -1, to ensure correct when packet loss occurs
  // TO set Macroblock Map (mark all MBs as 'have to be concealed')
  for(i=0; i<(int)img->PicSizeInMbs; i++)
  {
    img->mb_data[i].slice_nr = -1; 
    img->mb_data[i].ei_flag = 1;
  }
}

/*!
 ************************************************************************
 * \brief
 *    exit a frame
 ************************************************************************
 */
void exit_frame(struct img_par *img, struct inp_par *inp)
{
  img->pre_frame_num = img->frame_num;
}

/*!
 ************************************************************************
 * \brief
 *    write the encoding mode and motion vectors of current 
 *    MB to the buffer of the error concealment module.
 ************************************************************************
 */

void ercWriteMBMODEandMV(struct img_par *img,struct inp_par *inp)
{
  extern objectBuffer_t *erc_object_list;
  int i, ii, jj, currMBNum = img->current_mb_nr;
  int mbx = xPosMB(currMBNum,img->width), mby = yPosMB(currMBNum,img->width);
  objectBuffer_t *currRegion, *pRegion;
  Macroblock *currMB = &img->mb_data[currMBNum];
  int***  mv;

  currRegion = erc_object_list + (currMBNum<<2);

  if(img->type != B_SLICE) //non-B frame
  {
    for (i=0; i<4; i++)
    {
      pRegion             = currRegion + i;
      pRegion->regionMode = (currMB->mb_type  ==I16MB  ? REGMODE_INTRA      :
                             currMB->b8mode[i]==IBLOCK ? REGMODE_INTRA_8x8  :
                             currMB->b8mode[i]==0      ? REGMODE_INTER_COPY :
                             currMB->b8mode[i]==1      ? REGMODE_INTER_PRED : REGMODE_INTER_PRED_8x8);
      if (currMB->b8mode[i]==0 || currMB->b8mode[i]==IBLOCK)  // INTRA OR COPY
      {
        pRegion->mv[0]    = 0;
        pRegion->mv[1]    = 0;
        pRegion->mv[2]    = 0;
      }
      else
      {
        if (currMB->b8mode[i]>=5 && currMB->b8mode[i]<=7)  // SMALL BLOCKS
        {
          ii              = 4*mbx + (i%2)*2 + BLOCK_SIZE;
          jj              = 4*mby + (i/2)*2;
          pRegion->mv[0]  = (dec_picture->mv[LIST_0][ii][jj][0] + dec_picture->mv[LIST_0][ii+1][jj][0] + dec_picture->mv[LIST_0][ii][jj+1][0] + dec_picture->mv[LIST_0][ii+1][jj+1][0] + 2)/4;
          pRegion->mv[1]  = (dec_picture->mv[LIST_0][ii][jj][1] + dec_picture->mv[LIST_0][ii+1][jj][1] + dec_picture->mv[LIST_0][ii][jj+1][1] + dec_picture->mv[LIST_0][ii+1][jj+1][1] + 2)/4;
        }
        else // 16x16, 16x8, 8x16, 8x8
        {
          pRegion->mv[0]  = dec_picture->mv[LIST_0][4*mbx+(i%2)*2+BLOCK_SIZE][4*mby+(i/2)*2][0];
          pRegion->mv[1]  = dec_picture->mv[LIST_0][4*mbx+(i%2)*2+BLOCK_SIZE][4*mby+(i/2)*2][1];
        }
        erc_mvperMB      += mabs(pRegion->mv[0]) + mabs(pRegion->mv[1]);
        pRegion->mv[2]    = refFrArr[4*mby+(i/2)*2][4*mbx+(i%2)*2];
      }
    }
  }
  else  //B-frame
  {
    for (i=0; i<4; i++)
    {
      ii                  = 4*mbx + (i%2)*2 + BLOCK_SIZE;
      jj                  = 4*mby + (i/2)*2;
      pRegion             = currRegion + i;
      pRegion->regionMode = (currMB->mb_type  ==I16MB  ? REGMODE_INTRA      :
                             currMB->b8mode[i]==IBLOCK ? REGMODE_INTRA_8x8  : REGMODE_INTER_PRED_8x8);
      if (currMB->mb_type==I16MB || currMB->b8mode[i]==IBLOCK)  // INTRA
      {
        pRegion->mv[0]    = 0;
        pRegion->mv[1]    = 0;
        pRegion->mv[2]    = 0;
      }
      else
      {
        mv                = (currMB->b8mode[i]==0 && currMB->b8pdir[i]==2 ? 0/*img->dbMV*/ : currMB->b8pdir[i]==1 ? dec_picture->mv[LIST_1] : dec_picture->mv[LIST_0]);
        pRegion->mv[0]    = (mv[ii][jj][0] + mv[ii+1][jj][0] + mv[ii][jj+1][0] + mv[ii+1][jj+1][0] + 2)/4;
        pRegion->mv[1]    = (mv[ii][jj][1] + mv[ii+1][jj][1] + mv[ii][jj+1][1] + mv[ii+1][jj+1][1] + 2)/4;
        erc_mvperMB      += mabs(pRegion->mv[0]) + mabs(pRegion->mv[1]);
        if (currMB->b8pdir[i]==0 || (currMB->b8pdir[i]==2 && currMB->b8mode[i]!=0)) // forward or bidirect
        {
          pRegion->mv[2]  = (dec_picture->ref_idx[LIST_1][ii-4][jj]-1);
          ///???? is it right, not only "img->fw_refFrArr[jj][ii-4]"
        }
        else
        {
          pRegion->mv[2]  = 0;
        }
      }
    }
  }
}


/*!
 ************************************************************************
 * \brief
 *    decodes one slice
 ************************************************************************
 */
void decode_one_slice(struct img_par *img,struct inp_par *inp)
{

  Boolean end_of_slice = FALSE;
  int read_flag;
  img->cod_counter=-1;

  set_ref_pic_num();

  //reset_ec_flags();

  while (end_of_slice == FALSE) // loop over macroblocks
  {

#if TRACE
  fprintf(p_trace,"\n*********** POC: %i (I/P) MB: %i Slice: %i Type %d **********\n", img->ThisPOC, img->current_mb_nr, img->current_slice_nr, img->type);
#endif

    // Initializes the current macroblock
    start_macroblock(img,inp, img->current_mb_nr);
    // Get the syntax elements from the NAL
    read_flag = read_one_macroblock(img,inp);

    decode_one_macroblock(img,inp);

    if(img->MbaffFrameFlag && dec_picture->mb_field[img->current_mb_nr])
    {
      img->num_ref_idx_l0_active >>= 1;
      img->num_ref_idx_l1_active >>= 1;
    }

    //ercWriteMBMODEandMV(img,inp);
#ifdef ESLCPP
    end_of_slice=(Boolean)exit_macroblock(img,inp,(!img->MbaffFrameFlag||img->current_mb_nr%2));
#else
    end_of_slice=exit_macroblock(img,inp,(!img->MbaffFrameFlag||img->current_mb_nr%2));
#endif
  }
  //reset_ec_flags();

}


void decode_frame_slice(struct img_par *img,struct inp_par *inp, int current_header)
{
  Slice *currSlice = img->currentSlice;

  if (active_pps->entropy_coding_mode_flag)
  {
    init_contexts (img);
    cabac_new_slice();
  }

  // init new frame
  if (current_header == SOP)
    init_frame(img, inp);

  if ( (img->weighted_bipred_idc > 0  && (img->type == B_SLICE)) || (img->weighted_pred_flag && img->type !=I_SLICE))
    fill_wp_params(img);

  /*
  if (current_header == SOP)
  {
    if (img->number == 0) 
      ercInit(img->width, img->height, 1);
    // reset all variables of the error concealmnet instance before decoding of every frame.
    // here the third parameter should, if perfectly, be equal to the number of slices per frame.
    // using little value is ok, the code will alloc more memory if the slice number is larger
    ercReset(erc_errorVar, img->PicSizeInMbs, img->PicSizeInMbs, img->width);
    erc_mvperMB = 0;
  }
  */

  // decode main slice information
  if ((current_header == SOP || current_header == SOS) && currSlice->ei_flag == 0)
    decode_one_slice(img,inp);
    
  // setMB-Nr in case this slice was lost
//  if(currSlice->ei_flag)  
//    img->current_mb_nr = currSlice->last_mb_nr + 1;

//! This code doesn't work with FMO or a slice-lossy environment!
//! StW NEEDS FIXING
  /*if(img->current_mb_nr != img->PicSizeInMbs)
    currSlice->next_header = SOS; */
}



void decode_field_slice(struct img_par *img,struct inp_par *inp, int current_header)
{
  Slice *currSlice = img->currentSlice;

  if (active_pps->entropy_coding_mode_flag)
  {
    init_contexts (img);
    cabac_new_slice();
  }
  
  // init new frame
  if (current_header == SOP)
  {
    if (img->structure == TOP_FIELD)
      init_top(img, inp); // set up field buffer in this function
    else
    {
      init_bottom(img, inp);
    }
  }
  
  if ( (img->weighted_bipred_idc > 0  && (img->type == B_SLICE)) || (img->weighted_pred_flag && img->type !=I_SLICE))
    fill_wp_params(img);
 
  /*
  if (current_header == SOP)
  {
    if (img->number == 0) 
      ercInit(img->width, 2*img->height, 1);
    // reset all variables of the error concealmnet instance before decoding of every frame.
    // here the third parameter should, if perfectly, be equal to the number of slices per frame.
    // using little value is ok, the code will alloc more memory if the slice number is larger
    ercReset(erc_errorVar, img->PicSizeInMbs, img->PicSizeInMbs, img->width);
    erc_mvperMB = 0;
  }
  */

  // decode main slice information
  if ((current_header == SOP || current_header == SOS) && currSlice->ei_flag == 0)
    decode_one_slice(img,inp);
  
  // setMB-Nr in case this slice was lost
//  if(currSlice->ei_flag)  
//    img->current_mb_nr = currSlice->last_mb_nr + 1;

//! This code doesn't work with FMO or a slice lossy environment or out-of-order slices
  /*if(img->current_mb_nr != img->PicSizeInMbs)
    currSlice->next_header = SOS; */
}

/*!
 ************************************************************************
 * \brief
 *    Initializes the parameters for a new field
 ************************************************************************
 */
void init_top(struct img_par *img, struct inp_par *inp)
{
  int i;

  if (dec_picture)
  {
    // this may only happen on slice loss
    store_picture_in_dpb(dec_picture);
  }
  dec_picture = alloc_storable_picture (TOP_FIELD, img->width, img->height, img->width_cr, img->height_cr);
  dec_picture->poc=img->toppoc;
  dec_picture->top_poc=img->toppoc;
  dec_picture->bottom_poc=img->bottompoc;
  dec_picture->pic_num = img->frame_num;
  dec_picture->coded_frame = (img->structure==FRAME);
  dec_picture->mb_adaptive_frame_field_flag = img->MbaffFrameFlag;

  img->number *= 2;
  img->current_slice_nr=0;

  img->mb_y = img->mb_x = 0;
  img->block_y = img->pix_y = img->pix_c_y = 0; // define vertical positions
  img->block_x = img->pix_x = img->pix_c_x = 0; // define horizontal positions

  if (img->type > SI_SLICE)
  {
    set_ec_flag(SE_PTYPE);
    img->type = P_SLICE;  // concealed element
  }

  // allocate memory for frame buffers
  if (img->idr_flag) 
  {
    init_global_buffers(inp, img); 
  }

  if(img->constrained_intra_pred_flag)
  {
    for (i=0; i<(int)img->PicSizeInMbs; i++)
    {
      img->intra_block[i] = 1;
    }
  }

  // WYK: Oct. 8, 2001. Set the slice_nr member of each MB to -1, to ensure correct when packet loss occurs
  for(i=0; i<(int)img->PicSizeInMbs; i++)
    img->mb_data[i].slice_nr = -1;

}

/*!
 ************************************************************************
 * \brief
 *    Initializes the parameters for a new field
 ************************************************************************
 */
void init_bottom(struct img_par *img, struct inp_par *inp)
{
  int i;

  if (dec_picture)
  {
    // this may only happen on slice loss
    store_picture_in_dpb(dec_picture);
  }
  dec_picture = alloc_storable_picture (BOTTOM_FIELD, img->width, img->height, img->width_cr, img->height_cr);
  dec_picture->poc=img->bottompoc;
  dec_picture->top_poc=img->toppoc;
  dec_picture->bottom_poc=img->bottompoc;
  dec_picture->pic_num = img->frame_num;
  dec_picture->coded_frame = (img->structure==FRAME);
  dec_picture->mb_adaptive_frame_field_flag = img->MbaffFrameFlag;

  img->number++;
  img->current_slice_nr=0;

  img->mb_y = img->mb_x = 0;
  img->block_y = img->pix_y = img->pix_c_y = 0; // define vertical positions
  img->block_x = img->pix_x = img->pix_c_x = 0; // define horizontal positions


  if (img->type > SI_SLICE)
  {
    set_ec_flag(SE_PTYPE);
    img->type = P_SLICE;  // concealed element
  }

  // allocate memory for frame buffers
  if (img->idr_flag) 
  {
    init_global_buffers(inp, img); 
  }

  if(img->constrained_intra_pred_flag)
  {
    for (i=0; i<(int)img->PicSizeInMbs; i++)
    {
      img->intra_block[i] = 1;
    }
  }

  // WYK: Oct. 8, 2001. Set the slice_nr member of each MB to -1, to ensure correct when packet loss occurs
  for(i=0; i<(int)img->PicSizeInMbs; i++)
    img->mb_data[i].slice_nr = -1;

}

/*!
 ************************************************************************
 * \brief
 *    Prepare field and frame buffer after frame decoding
 ************************************************************************
 */
void frame_postprocessing(struct img_par *img, struct inp_par *inp)
{
}

/*!
 ************************************************************************
 * \brief
 *    Prepare field and frame buffer after field decoding
 ************************************************************************
 */
void field_postprocessing(struct img_par *img, struct inp_par *inp)
{
  img->height *= 2;
  img->height_cr *= 2;
  img->number /= 2;
}



void reset_wp_params(struct img_par *img)
{
  int i,comp;
  int log_weight_denom;

  for (i=0; i<MAX_REFERENCE_PICTURES; i++)
  {
    for (comp=0; comp<3; comp++)
    {
      log_weight_denom = (comp == 0) ? img->luma_log2_weight_denom : img->chroma_log2_weight_denom;
      img->wp_weight[0][i][comp] = 1<<log_weight_denom;
      img->wp_weight[1][i][comp] = 1<<log_weight_denom;
    }
  }
}


void fill_wp_params(struct img_par *img)
{
  int i, j;
  int comp;
  int log_weight_denom;
  int p0, pt;
  int bframe = (img->type==B_SLICE);
  int max_bwd_ref, max_fwd_ref;
  int x,z;

  max_fwd_ref = img->num_ref_idx_l0_active;
  max_bwd_ref = img->num_ref_idx_l1_active;

  if (img->weighted_bipred_idc == 2 && bframe)
  {
    img->luma_log2_weight_denom = 5;
    img->chroma_log2_weight_denom = 5;
    img->wp_round_luma = 16;
    img->wp_round_chroma = 16;

    for (i=0; i<MAX_REFERENCE_PICTURES; i++)
    {
      for (comp=0; comp<3; comp++)
      {
        log_weight_denom = (comp == 0) ? img->luma_log2_weight_denom : img->chroma_log2_weight_denom;
        img->wp_weight[0][i][comp] = 1<<log_weight_denom;
        img->wp_weight[1][i][comp] = 1<<log_weight_denom;
        img->wp_offset[0][i][comp] = 0;
        img->wp_offset[1][i][comp] = 0;
      }
    }
  }

  if (bframe)
  {
    for (i=0; i<max_fwd_ref; i++)
    {
      for (j=0; j<max_bwd_ref; j++)
      {
        for (comp = 0; comp<3; comp++)
        {
          log_weight_denom = (comp == 0) ? img->luma_log2_weight_denom : img->chroma_log2_weight_denom;
          if (img->weighted_bipred_idc == 1)
          {
            img->wbp_weight[0][i][j][comp] =  img->wp_weight[0][i][comp];
            img->wbp_weight[1][i][j][comp] =  img->wp_weight[1][j][comp];
          }
          else if (img->weighted_bipred_idc == 2)
          {
            pt = listX[LIST_1][j]->poc - listX[LIST_0][i]->poc;
            if (pt == 0 || listX[LIST_1][j]->is_long_term || listX[LIST_0][i]->is_long_term)
            {
              img->wbp_weight[0][i][j][comp] =   32;
              img->wbp_weight[1][i][j][comp] =   32;
            }
            else
            {
              p0 = img->ThisPOC - listX[LIST_0][i]->poc;

              x = (16384 + (pt>>1))/pt;
              z = Clip3(-1024, 1023, (x*p0 + 32 )>>6);
              img->wbp_weight[1][i][j][comp] = z >> 2;
              img->wbp_weight[0][i][j][comp] = 64 - img->wbp_weight[1][i][j][comp];
              if (img->wbp_weight[1][i][j][comp] < -64 || img->wbp_weight[1][i][j][comp] > 128)
              {
                img->wbp_weight[1][i][j][comp] = 32;
                img->wbp_weight[0][i][j][comp] = 32;
                img->wp_offset[0][i][comp] = 0;
                img->wp_offset[1][i][comp] = 0;
              }
            }
          }
        }
     }
   }
 }
}
