
/*!
 ************************************************************************
 *  \file
 *     parset.c
 *  \brief
 *     Parameter Sets
 *  \author
 *     Main contributors (see contributors.h for copyright, address and affiliation details)
 *     - Stephan Wenger          <stewe@cs.tu-berlin.de>
 *
 ***********************************************************************
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "global.h"
#include "parsetcommon.h"
#include "parset.h"
#include "nalu.h"
#include "memalloc.h"
#include "fmo.h"
#include "cabac.h"
#include "vlc.h"
#include "mbuffer.h"

#if TRACE
#define SYMTRACESTRING(s) strncpy(sym->tracestring,s,TRACESTRING_SIZE)
#else
#define SYMTRACESTRING(s) // do nothing
#endif

extern int UsedBits;      // for internal statistics, is adjusted by se_v, ue_v, u_1

seq_parameter_set_rbsp_t SeqParSet[MAXSPS];
pic_parameter_set_rbsp_t PicParSet[MAXPPS];
                          
// fill sps with content of p

int InterpretSPS (DataPartition *p, seq_parameter_set_rbsp_t *sps)
{
  unsigned i;
  int reserved_zero;
  Bitstream *s = p->bitstream;

  assert (p != NULL);
  assert (p->bitstream != NULL);
  assert (p->bitstream->streamBuffer != 0);
  assert (sps != NULL);

  UsedBits = 0;

  sps->profile_idc                            = u_v  (8, "SPS: profile_idc"                           , s);
#ifdef ESLCPP
  sps->constrained_set0_flag                  = (Boolean)u_1  (   "SPS: constrained_set0_flag"                 , s);
  sps->constrained_set1_flag                  = (Boolean)u_1  (   "SPS: constrained_set1_flag"                 , s);
  sps->constrained_set2_flag                  = (Boolean)u_1  (   "SPS: constrained_set2_flag"                 , s);
#else
  sps->constrained_set0_flag                  = u_1  (   "SPS: constrained_set0_flag"                 , s);
  sps->constrained_set1_flag                  = u_1  (   "SPS: constrained_set1_flag"                 , s);
  sps->constrained_set2_flag                  = u_1  (   "SPS: constrained_set2_flag"                 , s);
#endif
  reserved_zero                               = u_v  (5, "SPS: reserved_zero_5bits"                   , s);
  assert (reserved_zero==0);

  sps->level_idc                              = u_v  (8, "SPS: level_idc"                             , s);
  

  sps->seq_parameter_set_id                   = ue_v ("SPS: seq_parameter_set_id"                     , s);
  sps->log2_max_frame_num_minus4              = ue_v ("SPS: log2_max_frame_num_minus4"                , s);
  sps->pic_order_cnt_type                     = ue_v ("SPS: pic_order_cnt_type"                       , s);

  if (sps->pic_order_cnt_type == 0)
    sps->log2_max_pic_order_cnt_lsb_minus4 = ue_v ("SPS: log2_max_pic_order_cnt_lsb_minus4"           , s);
  else if (sps->pic_order_cnt_type == 1)
  {
#ifdef ESLCPP
    sps->delta_pic_order_always_zero_flag      = (Boolean)u_1  ("SPS: delta_pic_order_always_zero_flag"       , s);
#else
    sps->delta_pic_order_always_zero_flag      = u_1  ("SPS: delta_pic_order_always_zero_flag"       , s);
#endif
    sps->offset_for_non_ref_pic                = se_v ("SPS: offset_for_non_ref_pic"                 , s);
    sps->offset_for_top_to_bottom_field        = se_v ("SPS: offset_for_top_to_bottom_field"         , s);
    sps->num_ref_frames_in_pic_order_cnt_cycle = ue_v ("SPS: num_ref_frames_in_pic_order_cnt_cycle"  , s);
    for(i=0; i<sps->num_ref_frames_in_pic_order_cnt_cycle; i++)
      sps->offset_for_ref_frame[i]               = se_v ("SPS: offset_for_ref_frame[i]"              , s);
  }
  sps->num_ref_frames                        = ue_v ("SPS: num_ref_frames"                         , s);
#ifdef ESLCPP
  sps->gaps_in_frame_num_value_allowed_flag  = (Boolean)u_1  ("SPS: gaps_in_frame_num_value_allowed_flag"   , s);
#else
  sps->gaps_in_frame_num_value_allowed_flag  = u_1  ("SPS: gaps_in_frame_num_value_allowed_flag"   , s);
#endif
  sps->pic_width_in_mbs_minus1               = ue_v ("SPS: pic_width_in_mbs_minus1"                , s);
  sps->pic_height_in_map_units_minus1        = ue_v ("SPS: pic_height_in_map_units_minus1"         , s);
#ifdef ESLCPP
  sps->frame_mbs_only_flag                   = (Boolean)u_1  ("SPS: frame_mbs_only_flag"                    , s);
#else
  sps->frame_mbs_only_flag                   = u_1  ("SPS: frame_mbs_only_flag"                    , s);
#endif
  if (!sps->frame_mbs_only_flag)
  {
#ifdef ESLCPP
    sps->mb_adaptive_frame_field_flag          = (Boolean)u_1  ("SPS: mb_adaptive_frame_field_flag"           , s);
#else
    sps->mb_adaptive_frame_field_flag          = u_1  ("SPS: mb_adaptive_frame_field_flag"           , s);
#endif
  }
#ifdef ESLCPP
  sps->direct_8x8_inference_flag             = (Boolean)u_1  ("SPS: direct_8x8_inference_flag"              , s);
  sps->frame_cropping_flag                   = (Boolean)u_1  ("SPS: frame_cropping_flag"                , s);
#else
  sps->direct_8x8_inference_flag             = u_1  ("SPS: direct_8x8_inference_flag"              , s);
  sps->frame_cropping_flag                   = u_1  ("SPS: frame_cropping_flag"                , s);
#endif

  if (sps->frame_cropping_flag)
  {
    sps->frame_cropping_rect_left_offset      = ue_v ("SPS: frame_cropping_rect_left_offset"           , s);
    sps->frame_cropping_rect_right_offset     = ue_v ("SPS: frame_cropping_rect_right_offset"          , s);
    sps->frame_cropping_rect_top_offset       = ue_v ("SPS: frame_cropping_rect_top_offset"            , s);
    sps->frame_cropping_rect_bottom_offset    = ue_v ("SPS: frame_cropping_rect_bottom_offset"         , s);
  }
#ifdef ESLCPP
  sps->vui_parameters_present_flag           = (Boolean)u_1  ("SPS: vui_parameters_present_flag"            , s);
#else
  sps->vui_parameters_present_flag           = u_1  ("SPS: vui_parameters_present_flag"            , s);
#endif
  if (sps->vui_parameters_present_flag)
  {
    printf ("VUI sequence parameters present but not supported, ignored, proceeding to next NALU\n");
  }
  sps->Valid = TRUE;
  return UsedBits;
}


int InterpretPPS (DataPartition *p, pic_parameter_set_rbsp_t *pps)
{
  unsigned i;
  int NumberBitsPerSliceGroupId;
  Bitstream *s = p->bitstream;
  
  assert (p != NULL);
  assert (p->bitstream != NULL);
  assert (p->bitstream->streamBuffer != 0);
  assert (pps != NULL);

  UsedBits = 0;

  pps->pic_parameter_set_id                  = ue_v ("PPS: pic_parameter_set_id"                   , s);
  pps->seq_parameter_set_id                  = ue_v ("PPS: seq_parameter_set_id"                   , s);
#ifdef ESLCPP
  pps->entropy_coding_mode_flag              = (Boolean)u_1  ("PPS: entropy_coding_mode_flag"               , s);
#else
  pps->entropy_coding_mode_flag              = u_1  ("PPS: entropy_coding_mode_flag"               , s);
#endif

  //! Note: as per JVT-F078 the following bit is unconditional.  If F078 is not accepted, then
  //! one has to fetch the correct SPS to check whether the bit is present (hopefully there is
  //! no consistency problem :-(
  //! The current encoder code handles this in the same way.  When you change this, don't forget
  //! the encoder!  StW, 12/8/02
#ifdef  ESLCPP
  pps->pic_order_present_flag                = (Boolean)u_1  ("PPS: pic_order_present_flag"                 , s);
#else
  pps->pic_order_present_flag                = u_1  ("PPS: pic_order_present_flag"                 , s);
#endif

  pps->num_slice_groups_minus1               = ue_v ("PPS: num_slice_groups_minus1"                , s);

  // FMO stuff begins here
  if (pps->num_slice_groups_minus1 > 0)
  {
    pps->slice_group_map_type               = ue_v ("PPS: slice_group_map_type"                , s);
    if (pps->slice_group_map_type == 0)
    {
      for (i=0; i<=pps->num_slice_groups_minus1; i++)
        pps->run_length_minus1 [i]                  = ue_v ("PPS: run_length_minus1 [i]"              , s);
    }
    else if (pps->slice_group_map_type == 2)
    {
      for (i=0; i<pps->num_slice_groups_minus1; i++)
      {
        //! JVT-F078: avoid reference of SPS by using ue(v) instead of u(v)
        pps->top_left [i]                          = ue_v ("PPS: top_left [i]"                        , s);
        pps->bottom_right [i]                      = ue_v ("PPS: bottom_right [i]"                    , s);
      }
    }
    else if (pps->slice_group_map_type == 3 ||
             pps->slice_group_map_type == 4 ||
             pps->slice_group_map_type == 5)
    {
#ifdef ESLCPP
      pps->slice_group_change_direction_flag     = (Boolean)u_1  ("PPS: slice_group_change_direction_flag"      , s);
#else
      pps->slice_group_change_direction_flag     = u_1  ("PPS: slice_group_change_direction_flag"      , s);
#endif
      pps->slice_group_change_rate_minus1        = ue_v ("PPS: slice_group_change_rate_minus1"         , s);
    }
    else if (pps->slice_group_map_type == 6)
    {
      if (pps->num_slice_groups_minus1+1 >4)
        NumberBitsPerSliceGroupId = 3;
      else if (pps->num_slice_groups_minus1+1 > 2)
        NumberBitsPerSliceGroupId = 2;
      else
        NumberBitsPerSliceGroupId = 1;
      //! JVT-F078, exlicitly signal number of MBs in the map
      pps->num_slice_group_map_units_minus1      = ue_v ("PPS: num_slice_group_map_units_minus1"               , s);
      for (i=0; i<=pps->num_slice_group_map_units_minus1; i++)
        pps->slice_group_id[i] = u_v (NumberBitsPerSliceGroupId, "slice_group_id[i]", s);
    }
  }

  // End of FMO stuff

  pps->num_ref_idx_l0_active_minus1          = ue_v ("PPS: num_ref_idx_l0_active_minus1"           , s);
  pps->num_ref_idx_l1_active_minus1          = ue_v ("PPS: num_ref_idx_l1_active_minus1"           , s);
#ifdef ESLCPP
  pps->weighted_pred_flag                    = (Boolean)u_1  ("PPS: weighted prediction flag"               , s);
  pps->weighted_bipred_idc                   = (Boolean)u_v  ( 2, "PPS: weighted_bipred_idc"                , s);
#else
  pps->weighted_pred_flag                    = u_1  ("PPS: weighted prediction flag"               , s);
  pps->weighted_bipred_idc                   = u_v  ( 2, "PPS: weighted_bipred_idc"                , s);
#endif
  pps->pic_init_qp_minus26                   = se_v ("PPS: pic_init_qp_minus26"                    , s);
  pps->pic_init_qs_minus26                   = se_v ("PPS: pic_init_qs_minus26"                    , s);
  pps->chroma_qp_index_offset                = se_v ("PPS: chroma_qp_index_offset"                 , s);
#ifdef ESLCPP
  pps->deblocking_filter_control_present_flag = (Boolean)u_1 ("PPS: deblocking_filter_control_present_flag" , s);
  pps->constrained_intra_pred_flag           = (Boolean)u_1  ("PPS: constrained_intra_pred_flag"            , s);
  pps->redundant_pic_cnt_present_flag        = (Boolean)u_1  ("PPS: redundant_pic_cnt_present_flag"         , s);
#else
  pps->deblocking_filter_control_present_flag = u_1 ("PPS: deblocking_filter_control_present_flag" , s);
  pps->constrained_intra_pred_flag           = u_1  ("PPS: constrained_intra_pred_flag"            , s);
  pps->redundant_pic_cnt_present_flag        = u_1  ("PPS: redundant_pic_cnt_present_flag"         , s);
#endif

  pps->Valid = TRUE;
  return UsedBits;
}


void DumpSPS (seq_parameter_set_rbsp_t *sps)
{
  printf ("Dumping a sequence parset, to be implemented\n");
};

void DumpPPS (pic_parameter_set_rbsp_t *pps)
{
  printf ("Dumping a picture parset, to be implemented\n");
}

void PPSConsistencyCheck (pic_parameter_set_rbsp_t *pps)
{
  printf ("Consistency checking a picture parset, to be implemented\n");
//  if (pps->seq_parameter_set_id invalid then do something)
}

void SPSConsistencyCheck (seq_parameter_set_rbsp_t *sps)
{
  printf ("Consistency checking a sequence parset, to be implemented\n");
}

void MakePPSavailable (int id, pic_parameter_set_rbsp_t *pps)
{
  assert (pps->Valid == TRUE);

  if (PicParSet[id].Valid == TRUE && PicParSet[id].slice_group_id != NULL)
    free (PicParSet[id].slice_group_id);

  memcpy (&PicParSet[id], pps, sizeof (pic_parameter_set_rbsp_t));
#ifdef ESLCPP
  if ((PicParSet[id].slice_group_id = (unsigned int*)calloc (PicParSet[id].num_slice_group_map_units_minus1+1, sizeof(int))) == NULL)
#else
  if ((PicParSet[id].slice_group_id = calloc (PicParSet[id].num_slice_group_map_units_minus1+1, sizeof(int))) == NULL)
#endif
    no_mem_exit ("MakePPSavailable: Cannot calloc slice_group_id");
  
  memcpy (PicParSet[id].slice_group_id, pps->slice_group_id, (pps->num_slice_group_map_units_minus1+1)*sizeof(int));
#if ESLDEBUG
  printf("=====================================================\n");
  printf("[Info] PPS parameter set:\n");
  printf(" pps.Valid                                  = %x\n", pps->Valid);
  printf(" pps.pic_parameter_set_id                   = %x\n", pps->pic_parameter_set_id    );
  printf(" pps.seq_parameter_set_id                   = %x\n", pps->seq_parameter_set_id    );
  printf(" pps.entropy_coding_mode_flag               = %x\n", pps->entropy_coding_mode_flag);
  printf(" pps.pic_order_present_flag                 = %x\n", pps->pic_order_present_flag  );
  printf(" pps.num_slice_groups_minus1                = %x\n", pps->num_slice_groups_minus1 );
  // FMO stuff begins here
  if (pps->num_slice_groups_minus1 > 0){
    printf(" pps.slice_group_map_type                   = %x\n", pps->slice_group_map_type    );
    if (pps->slice_group_map_type == 0){
      for(unsigned i=0; i<=pps->num_slice_groups_minus1; i++)
        printf(" pps.run_length_minus1[%d]                  = %x\n", i, pps->run_length_minus1[i]);
    }
    else if (pps->slice_group_map_type == 2){
      for(unsigned i=0; i<pps->num_slice_groups_minus1; i++){
        printf(" pps.top_left[%d]                           = %x\n", i, pps->top_left [i]    );
        printf(" pps.bottom_right[%d]                       = %x\n", i, pps->bottom_right [i]);
      }
    }
    else if (pps->slice_group_map_type == 3 ||
             pps->slice_group_map_type == 4 ||
             pps->slice_group_map_type == 5){
      printf(" pps.slice_group_change_direction_flag      = %x\n", pps->slice_group_change_direction_flag);
      printf(" pps.slice_group_change_rate_minus1         = %x\n", pps->slice_group_change_rate_minus1   );
    }
    else if (pps->slice_group_map_type == 6){
      printf(" pps.num_slice_group_map_units_minus1   = %x\n", pps->num_slice_group_map_units_minus1 );
      for(unsigned i=0; i<=pps->num_slice_group_map_units_minus1; i++)
        printf(" pps.slice_group_id[%d]                     = %x\n", i, pps->slice_group_id[i] );
    }
  }
  printf(" pps.num_ref_idx_l0_active_minus1           = %x\n", pps->num_ref_idx_l0_active_minus1          );
  printf(" pps.num_ref_idx_l1_active_minus1           = %x\n", pps->num_ref_idx_l1_active_minus1          );
  printf(" pps.weighted_pred_flag                     = %x\n", pps->weighted_pred_flag                    );
  printf(" pps.weighted_bipred_idc                    = %x\n", pps->weighted_bipred_idc                   );
  printf(" pps.pic_init_qp_minus26                    = %x\n", pps->pic_init_qp_minus26                   );
  printf(" pps.pic_init_qs_minus26                    = %x\n", pps->pic_init_qs_minus26                   );
  printf(" pps.chroma_qp_index_offset                 = %x\n", pps->chroma_qp_index_offset                );
  printf(" pps.deblocking_filter_control_present_flag = %x\n", pps->deblocking_filter_control_present_flag);
  printf(" pps.constrained_intra_pred_flag            = %x\n", pps->constrained_intra_pred_flag           );
  printf(" pps.redundant_pic_cnt_present_flag         = %x\n", pps->redundant_pic_cnt_present_flag        );
  printf("=====================================================\n");
#endif
}

void MakeSPSavailable (int id, seq_parameter_set_rbsp_t *sps)
{
  assert (sps->Valid == TRUE);
  memcpy (&SeqParSet[id], sps, sizeof (seq_parameter_set_rbsp_t));
#if ESLDEBUG
  printf("=====================================================\n");
  printf("[Info] SPS parameter set:\n");
  printf(" sps.Valid                                = %x\n", sps->Valid                );
  printf(" sps.profile_idc                          = %x\n", sps->profile_idc          );
  printf(" sps.constrained_set0_flag                = %x\n", sps->constrained_set0_flag);
  printf(" sps.constrained_set1_flag                = %x\n", sps->constrained_set1_flag);
  printf(" sps.constrained_set2_flag                = %x\n", sps->constrained_set2_flag);
  printf(" sps.level_idc                            = %x\n", sps->level_idc            );
  printf(" sps.seq_parameter_set_id                 = %x\n", sps->seq_parameter_set_id );
  printf(" sps.log2_max_frame_num_minus4            = %x\n", sps->log2_max_frame_num_minus4);
  printf(" sps.pic_order_cnt_type                   = %x\n", sps->pic_order_cnt_type       );
  if (sps->pic_order_cnt_type == 0)
    printf(" sps.log2_max_pic_order_cnt_lsb_minus4    = %x\n", sps->log2_max_pic_order_cnt_lsb_minus4);
  else if (sps->pic_order_cnt_type == 1){
    printf(" sps.delta_pic_order_always_zero_flag     = %x\n", sps->delta_pic_order_always_zero_flag );
    printf(" sps.offset_for_non_ref_pic               = %x\n", sps->offset_for_non_ref_pic           );
    printf(" sps.offset_for_top_to_bottom_field       = %x\n", sps->offset_for_top_to_bottom_field   );
    printf(" sps.num_ref_frames_in_pic_order_cnt_cycle = %x\n", sps->num_ref_frames_in_pic_order_cnt_cycle);
    for(unsigned i=0; i<sps->num_ref_frames_in_pic_order_cnt_cycle; i++)
      printf(" sps.offset_for_ref_frame[%d]             = %x\n", i, sps->offset_for_ref_frame[i]);
  }
  printf(" sps.num_ref_frames                       = %x\n", sps->num_ref_frames                      );
  printf(" sps.gaps_in_frame_num_value_allowed_flag = %x\n", sps->gaps_in_frame_num_value_allowed_flag);
  printf(" sps.pic_width_in_mbs_minus1              = %x\n", sps->pic_width_in_mbs_minus1             );
  printf(" sps.pic_height_in_map_units_minus1       = %x\n", sps->pic_height_in_map_units_minus1      );
  printf(" sps.frame_mbs_only_flag                  = %x\n", sps->frame_mbs_only_flag                 );
  if (!sps->frame_mbs_only_flag)
    printf(" sps.mb_adaptive_frame_field_flag         = %x\n", sps->mb_adaptive_frame_field_flag        );
  printf(" sps.direct_8x8_inference_flag            = %x\n", sps->direct_8x8_inference_flag           );
  printf(" sps.frame_cropping_flag                  = %x\n", sps->frame_cropping_flag                 );
  if (sps->frame_cropping_flag){
    printf(" sps.frame_cropping_rect_left_offset      = %x\n", sps->frame_cropping_rect_left_offset  );
    printf(" sps.frame_cropping_rect_right_offset     = %x\n", sps->frame_cropping_rect_right_offset );
    printf(" sps.frame_cropping_rect_top_offset       = %x\n", sps->frame_cropping_rect_top_offset   );
    printf(" sps.frame_cropping_rect_bottom_offset    = %x\n", sps->frame_cropping_rect_bottom_offset);
  }
  printf(" sps.vui_parameters_present_flag          = %x\n", sps->vui_parameters_present_flag      );
  printf("=====================================================\n");
#endif
}


void ProcessSPS (NALU_t *nalu)
{
  DataPartition *dp = AllocPartition(1);
  seq_parameter_set_rbsp_t *sps = AllocSPS();
  int dummy;

  memcpy (dp->bitstream->streamBuffer, &nalu->buf[1], nalu->len-1);
  dp->bitstream->code_len = dp->bitstream->bitstream_length = RBSPtoSODB (dp->bitstream->streamBuffer, nalu->len-1);
  dp->bitstream->ei_flag = 0;
  dp->bitstream->read_len = dp->bitstream->frame_bitoffset = 0;
  dummy = InterpretSPS (dp, sps);
  // DumpSPS (sps);
  // SPSConsistencyCheck (pps);
  MakeSPSavailable (sps->seq_parameter_set_id, sps);
  FreePartition (dp, 1);
  FreeSPS (sps);
}


void ProcessPPS (NALU_t *nalu)
{
  DataPartition *dp;
  pic_parameter_set_rbsp_t *pps;
  int dummy;

  dp = AllocPartition(1);
  pps = AllocPPS();
  memcpy (dp->bitstream->streamBuffer, &nalu->buf[1], nalu->len-1);
  dp->bitstream->code_len = dp->bitstream->bitstream_length = RBSPtoSODB (dp->bitstream->streamBuffer, nalu->len-1);
  dp->bitstream->ei_flag = 0;
  dp->bitstream->read_len = dp->bitstream->frame_bitoffset = 0;
  dummy = InterpretPPS (dp, pps);
  // DumpPPS (pps);
  // PPSConsistencyCheck (pps);
  MakePPSavailable (pps->pic_parameter_set_id, pps);
  FreePartition (dp, 1);
  FreePPS (pps);
}


void UseParameterSet (int PicParsetId)
{
  seq_parameter_set_rbsp_t *sps = &SeqParSet[PicParSet[PicParsetId].seq_parameter_set_id];
  pic_parameter_set_rbsp_t *pps = &PicParSet[PicParsetId];
  static unsigned int ExpectedDeltaPerPicOrderCntCycle;  // POC200301 Can it be deleted?
  int i;


  if (PicParSet[PicParsetId].Valid != TRUE)
    printf ("Trying to use an invalid (uninitialized) Picture Parameter Set with ID %d, expect the unexpected...\n", PicParsetId);
  if (SeqParSet[PicParSet[PicParsetId].seq_parameter_set_id].Valid != TRUE)
    printf ("PicParset %d references an invalid (uninitialized) Sequence Parameter Set with ID %d, expect the unexpected...\n", PicParsetId, PicParSet[PicParsetId].seq_parameter_set_id);

  sps =  &SeqParSet[PicParSet[PicParsetId].seq_parameter_set_id];

  
  if (active_sps != sps)
  {
    active_sps = sps;
    init_dpb();
  }

  active_pps = pps;

  // In theory, and with a well-designed software, the lines above
  // are everything necessary.  In practice, we need to patch many values
  // in img-> (but no more in inp-> -- these have been taken care of)


  // Sequence Parameter Set Stuff first

//  printf ("Using Picture Parameter set %d and associated Sequence Parameter Set %d\n", PicParsetId, PicParSet[PicParsetId].seq_parameter_set_id);

//  img->log2_max_frame_num_minus4 = sps->log2_max_frame_num_minus4;
  img->MaxFrameNum = 1<<(sps->log2_max_frame_num_minus4+4);
  
  img->pic_order_cnt_type = sps->pic_order_cnt_type;
  // POC200301
  if (img->pic_order_cnt_type < 0 || img->pic_order_cnt_type > 2)  // != 1
  {
    printf ("sps->pic_order_cnt_type %d, expected 1, expect the unexpected...\n", sps->pic_order_cnt_type);
    assert (sps->pic_order_cnt_type == 1);
    error ("pic_order_cnt_type != 1", -1000);
  }

  if (img->pic_order_cnt_type == 0)
  {
//    img->log2_max_pic_order_cnt_lsb_minus4 = sps->log2_max_pic_order_cnt_lsb_minus4;
  }
  else if (img->pic_order_cnt_type == 1) // POC200301
  {
    img->num_ref_frames_in_pic_order_cnt_cycle = sps->num_ref_frames_in_pic_order_cnt_cycle;

    if(img->num_ref_frames_in_pic_order_cnt_cycle >= MAXnum_ref_frames_in_pic_order_cnt_cycle)
      error("num_ref_frames_in_pic_order_cnt_cycle too large",-1011);

    img->delta_pic_order_always_zero_flag = sps->delta_pic_order_always_zero_flag;
//    if(img->delta_pic_order_always_zero_flag != 0) !KS2
//      error ("delta_pic_order_always_zero_flag != 0",-1002);
 
    img->offset_for_non_ref_pic = sps->offset_for_non_ref_pic;
  
    img->offset_for_top_to_bottom_field = sps->offset_for_top_to_bottom_field;
  

    ExpectedDeltaPerPicOrderCntCycle=0;
    if (sps->num_ref_frames_in_pic_order_cnt_cycle)
      for(i=0;i<(int)sps->num_ref_frames_in_pic_order_cnt_cycle;i++) 
      {
        img->offset_for_ref_frame[i] = sps->offset_for_ref_frame[i];
        ExpectedDeltaPerPicOrderCntCycle += sps->offset_for_ref_frame[i];
      }
  }
  
  img->PicWidthInMbs = (active_sps->pic_width_in_mbs_minus1 +1);
  img->PicHeightInMapUnits = (active_sps->pic_height_in_map_units_minus1 +1);
  img->FrameHeightInMbs = ( 2 - active_sps->frame_mbs_only_flag ) * img->PicHeightInMapUnits;

  img->width = img->PicWidthInMbs * MB_BLOCK_SIZE;
  img->width_cr = img->width /2;
  img->height = img->FrameHeightInMbs * MB_BLOCK_SIZE;
  img->height_cr = img->height / 2;

  // Picture Parameter Stuff

  img->weighted_pred_flag = pps->weighted_pred_flag;
  img->weighted_bipred_idc = pps->weighted_bipred_idc;

  img->pic_order_present_flag = pps->pic_order_present_flag;
  // POC200301 DELETE
//  if(img->pic_order_present_flag != 0)
//    error ("pic_order_present_flag != 0",-1004);

  img->constrained_intra_pred_flag = pps->constrained_intra_pred_flag;


  // currSlice->dp_mode is set by read_new_slice (NALU first byte available there)
#ifdef ESLCPP
  if ((SymbolMode)pps->entropy_coding_mode_flag == UVLC)
#else
  if (pps->entropy_coding_mode_flag == UVLC)
#endif
  {
    nal_startcode_follows = uvlc_startcode_follows;
    for (i=0; i<3; i++)
    {
      img->currentSlice->partArr[i].readSyntaxElement = readSyntaxElement_UVLC;
    }
  }
  else
  {
    nal_startcode_follows = cabac_startcode_follows;
    for (i=0; i<3; i++)
    {
      img->currentSlice->partArr[i].readSyntaxElement = readSyntaxElement_CABAC;
    }
  }
}
