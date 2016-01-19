/*!
 *************************************************************************************
 * \file annexb.c
 *
 * \brief
 *    Annex B Byte Stream format
 *
 * \author
 *    Main contributors (see contributors.h for copyright, address and affiliation details)
 *      - Stephan Wenger                  <stewe@cs.tu-berlin.de>
 *************************************************************************************
 */

#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "annexb.h"
#include "memalloc.h"

#ifndef ESLCPP
FILE *bits = NULL;                //!< the bit stream file
#endif

#ifdef ESLCPP
unsigned char *Buf;             //from
#endif

static int FindStartCode (unsigned char *Buf, int zeros_in_startcode);

/*!
 ************************************************************************
 * \brief
 *    Opens the bit stream file named fn
 * \return
 *    none
 ************************************************************************
 */
void OpenBitstreamFile (char *fn)
{
  if (NULL == (bits=fopen(fn, "rb")))
  {
    snprintf (errortext, ET_SIZE, "Cannot open Annex B ByteStream file '%s'", input->infile);
    error(errortext,500);
  }
}

void OpenBitstreamFile2 (unsigned char *bst)
{
   Buf = bst;//Assign bitstream-preloaded MEM address to Buf
}


/*!
 ************************************************************************
 * \brief
 *    Closes the bit stream file
 ************************************************************************
 */
void CloseBitstreamFile()
{
  fclose (bits);
}

void CloseBitstreamFile2()
{
  ;
}



/*!
 ************************************************************************
 * \brief
 *    Returns the size of the NALU (bits between start codes in case of
 *    Annex B.  nalu->buf and nalu->len are filled.  Other field in
 *    nalu-> remain uninitialized (will be taken care of by NALUtoRBSP.
 *
 * \return
 *     0 if there is nothing any more to read (EOF)
 *    -1 in case of any error
 *
 *  \note Side-effect: Returns length of start-code in bytes. 
 *
 * \note
 *   GetAnnexbNALU expects start codes at byte aligned positions in the file
 *
 ************************************************************************
 */

int GetAnnexbNALU (NALU_t *nalu)
{
  int info2, info3, pos = 0;
  int StartCodeFound, rewind;
  
#ifdef ESLCPP
  unsigned char *myBuf;
#else
  char *myBuf;
#endif
    
#ifdef ESLCPP
  if ((myBuf = (unsigned char*)calloc (nalu->max_size , sizeof(char))) == NULL) no_mem_exit("GetAnnexbNALU: Buf");
#else
  if ((myBuf = (char*)calloc (nalu->max_size , sizeof(char))) == NULL) no_mem_exit("GetAnnexbNALU: Buf");
#endif

  nalu->startcodeprefix_len=3;

  info2 = 0;
  info3 = 0;
  
//  if (3 != fread (Buf, 1, 3, bits))//read 3 Bytes
//  {
//    free(Buf);
//    return 0;
//  }
//  memcpy(myBuf, Buf, 3);//boy

  for (unsigned int i = 0; i < 3; i++)
 {
     myBuf[i] = Buf[i+FPindex];
 }  
  FPindex += 3;
  info2 = FindStartCode (myBuf, 2);
  if(info2 != 1) {
//    if(1 != fread(Buf+3, 1, 1, bits))//read 1 Bytes
//    {
//      free(Buf);
//      return 0;
//    }
    //memcpy(myBuf+3, Buf, 1);//boy
    myBuf[3] = Buf[FPindex];
    FPindex ++;  
    info3 = FindStartCode (myBuf, 3);
  }

  if (info2 != 1 && info3 != 1)
  {
    printf ("GetAnnexbNALU: no Start Code at the begin of the NALU, return -1\n");
    free(myBuf);
    return -1;
  }

  if( info2 == 1) {
    nalu->startcodeprefix_len = 3;
    pos = 3;
  }
  else if(info3 ==1 ) {
    pos = 4;
    nalu->startcodeprefix_len = 4;
  }
  else
    printf( " Panic: Error \n");

  StartCodeFound = 0;
  info2 = 0;
  info3 = 0;

  while (!StartCodeFound)
  {
    //if (feof (bits))
    if (FPindex == Bitstream_leng +1) 
    {
      nalu->len = (pos-1)-nalu->startcodeprefix_len;
      memcpy (nalu->buf, &myBuf[nalu->startcodeprefix_len], nalu->len);     
      nalu->forbidden_bit = (nalu->buf[0]>>7) & 1;
      nalu->nal_reference_idc = (nalu->buf[0]>>5) & 3;
      nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;

// printf ("GetAnnexbNALU, eof case: pos %d nalu->len %d, nalu->reference_idc %d, nal_unit_type %d \n", pos, nalu->len, nalu->nal_reference_idc, nalu->nal_unit_type);

#if TRACE
  fprintf (p_trace, "\n\nLast NALU in File\n\n");
  fprintf (p_trace, "Annex B NALU w/ %s startcode, len %d, forbidden_bit %d, nal_reference_idc %d, nal_unit_type %d\n\n",
    nalu->startcodeprefix_len == 4?"long":"short", nalu->len, nalu->forbidden_bit, nalu->nal_reference_idc, nalu->nal_unit_type);
  fflush (p_trace);
#endif
      free(myBuf);
      return pos-1;
    }
    //Buf[pos++] = fgetc (bits);
    //memcpy((void *)myBuf[pos++], Buf, 1);//boy
    myBuf[pos] = Buf[FPindex];
    pos++;
    FPindex++;
    info3 = FindStartCode(&myBuf[pos-4], 3);
    if(info3 != 1)
      info2 = FindStartCode(&myBuf[pos-3], 2);
    StartCodeFound = (info2 == 1 || info3 == 1);
  }
  // Here, we have found another start code (and read length of startcode bytes more than we should
  // have.  Hence, go back in the file
  rewind = 0;
  if(info3 == 1)
    rewind = -4;
  else if (info2 == 1)
    rewind = -3;
  else
    printf(" Panic: Error in next start code search \n");
    FPindex += rewind;
/*
  if (0 != fseek (bits, rewind, SEEK_CUR))
  {
    snprintf (errortext, ET_SIZE, "GetAnnexbNALU: Cannot fseek %d in the bit stream file", rewind);
    free(myBuf);
    error(errortext, 600);
  }
*/
  // Here the Start code, the complete NALU, and the next start code is in the Buf.  
  // The size of Buf is pos, pos+rewind are the number of bytes excluding the next
  // start code, and (pos+rewind)-startcodeprefix_len is the size of the NALU

  nalu->len = (pos+rewind)-nalu->startcodeprefix_len;
  memcpy (nalu->buf, &myBuf[nalu->startcodeprefix_len], nalu->len);
  nalu->forbidden_bit = (nalu->buf[0]>>7) & 1;
  nalu->nal_reference_idc = (nalu->buf[0]>>5) & 3;
  nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;


//printf ("GetAnnexbNALU, regular case: pos %d nalu->len %d, nalu->reference_idc %d, nal_unit_type %d \n", pos, nalu->len, nalu->nal_reference_idc, nalu->nal_unit_type);
#if TRACE
  fprintf (p_trace, "\n\nAnnex B NALU w/ %s startcode, len %d, forbidden_bit %d, nal_reference_idc %d, nal_unit_type %d\n\n",
    nalu->startcodeprefix_len == 4?"long":"short", nalu->len, nalu->forbidden_bit, nalu->nal_reference_idc, nalu->nal_unit_type);
  fflush (p_trace);
#endif
  
  free(myBuf);
  return (pos+rewind);

}


/*!
 ************************************************************************
 * \brief
 *    returns if new start code is found at byte aligned position buf.
 *    new-startcode is of form N 0x00 bytes, followed by a 0x01 byte.
 *
 *  \return
 *     1 if start-code is found or                      \n
 *     0, indicating that there is no start code
 *
 *  \param Buf
 *     pointer to byte-stream
 *  \param zeros_in_startcode
 *     indicates number of 0x00 bytes in start-code.
 ************************************************************************
 */
static int FindStartCode (unsigned char *Buf, int zeros_in_startcode)
{
  int info;
  int i;

  info = 1;
  for (i = 0; i < zeros_in_startcode; i++)
    if(Buf[i] != 0)
      info = 0;

  if(Buf[i] != 1)
    info = 0;
  return info;
}
