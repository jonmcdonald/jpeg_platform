/*!
 ************************************************************************
 * \file output.c
 *
 * \brief
 *    Output an image and Trance support
 *
 * \author
 *    Main contributors (see contributors.h for copyright, address and affiliation details)
 *    - Karsten Suehring               <suehring@hhi.de>
 ************************************************************************
 */


#include "contributors.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "global.h"
#include "mbuffer.h"
#include "image.h"

//cylin : initiator wrapper & H264 handshaking variable 
#include "global_sc.h"
sc_event event_transaction_req;
sc_event event_transaction_end;
unsigned char *tlmDataBuffer = 0;
unsigned int  tlmDataLenByte = 0;
unsigned int tlmDataRows;
unsigned int tlmDataColumns;
sc_event      event_frame_done;
bool          glb_frame_done;
sc_event event_frame_ready;

//-----

extern int total_frame;

FrameStore* out_buffer;
static int ppm_idx=0;

void convertYCbCr2RGB( int Yin,int Cbin, int Crin, int *CurColor)
{
    float Yf = ( float) Yin;
    float Cbf = ( float) Cbin;
    float Crf = ( float)  Crin;
    float Rf,Gf,Bf;

    Rf = 1.164f*( Yf - 16.0f)                        + 1.596f*(Crf-128.0f);
    Gf = 1.164f*( Yf - 16.0f) - 0.391f*( Cbf-128.0f) - 0.813f*(Crf-128.0f);
    Bf = 1.164f*( Yf - 16.0f) + 2.018* ( Cbf-128.0f) ;

    CurColor[0] = (Rf < 0 ) ? 0 : ( Rf >255) ? 0xFF : (( int)Rf);
    CurColor[1] = (Gf < 0 ) ? 0 : ( Gf >255) ? 0xFF : (( int)Gf);
    CurColor[2] = (Bf < 0 ) ? 0 : ( Bf >255) ? 0xFF : (( int)Bf);

    //if(DEBUG)
    //{
    //        printf("r=%d, g=%d, b=%d, rf=%f, gf=%f,bf=%f\n",CurColor[ 0], CurColor[ 1], CurColor[ 2],Rf,Gf,Bf);
    //}

    
}

/*!
 ************************************************************************
 * \brief
 *    Writes out a storable picture
 * \param p
 *    Picture to be written
 * \param p_out
 *    Output file
 ************************************************************************
 */
void 
write_picture_tlm(StorablePicture *p, FILE *p_out)
{


  int i,j;

  int crop_left, crop_right, crop_top, crop_bottom;
  int crop_vert_mult;
  
  int pixel_cnt=0;
  int ppm_width,ppm_height;



  if (active_sps->frame_mbs_only_flag)
  {
    crop_vert_mult = 2;
  }
  else
  {
    if (p->structure != FRAME)
    {
      crop_vert_mult = 2;
    }
    else
    {
      crop_vert_mult = 4;
    }
  }

  if (active_sps->frame_cropping_flag)
  {
    crop_left   = 2 * active_sps->frame_cropping_rect_left_offset;
    crop_right  = 2 * active_sps->frame_cropping_rect_right_offset;
    crop_top    = crop_vert_mult * active_sps->frame_cropping_rect_top_offset;
    crop_bottom = crop_vert_mult * active_sps->frame_cropping_rect_bottom_offset;
  }
  else
  {
    crop_left = crop_right = crop_top = crop_bottom = 0;
  }

  ppm_height = p->size_y - crop_bottom - crop_top;
  ppm_width  = p->size_x - crop_right - crop_left;

  ppm_idx++;

  tlmDataRows = ppm_width;
  tlmDataColumns = ppm_height;

  if (tlmDataLenByte < ppm_height * ppm_width * 3) {
    tlmDataLenByte = ppm_height * ppm_width * 3;
    delete [] tlmDataBuffer;
    tlmDataBuffer = new unsigned char [tlmDataLenByte];
  }

  unsigned char *dataPtr = tlmDataBuffer;
  for(i=crop_top;i<p->size_y-crop_bottom;i++)
    {

      for(j=crop_left;j<p->size_x-crop_right;j++)
      {
	  
	int Y  = p->imgY[ i][ j];
	int Cbi = i / 2;
	int Cbj = j / 2;
	int Cri = Cbi;
	int Crj = Cbj;
	int Cb = p->imgUV[ 0][Cbi][Cbj];  
	int Cr = p->imgUV[ 1][Cri][Crj];  
	unsigned int CurColor[ 3];
#ifdef  T_RGB
		convertYCbCr2RGB(Y,Cb,Cr, (int *)CurColor);
#else
	CurColor[ 0] = (unsigned char)Cr;
	CurColor[ 1] = (unsigned char)Y;
	CurColor[ 2] = (unsigned char)Cb;
#endif
	
        
	pixel_cnt++;
	
	
	//cylin: write pixel data to memory via TLM2.0 bus
	//if(pixel_cnt<5) cout<<sc_time_stamp()<<" write_picture_ppm().transaction_req notify"<<endl;
	
	// tlmDataBuffer[0] = CurColor[ 0]; 
	// tlmDataBuffer[1] = CurColor[ 1];
	// tlmDataBuffer[2] = CurColor[ 2] ;
	// tlmDataLenByte = 3;
        *dataPtr++ = CurColor[ 0];
        *dataPtr++ = CurColor[ 1];
        *dataPtr++ = CurColor[ 2];

        //        event_transaction_req.notify();
        //        wait(event_transaction_end);
      
        
	//if(pixel_cnt<5) cout<<sc_time_stamp()<<" write_picture_ppm().transaction_end"<<endl;
	//sc_stop(); wait();
          //-----------------------------------	

      }
      
    } //end of 2d for loop

      glb_frame_done = 1;
      event_frame_ready.notify();
      //MBMB cout<<"output.c :completed one frame, frame_done = :"<<glb_frame_done<<endl;
      wait(event_frame_done);
      //MBMB cout<<"output.c : frame_done = :"<<glb_frame_done<<endl;
}

void 
write_picture_ppm(StorablePicture *p, FILE *p_out)
{
  std::cerr << "Code must be changed correspnding to tlmDataBuffer changes" << std::endl;
  return;

  int i,j;

  int crop_left, crop_right, crop_top, crop_bottom;
  int crop_vert_mult;
  
  int pixel_cnt=0;
  char ppm_filename[ 100];
  FILE *ppm_ft;
  int ppm_width,ppm_height;

  if (active_sps->frame_mbs_only_flag)
  {
    crop_vert_mult = 2;
  }
  else
  {
    if (p->structure != FRAME)
    {
      crop_vert_mult = 2;
    }
    else
    {
      crop_vert_mult = 4;
    }
  }

  if (active_sps->frame_cropping_flag)
  {
    crop_left   = 2 * active_sps->frame_cropping_rect_left_offset;
    crop_right  = 2 * active_sps->frame_cropping_rect_right_offset;
    crop_top    = crop_vert_mult * active_sps->frame_cropping_rect_top_offset;
    crop_bottom = crop_vert_mult * active_sps->frame_cropping_rect_bottom_offset;
  }
  else
  {
    crop_left = crop_right = crop_top = crop_bottom = 0;
  }

  ppm_height = p->size_y - crop_bottom - crop_top;
  ppm_width  = p->size_x - crop_right - crop_left;

#ifdef  EN_PPM 
      //ppm file name generate
#ifdef  T_RGB
      sprintf (ppm_filename, "./log/ppm_out_img_%02d_rgb.ppm", ppm_idx);
#else
      sprintf (ppm_filename, "./log/ppm_out_img_%02d_yuv.ppm", ppm_idx);
#endif

      ppm_idx++;
      ppm_ft = fopen(ppm_filename,"wb");
      if( ppm_ft == NULL)
      {
            printf ( "[ERROR] cannot open ppm file %s\n!", ppm_filename);
            exit(1);
      }
      // ppm header file 

      fprintf(ppm_ft, "P3\n");
      fprintf(ppm_ft, "%d %d\n",ppm_width, ppm_height);
      fprintf(ppm_ft, "255\n");

#endif // ppm dump
      for(i=crop_top;i<p->size_y-crop_bottom;i++)
        for(j=crop_left;j<p->size_x-crop_right;j++)
        {
          int Y  = p->imgY[ i][ j];
          int Cbi = i / 2;
          int Cbj = j / 2;
          int Cri = Cbi;
          int Crj = Cbj;
          int Cb = p->imgUV[ 0][Cbi][Cbj];  
          int Cr = p->imgUV[ 1][Cri][Crj];  
          int CurColor[ 3];
 #ifdef  T_RGB
          convertYCbCr2RGB(Y,Cb,Cr, CurColor);
 #else
          CurColor[ 0] = Cr;
          CurColor[ 1] = Y;
          CurColor[ 2] = Cb;
 #endif
          
          
#ifdef EN_PPM         
          if( ( pixel_cnt%8)==0) fprintf(ppm_ft,"\n");
          fprintf(ppm_ft,"%d ",CurColor[0]);
          fprintf(ppm_ft,"%d ",CurColor[1]);
          fprintf(ppm_ft,"%d ",CurColor[2]);
          pixel_cnt++;


          //cylin: write pixel data to memory via TLM2.0 bus
          //if(pixel_cnt<5) cout<<sc_time_stamp()<<" write_picture_ppm().transaction_req notify"<<endl;
          //*reinterpret_cast<int *>(&tlmDataBuffer[0]) = CurColor[ 0];
          //*reinterpret_cast<int *>(&tlmDataBuffer[4]) = CurColor[ 1];
          //*reinterpret_cast<int *>(&tlmDataBuffer[8]) = CurColor[ 2];
          tlmDataBuffer[0] = CurColor[ 0];
          tlmDataBuffer[1] = CurColor[ 1];
          tlmDataBuffer[2] = CurColor[ 2];
          tlmDataLenByte = 3*1;

          // event_transaction_req.notify();
          // wait(event_transaction_end);
          //if(pixel_cnt<5) cout<<sc_time_stamp()<<" write_picture_ppm().transaction_end"<<endl;
          //sc_stop(); wait();
          //-----------------------------------

#endif


        } //end of 2d for loop
    
#ifdef EN_PPM         
      fflush(ppm_ft);
      fclose(ppm_ft);
#endif
}

/*!
 ************************************************************************
 * \brief
 *    Initialize output buffer for direct output
 ************************************************************************
 */
void init_out_buffer()
{
  out_buffer = alloc_frame_store();
}

/*!
 ************************************************************************
 * \brief
 *    Uninitialize output buffer for direct output
 ************************************************************************
 */
void uninit_out_buffer()
{
  free_frame_store(out_buffer);
  out_buffer=NULL;
}


void write_picture(StorablePicture *p, FILE *p_out)
{
#ifdef EN_YUV
      int i,j;
    
      int crop_left, crop_right, crop_top, crop_bottom;
      int crop_vert_mult;
                
      if (active_sps->frame_mbs_only_flag)
      {
        crop_vert_mult = 2;
      }
      else
      {
        if (p->structure != FRAME)
        {
          crop_vert_mult = 2;
        }
        else
        {
          crop_vert_mult = 4;
        }
      }
    
      if (active_sps->frame_cropping_flag)
      {
        crop_left   = 2 * active_sps->frame_cropping_rect_left_offset;
        crop_right  = 2 * active_sps->frame_cropping_rect_right_offset;
        crop_top    = crop_vert_mult * active_sps->frame_cropping_rect_top_offset;
        crop_bottom = crop_vert_mult * active_sps->frame_cropping_rect_bottom_offset;
      }
      else
      {
        crop_left = crop_right = crop_top = crop_bottom = 0;
      }
    
      for(i=crop_top;i<p->size_y-crop_bottom;i++)
        for(j=crop_left;j<p->size_x-crop_right;j++)
        {
          fputc(p->imgY[i][j],p_out);
        }
    
      crop_left   /= 2;
      crop_right  /= 2;
      crop_top    /= 2;
      crop_bottom /= 2;
    
      for(i=crop_top;i<p->size_y_cr-crop_bottom;i++)
        for(j=crop_left;j<p->size_x_cr-crop_right;j++)
        {
          fputc(p->imgUV[0][i][j],p_out);
        }
      for(i=crop_top;i<p->size_y_cr-crop_bottom;i++)
        for(j=crop_left;j<p->size_x_cr-crop_right;j++)
        {
          fputc(p->imgUV[1][i][j],p_out);
        }
      
      fflush(p_out);
#endif //YUV dump

  // dump ppm file or DirectFB out
#if ( EN_PPM)
  write_picture_ppm(p,p_out);
#endif

#if ( EN_TLM)
  write_picture_tlm(p,p_out);
#endif

}


/*!
 ************************************************************************
 * \brief
 *    Initialize picture memory with (Y:0,U:128,V:128)
 ************************************************************************
 */
void clear_picture(StorablePicture *p)
{
  int i;

  for(i=0;i<p->size_y;i++)
    memset(p->imgY[i], 0 ,p->size_x);
  for(i=0;i<p->size_y_cr;i++)
    memset(p->imgUV[0][i], 128 ,p->size_x_cr);
  for(i=0;i<p->size_y_cr;i++)
    memset(p->imgUV[1][i], 128 ,p->size_x_cr);
  fflush(p_out);
}

/*!
 ************************************************************************
 * \brief
 *    Write out not paired direct output fields. A second empty field is generated
 *    and combined into the frame buffer.
 * \param fs
 *    FrameStore that contains a single field
 * \param p_out
 *    Output file
 ************************************************************************
 */
void write_unpaired_field(FrameStore* fs, FILE *p_out)
{
  StorablePicture *p;
  assert (fs->is_used<3);
  if(fs->is_used &1)
  {
    // we have a top field
    // construct an empty bottom field
    p = fs->top_field;
    fs->bottom_field = alloc_storable_picture(BOTTOM_FIELD, p->size_x, p->size_y, p->size_x_cr, p->size_y_cr);
    clear_picture(fs->bottom_field);
    dpb_combine_field(fs);
    write_picture (fs->frame, p_out);
  }

  if(fs->is_used &2)
  {
    // we have a bottom field
    // construct an empty top field
    p = fs->bottom_field;
    fs->top_field = alloc_storable_picture(TOP_FIELD, p->size_x, p->size_y, p->size_x_cr, p->size_y_cr);
    clear_picture(fs->bottom_field);
    dpb_combine_field(fs);
    write_picture (fs->frame, p_out);
  }

  fs->is_used=3;
}

/*!
 ************************************************************************
 * \brief
 *    Write out unpaired fields from output buffer.
 * \param p_out
 *    Output file
 ************************************************************************
 */
void flush_direct_output(FILE *p_out)
{
  write_unpaired_field(out_buffer, p_out);

  free_storable_picture(out_buffer->frame);
  out_buffer->frame = NULL;
  free_storable_picture(out_buffer->top_field);
  out_buffer->top_field = NULL;
  free_storable_picture(out_buffer->bottom_field);
  out_buffer->bottom_field = NULL;
  out_buffer->is_used = 0;
}


/*!
 ************************************************************************
 * \brief
 *    Write a frame (from FrameStore)
 * \param fs
 *    FrameStore containing the frame
 * \param p_out
 *    Output file
 ************************************************************************
 */
void write_stored_frame( FrameStore *fs,FILE *p_out)
{
  // make sure no direct output field is pending
  flush_direct_output(p_out);

  if (fs->is_used<3)
  {
    write_unpaired_field(fs, p_out);
  }
  else
  {
    write_picture(fs->frame, p_out);
  }

  fs->is_output = 1;
}

/*!
 ************************************************************************
 * \brief
 *    Directly output a picture without storing it in the DPB. Fields 
 *    are buffered before they are written to the file.
 * \param p
 *    Picture for output
 * \param p_out
 *    Output file
 ************************************************************************
 */
void direct_output(StorablePicture *p, FILE *p_out)
{
  if (p->structure==FRAME)
  {
    // we have a frame (or complementary field pair)
    // so output it directly
    flush_direct_output(p_out);
    write_picture (p, p_out);
    if (p_ref)
      find_snr(snr, p, p_ref);
    free_storable_picture(p);
    return;
  }

  if (p->structure == TOP_FIELD)
  {
    if (out_buffer->is_used &1)
      flush_direct_output(p_out);
    out_buffer->top_field = p;
    out_buffer->is_used |= 1;
  }

  if (p->structure == BOTTOM_FIELD)
  {
    if (out_buffer->is_used &2)
      flush_direct_output(p_out);
    out_buffer->bottom_field = p;
    out_buffer->is_used |= 2;
  }

  if (out_buffer->is_used == 3)
  {
    // we have both fields, so output them
    dpb_combine_field(out_buffer);
    write_picture (out_buffer->frame, p_out);
    if (p_ref)
      find_snr(snr, out_buffer->frame, p_ref);
    free_storable_picture(out_buffer->frame);
    out_buffer->frame = NULL;
    free_storable_picture(out_buffer->top_field);
    out_buffer->top_field = NULL;
    free_storable_picture(out_buffer->bottom_field);
    out_buffer->bottom_field = NULL;
    out_buffer->is_used = 0;
  }
}
