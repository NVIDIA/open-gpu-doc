/*
 * SPDX-FileCopyrightText: Copyright (c) 1993-2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef NVENC_DRV_H
#define NVENC_DRV_H
//
// CLASS NV_C9B7_NVENC
// ===================
//
//
// 1  -  INTRODUCTION
//
// The NVENC is a video encoder engine. Currently only H.264 baseline, main
// and high profile are supported. For baseline, only 1 slicegroup is allowed.
// NVENC has built-in motion estimation that enables it to do encode on its own
// but also has the ability to work with external hints that could be produced
// by another motion search implementation such as CEA.<br>
// NVENC motion search is divided into a simple integer search module and a high
// perf subpel refinement module. The integer search capability of NVENC is
// limited (perf wise) so for higher perf the CEA approach should be used.
// Motion search/estimation (ME) results in a set of vectors with scores
// which it will evaluate for encoding cost and eligibility. It also searches
// intra encoding modes for the lowest cost encoding, picks a final winner
// from all inter and intra modes that have been evaluated and encodes the
// macroblock according to that mode. Skip and direct modes are also checked.
// Search and encoding involve forward dct and quantization, inverse dct and
// quantization, reconstruction, and optionally deblocking. Entropy encode can
// do either CAVLC or CABAC.
//
//
// 1.4. CLASS OPERATION
// ------------------------
//
// *** TBD ***
//
// If MVC is enabled, HW need to write the prefix NAL unit(14) when current view is a base view.
// As SVC is not supported in nvenc, so svc_extension_flag should be always false. Then, in the prefix NAL unit,
// only the syntax elements(H.7.3.1.1 NAL unit header MVC extension syntax) in the prefix NAL unit should be written:
// nal_unit_header_mvc_extension( ) {
//  non_idr_flag
//  priority_id
//  view_id
//  temporal_id
//  anchor_pic_flag
//  inter_view_flag
//  reserved_one_bit
// }
//
// 1.5. RESTRICTIONS
// ------------------------
//
// (1) Reference pictures to be accessed by NVENC must be in tiled 16x16 format
// (2) Input pictures are either all in BL format or all in tiled 16x16 format
// (3) Input pictures and reference pictures must be of the kind specified by
//     'pic_struct' (either fields or frames). NVENC cannot encode an interlaced
//     input picture (2 fields) as a frame or vice versa, and cannot use frame
//     reference pictures when encoding a field picture, or field reference
//     pictures when encoding a frame.
//
//
//
// 2  -  APPLICATION MEMORY STRUCTURES
//
// 2.1. DRV PICTURE SETUP BUFFER
// -----------------------------
//
// DrvPictureSetupBuffer contains sequence, picture, and slice level control
// data required for encoding.
//
// Input pictures (to be encoded) can be any size (HxV) but the encoded output
// is always an integer number of macroblocks high and wide, which means that
// H and V are rounded up to the next multiple of 16 (H'xV') and all pixels
// within this range are used to encode the macroblocks. NVENC does not clear,
// pad, duplicate or otherwise modify any pixels values within the H'xV' sized
// input picture contained in the input surface.
// The values for frame/pic_width_minus1 and frame/pic_height_minus1 contained
// in the config structures are the original width and height (display size)
// of the pictures, and from this the nvenc app will derive the cropping
// information. Cropping will only be done on the right and bottom side of the
// picture.

typedef struct
{
  unsigned int luma_log2_weight_denom              : 3;    // the base 2 logarithm of the denominator for all luma weighting factors. The value of luma_log2_weight_denom shall be in the range of 0 to 7, inclusive
  unsigned int chroma_log2_weight_denom            : 3;    // the base 2 logarithm of the denominator for all chroma weighting factors. The value of chroma_log2_weight_denom shall be in the range of 0 to 7, inclusive.
  unsigned int reversed                            : 26;

  unsigned char luma_weight_l0_flag[16];                   // equal to 1 specifies that weighting factors for the luma component of list 0 prediction are present, luma_weight_l0_flag[8~15] not used now
  unsigned char chroma_weight_l0_flag[16];                 // equal to 1 specifies that weighting factors for the chroma prediction values of list 0 prediction are present, chroma_weight_l0_flag[8~15] not used now

  unsigned char luma_weight_l1_flag[16];                   // have the same semantics as luma_weight_l0_flag, chroma_weight_l0_flag, not used now
  unsigned char chroma_weight_l1_flag[16];                 // not used now

  short        luma_weight_l0[16];                         // is the weighting factor applied to the luma prediction value for list 0 prediction using RefPicList0[ i ]. When luma_weight_l0_flag is equal to 1, the value of luma_weight_l0[ i ] shall be in the range of -128 to 127, inclusive. -128 to 255 for H265
  short        luma_offset_l0[16];                         // is the additive offset applied to the luma prediction value for list 0 prediction using RefPicList0[ i ]. The value of luma_offset_l0[ i ] shall be in the range of -128 to 127, inclusive. -2^9 to 2^9-1 for h265
  short        luma_weight_l1[16];                         // is the weighting factor applied to the luma prediction value for list 0 prediction using RefPicList0[ i ]. When luma_weight_l0_flag is equal to 1, the value of luma_weight_l0[ i ] shall be in the range of -128 to 127, inclusive. -128 to 255 for H265
  short        luma_offset_l1[16];                         // is the additive offset applied to the luma prediction value for list 0 prediction using RefPicList0[ i ]. The value of luma_offset_l0[ i ] shall be in the range of -128 to 127, inclusive. -2^9 to 2^9-1 for h265
  short        chroma_weight_l0[2][16];                    // has the similar meaning with luma_weight/offset
  short        chroma_offset_l0[2][16];                    // has the similar meaning with luma_weight/offset
  short        chroma_weight_l1[2][16];                    // has the similar meaning with luma_weight/offset
  short        chroma_offset_l1[2][16];                    // has the similar meaning with luma_weight/offset

} nvenc_pred_weight_table_s;   //  452 bytes

typedef struct
{
  unsigned short frame_width_minus1;                        // frame width in pixels minus 1, range 0-4095
  unsigned short frame_height_minus1;                       // frame height in pixels minus 1, range 0-4095
  unsigned short sfc_pitch;                                 // pitch of luma plane
  unsigned short sfc_pitch_chroma;                          // pitch of chroma plane
  unsigned short sfc_trans_mode;                            // least 3 significient bits are used to stand for 8 modes, normal, xflip, yflip etc.
  unsigned short reserved2;                                 // pad to int

  // do not use offset from cmod trace as golden, cmod behavior is different.
  unsigned int   luma_top_frm_offset;                       // offset of luma top field or frame in units of 256
  unsigned int   luma_bot_offset;                           // offset of luma bottom field in units of 256. Not used if frame format.
  unsigned int   chroma_top_frm_offset;                     // offset of chroma top field or frame, or offset of chroma U(Cb) plane in planar mode, both in units of 256
  unsigned int   chroma_bot_offset;                         // offset of chroma bottom field, not used if frame format. Or offset of chroma V(Cr) plane in planar mode. Both in units of 256.
  unsigned int   block_height                       : 7;    // BL mapping block height setting
  unsigned int   tiled_16x16                        : 1;    // Surface is 16x16 tiled instead of BL mapped (must be 1 for refpics)
  unsigned int   memory_mode                        : 2;    // 0: semi-planar, 1: planar
  unsigned int   nv21_enable                        : 1;    // the surface format is yuv or yvu
  unsigned int   input_bl_mode                      : 2;    // the input block linear mode: 0~gpu bl; 1~tegra bl; 2~naive bl this is only for cmod and cmod/plugin
  unsigned int   reserved                           : 19;   // pad to NvU32
} nvenc_h264_surface_cfg_s;                                 // 32 bytes

typedef struct
{
  unsigned int   profile_idc                        : 8;    // seq_parameters(_ext) regs
  unsigned int   level_idc                          : 8;
  unsigned int   chroma_format_idc                  : 2;    // 0=monochrome, 1=yuv4:2:0, 2-3=not supported
  unsigned int   pic_order_cnt_type                 : 2;    // only support values 0 and 2
  unsigned int   log2_max_frame_num_minus4          : 4;
  unsigned int   log2_max_pic_order_cnt_lsb_minus4  : 4;
  unsigned int   frame_mbs_only                     : 1;
  //mvc
  unsigned int   stereo_mvc_enable                  : 1;    //if this bit is enabled, the profile_idc should be 128
  unsigned int   separate_colour_plane_flag         : 1;    //if this bit is enabled, the chroma_format_idc should be 3
  unsigned int   lossless_qpprime_flag              : 1;    //if this bit is enabled and QP=0, bypass trans/q and encoded as lossless MB (hori or vert)
} nvenc_h264_sps_data_s;                                    // 4 bytes

typedef struct
{
  unsigned int   pic_param_set_id                   : 8;    // picture parameter set identification
  unsigned int   entropy_coding_mode_flag           : 1;    // select entropy copding mode: cabac, cavlc
  unsigned int   num_ref_idx_l0_active_minus1       : 5;    // number of currently active reference pictures in list 0 minus 1, range 0..30
  unsigned int   num_ref_idx_l1_active_minus1       : 5;    // number of currently active reference pictures in list 1 minus 1, range 0..30
  unsigned int   weighted_bipred_idc                : 2;    // weighted prediction mode for B: only 0 (default) and 2 (implicit) are supported
           int   pic_init_qp_minus26                : 6;    // initial QP value, range -26..+25
           int   chroma_qp_index_offset             : 5;    // offset to add to chroma QPy for QPc table indexing for Cb, range -12..+12

           int   second_chroma_qp_index_offset      : 5;    // offset to add to chroma QPy for QPc table indexing for Cr, range -12..+12
  unsigned int   constrained_intra_pred_flag        : 1;    // if set, intra prediction can only use pixels from macroblocks that are also intra coded
  unsigned int   deblocking_filter_control_present_flag : 1;// if set, deblock filter controls syntax elements will be present/encoded in the stream
  unsigned int   transform_8x8_mode_flag            : 1;    // if set, enables the use of transform size 8x8 (flags will be encoded in stream per macroblock)
  unsigned int   pic_order_present_flag             : 1;    // if set, pic_order control info is encoded in the stream
  unsigned int   weighted_pred_flag                 : 1;
  unsigned int   reserved0                          : 22;   // pad to full int
} nvenc_h264_pps_data_s;                                    // 8 bytes

typedef struct
{
  unsigned int   self_temporal_stamp_l0             : 3;    // stamp to use for L0 integer search in stamp based mode
  unsigned int   self_temporal_stamp_l1             : 3;    // stamp to use for L1 integer search in stamp based mode
  unsigned int   self_temporal_explicit             : 1;    // explicitly evaluate these vectors
  unsigned int   self_temporal_search               : 1;    // integer search enable for this hint
  unsigned int   self_temporal_refine               : 1;    // subpel search enable for this hint
  unsigned int   self_temporal_enable               : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)

  unsigned int   coloc_stamp_l0                     : 3;    // stamp to use for L0 integer search in stamp based mode
  unsigned int   coloc_stamp_l1                     : 3;    // stamp to use for L1 integer search in stamp based mode
  unsigned int   coloc_explicit                     : 1;    // explicitly evaluate these vectors
  unsigned int   coloc_search                       : 1;    // integer search enable for this hint
  unsigned int   coloc_refine                       : 1;    // subpel search enable for this hint
  unsigned int   coloc_enable                       : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)

  unsigned int   self_spatial_stamp_l0              : 3;    // stamp to use for L0 integer search in stamp based mode
  unsigned int   self_spatial_stamp_l1              : 3;    // stamp to use for L1 integer search in stamp based mode
  unsigned int   self_spatial_explicit              : 1;    // explicitly evaluate these vectors
  unsigned int   self_spatial_search                : 1;    // integer search enable for this hint
  unsigned int   self_spatial_refine                : 1;    // subpel search enable for this hint
  unsigned int   self_spatial_enable                : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)
  unsigned int   reserved0                          : 2;    // pad to full int

  unsigned int   external_stamp_l0_refidx0_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 0
  unsigned int   external_stamp_l0_refidx1_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 1
  unsigned int   external_stamp_l0_refidx2_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 2
  unsigned int   external_stamp_l0_refidx3_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 3
  unsigned int   external_stamp_l0_refidx4_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 4
  unsigned int   external_stamp_l0_refidx5_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 5
  unsigned int   external_stamp_l0_refidx6_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 6
  unsigned int   external_stamp_l0_refidx7_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 7
  unsigned int   external_stamp_l1_refidx0_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 0
  unsigned int   external_stamp_l1_refidx1_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 1
  unsigned int   reserved1                          : 2;    // pad to full int


  unsigned int   external_stamp_l1_refidx2_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 2
  unsigned int   external_stamp_l1_refidx3_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 3
  unsigned int   external_stamp_l1_refidx4_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 4
  unsigned int   external_stamp_l1_refidx5_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 5
  unsigned int   external_stamp_l1_refidx6_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 6
  unsigned int   external_stamp_l1_refidx7_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 7


  unsigned int   external_explicit                  : 1;    // explicitly evaluate these vectors
  unsigned int   external_search                    : 1;    // integer search enable for this hint
  unsigned int   external_refine                    : 1;    // subpel search enable for this hint
  unsigned int   external_enable                    : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)

  unsigned int   const_mv_stamp_l0                  : 3;    // stamp to use for L0 integer search in stamp based mode
  unsigned int   const_mv_stamp_l1                  : 3;    // stamp to use for L1 integer search in stamp based mode
  unsigned int   const_mv_explicit                  : 1;    // explicitly evaluate these vectors
  unsigned int   const_mv_search                    : 1;    // integer search enable for this hint
  unsigned int   const_mv_refine                    : 1;    // subpel search enable for this hint
  unsigned int   const_mv_enable                    : 1;    // hint enable (enables processing + fetching of data)

  unsigned int   stamp_refidx1_stamp    : 3;    // Stamp to use for multi-ref hint with refidx = 1
  unsigned int   stamp_refidx2_stamp    : 3;    // Stamp to use for multi-ref hint with refidx = 2
  unsigned int   stamp_refidx3_stamp    : 3;    // Stamp to use for multi-ref hint with refidx = 3
  unsigned int   stamp_refidx4_stamp    : 3;    // Stamp to use for multi-ref hint with refidx = 4
  unsigned int   stamp_refidx5_stamp    : 3;    // Stamp to use for multi-ref hint with refidx = 5
  unsigned int   stamp_refidx6_stamp    : 3;    // Stamp to use for multi-ref hint with refidx = 6
  unsigned int   stamp_refidx7_stamp    : 3;    // Stamp to use for multi-ref hint with refidx = 7
  unsigned int   reserved2              : 11;   // pad to full int

} nvenc_h264_me_hint_cfg_s;                     //16 bytes

typedef struct
{
  unsigned int   mvx_frac                           : 2;    // vector X component fraction
           int   mvx_int                            : 12;   // vector X component integer portion
  unsigned int   reserved_x                         : 2;    // padding X to 16 bit
  unsigned int   mvy_frac                           : 2;    // vector Y component fraction
           int   mvy_int                            : 10;   // vector Y component integer portion
  unsigned int   reserved_y                         : 4;    // padding Y to 16 bit
} nvenc_h264_me_const_vec_s;                                // 4 bytes

typedef struct
{
  unsigned int   bitmask[2]                         ;       // 8x8 search point pattern bitmask
  unsigned int   hor_adjust                         : 1;    // shift hor based on lsb of pred
  unsigned int   ver_adjust                         : 1;    // shift ver based on lsb of pred
  unsigned int   reserved                           : 30;
} nvenc_h264_me_stamp_s;                                    // 12 bytes

typedef struct
{
  unsigned int   mv_limit_enable                    : 1;    // 0=disable, 1=enable set the absolute MV range (Cisco flip camera request)
  unsigned int   left_mvx_frac                      : 2;    // vector X component fraction
           int   left_mvx_int                       : 12;   // vector X component integer portion
  unsigned int   reserved1                          : 1;    // padding X to 16 bit
  unsigned int   top_mvy_frac                       : 2;    // vector Y component fraction
           int   top_mvy_int                        : 10;   // vector Y component integer portion
  unsigned int   reserved2                          : 4;    // padding Y to 16 bit

  unsigned int   right_mvx_frac                     : 2;    // vector X component fraction
           int   right_mvx_int                      : 12;   // vector X component integer portion
  unsigned int   reserved3                          : 2;    // padding X to 16 bit
  unsigned int   bottom_mvy_frac                    : 2;    // vector Y component fraction
           int   bottom_mvy_int                     : 10;   // vector Y component integer portion
  unsigned int   reserved4                          : 4;    // padding Y to 16 bit
} nvenc_h264_me_limit_mv_s;                                 // 8 bytes

typedef struct
{
  unsigned char teb_K                               ;       // TEB K
  unsigned char teb_N                               ;       // TEB N
  unsigned char teb_P                               ;       // TEB P
  unsigned char teb_S                               : 4;    // TEB S
  unsigned char teb_mode                            : 4;    // TEB mode
} nvenc_me_tebif_s;

typedef struct
{
  unsigned int   me_predictor_mode                  : 1;    // 0=MDP, 1=const
  unsigned int   refinement_mode                    : 1;    // 0=hpel, 1=qpel
  unsigned int   lambda_mode                        : 1;    // 0=MDP, 1=const
  unsigned int   const_lambda                       : 11;   // U8.3 format lambda, used if lambda_mode=1
  unsigned int   refine_on_search_enable            : 1;    // 0=disable, 1=enable subpel refine for search candidates
  unsigned int   me_only_mode                       : 1;
  unsigned int   fps_mvcost                         : 1;
  unsigned int   sps_mvcost                         : 1;
  unsigned int   sps_cost_func                      : 1;
  unsigned int   me_icc_mode_mad                    : 1;

  unsigned int   sps_filter                         : 3;
  unsigned int   mc_filter                          : 3;
  unsigned int   vc1_fastuv_mc                      : 1;
  unsigned int   vc1_mc_rnd                         : 1;
  unsigned int   mbc_ctrl_arbitor                   : 2;
  unsigned int   mv_only_enable                     : 1;
  unsigned int   average_mvhint_enable              : 1;

  nvenc_h264_me_limit_mv_s      limit_mv;
  nvenc_h264_me_hint_cfg_s      predsrc;                    // predictor sources
  nvenc_h264_me_const_vec_s     l0_hint;                    // constant MV used as L0 hint
  nvenc_h264_me_const_vec_s     l1_hint;                    // constant MV used as L1 hint
  nvenc_h264_me_const_vec_s     l0_pred;                    // constant MV used as L0 predictor
  nvenc_h264_me_const_vec_s     l1_pred;                    // constant MV used as L1 predictor
  nvenc_h264_me_stamp_s         shape0;                     // priority 0 stamping pattern
  nvenc_h264_me_stamp_s         shape1;                     // priority 1 stamping pattern
  nvenc_h264_me_stamp_s         shape2;                     // priority 2 stamping pattern
  nvenc_h264_me_stamp_s         shape3;                     // priority 3 stamping pattern
  nvenc_h264_me_stamp_s         shape4;                     // priority 4 stamping pattern
  nvenc_h264_me_stamp_s         shape5;                     // priority 5 stamping pattern
  nvenc_h264_me_stamp_s         shape6;                     // priority 6 stamping pattern
  nvenc_h264_me_stamp_s         shape7;                     // priority 7 stamping pattern
  nvenc_me_tebif_s              teb_para;                   // TEB parameter
  unsigned int mbc_mb_size                        : 9;    // MBC size in MB
  unsigned int rmvp_source_half_scaled            : 1;    // only support picture size half scale down in horz&vert
  unsigned int partDecisionMadeByFPP              : 1;    // enable CU32 FPP;
  unsigned int penalty_factor_FPP                 : 6;    // MV with cost no larger than min_cost + ((min_cost * factor) >> 8) will go to SPS, not used from nvenc7
  unsigned int spatial_hint_pattern               : 4;
  unsigned int temporal_hint_pattern              : 6;
  unsigned int Cu16partDecisionMadeByFPP          : 1;    // enable CU16 making part decision
  unsigned int sps_mvp_from_fbm                   : 1;    //1:sps use fbm'MVP and lambda;0:based on me_predictor_mode and lambda_mode to set the mvp and lambda for sps
  unsigned int fbm_select_best_cu16_parttype_num    : 3;    // #winner partitions selected for cu16x16. valid range is 0~7;
  unsigned int fbm_op_winner_num_p_frame            : 3;    // FBM output number of integer winners for each PU. when fbm_op_winner_num_p_frame_cu8 has non-zero value, this field means the PU winners for CU64~CU16, i.e. w/o CU8
  unsigned int fbm_op_winner_num_b_frame_l0         : 3;    // FBM output number of integer winners for each PU
  unsigned int fbm_op_winner_num_b_frame_l1         : 3;    // FBM output number of integer winners for each PU
  unsigned int fbm_select_best_cu32_parttype_num    : 3;    // cu32 select best parttype num
  unsigned int sps_evaluate_merge_cand_num          : 3;    // number of merg candidate for sps to explicit evaluate
  unsigned int fps_quad_thresh_hold                 : 10;
  unsigned int external_hint_order                  : 1;    //0:Insert external hint at first ref; 1:insert external hint at last ref;
  unsigned int coloc_hint_order                     : 1;    //0:Insert coloc hint at first ref; 1: insert coloc hint at last ref.
  unsigned int reserved1                            : 5;
  short        ct_threshold                         ;    // for OFS only, threshold in Census Transform calculation
  //(1<<hint_type0)|(1<<hint_type1)|(1<<hint_type2)|(1<<hint_type3)|(1<<hint_type4) should be equal to 0x1f
  unsigned short hint_type0                        : 3; //0:const hint; 1:spatial hint; 2:temporal hint; 3:coloc hint; 4:external hint
  unsigned short hint_type1                        : 3; //the same as hint_type0
  unsigned short hint_type2                        : 3; //the same as hint_type0
  unsigned short hint_type3                        : 3; //the same as hint_type0
  unsigned short hint_type4                        : 3; //the same as hint_type0
  unsigned short pyramidal_hint_order              : 1;     // for OFS only, used when do pyramidal ME, 1: enable, the hint order will be: if one MB has external hint, use exhint, otherwise use spatial/temproal/const hints. 0: disable.
  unsigned char  left_hint_delay_N                 : 4;     // for OFS only, specify left hint delay MB number(1~8), used for high-perf mode (left_hint_delay_N need to set to 8 to achieve 4K@120FPS). set 0 for using default value, ie 3 for ofs/H265, 8 for H264
  unsigned char  reserved3                         : 4;

  unsigned char  cu8partDecisionMadeByFPP          : 1; //enable CU8x8 partition selection in FBM
  unsigned char  fbm_select_best_cu8_parttype_num  : 3; //#winner partitions selected for cu8x8. valid range is 0~7;
  unsigned char  fbm_op_winner_num_p_frame_cu8     : 3; // FBM output number of integer winners for each PU of CU size 8.
  unsigned char  reserved4                         : 1;

  unsigned char  cu64partDecisionMadeByFPP         : 1; //enable CU64x64 partition selection in FBM
  unsigned char  fbm_select_best_cu64_parttype_num : 3; //#winner partitions selected for cu64x64. valid range is 0~7;
  unsigned char  reserved5                         : 4;

  unsigned char  reserved[33];                              // padding to make me_control=192 to allow 3 reads of 64 in ucode
                                                            //     this is optimal size as 160=5 reads of 32 will exceed dma queue
} nvenc_h264_me_control_s;                                  // 192 bytes

typedef struct
{
  unsigned short intra_refresh_cnt;                         // distance between forced-intra MBs in slice; 0 means OFF
  unsigned short intra_refresh_offs;                        // first forced-intra MB in slice

  unsigned int   intra_luma4x4_mode_enable          : 9;    // bitmask indicating which intra luma 4x4 modes to enable
  unsigned int   intra_luma8x8_mode_enable          : 9;    // bitmask indicating which intra luma 8x8 modes to enable
  unsigned int   intra_luma16x16_mode_enable        : 4;    // bitmask indicating which intra luma 16x16 modes to enable
  unsigned int   intra_chroma_mode_enable           : 4;    // bitmask indicating which intra chroma modes to enable
  unsigned int   inter_penalty_factor_for_ip1       : 6;    // early teminate ip1 intra search if intra cost is larger than inter_cost + ((inter_cost * factor) >> 8)

  unsigned int   l0_part_16x16_enable               : 1;    // enable L0 prediction for 16x16
  unsigned int   l0_part_16x8_enable                : 1;    // enable L0 prediction for 16x8
  unsigned int   l0_part_8x16_enable                : 1;    // enable L0 prediction for 8x16
  unsigned int   l0_part_8x8_enable                 : 1;    // enable L0 prediction for 8x8
  unsigned int   l0_part_8x4_enable                 : 1;    // enable L0 prediction for 8x4
  unsigned int   l0_part_4x8_enable                 : 1;    // enable L0 prediction for 4x8
  unsigned int   l0_part_4x4_enable                 : 1;    // enable L0 prediction for 4x4
  unsigned int   l0_part_reserved                   : 1;    // reserved for future L0 prediction extensions
  unsigned int   l1_part_16x16_enable               : 1;    // enable L1 prediction for 16x16
  unsigned int   l1_part_16x8_enable                : 1;    // enable L1 prediction for 16x8
  unsigned int   l1_part_8x16_enable                : 1;    // enable L1 prediction for 8x16
  unsigned int   l1_part_8x8_enable                 : 1;    // enable L1 prediction for 8x8
  unsigned int   l1_part_reserved                   : 4;    // reserved for future L1 prediction extensions
  unsigned int   bi_part_16x16_enable               : 1;    // enable Bi prediction for 16x16
  unsigned int   bi_part_16x8_enable                : 1;    // enable Bi prediction for 16x8
  unsigned int   bi_part_8x16_enable                : 1;    // enable Bi prediction for 8x16
  unsigned int   bi_part_8x8_enable                 : 1;    // enable Bi prediction for 8x8
  unsigned int   bi_part_reserved                   : 4;    // reserved for future Bi prediction extensions
  unsigned int   bdirect_mode                       : 2;    // 0: disable, 1: spatial, 2: temporal
  unsigned int   bskip_enable                       : 1;    // enable b_skip encoding (requires b_direct encoding)
  unsigned int   pskip_enable                       : 1;    // enable p_skip encoding
  unsigned int   special_reserved                   : 4;    // reserved for future special prediction extensions

           short bias_inter_16x16;                          // bias for inter 16x16 (subtracts from inter 16x16 cost)
           short bias_inter_16x8;                           // bias for inter 16x8  (subtracts from inter 16x8  cost)

           short bias_inter_8x16;                           // bias for inter 8x16  (subtracts from inter 8x16  cost)
           short bias_inter_8x8;                            // bias for inter 8x8   (subtracts from inter 8x8   cost)

           short bias_pskip;                                // bias for skip       (subtracts from skip cost)
           short bias_bdir;                                 // bias for bdirect     (subtracts from bdirect cost)

           short bias_intra_over_inter;                     // bias for any intra   (subtracts from intra cost)
           short bias_intra_16x16;                          // bias for intra 16x16 (subtracts from intra 16x16 cost)

           short bias_intra_8x8;                            // bias for intra 8x8   (subtracts from intra 8x8 cost)
           short bias_intra_4x4;                            // bias for intra 4x4   (subtracts from intra 4x4 cost)

           short bias_intra_most_prob;                      // bias for intra most probable mode
  unsigned short mv_cost_bias;                              // bias applied to internal motion vector cost calculation

  unsigned short intra_nxn_bias_multiplier;                 // 0..255 range; used for internal bias calculation
  unsigned short intra_most_prob_bias_multiplier;           // 0..31 range; used for internal bias calculation

           short pskip_bias_multiplier;                     // S16 range; used for internal bias calculation
           short bdirect_bias_multiplier;                   // S16 range; used for internal bias calculation

  unsigned short pskip_esc_threshold;                       // if pskip cost is below this, disable full search & attempt pskip encode
  unsigned short bdirect_esc_threshold;                     // if bdirect cost is below this, disable full search & attempt bdirect encode

  unsigned short early_intra_disable_mpeb_threshold;        // if early intra cost is below this, disable full search & select intra
  unsigned short tempmv_wt_spread_threshold;                // The threshold for the sum of differences between 8x8s and 16x16 mvs. Used for temporal mv weight
  unsigned int   tempmv_wt_distort_threshold        : 16;   // The threshold for the cost of the 16x16 mv. Used for temporal mv weight
  unsigned int   mv_cost_predictor_control          : 1;    // 0=use 16x16 predictor only; 1=use accurate predictor (slower but better)
  unsigned int   mv_cost_enable                     : 1;    // enable mv cost calculations in mode decision
  unsigned int   intra_most_prob_force_on           : 1;    // always evaluate most probably 4x4 or 8x8 intra mode during full search
  unsigned int   early_intra_mode_control           : 2;    // 0=inter; 1=eval_intra; 2=ext_hint; 3=none (low perf,best qual)
  unsigned int   early_intra_mode_type_16x16dc      : 1;    // evaluate 16x16dc if early_intra_mode_control==1
  unsigned int   early_intra_mode_type_16x16h       : 1;    // evaluate 16x16h  if early_intra_mode_control==1
  unsigned int   early_intra_mode_type_16x16v       : 1;    // evaluate 16x16v  if early_intra_mode_control==1
  unsigned int   early_ip_is_final                  : 1;    // if set, result of early intra is final (disable full intra search)
  unsigned int   chroma_eval_mode                   : 1;    // for intra chroma search 0 = U and V; 1 = U only
  unsigned int   ip_search_mode                     : 3;    // which intra sizes to search before making IP decision b0=4x4,b1=8x8,b2=16x16
  unsigned int   multiply_bias_with_lambda          : 1;    // setting this to 1 will result in bias being multiplied by lambda
  unsigned int   force_ipcm                         : 1;    // 0=normal encode; 1=force mpeb/mpec encode as IPCM
  unsigned int   early_termination_ip1              : 1;    // enable IP1 early termination
           short bias_favor_intra_16x16_early;              // S16 range; used for internal bias calculation
  unsigned short priority_ipred_type_ip1            : 3;    // which intra type search first for early termination, must be a subset of ip_search_mode
  unsigned short intra_ssd_cnt_4x4                  : 4;    // SSD cost evaluation between N top intra modes selected by satd
  unsigned short intra_ssd_cnt_8x8                  : 4;    // SSD cost evaluation between N top intra modes selected by satd
  unsigned short intra_ssd_cnt_16x16                : 4;    // SSD cost evaluation between N top intra modes selected by satd
  unsigned short skip_evaluate_enable               : 1;    // 0: not check skip ssd cost; 1, evaluate skip ssd cost
  unsigned int   rdo_level                          : 3;    // 0:mode decision by satd  1:mode decision by ssd
  unsigned int   tu_search_num                      : 3;    // tu search part num per MB
  unsigned int   luma_residual_zero_eval            : 2;    // luma residual zero cost evaluate, bit0:intra; bit1:inter
  unsigned int   num_1div8_lambda_ssd               : 7;    // number of 1/8 lambda ssd for lambda_coef delta. lambda_coef = (1+num_1div8_lambda_ssd*1/8)*lambda_ssd
  unsigned int   ofs_mvx_range                      : 2;    // control mvx precision/range, 0: ouptput format: S10.5, range[-1024, 1023]; 1: output format: S11.4, range[-2048, 2047]; 2: output format: S12.3, range[-4096, 4095]; 3: output format: S12.2, range[-4096, 4095]
  unsigned int   reserved4                          : 15;   // pad to 32 bytes
  short          bias_tu_4x4;                               // bias for TU 4x4
  short          bias_tu_8x8;                               // bias for TU 8x8
  unsigned int   ssim_rdo                           : 2;    // SSIM-RDO: 0 disable, 1: enable SW SSIM-RDO, 2: enable HW SSIM-RDO,
  unsigned int   calc_ssim                          : 1;    // Calculate SSIM distortion: 0 disable, 1: enable. ssim_rdo = 1 => calc_ssim = 1
  unsigned int   reserved5                          : 29;   // reserved
  unsigned int   ssd_over_ssim_factor;                      // distortion ssd/ssim ratio, in U24.8 format
  unsigned int   reserved6[13];                             // reserved
} nvenc_h264_md_control_s;                                  // 128 bytes

typedef struct
{
  unsigned short qpp_run_vector_4x4;                        // cost values for 4x4 transform (16 bit total vector)
  unsigned short qpp_run_vector_8x8[3];                     // cost values for 8x8 transform (48 bit total vector)
                                                            // 2 bits each for first 12 coefs, then 1 bit each for next 24
  unsigned char  qpp_luma8x8_cost;                          // luma 8x8 cost threshold, 0 = throw out all coefs, range 0-15
  unsigned char  qpp_luma16x16_cost;                        // luma 16x16 cost threshold, 0 = throw out all coefs, range 0-15
  unsigned char  qpp_chroma_cost;                           // chroma cost threshold, 0 = throw out all coefs, range 0-15
  unsigned char  qpp_mode                           : 2;    // 0 = OFF, 1 = 8x8, 2 = 16x16_8x8
  unsigned char  reserved1                          : 4;    // padding
    //add by cl
  unsigned char  quant_intra_sat_flag               : 1;    // 0: no saturation is applied; 1: saturation is applied
  unsigned char  quant_inter_sat_flag               : 1;    // 0: no saturation is applied; 1: saturation is applied
  unsigned int   quant_intra_sat_limit              : 16;   // quantization saturation limit for intra MB
  unsigned int   quant_inter_sat_limit              : 16;   // quantization saturation limit for inter MB
    //~
  unsigned short dz_4x4_YI[16];                             // deadzone for 4x4 transform of Luma Intra
  unsigned short dz_4x4_YP[16];                             // deadzone for 4x4 transform of Luma Inter
  unsigned short dz_4x4_CI;                                 // deadzone for 4x4 transform of Chroma Intra
  unsigned short dz_4x4_CP;                                 // deadzone for 4x4 transform of Chroma Inter
  unsigned short dz_8x8_YI[16];                             // deadzone for 8x8 transform of Luma Intra, 16 values are mapped to 64 coefs
  unsigned short dz_8x8_YP[16];                             // deadzone for 8x8 transform of Luma Inter, 16 values are mapped to 64 coefs
  unsigned   int reserved2[11];                             // pad to 192 bytes to allow 3 reads of 64 bytes in ucode
                                                            //     this is optimal size as 160=5 reads of 32 will exceed dma queue
} nvenc_h264_quant_control_s;                               // 192 bytes

typedef struct
{
    unsigned int reorder_l0_cmd_count                 : 4;
    unsigned int reorder_l1_cmd_count                 : 4;
    unsigned int mmco_cmd_count                       : 4;
    unsigned int no_output_of_prior_pic_flag          : 1;
    unsigned int long_term_ref_flag                   : 1;
    unsigned int reserved                             :18;
} nvenc_h264_refpic_cmd_s;                                     // 4 bytes

typedef struct
{
  // ref_pic_list_reorder_cmd includes both opcode and op arguments
    unsigned int   reordering_of_pic_nums_idc         : 3;     //  Bit 31-29 (0-5  (Table 7-7)
    unsigned int   abs_diff_pic_num_minus1            :17;     //  (opcode=0/1)
    unsigned int   long_term_pic_num                  : 4;     //  (opcode=2):
    unsigned int   abs_diff_view_idx_minus1           : 4;     //  (opcode=4/5):
    unsigned int   reserved                           : 4;
} nvenc_h264_ref_pic_reorder_s;                                // 4 bytes

typedef struct
{
    unsigned int   mmco_cmd_id                        : 3;     // totally 6 commands

    unsigned int   difference_of_pic_nums_minus1      :17;     // 1) mark a short_term ref as long (mmco=3); 2) mark a short_term as un-used (mmco=1)
    unsigned int   long_term_pic_num                  : 3;     // [0 to MaxLongTermFrameIdx ]; mark a long as "unused for ref" (mmco=2);
    unsigned int   long_term_frame_idx                : 3;     // [0 to MaxLongTermFrameIdx] 1) mark a picture to long (mmco=3, mmco=6)
    unsigned int   max_long_term_frame_idx_plus1      : 4;     // [0, num_ref_frames] max long term frm indexes allowed for long term pictures.
                                                               // max_long_term_frame_idx_plus1=0 -> MaxLongTermFrameIdx="no-long-term"
                                                               // max_long_term_frame_idx_plus1>0 -> MaxLongTermFrameIdx= max_long_term_frame_idx_plus1-1
    unsigned int   reserved                           : 2;
} nvenc_h264_mmco_s;                                           // 4 bytes

typedef struct
{
  // int0
  unsigned int   num_mb                            : 19;     // number of macroblocks in this slice, support upto 8kx8k
  unsigned int   qp_avr                            :  8;     // 6bit value used by hw. Other bits can be used for rounding.
  unsigned int   reserved                          :  5;
  // int1
  unsigned int   slice_tgt_rate;                            // target bit rate (size) (if RC mode == slice)

  // int2
           char  slice_alpha_c0_offset_div2         : 4;
           char  slice_beta_offset_div2             : 4;
  unsigned char  qp_slice_min;                              // min slice qp value
  unsigned short qp_slice_max                       : 8;    // max slice qp value
  unsigned short force_intra                        : 1;    // force entire slice to be intra
  unsigned short disable_deblocking_filter_idc      : 2;
  unsigned short cabac_init_idc                     : 2;
  unsigned short disable_slice_header               : 1;    // if set to 1, disables HW slice header output
  unsigned short reserved0                          : 2;

  // int3
  unsigned int   num_ref_idx_active_override_flag   : 1;
  unsigned int   num_ref_idx_l0_active_minus1       : 5;    // [0.31]
  unsigned int   num_ref_idx_l1_active_minus1       : 5;    // [0,31]
  unsigned int   reserved1                          : 9;    // padding
  unsigned int   me_control_idx                     : 4;    // index in array of nvenc_h264_me_control_s to use for this slice
  unsigned int   md_control_idx                     : 4;    // index in array of nvenc_h264_md_control_s to use for this slice
  unsigned int   q_control_idx                      : 4;    // index in array of nvenc_h264_quant_control_s to use for this slice

  // int 4
  unsigned int   limit_slice_top_boundary           : 1;    // limit ME to slice boundaries
  unsigned int   limit_slice_bot_boundary           : 1;    // limit ME to slice boundaries
  unsigned int   limit_slice_left_boundary          : 1;    // limit ME to slice boundaries
  unsigned int   limit_slice_right_boundary         : 1;    // limit ME to slice boundaries
  unsigned int   ROI_enable                         : 1;    // Enable region  of interest processing
  unsigned int   ROI_me_control_idx                 : 4;
  unsigned int   ROI_md_control_idx                 : 4;
  unsigned int   ROI_q_control_idx                  : 4;
  unsigned int   ROI_qp_delta                       : 8;    // QP delta in ROI relative to picture outside ROI
  unsigned int   wp_control_idx                     : 4;    // index in array of nvenc_pred_weight_table_s to use for this slice
  unsigned int   reserved2                          : 3;    // reserved for alignment

  // int 5
  unsigned int   ROI_top_mbx                        : 8;    // top left mbx of the ROI
  unsigned int   ROI_top_mby                        : 8;
  unsigned int   ROI_bot_mbx                        : 8;    // bottom right mbx of the ROI
  unsigned int   ROI_bot_mby                        : 8;

  // int 6: ref_pic_cmd_cfg
  nvenc_h264_refpic_cmd_s        ref_pic_cmd_cfg;

  // int 7-22: RefPicListReorder/Modification: Defined in Section 7.4.3.1
  nvenc_h264_ref_pic_reorder_s   ref_pic_list_reorder_cmd[2][8];     // size shall not exceed num_ref_idx_lx_active_minus1 + 1 (8+8=16)

  //int 23-30: MMCO: Defined in Section: 7.4.3.3
  nvenc_h264_mmco_s              mmco_cmd[8] ;

  // int 31
  unsigned int   reserved3;

} nvenc_h264_slice_control_s;                               // 128 bytes


// Quarter-resolution modifier flags for first_pass_source_half_scaled
#define NVENC_QRES_FLAG_ENABLE          0x1     // Set if quarter-resolution is enabled
#define NVENC_QRES_FLAG_DECIMATED       0x10    // Set to indicate that subsampling was performed with decimation rather than low-pass
#define NVENC_QRES_FLAG_PASS2REFS       0x20    // Set to indicate that 1st-pass uses subsampled 2nd pass references instead of self-generated references

// Lookahead macros for ab_beta(INFO1) and prev_act(INFO2)
#define NVENC_LOOKAHEAD_INFO1(Wp2Wi)        ((unsigned char)(Wp2Wi))    // clamp((int)(Wp*256.0/Wi), 0, 255) -> ratio of inter over intra complexity, 0=no lookahead
// Lookahead INFO2: (depth<<8)|(nextI)
// nextI: 1..255=distance to next scene change, 0=unknown or greater than lookahead depth;
// depth: 1..255=max lookahead depth (0=no lookahead)
#define NVENC_LOOKAHEAD_INFO2(nextI,depth)  (((unsigned short)(nextI)) | (((unsigned short)(depth))<<8))

// HRD conformance and misc RC flags
#define NVENC_RC_HRD_VCL                0x1 // vcl_cpb_size and vcl_bitrate
#define NVENC_RC_HRD_NAL                0x2 // nal_cpb_size and nal_bitrate
#define NVENC_RC_HRD_STRICTGOPTARGET    0x4 // Strict GOP target (minimizes GOP-to-GOP rate fluctuations)
#define NVENC_RC_FIXED_PBRATIO          0x8U // Disable dynamic B-frame rhopbi adjustments

 // needed only if picture level RC is enabled; as part of picture level RC, it will also perform HRD verification
 // The contents of this file is produced by the driver during sequence level operations and should not be modified.
typedef struct
{
  unsigned char  hrd_type;                                  // nal (=2) and vcl (=1) type
  unsigned char  QP[3];                                     // QP for 0:P picture, ,1:B picture 2; I picture
  unsigned char  minQP[3];                                  // min QP for 0:P picture, ,1:B picture 2; I picture
  unsigned char  maxQP[3];                                  // max QP for 0:P picture, ,1:B picture 2; I picture
  unsigned char  maxQPD;                                    // max QP delta between two consecutive QP updates
  unsigned char  baseQPD;                                   // initial QP delta between two consecutive QP updates
           int   rhopbi[3];                                 // 23.8 signed fixed point quant ratios P/I, B/I, I/I(=1)

           int   framerate;                                 // fps
  unsigned int   buffersize;                                // total buffer size

           int   nal_cpb_size;                              // size in bits
           int   nal_bitrate;                               // rate in bps
           int   vcl_cpb_size;                              // size in bits
           int   vcl_bitrate;                               // rate in bps

  unsigned int   gop_length;                                // I period, gop_length == 0xffffffff is used for infinite gop length
           int   Np;                                        // 27.4 signed fixed (gop_length + num_b_frames -1)/(num_b_frames + 1)
           int   Bmin;                                      // 23.8 signed fixed point min buffer level, updated by driver at sequence level only.
           int   Ravg;                                      // 23.8 signed fixed point average rate, updated by driver at sequence level only.
           int   R;                                         // 23.8 signed fixed point rate

  unsigned char  ab_alpha;                                  // VBR/CBR: min/target quality level (1..51); rcmode4: weight of prev frame activity compared to part of the current picture
  unsigned char  ab_beta;                                   // lookahead info1: ratio of inter over intra complexity (fix8); rcmode4: weight of current activity compared to neighbors
  unsigned short prev_act;                                  // lookahead info2: nextI, depth and valid flag; rcmode4: average activity level of the first reference (L0)

  unsigned char  aqMode                             : 3;    // 0: disable adaptive quantization, 1: AQ mode fast, 2: AQ mode full
  unsigned char  dump_aq_stats                      : 1;    // 1: reads aq stats
  unsigned char  is_emphasis_level                  : 1;    // 1: enables emphasis level map, treats QP map as level map
  unsigned char  reserve0                           : 3;    // Reserved for alignment
  unsigned char  single_frame_VBV;                          // 1: VBV buffer size is set to average frame size; 0: otherwise
  unsigned char  two_pass_rc;                               // 0: single pass rc , 1: first pass of 2 pass rc 2: second pass of 2 pass rc
  unsigned char  rc_class;                                  // reserved

  unsigned char  first_pass_source_half_scaled;             // 0 : first pass on half resolution 1: first pass on full resolution (along with NVENC_QRES_FLAG_XXX)
  unsigned char  iSizeRatioX;                               // ratio between I picture target size over average picture size numerator
  unsigned char  iSizeRatioY;                               // ratio between I picture target size over average picture size denominator
  unsigned char  ext_scene_change_flag;                     // Scene change flag set as hint by external preprocessing unit. 0: No scene change, 1: Current picture is first in scene
  unsigned int   ext_intra_satd;                            // If non zero this represents the intra SATD for current picture computed by external preprocessing unit
  unsigned char  ext_picture_rc_hints;                      // picture rc hints are set by external source. 0: no hints, 1: hints are avaialble for current picture
  unsigned char  session_max_qp;                            // Current frame qp will never exceed this value when ext_picture_rc_hints is set
  unsigned char  reserve[2];
} nvenc_h264_rc_s;                                          // 88 bytes

typedef struct
{
  unsigned int   var_min;
  unsigned int   var_max;
  unsigned int   var_avg;
  unsigned char  reserved[4];
} nvenc_aq_stat_s;                                         // 16 bytes

typedef struct
{
           char  l0[8];                                     // reference picture list 0
           char  l1[8];                                     // reference picture list 1
           char  temp_dist_l0[8];                           // temporal distance of each ref pic in list 0 DiffPicOrderCnt( currPicOrField, pic0) )
           char  temp_dist_l1[8];                           // temporal distance of each ref pic in list 1 DiffPicOrderCnt( currPicOrField, pic1) )
           short dist_scale_factor[8][8];                   // (h264 spec eq 8-203) array [refidx1][refidx0] signed 11 bit values
  unsigned int   diff_pic_order_cnt_zero[2];                // This is a 2-dimensional array of booleans (1 bit flags) indicating that DiffPicOrderCnt(refidx1, refidx0) == 0
  unsigned int   max_slice_size;                            // for use in slice_mode 1 only:  dynamic insertion at/before this boundary (#bytes)
  unsigned int   max_byte_count_before_resid_zero;          // Maximum byte count before micrcode forces the residuals to zero.
           int   delta_pic_order_cnt_bottom;                // direct value of syntax element to be put in every slice of the picture
  unsigned short frame_num;                                 // value for slice header syntax element
  unsigned short pic_order_cnt_lsb;                         // value of the corresponding syntax element
  unsigned short idr_pic_id;
  unsigned short colour_plane_id;
  unsigned int   longTermFlag;                              // Bits 7:0 for pictures in RefPicList0 and bits 23:16 for pictures in RefPicList1
  unsigned char  num_forced_slices_minus1;                  // number of forced slice boundary locations minus1(number of entries in h264_slice_control_s)
  unsigned char  num_me_controls_minus1;                    // number of nvenc_h264_me_control_s entries in array minus1
  unsigned char  num_md_controls_minus1;                    // number of nvenc_h264_md_control_s entries in array minus1
  unsigned char  num_q_controls_minus1;                     // number of nvenc_h264_quant_control_s entries in array minus1
  unsigned int   slice_control_offset;                      // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                            // to start of array of "nvenc_h264_slice_control_s" structures
  unsigned int   me_control_offset;                         // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                            // to start of array of "nvenc_h264_me_control_s" structures
  unsigned int   md_control_offset;                         // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                            // to start of array of "nvenc_h264_md_control_s" structures
  unsigned int   q_control_offset;                          // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                            // to start of array of "nvenc_h264_quant_control_s" structures
  unsigned int   hist_buf_size;                             // size in bytes of the buffer allocated for history
  unsigned int   bitstream_buf_size;                        // size in bytes of the buffer allocated for bitstream slice/mb data
  unsigned int   bitstream_start_pos;                       // start position in bitstream buffer where data should be written (byte offset)
  unsigned int   pic_struct                         : 2;    // 0 = frame, 1 = top/first field, 2 = bot/second field
  unsigned int   pic_type                           : 2;    // 0 = P, 1 = B, 2 = I, 3 = IDR
  unsigned int   ref_pic_flag                       : 1;    // reference picture (0 = no, 1 = yes)
  unsigned int   slice_mode                         : 1;    // 0 = dynamic slice insertion based on slice size
                                                            // 1 = static slice insertion based on slice control array
                                                            //     (insert slices as defined in array, if all array elements used
                                                            //      before end of picture is reached, start over with entry 0)
  unsigned int   ipcm_rewind_enable                 : 1;
  unsigned int   cabac_zero_word_enable             : 1;

  //mvc
  unsigned int   base_view                          : 1;    // 1: current view component is a base view; 0: non-base view
  unsigned int   priority_id                        : 6;    // priority identifier for the NAL unit
  unsigned int   view_id                            : 10;   // view identifier for the NAL unit
  unsigned int   temporal_id                        : 3;    // temporal identifier for the NAL unit
  unsigned int   anchor_pic_flag                    : 1;    // equal to 1 specifies that the current access unit is an anchor access unit
  unsigned int   inter_view_flag                    : 1;    // equal to 1 specifies that the current view component may be used for inter-view prediction by other view components in the current access unit
  unsigned int   opm_enable                         : 1;    // check with OPM if encryption needed
  unsigned int   temporal_hint8x8_enable            : 1;    // 8x8 hint enable
  char           cur_interview_ref_pic;                     //current interview ref pic
  char           prev_interview_ref_pic;                    //previous interview ref pic to replace current interview ref pic when write the regs ME_MBC_REFLIST0_DPBLUT and ME_MBC_REFLIST1_DPBLUT
  unsigned char  codec                              : 3;
  unsigned char  e4byteStartCode                    : 1;    // if enable to 1, all the slices in a picture will use 4 byte start code 00000001(used in rtp mode), original is 000001
  unsigned char  qpfifo                             : 1;    // 0 = Polling mode, 1 = Interrupt mode.
  unsigned int   me_candidates_output_struct        : 1;    // 0 = me-only mode, 1 = MV & cost for all blocks (hybrid format)
  unsigned int   me_candidates_output_enable        : 1;    // hybrid mode results configuration (see me_candidates_output_struct). 0 = results disabled. 1 = results enabled (for fullscale image).
  unsigned char  medma_output_cost_mode             : 1;    // 0 = residual cost only. 1 = residual cost + mv cost
  unsigned char  intraRefreshCount;                         // Set to number of pictures over which intra refresh will happen. Set to 0 for non intra refresh picture
  unsigned int   mpec_threshold                     : 30;   // collect mpec stats after threshold mbs
                                                            // when mpec_stat_on is enabled and mpec_threshold is not equal to picWidthInMbs,
                                                            // rc mode 0 will be used to collect stats
  unsigned int   me_lambda_fifo_enable              : 1;    // lambda map fifo enable. 1: enable. 0: disable
  unsigned int   me_candidates_rdo_mode             : 1;    // me candidates rdo mode enable. 1: enable. 0: disable.
  unsigned int   slice_stat_offset;                         // offset from start of top level "nvenc_stat_data_s" structure
                                                            // to start of array of "nvenc_slice_stat_s" structure
  unsigned int   mpec_stat_offset;                          // offset from start of top level "nvenc_stat_data_s" structure
                                                            // to start of array of "nvenc_mpec_stat_s" structure
  unsigned int   wp_control_offset;                         // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                            // to start of array of "nvenc_pred_weight_table_s" structure
                                                            // to start of array of "nvenc_mpec_stat_s" structure

  unsigned char  num_wp_controls_minus1;                    // number of nvenc_pred_weight_table_s entries in array minus 1
  unsigned char  b_as_ref                           : 1;    // use b frame as reference
  unsigned char  ofs_mode                           : 2;    // ofs mode. 0: OFF; 1: Optical Flow; 2: Stereo (output mvx only), 3: Stereo (output mvx/mvy)
  unsigned char  new_subframe                       : 1;
  unsigned char  bit_depth_minus_8                  : 4;
  unsigned char  stripID;
  unsigned char  strips_in_frame;
  unsigned short slice_encoding_row_start;                  // start mb row for slice encoding
  unsigned short slice_encoding_row_num;                    // num of mb row for slice encoding

  unsigned char  segment_enable                     : 1;    // used by ofs mode only for segment map enable flag, 0: (default) disable/ 1: enable
  unsigned char  background_seg                     : 5;    // used by ofs mode only when segment map is enabled, to specify the segment id of background
  unsigned char  input_sub_sampling                 : 1;    // used by PDMA 2x2 sub-sampling
  unsigned char  output_sub_sampling                : 1;    // used by MPEB 2x2 sub-sampling output
  unsigned char  segment_P2                            ;
  unsigned char  segment_thr                           ;
  unsigned char  accurate_skip_mv                   : 1;
  unsigned char  intra_sel_4x4_enable               : 1;    // seletive intra4x4 , 0:disable(always do intra4x4 serach), 1:enable(enable or disable intra4x4 search based on pixel variance)
  unsigned int   enable_source_image_padding        : 1;    // enable/disable HW source image padding
  unsigned int   luma_padding_mode                  : 2;    // HW souce padding mode for luma: 0: copy frame boundary, 1: zero padding, 2: half value, i.e 1<<(bitdepth-1)
  unsigned int   chroma_padding_mode                : 2;    // HW souce padding mode for chroma: 0: copy frame boundary, 1: zero padding, 2: half value, i.e 1<<(bitdepth-1)
  unsigned int   stats_fifo_enable                  : 1;    // enable per-mb short stats fifo
  unsigned int   chroma_skip_threshold_4x4         : 16;    // low 3 bits is the factional, to check if chroma residual is big in skip mb, can't simply set chroma residual to 0 even though luma has been decided to skip
  unsigned int   intra_sel_4x4_threshold           : 16;    // disable intra4x4 search when intra_sel_4x4_enable=true and max variance of 8x8 blocks in current MB is less than intra_sel_4x4_threshold
  unsigned int   aq_stat_offset;                            // start of array of "nvenc_aq_stat_s" structure
  unsigned int   act_stat_offset;                           // offset from start of "nvenc_rc_pic_stat_s" structure,
                                                            // in the RC stats buffer. This field points to the start
                                                            // of data for external (delta-)QP map.
  unsigned int   stats_fifo_offset;                         // offset from start of top level "nvenc_stat_data_s" structure
} nvenc_h264_pic_control_s;                                 // 276 bytes


// This is the top level config structure passed by driver to nvenc. It must be aligned
// to a 256B boundary. The first integer in this struct is a 'magic' number which driver
// must set to a specific value, and which the main Falcon app must check to see if it
// matches the expected value. This prevents running wrong combinations of traces/drvr
// code and falcon code. Encoding is as follows:
//  Bit 31..16 = 0xc9b7
//  Bit 15..8  = Version number
//  Bit 7..0   = Revision number
// This number changes whenever there is a change to the class. If the change is small and
// backward compatible, only the revision number is incremented. If the change is major or
// not backward compatible, the revision number is reset to zero and the version number
// incremented. Falcon app should check if the upper 24 bits match the expected value, and
// terminate with error code "NvC9B7EncErrorH264BadMagic" if there is a mismatch. The value
// for the current class is defined as:

#ifdef NV_NVENC_8_2
#define NV_NVENC_DRV_MAGIC_VALUE  0xC9B70006
#elif NV_NVENC_8_0
#define NV_NVENC_DRV_MAGIC_VALUE  0xC8B70006
#elif NV_NVENC_7_3
#define NV_NVENC_DRV_MAGIC_VALUE  0xC7B70006
#elif NV_NVENC_7_2
#define NV_NVENC_DRV_MAGIC_VALUE  0xC4B70006
#elif NV_NVENC_7_0
#define NV_NVENC_DRV_MAGIC_VALUE  0xC5B70006
#elif NV_NVENC_6_8
#define NV_NVENC_DRV_MAGIC_VALUE  0xB6B70006
#elif NV_NVENC_6_6
#define NV_NVENC_DRV_MAGIC_VALUE  0xB4B70006
#elif NV_NVENC_6_4
#define NV_NVENC_DRV_MAGIC_VALUE  0xC3B70006
#elif NV_NVENC_6_2
#define NV_NVENC_DRV_MAGIC_VALUE  0xC2B70006
#elif NV_NVENC_6_0
#define NV_NVENC_DRV_MAGIC_VALUE  0xc1b70006
#elif NV_NVENC_5_0
#define NV_NVENC_DRV_MAGIC_VALUE  0xd0b70006
#elif NV_NVENC_1_0
#define NV_NVENC_DRV_MAGIC_VALUE  0xc0b70006
#elif NV_MSENC_2_0
#define NV_MSENC_DRV_MAGIC_VALUE  0xa0b70006
#else
#define NV_MSENC_DRV_MAGIC_VALUE  0x90b70006
#endif

#define HIST_BLOCK_SIZE  192
// Move all local define of these values to this file
//align to current AV1HIST MAS
const static int AV1_HIST_PIXEL_FRAME_SIZE  =  (8 * 8192);
const static int AV1_HIST_MV_SB_SIZE        =  (256 * 4);

typedef struct
{
  // 256 aligned
  unsigned int                      magic;                  // magic number, see text above              4 bytes
  nvenc_h264_surface_cfg_s          refpic_cfg;             // defines layout of reference pictures     32 bytes
  nvenc_h264_surface_cfg_s          input_cfg;              // defines layout of input pictures         32 bytes
  nvenc_h264_surface_cfg_s          outputpic_cfg;          // defines layout of reconstructed pictures 32 bytes
  nvenc_h264_sps_data_s             sps_data;               //                                           4 bytes
  nvenc_h264_pps_data_s             pps_data;               //                                           8 bytes
  nvenc_h264_rc_s                   rate_control;           // Rate Control information                 88 bytes
  nvenc_h264_pic_control_s          pic_control;            //                                         276 bytes
  nvenc_h264_surface_cfg_s          half_scaled_outputpic_cfg; // defines layout of 2x2 subsampled reconstructed picture 32 bytes
  unsigned int                      gpTimer_timeout_val;    // GPTimer cycle count set from driver       4 bytes

} nvenc_h264_drv_pic_setup_s;                               // 512 bytes


// ****************************************************************************
// ****************************************************************************
// H265
// ****************************************************************************
// ****************************************************************************
typedef struct
{
  unsigned short frame_width_minus1;                        // frame width in pixels minus 1, range 0-4095
  unsigned short frame_height_minus1;                       // frame height in pixels minus 1, range 0-4095
  unsigned short sfc_pitch;                                 // pitch of luma plane
  unsigned short sfc_pitch_chroma;                          // pitch of chroma plane
  unsigned short sfc_trans_mode;                            // least 3 significient bits are used to stand for 8 modes, normal, xflip, yflip etc.
  unsigned short reserved2;                                 // pad to int

  unsigned int   luma_top_frm_offset;                       // offset of luma top field or frame in units of 256
  unsigned int   luma_bot_offset;                           // offset of luma bottom field in units of 256. Not used if frame format.
  unsigned int   chroma_top_frm_offset;                     // offset of chroma top field or frame, or offset of chroma U(Cb) plane in planar mode, both in units of 256
  unsigned int   chroma_bot_offset;                         // offset of chroma bottom field, not used if frame format. Or offset of chroma V(Cr) plane in planar mode. Both in units of 256.
  unsigned int   block_height                       : 7;    // BL mapping block height setting
  unsigned int   tiled_16x16                        : 1;    // Surface is 16x16 tiled instead of BL mapped (must be 1 for refpics)
  unsigned int   memory_mode                        : 2;    // 0: semi-planar, 1: planar
  unsigned int   nv21_enable                        : 1;    // the surface format is yuv or yvu
  unsigned int   input_bl_mode                      : 2;    // the input block linear mode: 0~gpu bl; 1~tegra bl; 2~naive bl this is only for cmod and cmod/plugin
  unsigned int   reserved                           : 19;   // pad to NvU32
} nvenc_h265_surface_cfg_s;                                 // 32 bytes

typedef struct
{
  unsigned int   chroma_format_idc                  : 2;    // 0=monochrome, 1=yuv4:2:0, 3=yuv4:4:4, 2=not supported
  unsigned int   separate_colour_plane_flag         : 1;    //if this bit is enabled, the chroma_format_idc should be 3
  unsigned int   log2_max_pic_order_cnt_lsb_minus4  : 4;
  unsigned int   min_cu_size                        : 2;    //0-8x8, 1-16x16, 2-32x32, 3-64x64
  unsigned int   max_cu_size                        : 2;    //0-8x8, 1-16x16, 2-32x32, 3-64x64
  unsigned int   min_tu_size                        : 2;    //0-4x4, 1-8x8, 2-16x16, 3-32x32
  unsigned int   max_tu_size                        : 2;    //0-4x4, 1-8x8, 2-16x16, 3-32x32
  unsigned int   reserved0                          : 1;    // pad to full short

  unsigned int   max_transform_depth_inter          : 2;    // max_transform_hierarchy_depth_inter
  unsigned int   max_transform_depth_intra          : 2;    // max_transform_hierarchy_depth_intra
  unsigned int   pcm_enabled_flag                   : 1;    // if this bit is 1 , PCM data is present is the sequence
  unsigned int   pcm_loop_filter_disable_flag       : 1;    // Disable loop filter and SAO for CU with pcm_flag to 1
  unsigned int   long_term_ref_pics_present_flag    : 1;    // if set to 1 , it indicates long term pictures are used for inter prediction.
  unsigned int   amp_enabled_flag                   : 1;    // if set  asymmetric motion partitions can be used for inter predictions.
  unsigned int   strong_intra_smoothing_enabled_flag : 1;   // if set , bilinear interpolation is conditionally used filtering process for intra prediction.
  unsigned int   num_short_term_ref_pic_sets        : 7;    // max value is 64
  unsigned int   num_long_term_ref_pics_sps         : 5;
  unsigned int   bit_depth_minus_8                  : 4;    // luma/chroma should have the same value
  unsigned int   sample_adaptive_offset_enabled_flag : 1;   // set 1 specifies that the sample adaptive offset process is applied to the reconstructed picture after the deblocking filter process
  unsigned int   sps_temporal_mvp_enabled_flag      : 1;    // if set, slice_temporal_mvp_enabled_flag is present in slice header
  unsigned int   reserved1                          : 21;
} nvenc_h265_sps_data_s;                                    // 8 bytes

typedef struct
{
  unsigned int   pic_param_set_id                            : 8;    // picture parameter set identification
  unsigned int   num_ref_idx_l0_active_minus1                : 5;    // number of currently active reference pictures in list 0 minus 1, range 0..30
  unsigned int   num_ref_idx_l1_active_minus1                : 5;    // number of currently active reference pictures in list 1 minus 1, range 0..30
  unsigned int   constrained_intra_pred_flag                 : 1;    // if set, intra prediction can only use pixels from macroblocks that are also intra coded
  unsigned int   transquant_bypass_enabled_flag              : 1;    // if set, transform ,scaling and inloop filtering is bypassed.
  unsigned int   transform_skip_enabled_flag                 : 1;    // if set , it enables transform bypass during residual coding.
  unsigned int   deblocking_filter_override_enabled_flag     : 1;    // it set, the deblocking_filter_override_flag is encoded in slice segment header
  unsigned int   pps_loop_filter_across_slices_enabled_flag  : 1;    // if set, it enables loop filter across slice boundary.
  unsigned int   cu_qp_delta_enabled_flag                    : 1;    // if set , it enables cu_qp_delta_abs is encoded at coding unit layer.
  unsigned int   lists_modification_present_flag             : 1;    // if set , it indicates reference picture list modification command might be present is slice segment header.
  unsigned int   output_flag_present_flag                    : 1;    // if set, pic_output syntax element is added to slice syntax header
  unsigned int   cabac_init_present_flag                     : 1;    // if set, cabac_init_flag is present in slice segment header.
  unsigned int   pps_slice_chroma_qp_offsets_present         : 1;    // if set to 1, cb_qp_offset and cr_qp_offset is signalled in the slice segment header.
  unsigned int   weighted_pred_flag                          : 1;    // weighted_pred_flag equal to 1 specifies that weighted prediction is applied to P slices
  unsigned int   weighted_bipred_flag                        : 1;    // weighted_bipred_flag equal to 1 specifies that weighted prediction is applied to B slices
  unsigned int   reserved                                    : 2;

           int   pic_init_qp_minus26                : 6;    // initial QP value, range -26..+25. -38...+25 for 10bit,  -74...+25 for 16bits. TODO: eable this syntex for pps
           int   pps_cb_qp_offset                   : 5;
           int   pps_cr_qp_offset                   : 5;
  unsigned int   dependent_slice_segments_enabled_flag : 1;
  unsigned int   sign_data_hiding_enabled_flag      : 1;
  unsigned int   diff_cu_qp_delta_depth             : 2;
  unsigned int   log2_parallel_merge_level_minus2   : 3;
  unsigned int   num_extra_slice_header_bits        : 3;

           int   reserved1                          : 6;
} nvenc_h265_pps_data_s;                                    // 8 bytes

 // needed only if picture level RC is enabled; as part of picture level RC, it will also perform HRD verification
 // The contents of this file is produced by the driver during sequence level operations and should not be modified.
typedef struct
{
  unsigned char  hrd_type;                                  // nal (=2) and vcl (=1) type
  unsigned char  QP[3];                                     // QP for 0:P picture, ,1:B picture 2; I picture
  unsigned char  minQP[3];                                  // min QP for 0:P picture, ,1:B picture 2; I picture
  unsigned char  maxQP[3];                                  // max QP for 0:P picture, ,1:B picture 2; I picture
  unsigned char  maxQPD;                                    // max QP delta between two consecutive QP updates
  unsigned char  baseQPD;                                   // initial QP delta between two consecutive QP updates
           int   rhopbi[3];                                 // 23.8 signed fixed point quant ratios P/I, B/I, I/I(=1)

           int   framerate;                                 // fps
  unsigned int   buffersize;                                // total buffer size

           int   nal_cpb_size;                              // size in bits
           int   nal_bitrate;                               // rate in bps
           int   vcl_cpb_size;                              // size in bits
           int   vcl_bitrate;                               // rate in bps

  unsigned int   gop_length;                                // I period, gop_length == 0xffffffff is used for infinite gop length
           int   Np;                                        // 27.4 signed fixed (gop_length + num_b_frames -1)/(num_b_frames + 1)
           int   Bmin;                                      // 23.8 signed fixed point min buffer level, updated by driver at sequence level only.
           int   Ravg;                                      // 23.8 signed fixed point average rate, updated by driver at sequence level only.
           int   R;                                         // 23.8 signed fixed point rate

  unsigned char  ab_alpha;                                  // VBR/CBR: min/target quality level (1..51); rcmode4: weight of prev frame activity compared to part of the current picture
  unsigned char  ab_beta;                                   // lookahead info1: ratio of inter over intra complexity (fix8); rcmode4: weight of current activity compared to neighbors
  unsigned short prev_act;                                  // lookahead info2: NVENC_LOOKAHEAD_INFO2; rcmode4: average activity level of the first reference (L0)

  unsigned char  reserved0;
  unsigned char  single_frame_vbv;                          // 1: VBV buffer size is set to average frame size; 0: otherwise
  unsigned char  two_pass_rc;                               // 0: single pass rc , 1: first pass of 2 pass rc 2: second pass of 2 pass rc
  unsigned char  rc_class;                                  // reserved
  unsigned char  first_pass_source_half_scaled;             // 0 : first pass on half resolution 1: first pass on full resolution (along with NVENC_QRES_FLAG_XXX)
  unsigned char  i_size_ratio_x;                            // ratio between I picture target size over average picture size numerator
  unsigned char  i_size_ratio_y;                            // ratio between I picture target size over average picture size denominator
  unsigned char  aqMode;                                    //  0: disable adaptive quantization 1: AQ mode fast (unimplemented) 2: AQ mode full (unimplemented) 3: External qp delta map
  unsigned char  dump_aq_stats;                             // 1: dump aq stats
  unsigned char  ext_scene_change_flag;                     // Scene change flag set as hint by external preprocessing unit. 0: No scene change, 1: Current picture is first in scene
  unsigned char  ext_picture_rc_hints;                      // picture rc hints are set by external source. 0: no hints, 1: hints are avaialble for current picture
  unsigned char  session_max_qp;                            // Current frame qp will never exceed this value when ext_picture_rc_hints is set
  unsigned int   ext_intra_satd;                            // If non zero this represents the intra SATD for current picture computed by external preprocessing unit
  unsigned char  reserved2[8];
} nvenc_h265_rc_s;                                          // 96 bytes

typedef struct
{
           int   inter_ref_pic_set_prediction_flag  : 1;
           int   delta_idx_minus1                   : 6;
           int   delta_rps_sign                     : 1;
           int   abs_delta_rps_minus1               : 16;
           int   reserved                           : 8;
  unsigned short used_by_curr_pic_flag;
  unsigned short use_delta_flag;
           short num_negative_pics;
           short num_positive_pics;
           short delta_poc_s0_minus1[16];
  unsigned short used_by_curr_pic_s0_flag;
           short delta_poc_s1_minus1[16];
  unsigned short used_by_curr_pic_s1_flag;
}nvenc_h265_strps_s;                   // 80bytes multiple of 4

typedef struct
{
           int   num_long_term_sps                  : 5;
           int   num_long_term_pics                 : 5;
           int   reserved0                          : 22;
  unsigned char  lt_idx_sps[16];
  unsigned short poc_lsb_lt[16];
  unsigned short used_by_curr_pic_lt_flag;
  unsigned short delta_poc_msb_present_flag;
           int   delta_poc_msb_cycle_lt[16];
}nvenc_h265_ltrps_s;                  // 120bytes multiple of 4

typedef struct
{
           int   NumPocTotalCurr                    : 5;
           int   ref_pic_list_mod_flag_l0           : 1;
           int   ref_pic_list_mod_flag_l1           : 1;
           int   reserved0                          : 25;
  unsigned char  list_entry_l0[16];
  unsigned char  list_entry_l1[16];
}nvenc_h265_ref_pic_list_modification_s; //36bytes multiple of 4

typedef struct
{
  unsigned int   vps_poc_lsb_not_present_flag           : 1;
  unsigned int   vps_default_ref_layers_active_flag     : 1;
  unsigned int   vps_max_one_active_ref_layer_flag      : 1;
  unsigned int   vps_num_direct_ref_layers              : 6;
  unsigned int   slh_discardable_flag                   : 1;    // mv_hevc, equal to 1 specifies that the coded picture is not used as a reference picture
  unsigned int   slh_cross_layer_bla_flag               : 1;    // mv_hevc
  unsigned int   slh_num_active_ref_layer_pics          : 6;    // active ref layer pics of current picture
  unsigned int   slh_inter_layer_pred_enabled_flag      : 1;
  unsigned int   slh_num_inter_layer_ref_pics_minus1    : 6;
  unsigned int   reserved0                              : 8;
  unsigned char  slh_inter_layer_pred_layer_idc[8];             //NumPicTotalCurr <= 8
}nvenc_mv_hevc_cfg_s;                   //12bytes multiple of 4

typedef struct
{
           char  l0[8];                                     // reference picture list 0
           char  l1[8];                                     // reference picture list 1
           char  temp_dist_l0[8];                           // temporal distance of each ref pic in list 0 DiffPicOrderCnt( currPicOrField, pic0) )
           char  temp_dist_l1[8];                           // temporal distance of each ref pic in list 1 DiffPicOrderCnt( currPicOrField, pic1) )
           short dist_scale_factor[8][8];                   // (h264 spec eq 8-203) array [refidx1][refidx0] signed 11 bit values
  unsigned int   diff_pic_order_cnt_zero[2];                // This is a 2-dimensional array of booleans (1 bit flags) indicating that DiffPicOrderCnt(refidx1, refidx0) == 0
  unsigned int   max_slice_size;                            // for use in slice_mode 1 only:  dynamic insertion at/before this boundary (#bytes)
  unsigned int   max_byte_count_before_resid_zero;          // Maximum byte count before micrcode forces the residuals to zero.

  unsigned short no_output_of_prior_pics_flag       : 1;    // used to modify the DPB after decoding a IDR or BLA picture
  unsigned short pic_output_flag                    : 1;    // affects the decoded picture output and removal processes as specified in Annex C
  unsigned short mode_decision_only_flag            : 1;    // only output h265 mode and mocomp result, no recon and bitstream output
  unsigned short cu_early_terminatiom_enable        : 1;    // 0: disable CU early termination, 1: enable CU early termination
  unsigned short input_sub_sampling                 : 1;    // used by PDMA 2x2 sub-sampling
  unsigned short output_sub_sampling                : 1;    // used by MPEB 2x2 sub-sampling output
  unsigned short reserved0                          : 10;
  unsigned short pic_order_cnt_lsb;                         // value of the corresponding syntax element

  unsigned char  num_forced_slices_minus1;                  // number of forced slice boundary locations minus1(number of entries in h264_slice_control_s)
  unsigned char  num_me_controls_minus1;                    // number of nvenc_h264_me_control_s entries in array minus1
  unsigned char  num_md_controls_minus1;                    // number of nvenc_h264_md_control_s entries in array minus1
  unsigned char  num_q_controls_minus1;                     // number of nvenc_h264_quant_control_s entries in array minus1
  unsigned int   slice_control_offset;                      // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                            // to start of array of "nvenc_h264_slice_control_s" structures
  unsigned int   me_control_offset;                         // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                            // to start of array of "nvenc_h264_me_control_s" structures
  unsigned int   md_control_offset;                         // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                            // to start of array of "nvenc_h264_md_control_s" structures
  unsigned int   q_control_offset;                          // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                            // to start of array of "nvenc_h264_quant_control_s" structures
  unsigned int   hist_buf_size;                             // size in bytes of the buffer allocated for history
  unsigned int   bitstream_buf_size;                        // size in bytes of the buffer allocated for bitstream slice/mb data
  unsigned int   bitstream_start_pos;                       // start position in bitstream buffer where data should be written (byte offset)
  unsigned int   pic_type                           : 2;    // 0 = P, 1 = B, 2 = I, 3 = IDR
  unsigned int   ref_pic_flag                       : 1;    // reference picture (0 = no, 1 = yes)
  unsigned int   slice_mode                         : 1;    // 0 = dynamic slice insertion based on slice size
                                                            // 1 = static slice insertion based on slice control array
                                                            //     (insert slices as defined in array, if all array elements used
                                                            //      before end of picture is reached, start over with entry 0)
  unsigned int   cabac_zero_word_enable             : 1;
  unsigned int   codec                              : 3;
  unsigned int   ceil_log2_pic_size_in_ctbs         : 6;    // ceillog2(picSizeInCtbs) for HW needs too much area/time for 16kx16k case
  unsigned int   sao_luma_mode                      : 5;
  unsigned int   sao_chroma_mode                    : 5;
  unsigned int   reserved1                          : 8;

  unsigned int   mpec_threshold;                            // collect mpec stats after threshold mbs
                                                            // when mpec_stat_on is enabled and mpec_threshold is not equal to picWidthInMbs,
                                                            // rc mode 0 will be used to collect stats
  unsigned int   slice_stat_offset;                         // offset from start of top level "nvenc_h265_stat_data_s" structure
                                                            // to start of array of "nvenc_h265_slice_stat_s" structure
  unsigned int   mpec_stat_offset;                          // offset from start of top level "nvenc_h265_stat_data_s" structure
                                                            // to start of array of "nvenc_h265_mpec_stat_s" structure
  unsigned int   aq_stat_offset;                            // to start of array of "nvenc_h265_aq_stat_s" structure
  unsigned int   short_term_ref_pic_set_sps_flag    : 1;    // if set to 1 ,the short term RPS is signalled using SPS, if set to 0 the short term RPS is encoded in the current slice header
  unsigned int   short_term_ref_pic_set_idx         : 6;    // used to signal index into the list of the short term RPS in the SPS.
  unsigned int   e4byteStartCode                    : 1;    // if enable to 1, all the slices in a picture will use 4 byte start code 00000001(used in rtp mode), original is 000001
  unsigned int   nal_unit_type                      : 6;    // specifies the type of RBSP data structure contained in the NAL unit
  unsigned int   nuh_layer_id                       : 6;    // reserved and shall be equal to 0.
  unsigned int   nuh_temporal_id_plus1              : 3;    // specifies the temporal_ip_plus1 of RBSP data structure contained in the NAL unit
  unsigned int   qpfifo                             : 1;    // 0 = Polling mode, 1 = Interrupt mode.
  unsigned int   enable_source_image_padding        : 1;    // enable/disable HW source image padding
  unsigned int   luma_padding_mode                  : 2;    // HW souce padding mode for luma: 0: copy frame boundary, 1: zero padding, 2: half value, i.e 1<<(bitdepth-1)
  unsigned int   chroma_padding_mode                : 2;    // HW souce padding mode for chroma: 0: copy frame boundary, 1: zero padding, 2: half value, i.e 1<<(bitdepth-1)
  unsigned int   stats_fifo_enable                  : 1;    // enable per-ctb short stats fifo
  unsigned int   reserved2                          : 2;

  nvenc_h265_strps_s    short_term_rps;                     // if short_term_ref_pic_set_sps_flag is set to 0, the nvenc_h265_strps_s  structure is signalled in the slice  segment segment  header.
  nvenc_h265_ltrps_s    long_term_rps;                      // if long_term_ref_pics_present_flag is set to 1 in SPS , nvenc_h265_ltrps_s structure is signalled in the slice segment segment header
  nvenc_h265_ref_pic_list_modification_s    ref_pic_list_modification; // if lists_modification_present_flag is set to 1, then nvenc_h265_ref_pic_list_modification_s  structure is encoded in slice segment header.
  unsigned int   rc_pass2_offset;

  unsigned int   wp_control_offset;                         // offset from start of top level "nvenc_h265_drv_pic_setup_s" structure
                                                            // to start of array of "nvenc_pred_weight_table_s" structure

  unsigned char  num_wp_controls_minus1;                    // number of nvenc_pred_weight_table_s entries in array minus 1
  unsigned char  stripID;
  unsigned char  strips_in_frame;
  unsigned char  slice_idx_start;
  unsigned short slice_encoding_row_start;                  // start ctb row for slice encoding
  unsigned short slice_encoding_row_num;                    // num of ctb row for slice encoding

  nvenc_mv_hevc_cfg_s mv_hevc_cfg;
  unsigned int   chroma_skip_threshold_4x4          : 16;   // low 3 bits is the factional, to check if chroma residual is big in skip mb, can't simply set chroma residual to 0 even though luma has been decided to skip
  unsigned int   longTermFlag                       : 16;   // Bits 7:0 for pictures in RefPicList0 and bits 15:8 for pictures in RefPicList1

          char   poc_entry_l0[8];                           // ref pic list to ref pic entry map
          char   poc_entry_l1[8];                           // ref pic list to ref pic entry map
  unsigned int   same_poc[2];                               // set to 1 when the poc of cur ref pic and neighbor ref pic are same, otherwise set it to 0.
  unsigned int   act_stat_offset;                            // offset from start of "nvenc_rc_pic_stat_s" structure, points to data for external QP map
  unsigned int   stats_fifo_offset;                         // offset from start of top level "nvenc_stat_data_s" structure
} nvenc_h265_pic_control_s;                                 // 536 bytes

typedef struct
{
  // 256 aligned
  unsigned int                      magic;                  // magic number, see text above             //  4 bytes
  nvenc_h265_surface_cfg_s          refpic_cfg;             // defines layout of reference pictures     // 32 bytes
  nvenc_h265_surface_cfg_s          input_cfg;              // defines layout of input pictures         // 32 bytes
  nvenc_h265_surface_cfg_s          outputpic_cfg;          // defines layout of reconstructed pictures // 32 bytes
  nvenc_h265_sps_data_s             sps_data;                                                           //  8 bytes
  nvenc_h265_pps_data_s             pps_data;                                                           //  8 bytes
  nvenc_h265_rc_s                   rate_control;           // Rate Control information                 // 96 bytes
  nvenc_h265_pic_control_s          pic_control;                                                        //536 bytes
  nvenc_h265_surface_cfg_s          half_scaled_outputpic_cfg; // defines layout of 2x2 subsampled reconstructed picture // 32 bytes
  unsigned int                      gpTimer_timeout_val;    // GPTimer cycle count set from driver       4 bytes
  unsigned char                     reserved[240];          // Pad to 1024 bytes

} nvenc_h265_drv_pic_setup_s;

typedef struct
{
  unsigned short intra_refresh_cnt;                         // distance between forced-intra MBs in slice; 0 means OFF
  unsigned short intra_refresh_offs;                        // first forced-intra MB in slice

  unsigned int   h265_intra_luma4x4_mode_enable;             //bitmask indicate which intra luma 4x4 modes to enable in ip1 intra search
  unsigned int   h265_intra_luma8x8_mode_enable;             //bitmask indicate which intra luma 8x8 modes to enable in ip1 intra search
  unsigned int   h265_intra_luma16x16_mode_enable;           //bitmask indicate which intra luma 16x16 modes to enable in ip1 intra search
  unsigned int   h265_intra_luma32x32_mode_enable;           //bitmask indicate which intra luma 32x32 modes to enable in ip1 intra search
  unsigned int   h265_intra_luma64x64_mode_enable;           //bitmask indicate which intra luma 64x64 modes to enable in ip1 intra search
  unsigned int   h265_intra_luma_mode_enable_leftbits : 30;  //bitmask indicate intra luma left modes(32~34) from ip1 luma4x4 to ip2 luma64x64.
  unsigned int   reserved0                          : 2;


  unsigned char  cu16x16_part_2Nx2N_enable       : 1;    // enable prediction for 2Nx2N
  unsigned char  cu16x16_part_2NxN_enable        : 1;    // enable prediction for 2NxN
  unsigned char  cu16x16_part_Nx2N_enable        : 1;    // enable prediction for Nx2N
  unsigned char  cu16x16_part_NxN_enable         : 1;    // enable prediction for NxN
  unsigned char  cu16x16_part_2NxnU_enable       : 1;    // enable prediction for 2NxnU
  unsigned char  cu16x16_part_2NxnD_enable       : 1;    // enable prediction for 2NxnD
  unsigned char  cu16x16_part_nLx2N_enable       : 1;    // enable prediction for nLx2N
  unsigned char  cu16x16_part_nRx2N_enable       : 1;    // enable prediction for nRx2N

  unsigned char  cu32x32_part_2Nx2N_enable       : 1;    // enable prediction for 2Nx2N
  unsigned char  cu32x32_part_2NxN_enable        : 1;    // enable prediction for 2NxN
  unsigned char  cu32x32_part_Nx2N_enable        : 1;    // enable prediction for Nx2N
  unsigned char  cu32x32_part_NxN_enable         : 1;    // enable prediction for NxN
  unsigned char  cu32x32_part_2NxnU_enable       : 1;    // enable prediction for 2NxnU
  unsigned char  cu32x32_part_2NxnD_enable       : 1;    // enable prediction for 2NxnD
  unsigned char  cu32x32_part_nLx2N_enable       : 1;    // enable prediction for nLx2N
  unsigned char  cu32x32_part_nRx2N_enable       : 1;    // enable prediction for nRx2N

  unsigned char  cu64x64_part_2Nx2N_enable       : 1;    // enable prediction for 2Nx2N
  unsigned char  cu64x64_part_2NxN_enable        : 1;    // enable prediction for 2NxN
  unsigned char  cu64x64_part_Nx2N_enable        : 1;    // enable prediction for Nx2N
  unsigned char  cu64x64_part_NxN_enable         : 1;    // enable prediction for NxN
  unsigned char  cu64x64_part_2NxnU_enable       : 1;    // enable prediction for 2NxnU
  unsigned char  cu64x64_part_2NxnD_enable       : 1;    // enable prediction for 2NxnD
  unsigned char  cu64x64_part_nLx2N_enable       : 1;    // enable prediction for nLx2N
  unsigned char  cu64x64_part_nRx2N_enable       : 1;    // enable prediction for nRx2N

  unsigned char  intra_mode_4x4_enable              : 1;    // enable intra prediction in mdp for 4x4
  unsigned char  intra_mode_8x8_enable              : 1;    // enable intra prediction in mdp for 8x8
  unsigned char  intra_mode_16x16_enable            : 1;    // enable intra prediction in mdp for 16x16
  unsigned char  intra_mode_32x32_enable            : 1;    // enable intra prediction in mdp for 32x32
  unsigned char  intra_mode_64x64_enable            : 1;    // enable intra prediction in mdp for 32x32
  unsigned char  priority_ipred_type_ip1            : 3;    // which intra type search first for early termination, must be a subset of ip_search_mode. reserved, not implemented in HW

  unsigned int   h265_intra_chroma_mode_enable      : 5;    //bitmask indicate which intra_chroma_pred_mode to enable
  unsigned int   inter_penalty_factor_for_ip1       : 6;    // early teminate ip1 intra search if intra cost is larger than inter_cost + ((inter_cost * factor) >> 8)
  unsigned int   tusize_4x4_enable                  : 1;    //enable 4x4 transform
  unsigned int   tusize_8x8_enable                  : 1;    //enable 8x8 transform
  unsigned int   tusize_16x16_enable                : 1;    //enable 16x16 transform
  unsigned int   tusize_32x32_enable                : 1;    //enable 32x32 transform
  unsigned int   intra_tusize_decision              : 1;    //enable intra tu size decision, reserved, not implemented in HW
  unsigned int   pskip_enable                       : 1;    //enable p_skip encoding
  unsigned int   tu_search_basedon_pusize           : 1;    //enable tu search based on pu size
  unsigned int   reserved1                          : 14;   // reserved for future special prediction extensions

           short bias_part_2Nx2N;                           //bias for 2Nx2N PU
           short bias_part_2NxN;                            //bias for 2NxN PU
           short bias_part_Nx2N;                            //bias for Nx2N PU
           short bias_part_NxN;                             //bias for NxN PU
           short bias_part_2NxnU;                           //bias for 2NxnU PU
           short bias_part_2NxnD;                           //bias for 2NxnD PU
           short bias_part_nLx2N;                           //bias for nLx2N PU
           short bias_part_nRxN;                            //bias for nRxN PU

           short bias_cusize_8x8;                           //bias for 8x8 CU
           short bias_cusize_16x16;                         //bias for 16x16 CU
           short bias_cusize_32x32;                         //bias for 32x32 CU
           short bias_cusize_64x64;                         //bias for 64x64 CU
           short bias_pskip;                                // bias for pskip       (subtracts from pskip cost)

           short bias_intra_over_inter;                     // bias for any intra   (subtracts from intra cost)
           short bias_intra_most_prob;                      // bias for intra most probable mode
           short bias_qpp;
           short bias_transform_skip;                       // bias for transform skip mode

  unsigned short mv_cost_bias;                              // bias applied to internal motion vector cost calculation
  unsigned short pskip_esc_threshold;                       // if pskip cost is below this, disable full search & attempt pskip encode

  unsigned short tempmv_wt_spread_threshold;                // The threshold for the sum of differences between 8x8s and 16x16 mvs. Used for temporal mv weight
  unsigned int   tempmv_wt_distort_threshold        : 16;   // The threshold for the cost of the 16x16 mv. Used for temporal mv weight
  unsigned int   mv_cost_predictor_control          : 1;    // 0=use 16x16 predictor only; 1=use accurate predictor (slower but better)
  unsigned int   mv_cost_enable                     : 1;    // enable mv cost calculations in mode decision
  unsigned int   chroma_eval_mode                   : 1;    // for intra chroma search 0 = U and V; 1 = U only
  unsigned int   ip_search_mode                     : 4;    // which intra sizes to search before making IP decision b0=4x4,b1=8x8,b2=16x16,b3=32x32
  unsigned int   multiply_bias_with_lambda          : 1;    // setting this to 1 will result in bias being multiplied by lambda
  unsigned int   force_ipcm                         : 1;    // 0=normal encode; 1=force mpeb/mpec encode as IPCM
  unsigned int   early_termination_ip1              : 1;    // enable IP1 early termination
  unsigned int   tu_search_num_cu16                 : 3;    // tu search part num in cu16
  unsigned int   tu_search_num_cu32                 : 3;    // tu search part num in cu32
  unsigned int   early_termination_mode_ip1;                //enable intra mode search for early termination, 32 bits for the 35 modes other than H, V and DC, these 3 modes enabled as default
  short bias_tu_4x4;                                        // bias for TU 4x4
  short bias_tu_8x8;                                        // bias for TU 8x8
  short bias_tu_16x16;                                      // bias for TU 16x16
  short bias_tu_32x32;                                      // bias for TU 32x32
  short num_1div8_lambda_ssd                        : 7;    // number of 1/8 lambda ssd for lambda_coef delta. lambda_coef = (1+num_1div8_lambda_ssd*1/8)*lambda_ssd
  short skip_evaluate_enable_cu8                    : 1;    // 0: not check skip ssd cost; 1, evaluate skip ssd cost
  short skip_evaluate_enable_cu16                   : 1;    // 0: not check skip ssd cost; 1, evaluate skip ssd cost
  short skip_evaluate_enable_cu32                   : 1;    // 0: not check skip ssd cost; 1, evaluate skip ssd cost
  short skip_evaluate_enable_cu64                   : 1;    // 0: not check skip ssd cost; 1, evaluate skip ssd cost
  short reserved2                                   : 1;
  short intra_ssd_cnt_4x4                           : 4;    // SSD cost evaluation between N top intra modes selected by satd
  short intra_ssd_cnt_8x8                           : 4;    // SSD cost evaluation between N top intra modes selected by satd
  short intra_ssd_cnt_16x16                         : 4;    // SSD cost evaluation between N top intra modes selected by satd
  short intra_ssd_cnt_32x32                         : 4;    // SSD cost evaluation between N top intra modes selected by satd
  short rdo_level                                   : 3;    // 0:cu size decided by satd in P frame  1:cu size decided by ssd
  short reserved3                                   : 1;    // pad to short

  short bias_skip;                                 // bias for skip mv's ssd (subtracts from skip mv's ssd cost)
  short bias_intra_32x32;                          // bias for intra 32x32 (subtracts from intra 32x32 cost)
  short bias_intra_16x16;                          // bias for intra 16x16 (subtracts from intra 16x16 cost)
  short bias_intra_8x8;                            // bias for intra 8x8   (subtracts from intra 8x8 cost)
  short bias_intra_4x4;                            // bias for intra 4x4   (subtracts from intra 4x4 cost)
  unsigned char  reserved[22];
} nvenc_h265_md_control_s;                                  // 128 bytes

typedef struct
{
  unsigned int   num_ctus                          : 19;   // number of ctus in this slice, support upto 8kx8k
  unsigned int   qp_avr                            : 8;    // 6bit value used by hw. Other bits can be used for rounding.
  unsigned int   reserved0                         : 5;

  unsigned int   dependent_slice_segment_flag      : 1;    // slice segment header syntax element present or not
  unsigned int   slice_type                        : 2;    // 0 = B, 1 = P, 2 = I
  unsigned int   five_minus_max_num_merge_cand     : 3;    // specifies the maximum number of merging MVP candidates supported
  unsigned int   cabac_init_flag                   : 1;
  unsigned int   deblocking_filter_override_flag   : 1;    // specifies the deblocking control params is preset in slice header.
  unsigned int   slice_deblocking_filter_disabled_flag            : 1;    // if set to 1 , it disables deblcoking for the current slice.
                                                                          // If deblocking_filter_override_flag is 0, this is set to pps_deblocking_filter_disabled_flag
  unsigned int   slice_loop_filter_across_slices_enabled_flag     : 1;    // if set to 1, it enables loop filter accross slice boundary.
  unsigned int   qp_slice_min                      : 8;    // min slice qp value
  unsigned int   qp_slice_max                      : 8;    // max slice qp value
  unsigned int   force_intra                       : 1;
  unsigned int   slice_sao_luma_flag               : 1;    // equal to 1 specifies that SAO is enabled for the luma component in the current slice
  unsigned int   slice_sao_chroma_flag             : 1;    // equal to 1 specifies that SAO is enabled for the chroma component in the current slice
  unsigned int   reserved1                         : 3;

           int   slice_beta_offset_div2            : 4;    // if deblocking_filter_override_flag = 1 , then slice_beta_offset_div2 is signalled in slice segment header
           int   slice_tc_offset_div2              : 4;    // if deblocking_filter_override_flag = 1 , then slice_tc_offset_div2 is signalled in slice segment header.
           int   slice_cb_qp_offset                : 5;    // offset added to pps_cb_qp_offset. It shall be in the range -12 to +12.
           int   slice_cr_qp_offset                : 5;    // offset added to pps_cr_qp_offset. It shall be in the range -12 to +12.
  unsigned int   slice_temporal_mvp_enabled_flag   : 1;    // set 1, temporal motion vector predictors may be used
  unsigned int   collocated_from_l0_flag           : 1;    // set 1, the collocated picture for tmvp is derived from ref. pic. list 0
  unsigned int   collocated_ref_idx                : 5;    // specifies ref. index of the collocated picture for tmvp
           int   reserved2                         : 7;

  unsigned int   num_ref_idx_active_override_flag  : 1;
  unsigned int   num_ref_idx_l0_active_minus1      : 5;    // [0.31]
  unsigned int   num_ref_idx_l1_active_minus1      : 5;    // [0,31]
  unsigned int   me_control_idx                    : 4;    // index in array of nvenc_h264_me_control_s to use for this slice
  unsigned int   md_control_idx                    : 4;    // index in array of nvenc_h264_md_control_s to use for this slice
  unsigned int   q_control_idx                     : 4;    // index in array of nvenc_h264_quant_control_s to use for this slice
  unsigned int   wp_control_idx                    : 4;    // index in array of nvenc_pred_weight_table_s to use for this slice
  unsigned int   reserved3                         : 5;

  unsigned int   limit_slice_top_boundary          : 1;    // limit ME to slice boundaries
  unsigned int   limit_slice_bot_boundary          : 1;    // limit ME to slice boundaries
  unsigned int   limit_slice_left_boundary         : 1;    // limit ME to slice boundaries
  unsigned int   limit_slice_right_boundary        : 1;    // limit ME to slice boundaries
  unsigned int   ROI_enable                        : 1;    // Enable region  of interest processing
  unsigned int   ROI_me_control_idx                : 4;
  unsigned int   ROI_md_control_idx                : 4;
  unsigned int   ROI_q_control_idx                 : 4;
  unsigned int   ROI_qp_delta                      : 8;    // QP delta in ROI relative to picture outside ROI
  unsigned int   reserved4                         : 7;    // reserved for alignment

  unsigned int   ROI_top_mbx                       : 16;   // top left mbx of the ROI
  unsigned int   ROI_top_mby                       : 16;   // top left mby of the ROI
  unsigned int   ROI_bot_mbx                       : 16;   // bottom right mbx of the ROI
  unsigned int   ROI_bot_mby                       : 16;   // bottom right mby of the ROI

  unsigned char  reserved5[36];                            // padding to 64 bytes
} nvenc_h265_slice_control_s;                              // 64 bytes

// h265 not support QPP, reserved it but not used
typedef struct
{
  // QPP has not been defined for H265
  unsigned short qpp_run_vector_4x4;                        // cost values for 4x4 transform (16 bit total vector)
  unsigned short qpp_run_vector_8x8[3];                     // cost values for 8x8 transform (48 bit total vector)
                                                            // 2 bits each for first 12 coefs, then 1 bit each for next 24
  unsigned char  qpp_luma8x8_cost;                          // luma 8x8 cost threshold, 0 = throw out all coefs, range 0-15
  unsigned char  qpp_luma16x16_cost;                        // luma 16x16 cost threshold, 0 = throw out all coefs, range 0-15
  unsigned char  qpp_chroma_cost;                           // chroma cost threshold, 0 = throw out all coefs, range 0-15
  unsigned char  qpp_mode                           : 2;    // 0 = OFF, 1 = 8x8, 2 = 16x16_8x8
  unsigned char  reserved0                          : 4;    // padding

  unsigned int   reserved1;                                 // padding to 16 bytes
} nvenc_h265_quant_control_s;                               // 16 bytes

//*****************************************************************************
// VP9
//*****************************************************************************

//vp9 filter config
typedef struct
{
    unsigned char filter_level[8];                  // loop filter strength for each segment.
             char ref_delta[4];                     // Loop filter strength adjustments based on frame type (intra, inter)
             char mode_delta[2];                    // Loop filter strength adjustments based on mode (zero, new mv)
    unsigned char disable           : 1;            // disable loop-filter
    unsigned char sharpness         : 3;            // sharpness level
    unsigned char baselevel_gt32    : 1;            // filter base level greater than 32 flag
    unsigned char reserved          : 3;
    unsigned char reserved2;
}nvenc_vp9_filter_cfg_s;  //16 bytes

typedef struct
{
    unsigned int round_chroma_ac   : 3;             // round for chroma AC
    unsigned int round_chroma_dc   : 3;             // round for chroma DC
    unsigned int round_luma_ac     : 3;             // round for luma AC
    unsigned int round_luma_dc     : 3;             // round for luma DC
    unsigned int round_delta_inter : 3;             // round delta for inter
    unsigned int reserved          : 17;
}vp9_round_cfg_s;

//vp9 quant config
typedef struct
{
    unsigned char  qindex_seg[8];                   // Base frame qindex (index in quant lookup table) for each segment 0...255
             int delta_y_dc       : 5;              // explicit qindex adjustment for y dccoefficient, -15...15
             int delta_uv_dc      : 5;              // qindex adjustment for uv accoefficient, -15...15
             int delta_uv_ac      : 5;              // qindex adjustment for uv dccoefficient, -15...15
             int reserved         : 17;
    //round parameters
    vp9_round_cfg_s round_seg[8];                   // round config for each segment
}nvenc_vp9_quant_cfg_s;  //44 bytes

// vp9 dma config for each tile
typedef struct
{
    unsigned int tile_width       : 6;              // tile width in super blocks(64x64)
    unsigned int tile_height      : 6;              // tile height in super blocks(64x64)
    unsigned int tile_pos_x       : 6;              // tile x-position in super blocks unit
    unsigned int reserved         : 14;
}nvenc_vp9_modedma_cfg_s;

//vp9 smalle config
typedef struct
{
    unsigned int high_prec_mv_enable : 1;           // high precision mv enable
    unsigned int temporal_mv_enable  : 1;           // temporal mv enable
    unsigned int transform_mode      : 3;           // transform mode: 0: only4x4, 1: allow_8x8, 2: allow_16x16, 3: allow_32x32, 4:transform_mode_select
    unsigned int lossless_enable     : 1;           // lossless encoding enable
    unsigned int mocomp_filter_type  : 3;           // mocomp filter type:0 eight-tap smooth, 1: eight-tap, 2:eight-tap sharp, 3: bilinear, 4:switchable
    unsigned int tile_enable         : 1;           // enable tile encoding
    unsigned int segment_enable      : 1;           // enable segment
    unsigned int segment_upd_enable  : 1;           // enable explicit map update for current frame. If true, then segment_temp_enable says how to update
    unsigned int segment_temp_enable : 1;           // Type of map update
    unsigned int reserved            : 19;
}nvenc_vp9_smalle_config_s;

//vp9 frame-level parameters
typedef struct
{
    unsigned int pic_width          : 13;           // picture width in pixels
    unsigned int pic_height         : 13;           // picture height in pixels
    unsigned int rec_disable        : 1;            // disable recon output
    unsigned int type               : 1;            // picture type: 0 Intra/1 Inter
    unsigned int refframe           : 2;            // reference frame type used 1 = last 2= golden 3= alt-ref
    unsigned int reserved           : 2;

    //ref-frame param
    unsigned char ref_index[8];                     // ref-frame index for each segment
    //skip param
    unsigned char skip_mode[8];                     // skip-mode enable for each segment: skip mode implies both the use of a (0,0) motion vector and that no residual will be coded
    //filter param
    nvenc_vp9_filter_cfg_s filter_cfg;              // filter config
    //quant param
    nvenc_vp9_quant_cfg_s quant_cfg;                // quant & round config

    nvenc_vp9_smalle_config_s smalle_cfg;           // small-e config
    unsigned int   num_tile_controls_minus1;        // number of nvenc_vp9_tile_control_s entries in array minus1
    unsigned int   tile_controls_offset;            // offset from start of top level "nvenc_vp9_pic_control_s" structure
                                                    // to start of array of "nvenc_vp9_tile_control_s" structures

    unsigned int bitstream_buf_start_pos;           // bitstream buffer offset start position byte offset
    unsigned int bitstream_buf_size;                // bitstream buffer size in bytes

    unsigned int combined_linebuf_size;             // combined line buffer size in bytes

    unsigned int filterline_linebuf_size;           // filter line line-buffer size in bytes

    unsigned int filtercol_linebuf_size;            // filter column line-buffer size in bytes
}nvenc_vp9_pic_control_s;  //112 bytes

typedef struct
{
    unsigned int col_pos      : 6;                  // tile position in Super Block of Y axis
    unsigned int bs_size      : 24;                 // tile size in Super Block
    unsigned int reserved     : 2;
    nvenc_vp9_modedma_cfg_s mode_dma_cfg;           // mode-dma config for this tile
    unsigned char tile_idx;                         // tile index in the picture
    unsigned char reserved2[55];
}nvenc_vp9_tile_control_s;  //64bytes

typedef struct
{
  unsigned char  QP[3];                                     // QP for 0:P picture, ,1:B picture 2; I picture
  unsigned char  minQP[3];                                  // min QP for 0:P picture, ,1:B picture 2; I picture
  unsigned char  maxQP[3];                                  // max QP for 0:P picture, ,1:B picture 2; I picture
  unsigned char  reserved[3];
           int   rhopbi[3];                                 // 23.8 signed fixed point quant ratios P/I, B/I, I/I(=1)

           int   framerate;                                 // fps
  unsigned int   buffersize;                                // total buffer size

           int   nal_cpb_size;                              // size in bits
           int   nal_bitrate;                               // rate in bps

           int   gop_length;                                // I period
           int   Bmin;                                      // 23.8 signed fixed point min buffer level, updated by driver at sequence level only.
           int   Ravg;                                      // 23.8 signed fixed point average rate, updated by driver at sequence level only.
           int   R;                                         // 23.8 signed fixed point rate


} nvenc_vp9_rc_s;                                           // 56 bytes, currently keep the same as vp8


typedef struct
{
  unsigned int   picture_index;                             // value received from SetPictureIndex method
  unsigned int   error_status                       : 2;    // report error if any
  unsigned int   ucode_error_status                 : 30;   // report error status from ucode to driver
  unsigned int   total_byte_count;                          // total bitstream size in bytes
  unsigned int   bitstream_start_pos;                       // byte_offset where VP9 BS DMA start writing bitstream
  unsigned short pic_type;                                  // inter/key frame type
  unsigned short avgQP;                                     // average QP
  unsigned int   cycle_count;                               // total cycles taked for execute. Written out only when DumpCycleCount in SetControlParams is set
  unsigned int   intra_cu32x32_count;                       // number of intra 32x32 CUs.
  unsigned int   inter_cu32x32_count;                       // number of inter 32x32 CUs.
  unsigned int   intra_cu16x16_count;                       // number of intra 16x16 CUs.
  unsigned int   inter_cu16x16_count;                       // number of inter 16x16 CUs.
  unsigned int   intra_cu8x8_count;                         // number of intra 8x8   CUs.
  unsigned int   inter_cu8x8_count;                         // number of inter 8x8   CUs.
  unsigned int   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
  unsigned int   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
  unsigned int   total_intra_cost;                          // sum of all intra best mode's SATDs, each mb has one intra SATD.
  unsigned int   total_inter_cost;                          // sum of all inter best mode's SATDs, each mb has one inter SATD.

} nvenc_vp9_pic_stat_s;                                     // 64 bytes

// ****************************************************************************
// ****************************************************************************
// VP8
// ****************************************************************************
// ****************************************************************************

// TO DO: Find out if we should define # maxSegments as 4
#define MAXSEGMENTS 4

typedef struct
{
  unsigned short frame_width_minus1;                        // frame width in pixels minus 1, range 0-4095
  unsigned short frame_height_minus1;                       // frame height in pixels minus 1, range 0-4095

  unsigned int sfc_pitch                            : 10;   // pitch of luma plane in macorblock units( i.e stride >>4)
  unsigned int sfc_pitch_chroma                     : 10;    // pitch of chroma plane in macroblock units
  unsigned int reserved2                            : 12;

  unsigned int   sfc_trans_mode                     : 4;    // least 3 significient bits are used to stand for 8 modes, normal, xflip, yflip etc.
  unsigned int   block_height                       : 7;    // BL mapping block height setting
  unsigned int   tiled_16x16                        : 1;    // Surface is 16x16 tiled instead of BL mapped (must be 1 for refpics)
  unsigned int   memory_mode                        : 2;    // 0: semi-planar, 1: planar
  unsigned int   input_bl_mode                      : 2;    // the input block linear mode: 0~gpu bl; 1~tegra bl; 2~naive bl  this is only for cmod and cmod/plugin
  unsigned int   reserved                           : 16;   // pad to int

  unsigned int   luma_top_frm_offset;                       // offset of luma top field or frame in units of 256
  unsigned int   luma_bot_offset;                           // offset of luma bottom field in units of 256. Not used if frame format.
  unsigned int   chroma_top_frm_offset;                     // offset of chroma top field or frame, or offset of chroma U(Cb) plane in planar mode, both in units of 256
  unsigned int   chroma_bot_offset;                         // offset of chroma bottom field, not used if frame format. Or offset of chroma V(Cr) plane in planar mode. Both in units of 256.

} nvenc_vp8_surface_cfg_s;                                  // 28 bytes

typedef struct
{
  //unsigned int   coeff_partitioning_enabled         : 1;    // 0: coefficient partitioning disabled 1: coeffient partitioning enabled (SE: redundant, same as multi_token_partition!=0)
  //unsigned int   num_segments_minus_one             : 2;    // number of segments
  unsigned int   horiz_scale                        : 2;    // 0=1, 1=5/4, 2=5/3, 3=2
  unsigned int   vert_scale                         : 2;    // 0=1, 1=5/4, 2=5/3, 3=2
  //unsigned int   golden_frame_enable                : 1;    // 0: golden frame disabled, 1: golden frame enabled
  //unsigned int   alt_ref_frame_enable               : 1;    // 0: alt_ref_frame_disabled, 1: alt_ref_frame_enabled
  //unsigned int   adaptive_golden_frame_selection    : 1;    // 0: fixed distance between golden frames 1: adaptive golden frame selection
  //unsigned int   adaptive_altref_frame_selection    : 1;    // 0: fixed distance between alt ref frames 1: adaptive alt ref frame selection
  unsigned int   reserved                           : 28;   // Align to word boundary

  //unsigned int   golden_frame_interval              : 16;   // golden frame interval if adaptive_golden_frame_selection is fixed
  //unsigned int   altref_frame_interval              : 16;   // altref frame interval if adaptive_altref_frame_selection is fixed

} nvenc_vp8_sequence_data_s;                                // 4 bytes

typedef struct
{
  unsigned int   frame_type                         : 1;    // 0=key frame, 1=interframe
  unsigned int   version                            : 3;    // VP8 version, also affects luma/chroma mocomp: 0=bicubic/bicubic, 1,2=bilinear/bilinear, 3..7=bilinear/fullpel (SE: perhaps separate flags for bilinear/fullpel?)
  unsigned int   show_frame                         : 1;    // 0=don't display, 1=display
  unsigned int   clr_type                           : 1;    // color space: 0=ITU-R BT.601, 1=reserved (multiplier free)
  unsigned int   clamp_type                         : 1;    // 0=clamping of reconstructed samples required, 1=no clamping necessary (SE: not supported by NVENC?)
    //  <loop filter parameters>
  unsigned int   filter_type                        : 2;    // 0=normal, 1=simple 2: disable (SE: filter_type==2 is redundant, same as filter_level==0)
  unsigned int   filter_level                       : 6;    // loop filter strength (1..63), 0=disabled
  unsigned int   sharpness_level                    : 3;    // loop filter sharpness (0..7)
  unsigned int   mode_ref_lf_delta_enabled          : 1;    // apply mb param dependent loop filter level deltas: 0=disabled, 1=enabled; if disabled the following parameters are not used
  unsigned int   mode_ref_lf_delta_update           : 1;    // update ref_lf_deltas and mode_lf_deltas: 0=keep previous, 1=update
  unsigned int   ref_lf_deltas_update_0             : 1;    // update ref_lf_deltas[0]: 0=keep previous, 1=update
  unsigned int   ref_lf_deltas_update_1             : 1;    // update ref_lf_deltas[1]: 0=keep previous, 1=update
  unsigned int   ref_lf_deltas_update_2             : 1;    // update ref_lf_deltas[2]: 0=keep previous, 1=update
  unsigned int   ref_lf_deltas_update_3             : 1;    // update ref_lf_deltas[3]: 0=keep previous, 1=update
  unsigned int   mode_lf_deltas_update_0            : 1;    // update mode_lf_deltas[0]: 0=keep previous, 1=update
  unsigned int   mode_lf_deltas_update_1            : 1;    // update mode_lf_deltas[1]: 0=keep previous, 1=update
  unsigned int   mode_lf_deltas_update_2            : 1;    // update mode_lf_deltas[2]: 0=keep previous, 1=update
  unsigned int   mode_lf_deltas_update_3            : 1;    // update mode_lf_deltas[3]: 0=keep previous, 1=update
  unsigned int   reserved0                          : 4;    // Align to word boundary
  signed   char  ref_lf_deltas[4];                          // reference frame dependent loop filter level delta (-63..+63)
                                                            // Index 0: macroblock is INTRA
                                                            //       1: macroblock is INTER and reference frame is LAST
                                                            //       2: macroblock is INTER and reference frame is GOLDEN
                                                            //       3: macroblock is INTER and reference frame is ALTREF
                                                            // lower 7 bits of each entry used as a signed value -63..+63, MS Bit is ignored by HW (SE: invalid values should be rejected?)
  signed   char  mode_lf_deltas[4];                         // macroblock mode dependent loop filter level delta (-63..+63)
                                                            // Index 0: B_PRED  (macroblock is INTRA 4x4)
                                                            //       1: ZEROMV  (macroblock is INTER and mode is ZEROMV)
                                                            //       2: MV      (macroblock is INTER and mode is neither ZEROMV nor SPLITMV)
                                                            //       3: SPLITMV (macroblock is INTER and mode is SPLITMV (not 16x16))
                                                            // lower 7 bits of each entry used as a signed value -63..+63, MS Bit is ignored by HW (SE: invalid values should be rejected?)
  // Other frame header parameters
  unsigned int   multi_token_partition              : 2;    // number of tcoeff token partitions (0,1,2,3 -> 1,2,4,8): num_part = 1 << multi_token_partition
  unsigned int   refresh_entropy_probs              : 1;    // 0=restore previous probs at end of frame, 1=keep updated probs for next frame
  unsigned int   mb_no_coeff_skip                   : 1;    // per-mb skip flag, 0=disable 1:enable (misnomer?)
  unsigned int   update_ymode_prob                  : 1;    // update ymode probs, 0=no, 1=yes
  unsigned int   update_uvmode_prob                 : 1;    // update uvmode probs, 0=no, 1=yes
  //unsigned int   coef_update_probs                  : 1;    // update tcoeff token probs, 0=no, 1=yes (SE: isnt't this redundant with coef_probs_update array?)

  unsigned int   base_qindex                        : 7;    // quantizer index (0..127)

  unsigned int   refresh_last_frame                 : 1;    // use decoded frame as last    reference frame
  unsigned int   refresh_golden_frame               : 1;    // use decoded frame as golden  reference frame
  unsigned int   refresh_alt_ref_frame              : 1;    // use decoded frame as alt_ref reference frame
  unsigned int   copy_buffer_to_gf                  : 2;    // update golden frame from:  0=none (keep), 1=last_frame, 2=alt_ref_frame
  unsigned int   copy_buffer_to_arf                 : 2;    // update alt_ref frame from: 0=none (keep), 1=last_frame, 2=golden_frame
  unsigned int   ref_frame_sign_bias_gf             : 1;    // golden frame is future frame (invert mv sign)
  unsigned int   ref_frame_sign_bias_arf            : 1;    // alt_ref frame is future frame (invert mv sign)
  unsigned int   fixed_ref_frame_probs              : 1;    //use fixed ref frame probability
  unsigned int   reserved1                          : 9;    // Align to word boundary


  signed int   y1dc_delta_q                       : 5;    // quantizer delta for luma   DC (-15..+15)
  signed int   y2dc_delta_q                       : 5;    // quantizer delta for WHT    DC (-15..+15)
  signed int   y2ac_delta_q                       : 5;    // quantizer delta for WHT    AC (-15..+15)
  signed int   uvdc_delta_q                       : 5;    // quantizer delta for chroma DC (-15..+15)
  signed int   uvac_delta_q                       : 5;    // quantizer delta for chroma AC (-15..+15)

  //  <segmentation parameters>
  unsigned int   segmentation_enabled               : 1;    // 0=disabled, 1=enabled; if disabled the following parameters are not used
  unsigned int   update_mb_segmentation_map         : 1;    // per-mb segment ID map update: 0=keep previous, 1=update (SE: have to provide the actual map)
  unsigned int   update_mb_segmentation_data        : 1;    // mb_segment_abs_delta, seg_filter_level, and seg_qindex update: 0=keep previous, 1=update
  unsigned int   mb_segment_abs_delta               : 1;    // seg_filter_level and seg_qindex are: 0=delta values, 1=absolute values
  unsigned int   reserved2                          : 3;    // Align to word boundary
  signed   char  seg_filter_level[MAXSEGMENTS];             // per-segment loop filter levels
                                                            // if mb_segment_abs_delta==0: relative to filter_level (-63..+63)
                                                            // if mb_segment_abs_delta==1: absolute value (0..63)
  signed   char  seg_qindex[MAXSEGMENTS];                   // per-segment qindex values
                                                            // if mb_segment_abs_delta==0: relative to base_qindex (-127..+127)
                                                            // if mb_segment_abs_delta==1: absolute value (0..127)

} nvenc_vp8_picture_data_s;                                 // 32 bytes

typedef struct
{
  unsigned int   early_intra_mode_control           : 2;    // 0=inter; 1=eval_intra; 2=ext_hint; 3=none (low perf,best qual)
  unsigned int   early_intra_mode_type_16x16dc      : 1;    // evaluate 16x16dc if early_intra_mode_control==1
  unsigned int   early_intra_mode_type_16x16h       : 1;    // evaluate 16x16h  if early_intra_mode_control==1
  unsigned int   early_intra_mode_type_16x16v       : 1;    // evaluate 16x16v  if early_intra_mode_control==1
  unsigned int   early_ip_is_final                  : 1;    // if set, result of early intra is final (disable full intra search)
  unsigned int   chroma_eval_mode                   : 1;    // for intra chroma search 0 = U and V; 1 = U only
  unsigned int   nearest_mv_enable                  : 1;    // enable nearest mv mode for 16x16
  unsigned int   near_mv_enable                     : 1;    // enable near mv mode for 16x16
  unsigned int   zero_mv_enable                     : 1;    // enable zero mv mode 16x16
  unsigned int   new_mv_enable                      : 1;    // enable new mv mode 16x16
  unsigned int   above4x4_mv_enable                 : 1;    // enable above4x4 mv mode for VP8 SPLITMV modes
  unsigned int   left4x4_mv_enable                  : 1;    // enable left4x4 mv mode for VP8 SPLITMV modes
  unsigned int   zero4x4_mv_enable                  : 1;    // enable zero4x4 mv mode for VP8 SPLITMV modes
  unsigned int   new4x4_mv_enable                   : 1;    // enable new4x4 mv mode for VP8 SPLITMV modes
  unsigned int   l0_part_16x16_enable               : 1;    // enable L0 prediction for 16x16
  unsigned int   l0_part_16x8_enable                : 1;    // enable L0 prediction for 16x8
  unsigned int   l0_part_8x16_enable                : 1;    // enable L0 prediction for 8x16
  unsigned int   l0_part_8x8_enable                 : 1;    // enable L0 prediction for 8x8
  unsigned int   pskip_enable                       : 1;    // enable p_skip encoding (still used by ME for special modes, can this be removed?)
  //unsigned int   use_variance                       : 1;    // to decide between inter and intra (Most likely wont be used)
  unsigned int   explicit_eval_nearestmv            : 1;    // enable explicit search vp8 nearest mv
  unsigned int   explicit_eval_nearmv               : 1;    // enable explicit search vp8 near mv
  unsigned int   explicit_eval_left_topmv           : 1;    // enable explicit search vp8 8x8 left_top mv
  unsigned int   explicit_eval_left_bottommv        : 1;    // enable explicit search vp8 8x8 left_bottom mv
  unsigned int   explicit_eval_top_leftmv           : 1;    // enable explicit search vp8 8x8 top_left mv
  unsigned int   explicit_eval_top_rightmv          : 1;    // enable explicit search vp8 8x8 top_right mv
  unsigned int   explicit_eval_zeromv               : 1;    // enable explicit search vp8 zero mv
  unsigned int   refframe_pic                       : 2;    // reference frame type used 1 = last 2= golden 3= alt-ref
  unsigned int   reserved0                          : 3;    // Pad to int

  unsigned int   intra_luma4x4_mode_enable          : 9;    // bitmask indicating which intra luma 4x4 modes to enable
  unsigned int   intra_luma16x16_mode_enable        : 4;    // bitmask indicating which intra luma 16x16 modes to enable
  unsigned int   intra_chroma_mode_enable           : 4;    // bitmask indicating which intra chroma modes to enable
  unsigned int   intra_tm_mode_enable               : 1;    // bitmask indicating intra 4x4 true motion mode enable
  unsigned int   ip_search_mode                     : 3;    // which intra sizes to search before making IP decision b0=4x4,b2=16x16                                                          // We dont need a bit for 8x8 for VP8 but it might be simpler for reg field to keep it?
  unsigned int   mv_cost_enable                     : 1;    // enable mv cost calculations in mode decision
  unsigned int   reserved1                          : 10;   // pad to full int

  unsigned short early_intra_disable_mpeb_threshold;        // if early intra cost is below this, disable full search & select intra
  //unsigned short lambda_multiplier_mvcost;                  // used for mv cost calculation
           short bias_favor_intra_16x16_early;              // bias for early intra (subtracts from early intra 16x16 cost)

  unsigned short lambda_multiplier_intra[4];                // used for intra mode cost calculation  [segment_id]

  unsigned int   mdpLambdaCoefMd                    :16;
  unsigned int   mdpLambdaCoefMv                    :16;

           short bias_intra_16x16;                          // bias for intra 16x16 (subtracts from intra 16x16 cost)
           short bias_intra_4x4;                            // bias for intra 4x4   (subtracts from intra 4x4 cost)

           short bias_inter_16x16;                          // bias for inter 16x16 (subtracts from inter 16x16 cost)
           short bias_inter_16x8;                           // bias for inter 16x8  (subtracts from inter 16x8  cost)

           short bias_inter_8x16;                           // bias for inter 8x16  (subtracts from inter 8x16  cost)
           short bias_inter_8x8;                            // bias for inter 8x8   (subtracts from inter 8x8   cost)

           short bias_nearmv;                               // bias for nearmv      (subtracts from nearmv cost) for 16x16
           short bias_nearestmv;                            // bias for nearestmv   (subtracts from nearestmv cost) for 16x16

           short bias_zeromv;                               // bias for zeromv      (subtracts from zeromv cost) for 16x16
           short bias_newmv;                                // bias for newmv       (subtracts from newmv cost) for 16x16

           short bias_above4x4mv;                           // bias for above4x4mv  (subtracts from above4x4 cost) for VP8 SPLITMV modes
           short bias_left4x4mv;                            // bias for left4x4mv  (subtracts from left4x4 cost) for VP8 SPLITMV modes

           short bias_zero4x4mv;                            // bias for zero4x4mv  (subtracts from zero4x4 cost) for VP8 SPLITMV modes
           short bias_new4x4mv;                             // bias for new4x4mv  (subtracts from new4x4 cost) for VP8 SPLITMV modes

           short mv_cost_bias;                              // bias applied to internal motion vector cost calculation
           short bias_intra_over_inter;                     // bias for any intra   (subtracts from intra cost)
  unsigned short intra_refresh_cnt;                         // distance between forced-intra MBs; 0 means OFF
  unsigned short intra_refresh_offs;                        // first forced-intra MB

  unsigned short tempmv_wt_spread_threshold;                // The threshold for the sum of difference between 8x8 and 16x16 mvs. Used for temporal mv weight.
  unsigned short tempmv_wt_distort_threshold;               // The threshold for cost of the 16x16 mv. Used for temporal mv weight.
} nvenc_vp8_md_control_s;                                   // 64 bytes

typedef struct
{
  unsigned int   self_temporal_stamp_l0             : 3;    // stamp to use for L0 integer search in stamp based mode
  //unsigned int   self_temporal_stamp_l1             : 3;    // stamp to use for L1 integer search in stamp based mode
  unsigned int   self_temporal_explicit             : 1;    // explicitly evaluate these vectors
  unsigned int   self_temporal_search               : 1;    // integer search enable for this hint
  unsigned int   self_temporal_refine               : 1;    // subpel search enable for this hint
  unsigned int   self_temporal_enable               : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)

  unsigned int   coloc_stamp_l0                     : 3;    // stamp to use for L0 integer search in stamp based mode
  //unsigned int   coloc_stamp_l1                     : 3;    // stamp to use for L1 integer search in stamp based mode
  unsigned int   coloc_explicit                     : 1;    // explicitly evaluate these vectors
  unsigned int   coloc_search                       : 1;    // integer search enable for this hint
  unsigned int   coloc_refine                       : 1;    // subpel search enable for this hint
  unsigned int   coloc_enable                       : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)

  unsigned int   self_spatial_stamp_l0              : 3;    // stamp to use for L0 integer search in stamp based mode
  //unsigned int   self_spatial_stamp_l1              : 3;    // stamp to use for L1 integer search in stamp based mode
  unsigned int   self_spatial_explicit              : 1;    // explicitly evaluate these vectors
  unsigned int   self_spatial_search                : 1;    // integer search enable for this hint
  unsigned int   self_spatial_refine                : 1;    // subpel search enable for this hint
  unsigned int   self_spatial_enable                : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)

  unsigned int   reserved0                          : 11;   // Pad to int

  unsigned int   external_stamp_lastref_stamp       : 3;    // Stamp to use for external L0 hint with refidx = 0
  unsigned int   external_stamp_goldref_stamp       : 3;    // Stamp to use for external L0 hint with refidx = 1
  unsigned int   external_stamp_altref_stamp        : 3;    // Stamp to use for external L0 hint with refidx = 2
  unsigned int   external_explicit                  : 1;    // explicitly evaluate these vectors
  unsigned int   external_search                    : 1;    // integer search enable for this hint
  unsigned int   external_refine                    : 1;    // subpel search enable for this hint
  unsigned int   external_enable                    : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)

  unsigned int   const_mv_stamp_l0                  : 3;    // stamp to use for L0 integer search in stamp based mode
  //unsigned int   const_mv_stamp_l1                  : 3;    // stamp to use for L1 integer search in stamp based mode
  unsigned int   const_mv_explicit                  : 1;    // explicitly evaluate these vectors
  unsigned int   const_mv_search                    : 1;    // integer search enable for this hint
  unsigned int   const_mv_refine                    : 1;    // subpel search enable for this hint
  unsigned int   const_mv_enable                    : 1;    // hint enable (enables processing + fetching of data)

  unsigned int   reserved1                          : 12;    // pad to int

} nvenc_vp8_me_hint_cfg_s;                                  // 8 bytes

// Same as H264 me_limit_mv struct
typedef struct
{
  unsigned int   mv_limit_enable                    : 1;    // 0=disable, 1=enable set the absolute MV range (Cisco flip camera request)
  unsigned int   left_mvx_frac                      : 2;    // vector X component fraction
           int   left_mvx_int                       : 12;   // vector X component integer portion
  unsigned int   reserved1                          : 1;    // padding X to 16 bit
  unsigned int   top_mvy_frac                       : 2;    // vector Y component fraction
           int   top_mvy_int                        : 10;   // vector Y component integer portion
  unsigned int   reserved2                          : 4;    // padding Y to 16 bit

  unsigned int   right_mvx_frac                     : 2;    // vector X component fraction
           int   right_mvx_int                      : 12;   // vector X component integer portion
  unsigned int   reserved3                          : 2;    // padding X to 16 bit
  unsigned int   bottom_mvy_frac                    : 2;    // vector Y component fraction
           int   bottom_mvy_int                     : 10;   // vector Y component integer portion
  unsigned int   reserved4                          : 4;    // padding Y to 16 bit

} nvenc_vp8_me_limit_mv_s;                                 // 8 bytes

typedef struct
{
  unsigned int   mvx_frac                           : 2;    // vector X component fraction
           int   mvx_int                            : 12;   // vector X component integer portion
  unsigned int   reserved_x                         : 2;    // padding X to 16 bit
  unsigned int   mvy_frac                           : 2;    // vector Y component fraction
           int   mvy_int                            : 10;   // vector Y component integer portion
  unsigned int   reserved_y                         : 4;    // padding Y to 16 bit

} nvenc_vp8_me_const_vec_s;                                // 4 bytes

// TO DO: We are wasting ~7 ints here we can combine this to pack hor_adjust and ver_adjust in diff int
//        but dont do it if we are going to throw it away in padding anyways
typedef struct
{
  unsigned int   bitmask[2]                         ;       // 8x8 search point pattern bitmask
  unsigned int   hor_adjust                         : 1;    // shift hor based on lsb of pred
  unsigned int   ver_adjust                         : 1;    // shift ver based on lsb of pred
  unsigned int   reserved                           : 30;

} nvenc_vp8_me_stamp_s;                                     // 12 bytes

typedef struct
{
  unsigned int   me_predictor_mode                  : 1;    // 0=MDP, 1=const
  unsigned int   refinement_mode                    : 1;    // 0=hpel, 1=qpel
  unsigned int   lambda_mode                        : 1;    // 0=MDP, 1=const
  unsigned int   const_lambda                       : 11;   // U8.3 format lambda, used if lambda_mode=1
  unsigned int   refine_on_search_enable            : 1;    // 0=disable, 1=enable subpel refine for search candidates
  unsigned int   me_only_mode                       : 1;
  unsigned int   fps_mvcost                         : 1;
  unsigned int   sps_mvcost                         : 1;
  unsigned int   sps_cost_func                      : 1;
  unsigned int   me_icc_mode_mad                    : 1;
  unsigned int   sps_filter                         : 3;
  unsigned int   mc_filter                          : 3;
  unsigned int   mbc_ctrl_arbitor                   : 2;
  unsigned int   average_mvhint_enable              : 1;
  unsigned int   rmvp_source_half_scaled            : 1;    // only support picture size half scale down in horz&vert
  unsigned int   reserved                           : 2;

  nvenc_vp8_me_limit_mv_s      limit_mv;
  nvenc_vp8_me_hint_cfg_s      predsrc;                     // predictor sources
  nvenc_vp8_me_const_vec_s     l0_hint;                     // constant MV used as L0 hint
  nvenc_vp8_me_const_vec_s     l1_hint;                     // constant MV used as L1 hint
  nvenc_vp8_me_const_vec_s     l0_pred;                     // constant MV used as L0 predictor
  // should this be made an array?
  nvenc_vp8_me_stamp_s         shape0;                      // priority 0 stamping pattern
  nvenc_vp8_me_stamp_s         shape1;                      // priority 1 stamping pattern
  nvenc_vp8_me_stamp_s         shape2;                      // priority 2 stamping pattern
  nvenc_vp8_me_stamp_s         shape3;                      // priority 3 stamping pattern
  nvenc_vp8_me_stamp_s         shape4;                      // priority 4 stamping pattern
  nvenc_vp8_me_stamp_s         shape5;                      // priority 5 stamping pattern
  nvenc_vp8_me_stamp_s         shape6;                      // priority 6 stamping pattern
  nvenc_vp8_me_stamp_s         shape7;                      // priority 7 stamping pattern

  nvenc_me_tebif_s             teb_para;                    // teb parameters
  unsigned char                mbc_mb_size;                 // MBC size in MB
  unsigned char                tempmv_scale;                // temporal mv scale
  unsigned char                reserved1[2];

} nvenc_vp8_me_control_s;                                   // 136 bytes

typedef struct
{
  unsigned int   qpp_run_vector_4x4                 : 16;   // cost values for 4x4 transform (16 bit total vector)
  unsigned int   qpp_luma8x8_cost                   : 4;    // luma 8x8 cost threshold, 0 = throw out all coefs, range 0-15
  unsigned int   qpp_luma16x16_cost                 : 4;    // luma 16x16 cost threshold, 0 = throw out all coefs, range 0-15
  unsigned int   qpp_chroma_cost                    : 4;    // chroma cost threshold, 0 = throw out all coefs, range 0-15
  unsigned int   qpp_mode                           : 2;    // 0 = OFF, 1 = 8x8, 2 = 16x16_8x8
  //unsigned int   quant_intra_sat_flag               : 1;    // 0: no saturation is applied; 1: saturation is applied
  //unsigned int   quant_inter_sat_flag               : 1;    // 0: no saturation is applied; 1: saturation is applied
  unsigned int   reserved                           : 2;

  //unsigned int   quant_intra_sat_limit              : 16;   // quantization saturation limit for intra MB
  //unsigned int   quant_inter_sat_limit              : 16;   // quantization saturation limit for inter MB

  unsigned short dz_4x4_YI[16];                             // deadzone for 4x4 transform of Luma Intra
  unsigned short dz_4x4_YP[16];                             // deadzone for 4x4 transform of Luma Inter
  unsigned short dz_4x4_CI;                                 // deadzone for 4x4 transform of Chroma Intra
  unsigned short dz_4x4_CP;                                 // deadzone for 4x4 transform of Chroma Inter

} nvenc_vp8_quant_control_s;                                // 72 bytes

typedef struct
{
           //char  temp_dist_l0[3];                           // temporal distance of each ref pic (last, gold, altref)
           //char  reserved;
  unsigned int   hist_buf_size;                             // size in bytes of the buffer allocated for history
  unsigned int   bitstream_buf_size;                        // size in bytes of the buffer allocated for bitstream slice/mb data
  unsigned int   bitstream_start_pos;                       // start position in bitstream buffer where data should be written (byte offset)
  unsigned int   bitstream_residual_buf_size;               // size in bytes of the buffer allocated for residual partition bitstream slice/mb data
  unsigned int   bitstream_residual_start_pos[8];           // start position in residual partition bitstream buffer where data for residual partitions should be written (byte offset)

} nvenc_vp8_pic_control_s;                                 // 48 bytes

typedef struct
{
  //unsigned char  hrd_type;                                  // nal (=2) and vcl (=1) type
  unsigned char  QP[3];                                     // QP for 0:P picture, ,1:B picture 2; I picture
  unsigned char  minQP[3];                                  // min QP for 0:P picture, ,1:B picture 2; I picture
  unsigned char  maxQP[3];                                  // max QP for 0:P picture, ,1:B picture 2; I picture
  //unsigned char  maxQPD;                                    // max QP delta between two consecutive QP updates
  //unsigned char  baseQPD;                                   // initial QP delta between two consecutive QP updates
  unsigned char  reserved[3];
           int   rhopbi[3];                                 // 23.8 signed fixed point quant ratios P/I, B/I, I/I(=1)

           int   framerate;                                 // fps
  unsigned int   buffersize;                                // total buffer size

           int   nal_cpb_size;                              // size in bits
           int   nal_bitrate;                               // rate in bps
           //int   vcl_cpb_size;                              // size in bits
           //int   vcl_bitrate;                               // rate in bps

           int   gop_length;                                // I period
           //int   Np;                                        // 27.4 signed fixed (gop_length + num_b_frames -1)/(num_b_frames + 1)
           int   Bmin;                                      // 23.8 signed fixed point min buffer level, updated by driver at sequence level only.
           int   Ravg;                                      // 23.8 signed fixed point average rate, updated by driver at sequence level only.
           int   R;                                         // 23.8 signed fixed point rate

  //unsigned char  ab_alpha;                                  // weight of prev frame activity compared to part of the current picture
  //unsigned char  ab_beta;                                   // weight of current activity compared to neighbors
  //unsigned short prev_act;                                  // average activity level of the first reference (L0)
} nvenc_vp8_rc_s;                                          // 56 bytes

typedef struct
{
  // 256 aligned
  unsigned int                      magic;                  // magic number
  nvenc_vp8_surface_cfg_s           refpic_cfg;             // 28 bytes
  nvenc_vp8_surface_cfg_s           input_cfg;              // 28 bytes
  nvenc_vp8_surface_cfg_s           outputpic_cfg;          // 28 bytes
  nvenc_vp8_sequence_data_s         sequence_data;          // 4 bytes
  nvenc_vp8_picture_data_s          picture_data;           // 32 bytes
  nvenc_vp8_pic_control_s           pic_control;            // 48 bytes
  nvenc_vp8_me_control_s            me_control;             // 136 bytes
  nvenc_vp8_md_control_s            md_control;             // 64 bytes
  nvenc_vp8_quant_control_s         quant_control;          // 72 bytes
  nvenc_vp8_rc_s                    rate_control;           // 56 bytes
  unsigned char                     reserved[16];           // pad to 512 bytes

} nvenc_vp8_drv_pic_setup_s;                                // 512 bytes


// 2.2. Colocated Motion Data Buffer
// ---------------------------------
//
// This buffer will be used to store co-located motion data. This data is produced for
// pictures that are marked as reference pictures. The buffer is not needed for non-
// reference pictures and in such case the corresponding method may be skipped. The
// data in this buffer will be needed for future pictures encoded by both CEA and NVENC.
// There are two of these buffers: one output, written only if the current picture is a
// reference picture, and one input, read if the current picture is being encoded
// as a B picture, and possible also during P picture to serve as hints for motion search.
// Each buffer holds coloc data for all macroblocks in the picture. This
// data is packed into an array of 16 entries of 4 bytes. Each entry holds one motion
// vector and 6 bits of additional data, which is formatted as follows:
//
// coldata array entry N:      contents of 'e':
//     0, 4, 8, 12                 0    = bottom field flag for ref picture
//                                 4..1 = reference picture number (index in DPB)
//                                 5    = zero
//     1, 5, 9, 13                 3..0 = ColZero flag for blocks N+2 .. N-1
//                                 5..4 = subpartition size for 8x8 block (N-1)/4
//                                         (0=8x8, 1=8x4, 2=4x8, 3=4x4)
//     15                          0    = field mode flag
//                                 1    = intra coded
//                                 3..2 = partition size for macroblock
//                                         (0=16x16, 1=16x8, 2=8x16, 3=8x8)
//                                 5..4 = zero
//     all others                  5..0 = zero
//
// Note that motion vectors are duplicated for each 4x4 index from the top-left 4x4 block
// of each (sub)partition. Note also that since NVENC does not currently support sub-8x8
// inter partitioning, the 4 vectors within an 8x8 are replicated values.

typedef struct
{
  unsigned int   mvx                                : 14;   // bit  0..13: x component of MV
  unsigned int   mvy                                : 12;   // bit 14..25: y component of MV
  unsigned int   e                                  : 6;    // bit 26..31: extra data, different per entry
} nvenc_h264_coloc_entry_s;

typedef struct                                              // 64 byte aligned block
{
  nvenc_h264_coloc_entry_s    coldata[16];                  // col data for 1 macroblock = array of 16 entries
} nvenc_h264_coloc_mb_s;


// 2.3. History Buffer
// -------------------
//
// This buffer will be used to keep history data (data produced for one macroblock that
// must be kept until the macroblock in the row directly below is processed). This buffer
// should be at least as large as (((frame_width_minus1+16) / 16) + 16)*128 bytes.
//
// After the execute completes, the data in this buffer is no longer needed and the
// buffer can be discarded/deallocated.
//
// The internal structure of the history record is implementation specific and therefore
// subject to change. The data is produced and consumed by NVENC only and should never be
// interpreted by any other engine or by software. The current structure is defined in
// the following visio document in the HW development tree:
//
// doc/nvenc/arch/hist_layout.vsd
//
// 2.4. Reference and Input Picture Buffer
// ---------------------------------------
//
// Reference Picture Buffer contains reference pictures produced by NVENC during
// encode opeations of previous pictures. The Input Picture Buffer contains the
// current picture that needs to be encoded into a bitstream.
//
// Each picture is actually a separate buffer with its own surface offset method.
// Relative position of luma, chroma fields are defined in the picture setup buffer
// and shown in the figure below.
//
// Driver needs to send SetRefPicOffset#() method for only the used reference pictures
// for the current execute.
//
// Pictures are by default stored in NV24 field or NV12 frame format. NVENC will
// always output in the same format as the current picture that is being encoded
// (frame or field). True picaff (using frame refs to encode a field or using field
// refs to encode a frame) is NOT supported.
//
// The input picture must contain fields if the picture is to be encoded as fields,
// and must contain frames if the picture is to be encoded as a frame.
//
//
//    +---------------+ <-- RefPicOffset (start of buffer)
//    |XXXXXXXXXXXXXXX|
//    +---------------+ <-- luma_top_frm_offset (in picture setup buffer, in units of 256 bytes)
//    |TOP/FRM luma   |
//    |               |
//    +XXXXXXXXXXXXXXX+
//    +---------------+ <-- luma_bot_offset
//    |BOTTOM luma    |
//    |               |
//    +XXXXXXXXXXXXXXX+
//    +---------------+ <-- chroma_top_frm_offset
//    |TOP/FRM chroma |
//    +XXXXXXXXXXXXXXX+
//    +---------------+ <-- chroma_bot_offset
//    |BOTTOM chroma  |
//    +XXXXXXXXXXXXXXX+
//    +---------------+ <-- end of buffer
//
//
// 2.5. CEA Hints Data Buffer
// ---------------------------
//
// CEA (a CUDA based integer motion search kernel) can provide hints to NVENC.
// These hints can either be taken as explicit vectors to evaluate or as guides
// for NVENC's built-in integer search.
// In this mode of operation, CEA and NVENC typically work on the same frame,
// with CEA slightly leading (by a few macroblock rows). NVENC would then read
// in the data already produced by CEA for the current picture and use it. CEA
// has to provide information on how many macroblocks worth of data it has
// already completed and written out into this buffer, so that NVENC can read
// up to that point. The buffer must be big enough to contain all data for the
// entire frame.
//
// This buffer consists of 2 sections: a header and a data section. One such
// buffer must be provided/allocated by driver for each picture that needs to
// be encoded. The header section must be prefilled with zeroes by driver. The
// data section can remain uninitialized.
//
// The data section contains the motion vector candidates for each macroblock.
// It is written by CEA and read by NVENC.
//
// The header section contains flow control and status information produced by
// CEA for this picture, and contains an offset to the start of the data
// section. The header section begins at offset 0 in this buffer.


typedef struct
{
  unsigned int   cand_cnt_blk_16x16                 : 4;   // 3:0 Number of candidates per block types
  unsigned int   cand_cnt_blk_16x8                  : 4;   // 7:4
  unsigned int   cand_cnt_blk_8x16                  : 4;   // 11:8
  unsigned int   cand_cnt_blk_8x8                   : 4;   // 15:12
  unsigned int   cand_cnt_blk_8x4                   : 4;   // 19:16
  unsigned int   cand_cnt_blk_4x8                   : 4;   // 23:20
  unsigned int   cand_cnt_blk_4x4                   : 4;   // 27:24
  unsigned int   cand_bdirect_spatial_zmv           : 1;   // 28 enable Spatial direct enable ZMV 4x4 scores
  unsigned int   cand_bdirect_spatial_nzmv          : 1;   // 29 enable Spatial direct enable non-zero MV 4x4 scores
  unsigned int   cand_bdirect_temporal              : 1;   // 30 enable Temporal direct mode. Only valid for Bi-type.
  unsigned int   cand_pskip                         : 1;   // 31 enable p-skip mode. Only valid for L0 controls.
} cea_cand_count_blktype_s;

typedef struct
{
  // 64 aligned
  unsigned int              picture_index;                  // picture index given by driver (via method)
  unsigned int              data_start;                     // start offset in buffer of data section which must be 256B aligned
  // the following fields are only there right now to make it easier to implement the cmodel plugin mode
  // they might be removed at a later stage
  unsigned int              cea_lambda;
  cea_cand_count_blktype_s  blk_counts[3];
  int                       reserved[10];                   //padded by 8 bytes to align the end of the structure to 32 bytes for dmread operations
} cea_hints_header_s;


// Please refer to the document named "nvenc_cea_mvinfo_format.doc" for more information.
// This document can be found in the nvenc HW development tree under doc/nvenc/arch

typedef struct
{
  int              mvx         : 12;                        // 11:00 x component of integer pixel MV (relative to current MB) S12.0
  int              mvy         : 10;                        // 21:12 y component of integer pixel MV (relative to current MB) S10.0
  int              refidx      : 5;                         // 26:22 reference index (31=invalid)
  int              dir         : 1;                         // 27:27 0=L0 1=L1
  int              part_type   : 2;                         // 29:28 0=16x16 1=16x8 2=8x16 3=8x8 (blocks in partition must be consecutive)
  int              last_of_part: 1;                         // 30:30 last MV of (sub) partition
  int              last_of_mb  : 1;                         // 31:31 last MV of macroblock
} cea_hint_s;

// External hint data structure for Orin AV1
typedef struct
{
  short            refidx         : 5;                      // 4:0 reference index (31=invalid)
  short            direction      : 1;                      // 5:5 0=L0 1=L1
  short            bi             : 1;                      // 6:6 0=single mv, 1=compound mv
  short            partition_type : 3;                      // 9:7 parition type: 0: 2NX2N, 1:2NxN, 2:Nx2N. reserved 3bits for modes in later projects
  short            x8             : 3;                      // 12:10 x8: current parition's top left x position in 8 pixel unit
  short            last_of_cu     : 1;                      // 13:13 last mv of current CU
  short            last_of_sb     : 1;                      // 14:14 last mv of current SB
  short            reserved0      : 1;

  short            mvx            : 14;                     // 13:0  mvx, in S12.2 format
  short            cu_size        : 2;                      // 15:14 CU size: 0: 8x8, 1: 16x16, 2:32x32, 3:64x64

  short            mvy            : 12;                     // 11:0 mvy, in S10.2 format
  short            y8             : 3;                      // 14:12 y8: current parition's top left y position in 8 pixel unit
  short            reserved1      : 1;
} av1_hint_data;  //6 bytes


typedef struct
{
  unsigned int     segID[4];                                //segment ID for one MB. each 4x4 block has a segID which is 8 bits, total is 8bitx16 = 128 bits. maximun segments supported is 32
} cea_segment_s;



// 2.6. Encoder Status Buffer
// ---------------------------
//
// This buffer provides some additional data about the encoded bit stream that can
// be used for rate control and multi-pass encoding as well as status information
// that can be used for debug.

typedef struct
{
  unsigned int   picture_index;                             // value received from SetPictureIndex method
  unsigned int   error_status                       : 2;    // report error if any
  unsigned int   ucode_error_status                 : 30;   // report error status from ucode to driver
  unsigned int   total_bit_count;                           // picture size in bits
  unsigned int   type1_bit_count;                           // type1 bit count for the entire picture
  unsigned short pic_type;                                  // copied from pic_type in nvenc_h264_pic_control_s
  unsigned short num_slices;                                // number of slices produced
  unsigned short ave_activity;                              // report average activity if activity based RC enabled
  unsigned short avgQP;                                     // average QP
  unsigned int   cycle_count;                               // total cycles taked for execute. Written out only when DumpCycleCount in SetControlParams is set
           int   hrdFullness;
  unsigned int   bitstream_start_pos;                       // byte_offset where mpec will start writing slice header
  unsigned int   last_valid_byte_offset;                    // greatest offset in output buffer which is assured to have valid compressed data
  unsigned short intra_mb_count;                            // number of intra MBs.
  unsigned short inter_mb_count;                            // number of inter MBs.
  unsigned int   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
  unsigned int   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
  unsigned int   total_intra_cost;                          // sum of all intra best mode's SATDs, each mb has one intra SATD.
  unsigned int   total_inter_cost;                          // sum of all inter best mode's SATDs, each mb has one inter SATD.
  unsigned int   complexity;                                // sum of mbRowBitcount * current QS
           short average_mvx;                               // average mvx info, can be used for ME hint
           short average_mvy;                               // average mvy info, can be used for ME hint
  unsigned short actual_min_qp_used;                        // actual minimum qp used in the frame
  unsigned short actual_max_qp_used;                        // actual maximum qp used in the frame
  unsigned int  hw_perf_me_sps;                             // me slice per second
  unsigned int  hw_perf_me_fps;                             // me frame per second
  unsigned int  hw_perf_mpec_active_cnt;                    // mpec active count
  unsigned int  hw_perf_mpeb_active_cnt;                    // mpeb active count
  unsigned long long int total_dist_ssd;                    // sum of all ssd distortion of the MB best mode
  unsigned int  total_dist_ssim;                            // sum of all ssim distortion of the MB best mode
  unsigned int  best_satd_l0;                               // sum of all best satd cost of L0
  unsigned int  best_satd_l1;                               // sum of all best satd cost of L1
  unsigned int reserved[5];                                 // align to 128 bytes

} nvenc_pic_stat_s;                                         // 128 bytes


typedef struct
{
  unsigned int   slice_offset;                              // slice offset in bytes
  unsigned int   slice_size;                                // slice size in bytes
  unsigned int   slh_bit_count;                             // slice header size in bits
  unsigned char  reserved[4];
} nvenc_slice_stat_s;                                       // 16 bytes per status


typedef struct
{
  unsigned int   intra_res_bit_count;                       // #bits spent on intra residuals
  unsigned int   inter_res_bit_count;                       // #bits spent on inter residuals
  unsigned int   intra_mode_bit_count;                      // #bits spent on intra modes (mbtype, pred mode)
  unsigned int   inter_mode_bit_count;                      // #bits spent on inter modes (mbtype, submbtype, refidx, mv)
  unsigned int   ipcm_bit_count;                            // # bits spent on MB mode that caused IPCM rewind.
  unsigned int   type1_bit_count;                           // count of bits in type I bitstream (excludes leading_zero_8bits,
                                                            // start_codeprefix_one_3bytes and trailing_zero_8bits)
  unsigned int   total_bit_count;                           // total number of bits produced

  unsigned int   total_bin_count;                           // total number of bins produced by cabac
  unsigned int   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
  unsigned int   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
  unsigned int   cumulative_ipcm_cost;                      // sum of all inter SA(T)Ds for the MBs that caused IPCM rewind
  unsigned int   cumulative_skip_cost;                      // sum of all P and B skip MB SA(T)Ds

  unsigned short intra_mb_count;                            // number of intra MBs.
  unsigned short inter_mb_count;                            // number of inter MBs.
  unsigned short skip_mb_count;                             // number of skip MBs.
  unsigned short ipcm_mb_count;                             // number of ipcm MBs.

  unsigned int   total_intra_cost;                          // sum of all intra best mode's SATDs, each ctb has one intra SATD.
  unsigned int   total_inter_cost;                          // sum of all inter best mode's SATDs, each ctb has one inter SATD.
} nvenc_mpec_stat_s;                                       // 64 bytes per status

typedef struct
{
  unsigned int   qp_val                             : 8  ;  // qp used for encoding this block
  unsigned int   total_bit_count                    : 24 ;  // total number of bits produced
  unsigned int   res_cost                           : 24 ;  // satd cost of residual error
  unsigned int   flags                              : 8  ;  // flags
} nvenc_stats_fifo_s;                                       // 8 bytes per stat

typedef struct
{
  nvenc_pic_stat_s   pic_stat;                              // 64Bytes
  nvenc_slice_stat_s slice_stat[((4096/16)*(4096/16)) + 3]; // To account for 1mb/slice and 4kx4k, plus 3 for alignment
  nvenc_mpec_stat_s  mpec_stat[((4096/16)*(4096/16)) + 3];  // To account for stat/1mb and 4kx4k, plus 3 for alignment
  nvenc_aq_stat_s    aq_stats[((4096/16)*(4096/16)) + 3];   // AQ stats from PDMA
  nvenc_stats_fifo_s stats_fifo[((4096/16)*(4096/16)) + 3];
} nvenc_stat_data_s;

typedef struct
{
  unsigned int   picture_index;                             // value received from SetPictureIndex method
  unsigned int   error_status                       : 2;    // report error if any
  unsigned int   ucode_error_status                 : 30;   // report error status from ucode to driver
  unsigned int   total_bit_count;                           // picture size in bits
  unsigned int   type1_bit_count;                           // type1 bit count for the entire picture
  unsigned short pic_type;                                  // copied from pic_type in nvenc_h264_pic_control_s
  unsigned short num_slices;                                // number of slices produced, maxinum slice number is 65535
  unsigned short ave_activity;                              // report average activity if activity based RC enabled
  unsigned short avgQP;                                     // average QP
  unsigned int   cycle_count;                               // total cycles taked for execute. Written out only when DumpCycleCount in SetControlParams is set
           int   hrdFullness;
  unsigned int   bitstream_start_pos;                       // byte_offset where mpec will start writing slice header
  unsigned int   last_valid_byte_offset;                    // greatest offset in output buffer which is assured to have valid compressed data
  unsigned int   intra_cu32x32_count;                       // number of intra 32x32 CUs.
  unsigned int   inter_cu32x32_count;                       // number of inter 32x32 CUs.
  unsigned int   intra_cu16x16_count;                       // number of intra 16x16 CUs.
  unsigned int   inter_cu16x16_count;                       // number of inter 16x16 CUs.
  unsigned int   intra_cu8x8_count;                         // number of intra 8x8   CUs.
  unsigned int   inter_cu8x8_count;                         // number of inter 8x8   CUs.
  unsigned int   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
  unsigned int   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
  unsigned int   total_intra_cost;                          // sum of all intra best mode's SATDs, each mb has one intra SATD.
  unsigned int   total_inter_cost;                          // sum of all inter best mode's SATDs, each mb has one inter SATD.
           short average_mvx;                               // average mvx info, can be used for ME hint
           short average_mvy;                               // average mvy info, can be used for ME hint
  unsigned short actual_min_qp_used;                        // actual minimum qp used in the frame
  unsigned short actual_max_qp_used;                        // actual maximum qp used in the frame
  unsigned int  hw_perf_me_sps;                             // me slice per second
  unsigned int  hw_perf_me_fps;                             // me frame per second
  unsigned int  hw_perf_mpec_active_cnt;                    // mpec active count
  unsigned int  hw_perf_mpeb_active_cnt;                    // mpeb active count
  unsigned char  reserved[24];
} nvenc_h265_pic_stat_s;                                    // 128 bytes

typedef struct
{
  unsigned int   slice_offset;                              // slice offset in bytes
  unsigned int   slice_size;                                // slice size in bytes
  unsigned int   slh_bit_count;                             // slice header size in bits
  unsigned int  slice_size_ctbs;                            // slice size in ctbs
}nvenc_h265_slice_stat_s;                                   // 16 bytes per status

typedef struct
{
  unsigned int   intra_res_bit_count;                       // #bits spent on intra residuals
  unsigned int   inter_res_bit_count;                       // #bits spent on inter residuals
  unsigned int   intra_mode_bit_count;                      // #bits spent on intra modes (mbtype, pred mode)
  unsigned int   inter_mode_bit_count;                      // #bits spent on inter modes (mbtype, submbtype, refidx, mv)
  unsigned int   ipcm_bit_count;                            // # bits spent on MB mode that caused IPCM rewind.
  unsigned int   type1_bit_count;                           // count of bits in type I bitstream (excludes leading_zero_8bits,
                                                            // start_codeprefix_one_3bytes and trailing_zero_8bits)
  unsigned int   total_bit_count;                           // total number of bits produced

  unsigned int   total_bin_count;                           // total number of bins produced by cabac
  unsigned int   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
  unsigned int   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
  unsigned int   cumulative_ipcm_cost;                      // sum of all inter SA(T)Ds for the MBs that caused IPCM rewind
  unsigned int   cumulative_skip_cost;                      // sum of all P and B skip MB SA(T)Ds
  unsigned int   total_intra_cost;                          // sum of all intra best mode's SATDs, each ctb has one intra SATD.
  unsigned int   total_inter_cost;                          // sum of all inter best mode's SATDs, each ctb has one inter SATD.

  unsigned int   intra_cu32x32_count;                       // number of intra 32x32 CUs.
  unsigned int   inter_cu32x32_count;                       // number of inter 32x32 CUs.
  unsigned int   skip_cu32x32_count;                        // number of skip 32x32 CUs.
  unsigned int   ipcm_cu32x32_count;                        // number of ipcm 32x32 CUs.
  unsigned int   intra_cu16x16_count;                       // number of intra 16x16 CUs.
  unsigned int   inter_cu16x16_count;                       // number of inter 16x16 CUs.
  unsigned int   skip_cu16x16_count;                        // number of skip 16x16 CUs.
  unsigned int   ipcm_cu16x16_count;                        // number of ipcm 16x16 CUs.
  unsigned int   intra_cu8x8_count;                         // number of intra 8x8 CUs.
  unsigned int   inter_cu8x8_count;                         // number of inter 8x8 CUs.
  unsigned int   skip_cu8x8_count;                          // number of skip 8x8 CUs.
  unsigned int   ipcm_cu8x8_count;                          // number of ipcm 8x8 CUs.

  unsigned int   total_emulated_bit_count;                  // total number of emulated bits in total_bit_count
  unsigned char  reserved[20];
} nvenc_h265_mpec_stat_s;                                   // 128 bytes per status

typedef struct
{
  unsigned int   var_min;
  unsigned int   var_max;
  unsigned int   var_avg;
  unsigned char  reserved[4];
} nvenc_h265_aq_stat_s;                                     // 16 bytes

typedef struct
{
  unsigned int   qp_val                             : 8  ;  // qp used for encoding this block
  unsigned int   total_bit_count                    : 24 ;  // total number of bits produced
  unsigned int   res_cost                           : 24 ;  // satd cost of residual error
  unsigned int   flags                              : 8  ;  // flags
} nvenc_h265_stats_fifo_s;                                  // 8 bytes per stat

typedef struct
{
  nvenc_h265_pic_stat_s   pic_stat;                              // 64Bytes
  nvenc_h265_slice_stat_s slice_stat[((4096/16)*(4096/16)) + 3]; // To account for 1mb/slice and 4kx4k, plus 3 for alignment. support upto 65535 slices
  nvenc_h265_mpec_stat_s  mpec_stat[((4096/16)*(4096/16)) + 3];  // To account for stat/1mb and 4kx4k, plus 3 for alignment. support upto 65535 slices
  nvenc_h265_aq_stat_s    aq_stats [((4096/16)*(4096/16)) + 3];  // AQ stats from pdma. support upto 65535 slices
  nvenc_h265_stats_fifo_s stats_fifo[((4096/16)*(4096/16)) + 3];
} nvenc_h265_stat_data_s;

typedef struct
{
    unsigned int   picture_index;                             // value received from SetPictureIndex method
    unsigned int   error_status                       : 2;    // report error if any
    unsigned int   ucode_error_status                 : 30;   // report error status from ucode to driver
    unsigned int   total_bit_count;                           // picture size in bits
    unsigned int   type1_bit_count;                           // type1 bit count for the entire picture
    unsigned short num_tiles;                                 // number of tiles produced
    unsigned char  av1_start_QP;                              // AV1 QP value at frame encoding start (to be inserted into frame header by driver)
    unsigned char  loop_filter_level;                         // loop filer value computed by ucode (to be inserted into frame header by driver)
    unsigned int   pic_type                           : 14;   // copied from pic_type in nvenc_h264_pic_control_s
    unsigned int   cdef_damping_minus_3               : 2;    // CDEF damping value computed by ucode (to be inserted into frame header by driver)
    int            avgQP                              : 16;   // average QP
    unsigned int   cycle_count;                               // total cycles taked for execute. Written out only when DumpCycleCount in SetControlParams is set
    unsigned int   hrdFullness;
    unsigned int   bitstream_start_pos;                       // byte_offset where mpec will start writing slice header
    unsigned int   intra_cu64x64_count;                       // number of intra 64x64 CUs.
    unsigned int   inter_cu64x64_count;                       // number of inter 64x64 CUs.
    unsigned int   intra_cu32x32_count;                       // number of intra 32x32 CUs.
    unsigned int   inter_cu32x32_count;                       // number of inter 32x32 CUs.
    unsigned int   intra_cu16x16_count;                       // number of intra 16x16 CUs.
    unsigned int   inter_cu16x16_count;                       // number of inter 16x16 CUs.
    unsigned int   intra_cu8x8_count;                         // number of intra 8x8   CUs.
    unsigned int   inter_cu8x8_count;                         // number of inter 8x8   CUs.
    unsigned int   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
    unsigned int   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
    unsigned int   total_intra_cost;                          // sum of all intra best mode's SATDs, each mb has one intra SATD.
    unsigned int   total_inter_cost;                          // sum of all inter best mode's SATDs, each mb has one inter SATD.
    short          average_mvx;                               // average mvx info, can be used for ME hint
    short          average_mvy;                               // average mvy info, can be used for ME hint
    short          actual_min_qp_used;                        // actual minimum qp used in the frame
    short          actual_max_qp_used;                        // actual maximum qp used in the frame
    unsigned int   hw_perf_me_sps;                            // me slice per second
    unsigned int   hw_perf_me_fps;                            // me frame per second
    unsigned int   hw_perf_entropy_active_cnt;                // Entropy active count
    unsigned int   hw_perf_rdo_active_cnt;                    // RDO active count
    unsigned short ave_activity;                              // report average activity if activity based RC enabled
    unsigned char  reserved[18];
} nvenc_av1_pic_stat_s;                                   // 128 bytes

typedef struct
{
    unsigned int   tile_offset;                               // tile offset in bytes
    unsigned int   tile_size;                                 // tile size in bytes
    unsigned short tile_bitstream_idx;                        // Index of bistream base address the tile offset refers to (for multi entropy cores use - set to 0 otherwise)
    unsigned short tile_idx;                                  // Tile index at strip level
    unsigned int   tile_size_sbs;                             // tile size in sbs
}nvenc_av1_tile_stat_s;                                       // 16 bytes per status

typedef struct
{
    unsigned int   res_bit_count;                             // #bits spent on residuals
    unsigned int   mode_bit_count;                            // #bits spent on modes (mbtype, pred mode)
    unsigned int   type1_bit_count;                           // count of bits in type I bitstream (excludes leading_zero_8bits,
                                                              // start_codeprefix_one_3bytes and trailing_zero_8bits)
    unsigned int   total_bit_count;                           // total number of bits produced

    unsigned int   total_bin_count;                           // total number of bins produced by arithmetic coder
    unsigned int   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
    unsigned int   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
    unsigned int   total_intra_cost;                          // sum of all intra best mode's SATDs, each ctb has one intra SATD.
    unsigned int   total_inter_cost;                          // sum of all inter best mode's SATDs, each ctb has one inter SATD.

    unsigned int   intra_cu64x64_count;                       // number of intra 64x64 CUs.
    unsigned int   inter_cu64x64_count;                       // number of inter 64x64 CUs.
    unsigned int   intra_cu32x32_count;                       // number of intra 32x32 CUs.
    unsigned int   inter_cu32x32_count;                       // number of inter 32x32 CUs.
    unsigned int   intra_cu16x16_count;                       // number of intra 16x16 CUs.
    unsigned int   inter_cu16x16_count;                       // number of inter 16x16 CUs.
    unsigned int   intra_cu8x8_count;                         // number of intra 8x8 CUs.
    unsigned int   inter_cu8x8_count;                         // number of inter 8x8 CUs.


    unsigned int   total_emulated_bit_count;                  // total number of emulated bits in total_bit_count
    unsigned char  reserved[56];
} nvenc_av1_mpec_stat_s;                            // 128 bytes per status

typedef struct
{
    unsigned int   var_min;
    unsigned int   var_max;
    unsigned int   var_avg;
    unsigned char  reserved[4];
} nvenc_av1_aq_stat_s;                              // 16 bytes

typedef struct
{
  unsigned int   qp_val                             : 8  ;  // qp used for encoding this block
  unsigned int   total_bit_count                    : 24 ;  // total number of bits produced
  unsigned int   res_cost                           : 24 ;  // satd cost of residual error
  unsigned int   flags                              : 8  ;  // flags
} nvenc_av1_stats_fifo_s;                                   // 8 bytes per stat

typedef struct
{
    nvenc_av1_pic_stat_s   pic_stat;                                 // 64Bytes
    nvenc_av1_tile_stat_s  tile_stat[128];                           // maximum tile number is 128 per AV1 spec
    nvenc_av1_mpec_stat_s  mpec_stat[((8192 / 64)*(8192 / 64)) ];    // To account for stat/1SB and 8kx8k, plus 3 for alignment?
    nvenc_av1_aq_stat_s    aq_stats[((8192 / 64)*(8192 / 64)) ];     // AQ stats from pdma
    nvenc_av1_stats_fifo_s stats_fifo[((8192 / 64)*(8192 / 64)) ];
} nvenc_av1_stat_data_s;


typedef struct
{
  unsigned int   intra_res_bit_count;                       // #bits spent on intra residuals
  unsigned int   inter_res_bit_count;                       // #bits spent on inter residuals
  unsigned int   intra_mode_bit_count;                      // #bits spent on intra modes
  unsigned int   inter_mode_bit_count;                      // #bits spent on inter modes
  unsigned int   total_bit_count;                           // total number of bits produced
  unsigned int   total_bin_count;                           // total number of bins produced by bac
  unsigned int   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
  unsigned int   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
  unsigned int   cumulative_skip_cost;                      // sum of all P skip MB SA(T)Ds
  unsigned short intra_mb_count;                            // number of intra MBs.
  unsigned short inter_mb_count;                            // number of inter MBs.
  unsigned short skip_mb_count;                             // number of skip MBs.
  unsigned short reserved0;                                 // align to a word boundary
} nvenc_vp8_segment_stat_s;                                 // 44 Bytes

typedef struct
{
  unsigned int bitstream_byte_count;                        // output bytes in main partition bitstream buffer
  unsigned int bitstream_res_byte_count[8];                 // output bytes in residual partition bitstream buffers
  unsigned int cycle_count;                                 // total cycles taked for execute. Written out only when DumpCycleCount in SetControlParams is set
  unsigned int   picture_index;                             // value received from SetPictureIndex method
  unsigned int   error_status;                              // report error if any
  unsigned int   total_bit_count;                           // picture size in bits /*(sum of header's & all the partition's bits. actually redundant)*/
  unsigned short pic_type;                                  // copied from pic_type in nvenc_h264_pic_control_s
  unsigned short ave_activity;                              // report average activity if activity based RC enabled
  unsigned short num_segments;                              // number of segments produced
  unsigned short base_qindex;                               // base_qindex used for the frame
  nvenc_vp8_segment_stat_s segment_stat[4];
  unsigned int   total_intra_cost;                          // sum of all intra best mode's SATDs, each mb has one intra SATD.
  unsigned int   total_inter_cost;                          // sum of all inter best mode's SATDs, each mb has one inter SATD.
  unsigned int reserved1[3];
} nvenc_vp8_stat_data_s;                                    // 256 Bytes

// Stats stored in HW format.
typedef struct
{
  unsigned int coef_token_cnt[3][3][4][8][4];               // [value/4][ctx][value%4][band][type]    4608 bytes (4096 + 512)
  unsigned int mv_stats[20][4];
} nvenc_vp8_token_data_s;

                        // 8 bytes


//////////////////////////////////////////////////////////////////
// ****************************************************************************
// ****************************************************************************
// AV1
// ****************************************************************************
// ****************************************************************************
//AV1 loop filter config
typedef struct
{
    unsigned char   filter_level[4];                       // loop filter strength based on edge direction and component - vertical/horizontal edge luma (0,1) and chroma (2,3)
    char            ref_delta[8];                          // Loop filter level adjustments based on reference frame type - 0 = Intra, Last, Last2+Last3, GF, BRF, ARF2, AR
    int             mode_delta_0             : 8;          // Loop filter level adjustments based on mode type - 0 = ZERO_MV, MV
    int             mode_delta_1             : 8;          // Loop filter level adjustments based on mode type - 0 = ZERO_MV, MV
    unsigned int    update_ref_delta_flag    : 8;          // update delta for reference i if bit i is set
    unsigned int    update_mode_delta0       : 1;
    unsigned int    update_mode_delta1       : 1;
    unsigned int    delta_enabled            : 1;          // disable loop-filter
    unsigned int    sharpness                : 3;          // sharpness level
    unsigned int    reserved                 : 2;
}nvenc_av1_loop_filter_cfg_s;  //16 bytes

//AV1 CDEF config
// NOTE: the following is mandated by the current HW implementation:
// y_strength_0 and uv_strength_0 must always be set to 0
// Luma/chroma strength combination 0 must always be (y_strength_0, uv_strength_0) i.e. (0,0)
typedef struct
{
    unsigned int   cdef_damping_minus_3         : 2;        // controls the amount of damping in the deringing filter
    unsigned int   y_strength_0                 : 6;        // specify (strength primary filter 0 << 2) | strength secondary filter 0 (luma)
    unsigned int   y_strength_1                 : 6;        // specify (strength primary filter 1 << 2) | strength secondary filter 1 (luma)
    unsigned int   y_strength_2                 : 6;        // specify (strength primary filter 2 << 2) | strength secondary filter 2 (luma)
    unsigned int   y_strength_3                 : 6;        // specify (strength primary filter 3 << 2) | strength secondary filter 3 (luma)
    unsigned int   y_strength_4                 : 6;        // specify (strength primary filter 4 << 2) | strength secondary filter 4 (luma). Not supported.
    unsigned int   y_strength_5                 : 6;        // specify (strength primary filter 5 << 2) | strength secondary filter 5 (luma). Not supported.
    unsigned int   y_strength_6                 : 6;        // specify (strength primary filter 6 << 2) | strength secondary filter 6 (luma). Not supported.
    unsigned int   y_strength_7                 : 6;        // specify (strength primary filter 7 << 2) | strength secondary filter 7 (luma). Not supported.
    unsigned int   cdef_bits                    : 2;        // support up to 3. the number of bits needed to specify w2ich CDEF filter to apply
    unsigned int   uv_strength_0                : 6;        // specify (strength primary filter 0 << 2) | strength secondary filter 0 (chroma)
    unsigned int   uv_strength_1                : 6;        // specify (strength primary filter 1 << 2) | strength secondary filter 1 (chroma)
    unsigned int   uv_strength_2                : 6;        // specify (strength primary filter 2 << 2) | strength secondary filter 2 (chroma)
    unsigned int   uv_strength_3                : 6;        // specify (strength primary filter 3 << 2) | strength secondary filter 3 (chroma)
    unsigned int   uv_strength_4                : 6;        // specify (strength primary filter 4 << 2) | strength secondary filter 4 (chroma). Not supported.
    unsigned int   uv_strength_5                : 6;        // specify (strength primary filter 5 << 2) | strength secondary filter 5 (chroma). Not supported.
    unsigned int   uv_strength_6                : 6;        // specify (strength primary filter 6 << 2) | strength secondary filter 6 (chroma). Not supported.
    unsigned int   reserved0                    : 2;        // Reserved bits
    unsigned int   uv_strength_7                : 6;        // specify (strength primary filter 7 << 2) | strength secondary filter 7 (chroma). Not supported.
    unsigned int   cdef_y_strength_num_minus_1  : 3;        // HW will only considers y strength values from index 0 to cdef_y_strength_num_minus_1 (from 0 to 7)
    unsigned int   cdef_uv_strength_num_minus_1 : 3;        // HW will only considers uv strength values from index 0 to cdef_uv_strength_num_minus_1 (from 0 to 7)
    unsigned int   cdef_combined_idx_minus_1    : 3;        // Specify max number of unique (y_strength_x, uv_strength_y) combinations for HW to consider  (from 0 to 7)
                                                            // bits[2:0] must be set to 0
    unsigned int   reserved1                    : 17;       // Reserved bits
    unsigned int   cdef_combined_luma_idx       : 24;       // List of (cdef_combined_idx_minus_1 + 1) y strength indexes, each index using 3 bits and bits[3(n+1)-1:3n] corresponding to index of y_strength in combination n
    unsigned int   reserved2                    : 8;        // Reserved bits
    unsigned int   cdef_combined_chroma_idx     : 24;       // List of (cdef_combined_idx_minus_1 + 1) uv strength indexes, each index using 3 bits and bits[3(n+1)-1:3n] corresponding to index of uv_strength in combination n
                                                            // bits[2:0] must be set to 0
    unsigned int   reserved3                    : 8;        // Reserved bits
}nvenc_av1_cdef_cfg_s;  //24 bytes

//AV1 Loop Restoration Filter config
typedef struct
{
    unsigned int   y_lr_type            : 2;          // luma restoration type
    unsigned int   uv_lr_type           : 2;          // chroma restoration type
    unsigned int   y_lr_unit_size       : 2;          // luma restoration unit size 0:32x32, 1:64x64, 2:128x128,3:256x256;
    unsigned int   uv_lr_unit_size      : 2;          // chroma restoration unit size 0:32x32, 1:64x64, 2:128x128,3:256x256;
    unsigned int   lr_codebook_enable   : 16;         // bit enable for up to 16 codebooks
    unsigned int   lr_pe_on_edge_enable : 1;          // enable bit for LR parameter estimation on picture boundary SBs
    unsigned int   reserved             : 7;          // Reserved bits
}nvenc_av1_lr_filter_cfg_s;  // 4 bytes

//AV1 quant config
typedef struct
{
    unsigned char   base_q_idx;              // Base frame qindex (index in quant lookup table)  0...255
    char   delta_y_dc;              // explicit qindex adjustment for y dccoefficient, -63...63
    char   delta_uv_dc;             // qindex adjustment for uv accoefficient, -63...63
    char   delta_uv_ac;             // qindex adjustment for uv dccoefficient, -63...63
}nvenc_av1_quant_cfg_s;  //4 bytes

typedef struct
{
    unsigned short   frame_width_minus1;                        // frame width in pixels minus 1, range 191-8191
    unsigned short   frame_height_minus1;                       // frame height in pixels minus 1, range 127-8191
    unsigned short   sfc_pitch;                                 // pitch of luma plane
    unsigned short   sfc_pitch_chroma;                          // pitch of chroma plane
    unsigned short   sfc_trans_mode;                            // least 3 significient bits are used to stand for 8 modes, normal, xflip, yflip etc.
    unsigned short   reserved2;                                 // pad to NvS32

    unsigned int   luma_top_frm_offset;                         // offset of luma top field or frame in units of 256
    unsigned int   luma_bot_offset;                             // (deprecated) offset of luma bottom field in units of 256. Not used if frame format.
    unsigned int   chroma_top_frm_offset;                       // offset of chroma top field or frame, or offset of chroma U(Cb) plane in planar mode, both in units of 256
    unsigned int   chroma_bot_offset;                           // (deprecated) offset of chroma bottom field, not used if frame format. Or offset of chroma V(Cr) plane in planar mode. Both in units of 256.
    unsigned int   block_height     : 7;                        // BL mapping block height setting
    unsigned int   tiled_16x16      : 1;                        // (deprecated) Surface is 16x16 tiled instead of BL mapped (must be 1 for refpics) (deprecated)
    unsigned int   memory_mode      : 2;                        // (deprecated) 0: semi-planar, 1: planar
    unsigned int   nv21_enable      : 1;                        // (deprecated) the surface format is yuv or yvu
    unsigned int   input_bl_mode    : 2;                        // the input block linear mode: 0~gpu bl; 1~tegra bl; 2~naive bl this is only for cmod and cmod/plugin. TBL only for Orin.
    unsigned int   src_in_lsb       : 1;                        // pixel is LSB aligned in 16bit container, only valid for high-bitdepth
    unsigned int   reserved         : 18;                       // pad to NvU32
} nvenc_av1_surface_cfg_s;                                      // 32 bytes

typedef struct
{
    //sequence header
    unsigned int   min_cu_size                         : 2;            //0-8x8, 1-16x16, 2-32x32, 3-64x64. Orin AV1 supports 1~3
    unsigned int   max_cu_size                         : 2;            //0-8x8, 1-16x16, 2-32x32, 3-64x64  Orin AV1 supports 1~3

    unsigned int   chroma_format_idc                   : 2;            // 0=not supported, 1=yuv4:2:0, 2=yuv4:2:2, 3=yuv4:4:4. Only support YUV420
    unsigned int   monochrome_flag                     : 1;            // only luma component if flag is set. Not supported.
    unsigned int   bit_depth_minus_8                   : 4;            // luma/chroma should have the same value. support 0 (8bit), 2(10bit), 4(12bit) for Orin AV1
    unsigned int   input_bit_depth_minus8              : 4;            // Internal high-bitdepth support in HW
                                                                       // the field is to specify the bit-depth of source yuv. the decoded yuv bitdepth is specified nvenc_av1_seq_hdr_data_s::bit_depth_minus_8; HW will do the bitdepth conversion from source yuv-> recon yuv
                                                                       //
    unsigned int   enable_filter_intra                 : 1;            // enables/disables filterintra. Not supported
    unsigned int   enable_intra_edge_filter            : 1;            // enables/disables corner/edge/upsampling. Not supported
    unsigned int   enable_interintra_compound          : 1;            // enables/disables interintra_compound. Not supported
    unsigned int   enable_masked_compound              : 1;            // enables/disables masked compound. Not supported
    unsigned int   enable_dual_filter                  : 1;            // enable or disable vertical and horiz filter selection. Not supported
    unsigned int   force_screen_content_tools          : 2;            // 0 - force off. Not supported.
                                                                       // 1 - force on
                                                                       // 2 - adaptive
    unsigned int   force_integer_mv                    : 2;            // Not supported.0 - Not to force. MV can be in 1/4 or 1/8
                                                                       // 1 - force to integer
                                                                       // 2 - adaptive
    unsigned int   enable_jnt_comp                     : 1;            // enables/disables joint compound modes. not supported. Not supported.
    unsigned int   enable_ref_frame_mvs                : 1;            // enables/disables ref frame mvs. not supported. Not supported.
    unsigned int   enable_warped_motion                : 1;            // enables/disables warped motion for sequence. Not supported.
    unsigned int   enable_superres                     : 1;            // enables/disables superres for sequence. Not supported.
    unsigned int   enable_cdef                         : 1;            // enables/disables CDEF
    unsigned int   enable_restoration                  : 1;            // enables/disables restoration filter, only support self-guided filter
    unsigned int   reserved0                           : 2;            // reserved bits
} nvenc_av1_seq_hdr_data_s;                                            // 4 bytes

//frame header
typedef struct
{
    unsigned int   frame_type                      : 2;          // 0:Key frame, 1:Inter frame, 2:intra only, 3:s-frame
    unsigned int   disable_cdf_update              : 1;          // disable CDF update during symbol decoding
    unsigned int   allow_screen_content_tools      : 1;          // screen content tool enable. Not supported.
    unsigned int   force_integer_mv                : 1;          // AMVR enable. Not supported.
    unsigned int   allow_high_precision_mv         : 1;          // 1/8 precision mv enabl. Not supported.
    unsigned int   switchable_motion_mode          : 1;          // 0: simple motion mode, 1: SIMPLE, OBMC, LOCAL  WARP. Not supported, fixed to 0.
    unsigned int   is_filter_switchable            : 1;          // 1: filter selection is signaled at block level
    unsigned int   use_ref_frame_mvs               : 1;          // Not supported. enable/disable use of motion vector info from previous frame
    unsigned int   frame_end_update_cdf            : 1;          // enable/disable CDF update at end of frame
    unsigned int   interp_filter                   : 3;          // interpolation filter : EIGHTTAP,...., SWITCHABLE. Only support EIGHTTAP &EIGHTTAP_SMOOTH.
    unsigned int   delta_q_present_flag            : 1;          // quantizer index delta values are present in the block level
    unsigned int   delta_q_res                     : 2;          // left shift will apply to decoded quantizer index delta values. Not supported.
    unsigned int   allow_intrabc                   : 1;          // IBC enable. Not supported.
    unsigned int   delta_lf_present_flag           : 1;          // specify whether loop filter delta values are present in the block level. Not supported.
    unsigned int   delta_lf_res                    : 2;          // specify the left shift will apply to decoded loop filter values. Not supported.
    unsigned int   delta_lf_multi                  : 1;          // seperate loop filter deltas for Hy,Vy,U,V edges. Not supported.
    unsigned int   using_qmatrix                   : 1;          // enable quant matrix. Not supported.
    unsigned int   frame_tx_mode                   : 2;          // 0:LARGEST (32x32), 1:SELECT, 2: ONLY4x4 (lossless only)
    unsigned int   frame_reference_mode            : 2;          // 0: single, 1: select, 2: compound
    unsigned int   skip_mode_flag                  : 1;          // skip mode present flag. Not supported.
    unsigned int   allow_warped_motion             : 1;          // frame level warped motion enable. Not supported.
    unsigned int   reduced_tx_set_flag             : 1;          // whether the frame is  restricted to a reduced subset of the full set of transform types. Not supported.
    unsigned int   coded_lossless                  : 1;          // whether lossless coding is used for frame or not
    unsigned int   reserved0                       : 2;          // reserved bits

    unsigned int   ssim_rdo_enable                 : 1;          //enable variance based SSIM-RDO
    unsigned int   ssim_rdo_alt_frame_scale        : 8;          //frame level scaling factor for altref frame
    unsigned int   bicompound_enable               : 1;          //enable bi-compound for B-frames.
    unsigned int   unicompound_enable              : 1;          //enable uni-compound for P/B-frames. (only support in Pframes)
    unsigned int   mc_hfilter                      : 4;          //bitmask for horizontal filters enabled 0: EIGHTTAP, 1: EIGHTTAP_SMOOTH,
    unsigned int   mc_vfilter                      : 4;          //bitmask for vertical filters enabled 0: EIGHTTAP, 1: EIGHTTAP_SMOOTH,

    unsigned int   segmentation_enable             : 1;          //enable segmentation
    unsigned int   segmentation_update_map         : 1;          //enable segmentation update map ,force 1
    unsigned int   segment_temporal_update         : 1;          //enable segmentation temporal update,force 0
    unsigned int   segIdPreSkip                    : 1;          //segment id pre skip encoding ,force 1
    unsigned int   LastActiveSegId                 : 3;          //highest number of active segment . force 2

    unsigned int   reserved1                       : 6;



    unsigned char  ref_frame_sign_bias[8];                       //Used by AV1 MVP. A sign bias equal to 0 indicates that the reference frame is a forwards reference (
    nvenc_av1_loop_filter_cfg_s       lf_cfg;                    // loop filter configuration
    nvenc_av1_cdef_cfg_s              cdef_cfg;                  // CDEF configuration
    nvenc_av1_quant_cfg_s             quant_cfg;                 // quantizer configuration
    nvenc_av1_lr_filter_cfg_s         lrf_cfg;                   // loop restoration filter configuration
} nvenc_av1_frame_hdr_data_s;                                    // 64 bytes

// needed only if picture level RC is enabled; as part of picture level RC, it will also perform HRD verification
// The contents of this file is produced by the driver during sequence level operations and should not be modified.
typedef struct
{
    unsigned char    hrd_type;                                  // nal (=2) and vcl (=1) type
    unsigned char    QP[3];                                     // QP for 0:P picture, ,1:B picture 2; I picture
    unsigned char    minQP[3];                                  // min QP for 0:P picture, ,1:B picture 2; I picture
    unsigned char    maxQP[3];                                  // max QP for 0:P picture, ,1:B picture 2; I picture
    char    maxQPD;                                    // max QP delta between two consecutive QP updates
    char    baseQPD;                                   // initial QP delta between two consecutive QP updates
    unsigned int   rhopbi[3];                                 // 23.8 unsigned fixed point quant ratios P/I, B/I, I/I(=1)

    unsigned int   framerate;                                 // fps
    unsigned int   buffersize;                                // total buffer size

    unsigned int   nal_cpb_size;                              // size in bits
    unsigned int   nal_bitrate;                               // rate in bps
    unsigned int   vcl_cpb_size;                              // size in bits
    unsigned int   vcl_bitrate;                               // rate in bps

    unsigned int   gop_length;                                // I period, gop_length == 0xffffffff is used for infinite gop length
    unsigned int   Np;                                        // 27.4 unsigned fixed (gop_length + num_b_frames -1)/(num_b_frames + 1)
    unsigned int   Bmin;                                      // 23.8 unsigned fixed point min buffer level, updated by driver at sequence level only.
    unsigned int   Ravg;                                      // 23.8 unsigned fixed point average rate, updated by driver at sequence level only.
    unsigned int   R;                                         // 23.8 unsigned fixed point rate

    unsigned char    ab_alpha;                                  // VBR/CBR: min/target quality level (1..51); rcmode4: weight of prev frame activity compared to part of the current picture
    unsigned char    ab_beta;                                   // lookahead info1: ratio of inter over intra complexity (fix8); rcmode4: weight of current activity compared to neighbors
    unsigned short   prev_act;                                  // lookahead info2: NVENC_LOOKAHEAD_INFO2; rcmode4: average activity level of the first reference (L0)

    unsigned char    reserved0;
    unsigned char    single_frame_vbv;                          // 1: VBV buffer size is set to average frame size; 0: otherwise
    unsigned char    two_pass_rc;                               // 0: single pass rc , 1: first pass of 2 pass rc 2: second pass of 2 pass rc
    unsigned char    rc_class;                                  // reserved
    unsigned char    first_pass_source_half_scaled;             // 0 : first pass on half resolution 1: first pass on full resolution (along with NVENC_QRES_FLAG_XXX)
    unsigned char    i_size_ratio_x;                            // ratio between I picture target size over average picture size numerator
    unsigned char    i_size_ratio_y;                            // ratio between I picture target size over average picture size denominator
    unsigned char    aqMode;                                    // 0: disable adaptive quantization 1: AQ mode fast (unimplemented) 2: AQ mode full (unimplemented) 3: External qp delta map
    unsigned char    dump_aq_stats;                             // 1: dump aq stats
    unsigned char    ext_scene_change_flag;                     // Scene change flag set as hint by external preprocessing unit. 0: No scene change, 1: Current picture is first in scene
    unsigned char    ext_picture_rc_hints;                      // picture rc hints are set by external source. 0: no hints, 1: hints are avaialble for current picture
    unsigned char    session_max_qp;                            // Current frame qp will never exceed this value when ext_picture_rc_hints is set
    unsigned int     ext_intra_satd;                            // If non zero this represents the intra SATD for current picture computed by external preprocessing unit
    unsigned char    reserved2[8];
} nvenc_av1_rc_s;                                   // 96 bytes
//////////////////////////////////////////////////////////////////


typedef struct
{
  char  l0[4];                                              // reference picture list 0, maximum 4 forward reference frame
  char  l1[3];                                              // reference picture list 1, maximum 3 backward reference frame
  char  temp_dist_l0[4];                                    // temporal distance of each ref pic in list 0 DiffPicOrderCnt( currPicOrField, pic0) )
  char  temp_dist_l1[3];                                    // temporal distance of each ref pic in list 1 DiffPicOrderCnt( currPicOrField, pic1) )
  char  poc_entry_l0[4];                                    // ref pic list to ref pic entry map
  char  poc_entry_l1[3];                                    // ref pic list to ref pic entry map
  char  reserved0[3];

  unsigned int   max_byte_count_before_resid_zero;          // Maximum byte count before microcode forces the residuals to zero.

  unsigned int   mode_decision_only_flag      : 1;          // only output av1 mode and mocomp result, no recon and bitstream output. Not supported.
  unsigned int   cu_early_terminatiom_enable  : 1;          // 0: disable CU early termination, 1: enable CU early termination. Not supported.
  unsigned int   pic_output_flag              : 1;          // enable recon picture output
  unsigned int   input_sub_sampling           : 1;          // used by PDMA 2x2 sub-sampling. Not supported.
  unsigned int   output_sub_sampling          : 1;          // used by MPEB 2x2 sub-sampling output. Not supported.
  unsigned int   num_ref_idx_l0_active        : 3;          // [0..4]
  unsigned int   num_ref_idx_l1_active        : 3;          // [0..3]
  unsigned int   tile_idx_start               : 8;          // index of entry in av1_tile_control_s corresponding to first tile in strip
  unsigned int   num_strip_tiles_minus1       : 8;          // number of tiles in strip (= num_tiles_minus1 if single strip)
  unsigned int   force_lf_on_flag             : 1;          // minimum level of loop filter is set to 1 instead of 0 when flag is set
  unsigned int   force_lf_off_flag            : 1;          // maximum level of loop filter is set to 0 when flag is set (i.e. filtering off)
  unsigned int   vp9_skip_chroma              : 1;          // HW will only process luma and skip chroma. used for vp9 autoprofile TODO: temporary add here, need to move to VP9 dedicated structrue.
  unsigned int   reserved1                    : 2;

  unsigned int   max_tu_depth_inter           : 2;          // max_transform_hierarchy_depth_inter. Orin AV1 supports 0~2
  unsigned int   max_tu_depth_intra           : 2;          // max_transform_hierarchy_depth_intra. Orin AV1 supports 0
  unsigned int   num_tiles_minus1             : 8;          // number of entries in av1_tile_control_s
  unsigned int   enable_source_image_padding  : 1;          // enable/disable HW source image padding
  unsigned int   luma_padding_mode            : 2;          // HW souce padding mode for luma: 0: copy frame boundary, 1: zero padding, 2: half value, i.e 1<<(bitdepth-1)
  unsigned int   chroma_padding_mode          : 2;          // HW souce padding mode for chroma: 0: copy frame boundary, 1: zero padding, 2: half value, i.e 1<<(bitdepth-1)
  unsigned int   reserved2                    : 15;
  unsigned int   prob_tables_offset;                        // When non-zero, ucode will adjust the base address of the probability tables surfaces based on picture QP value as follows
  unsigned int   tile_control_offset;                       // offset from start of top level "nvenc_av1_drv_pic_setup_s" structure
  unsigned int   hist_buf_size;                             // size in bytes of the buffer allocated for history
  unsigned int   bitstream_buf_size;                        // size in bytes of the buffer allocated for bitstream slice/mb data
  unsigned int   bitstream_start_pos;                       //start position in bitstream buffer where data should be written (byte offset)
  unsigned int   pic_type                      :  2;        // 0 = P, 1 = B, 2 = I, 3 = IDR
  unsigned int   ref_pic_flag                  :  1;        // reference picture (0 = no, 1 = yes)
  unsigned int   codec                         :  3;
  unsigned int   qpfifo                        :  1;        // 0 = Polling mode, 1 = Interrupt mode.
  unsigned int   num_tile_cols                 :  8;        // Number of tile columns in picture
  unsigned int   num_tile_rows                 :  8;        // Number of tile rows in picture
  unsigned int   ctx_update_tile_id            :  8;        // update cdf for the specified tile id. value 255 is reserved to enable HW auto update CDF from tile with maximun tile data length
  unsigned int   stats_fifo_enable             :  1;        // enable per-sb short stats fifo

                                                            // 1 = static slice insertion based on slice control array
                                                                //     (insert slices as defined in array, if all array elements used
                                                                //      before end of picture is reached, start over with entry 0)

  unsigned int   mpec_threshold;                            // collect mpec stats after threshold mbs
                                                                // when mpec_stat_on is enabled and mpec_threshold is not equal to picWidthInMbs,
  unsigned int   tile_stat_offset;                          // offset from start of top level "nvenc_av1_stat_data_s" structure
                                                                // rc mode 0 will be used to collect stats
  unsigned int   mpec_stat_offset;                          // offset from start of top level "nvenc_stat_data_s" structure
                                                                // to start of array of "nvenc_mpec_stat_s" structure
  unsigned int   aq_stat_offset;                            // to start of array of "nvenc_av1_aq_stat_s" structure

  unsigned int   rc_pass2_offset;

  unsigned int   act_stat_offset;                           // offset from start of "nvenc_rc_pic_stat_s" structure, points to data for external QP map
  unsigned int   external_hint_number;                      // total external hint number for current frame, maximun external hints per SB: 128.

  unsigned char    stripID;                                 // strip index (from 0 to strips_in_frame-1)
  unsigned char    strips_in_frame;                         // total number of strips in frame
  unsigned short   strip_width_in_sbs;                      // strip width in SB unit,must be frame_width
  unsigned short   strip_height_in_sbs;                     // strip height in SB unit
  unsigned short   strip_width_offset_in_sbs;               // Column index of first SB in strip ,must be 0
  unsigned short   strip_height_offset_in_sbs;              // Row index of first SB in strip
  unsigned short   reserved5;

  unsigned int   stats_fifo_offset;                         // offset from start of top level "nvenc_stat_data_s" structure
  unsigned char  reserved6[8];
} nvenc_av1_pic_control_s;                                  // 112 bytes

typedef struct
{
    unsigned int cu8x8_part_2Nx2N_enable       : 1;            // enable prediction for Inter CU8x8 2Nx2N
    unsigned int cu16x16_part_2Nx2N_enable     : 1;            // enable prediction for Inter CU16x16 2Nx2N
    unsigned int cu16x16_part_2NxN_enable      : 1;            // enable prediction for Inter CU16x16 2NxN
    unsigned int cu16x16_part_Nx2N_enable      : 1;            // enable prediction for Inter CU16x16 Nx2N
    unsigned int cu32x32_part_2Nx2N_enable     : 1;            // enable prediction for Inter CU32x32 2Nx2N
    unsigned int cu32x32_part_2NxN_enable      : 1;            // enable prediction for Inter CU32x32 2NxN
    unsigned int cu32x32_part_Nx2N_enable      : 1;            // enable prediction for Inter CU32x32 Nx2N
    unsigned int cu64x64_part_2Nx2N_enable     : 1;            // enable prediction for Inter CU64x64 2Nx2N
    unsigned int cu64x64_part_2NxN_enable      : 1;            // enable prediction for Inter CU64x64 2NxN
    unsigned int cu64x64_part_Nx2N_enable      : 1;            // enable prediction for Inter CU64x64 Nx2N
    unsigned int tusize_4x4_enable             : 1;            //enable 4x4 transform
    unsigned int tusize_8x8_enable             : 1;            //enable 8x8/8x4/4x8 transform
    unsigned int tusize_16x16_enable           : 1;            //enable 16x16/16x8/8x16 transform
    unsigned int tusize_32x32_enable           : 1;            //enable 32x32/32x16/16x32 transform
    unsigned int tusize_64x64_enable           : 1;            //enable 64x64/64x32/32x16 transform
    unsigned int intra_tusize_decision         : 1;            //enable intra tu size decision, reserved, not implemented in HW
    unsigned int pskip_enable                  : 1;            //enable p_skip encoding. Not supported
    unsigned int tu_search_basedon_pusize      : 1;            //enable tu search based on pu size. Not supported
    unsigned int multiply_bias_with_lambda     : 1;            // setting this to 1 will result in bias being multiplied by lambda
    unsigned int skip_evaluate_enable_cu8      : 1;            // 0: not check skip ssd cost; 1, evaluate skip ssd cost. Not supported
    unsigned int skip_evaluate_enable_cu16     : 1;            // 0: not check skip ssd cost; 1, evaluate skip ssd cost. Not supported
    unsigned int skip_evaluate_enable_cu32     : 1;            // 0: not check skip ssd cost; 1, evaluate skip ssd cost. Not supported
    unsigned int skip_evaluate_enable_cu64     : 1;            // 0: not check skip ssd cost; 1, evaluate skip ssd cost. Not supported
    unsigned int inter_rdo_cnt_8x8             : 3;            // #RD cost evaluation for inter top N SATD winner partitions of CU8x8. support 0~4
    unsigned int inter_rdo_cnt_16x16           : 3;            // #RD cost evaluation for inter top N SATD winner partitions of CU16x16. support 0~4
    unsigned int inter_rdo_cnt_32x32           : 3;            // #RD cost evaluation for inter top N SATD winner partitions of CU32x32. support 0~4

    unsigned int inter_rdo_cnt_64x64           : 3;            // #RD cost evaluation for inter top N SATD winner partitions of Cu64x64. support 0~4
    unsigned int intra_rdo_cnt_4x4             : 3;            // RD cost evaluation for N top satd winner modes of intra 4x4. Not supported
    unsigned int intra_rdo_cnt_8x8             : 3;            // RD cost evaluation for N top satd winner modes of intra 8x8. support 0~4
    unsigned int intra_rdo_cnt_8x4             : 3;            // RD cost evaluation for N top satd winner modes of intra 8x4. support 0~4
    unsigned int intra_rdo_cnt_4x8             : 3;            // RD cost evaluation for N top satd winner modes of intra 4x8. support 0~4
    unsigned int intra_rdo_cnt_16x16           : 3;            // RD cost evaluation for N top satd winner modes of intra 16x16. support 0~4
    unsigned int intra_rdo_cnt_16x8            : 3;            // RD cost evaluation for N top satd winner modes of intra 16x8. support 0~4
    unsigned int intra_rdo_cnt_8x16            : 3;            // RD cost evaluation for N top satd winner modes of intra 8x16. support 0~4
    unsigned int intra_rdo_cnt_32x32           : 3;            // RD cost evaluation for N top satd winner modes of intra 32x32. support 0~4
    unsigned int intra_rdo_cnt_32x16           : 3;            // RD cost evaluation for N top satd winner modes of intra 32x16. support 0~4
    unsigned int luma_all_zero_check           : 1;            // enable TU level luma coefficient all zero check
    unsigned int chroma_all_zero_check         : 1;            // enable TU level chroma coefficient all zero check. Not supported

    unsigned int intra_rdo_cnt_16x32           : 3;            // RD cost evaluation for N top satd winner modes of intra 16x32. support 0~4
    unsigned int intra_rdo_cnt_64x64           : 3;            // RD cost evaluation for N top satd winner modes of intra 64x64. support 0~4
    unsigned int intra_rdo_cnt_64x32           : 3;            // RD cost evaluation for N top satd winner modes of intra 64x32. support 0~4
    unsigned int intra_rdo_cnt_32x64           : 3;            // RD cost evaluation for N top satd winner modes of intra 32x64. support 0~4
    unsigned int num_1div8_lambda_ssd          : 7;            // number of 1/8 lambda ssd for lambda_coef delta. lambda_coef = (1+num_1div8_lambda_ssd*1/8)*lambda_ssd
    unsigned int mv_cost_enable                : 1;            // enable mv cost calculations in mode decision
    unsigned int reserved0                     : 12;

    //Intra Luma angular delta mode controls: begin
    unsigned int intra_luma_dir_delta_64x64_enable_0;          // bit mask for enabling angular delta modes: {D135, D45, H, V} * {-3,-2,-1,0,1,2,3}
    unsigned int intra_luma_dir_delta_64x64_enable_1;          // bit mask for enabling angular delta modes: {D67, D203, D157, D113} * {-3,-2,-1,0,1,2,3}
    unsigned int intra_luma_dir_delta_64x32_enable_0;
    unsigned int intra_luma_dir_delta_64x32_enable_1;          // for 64x32 and 32x64

    unsigned int intra_luma_dir_delta_32x32_enable_0;          // bit mask for enabling angular delta modes: {D135, D45, H, V} * {-3,-2,-1,0,1,2,3}
    unsigned int intra_luma_dir_delta_32x32_enable_1;          // bit mask for enabling angular delta modes: {D67, D203, D157, D113} * {-3,-2,-1,0,1,2,3}
    unsigned int intra_luma_dir_delta_32x16_enable_0;          // for 32x16 and 16x32
    unsigned int intra_luma_dir_delta_32x16_enable_1;

    unsigned int intra_luma_dir_delta_16x16_enable_0;          // bit mask for enabling angular delta modes: {D135, D45, H, V} * {-3,-2,-1,0,1,2,3}
    unsigned int intra_luma_dir_delta_16x16_enable_1;          // bit mask for enabling angular delta modes: {D67, D203, D157, D113} * {-3,-2,-1,0,1,2,3}
    unsigned int intra_luma_dir_delta_16x8_enable_0;           // for 8x16 and 16x8
    unsigned int intra_luma_dir_delta_16x8_enable_1;

    unsigned int intra_luma_dir_delta_8x8_enable_0;          // bit mask for enabling angular delta modes: {D135, D45, H, V} * {-3,-2,-1,0,1,2,3}
    unsigned int intra_luma_dir_delta_8x8_enable_1;          // bit mask for enabling angular delta modes: {D67, D203, D157, D113} * {-3,-2,-1,0,1,2,3}
    //Intra Luma angular delta mode controls: end

    // 0:DC_PRED, 1: V_PRED, 2: H_PRED, 3:D45_PRED,
    // 4:D135_PRED, 5:D113_PRED, 6:D157_PRED, 7: D203_PRED,
    // 8: D67_PRED, 9:SMOOTH_PRED, 10:SMOOTH_V_PRED, 11:SMOOTH_H_PRED
    // 12: PAETH
    unsigned short intra_luma_base_mode_4x4_enable;              //bit mask of intra luma base 4x4 mode:
    unsigned short intra_luma_base_mode_8x8_enable;              //bit mask of intra luma base 8x8/8x4/4x8 mode
    unsigned short intra_luma_base_mode_16x16_enable;           //bit mask of intra luma base 16x16/16x8/8x16 modes
    unsigned short intra_luma_base_mode_32x32_enable;            //bit mask of intra luma base 32x32/32x16/16x32 modes
    unsigned short intra_luma_base_mode_64x64_enable;            //bit mask of intra luma base 64x64/32x64/64x32 modes

    short bias_intra_over_inter;                              // bias for any intra   (subtracts from intra cost)
    unsigned short mv_cost_bias;                              // bias applied to internal motion vector cost calculation

    short bias_tu_4x4;                                        // bias for TU 4x4
    short bias_tu_8x8;                                        // bias for TU 8x8
    short bias_tu_16x16;                                      // bias for TU 16x16
    short bias_tu_32x32;                                      // bias for TU 32x32
    short bias_tu_64x64;                                      // bias for TU 64x64
    short bias_tu_8x4;                                        // bias for TU 8x4
    short bias_tu_4x8;                                        // bias for TU 4x8
    short bias_tu_16x8;                                       // bias for TU 16x8
    short bias_tu_8x16;                                       // bias for TU 8x16
    short bias_tu_32x16;                                      // bias for TU 32x16
    short bias_tu_16x32;                                      // bias for TU 16x32

    short bias_skip;                                          // bias for skip mv's ssd (subtracts from skip mv's ssd cost). Not supported.
    short bias_inter_64x64;                                   // bias for inter 64x64 (subtracts from inter 64x64 cost)
    short bias_inter_64x32;                                   // bias for inter 64x32 (subtracts from inter 64x32 cost)
    short bias_inter_32x64;                                   // bias for inter 32x64 (subtracts from inter 32x64 cost)
    short bias_inter_32x32;                                   // bias for inter 32x32 (subtracts from inter 32x32 cost)
    short bias_inter_32x16;                                   // bias for inter 32x16 (subtracts from inter 32x16 cost)
    short bias_inter_16x32;                                   // bias for inter 16x32 (subtracts from inter 16x32 cost)
    short bias_inter_16x16;                                   // bias for inter 16x16 (subtracts from inter 16x16 cost)
    short bias_inter_16x8;                                    // bias for inter 16x8  (subtracts from inter 16x8 cost)
    short bias_inter_8x16;                                    // bias for inter 8x16  (subtracts from inter 8x16 cost)
    short bias_inter_8x8;                                     // bias for inter 8x8   (subtracts from inter 8x8 cost)
    short bias_intra_64x64;                                   // bias for intra 64x64 (subtracts from intra 64x64 cost)
    short bias_intra_64x32;                                   // bias for intra 64x32 (subtracts from intra 64x32 cost)
    short bias_intra_32x64;                                   // bias for intra 32x64 (subtracts from intra 32x64 cost)
    short bias_intra_32x32;                                   // bias for intra 32x32 (subtracts from intra 32x32 cost)
    short bias_intra_32x16;                                   // bias for intra 32x16 (subtracts from intra 32x16 cost)
    short bias_intra_16x32;                                   // bias for intra 16x32 (subtracts from intra 16x32 cost)
    short bias_intra_16x16;                                   // bias for intra 16x16 (subtracts from intra 16x16 cost)
    short bias_intra_16x8;                                    // bias for intra 16x8  (subtracts from intra 16x8 cost)
    short bias_intra_8x16;                                    // bias for intra 8x16  (subtracts from intra 8x16 cost)
    short bias_intra_8x8;                                     // bias for intra 8x8   (subtracts from intra 8x8 cost)
    short bias_intra_8x4;                                     // bias for intra 8x4   (subtracts from intra 8x4 cost)
    short bias_intra_4x8;                                     // bias for intra 4x8   (subtracts from intra 4x8 cost)
    short bias_intra_4x4;                                     // bias for intra 4x4   (subtracts from intra 4x4 cost)

    unsigned short tx_type_search_mask[2];                    // bit mask of tx type enabled. bit 0~15, corresponding to:
                                                              // tx type DCT_DCT, ADST_DCT, DCT_ADST, ADST_ADST, FLIPADST_DCT,
                                                              // DCT_FLIPADST, FLIPADST_FLIPADST, ADST_FLIPADST, FLIPADST_ADST,
                                                              // IDTX, V_DCT, H_DCT, V_ADST, H_ADST, V_FLIPADST, H_FLIPADST,
                                                              // 0: control for RDO stage. 1: control for RECON stage

    unsigned int zbin_factor_lt_148            : 8;           //Aggresive DZ control for zbin factor when quant value < 148. in 1/128 unit
    unsigned int zbin_factor_gt_148            : 8;           //Aggresive DZ control for zbin factor when quant value >= 148. in 1/128 unit
    unsigned int round_factor_eq_0             : 8;           // TBD
    unsigned int round_factor_ne_0             : 8;           // TBD
} nvenc_av1_md_control_s;                                     // 160 bytes

typedef nvenc_h264_me_control_s nvenc_me_control_s;

typedef struct
{
    // 256 aligned
    unsigned int                magic;                         // magic number, see text above   //  4 bytes
    nvenc_av1_surface_cfg_s     refpic_cfg;                    // defines layout of reference pictures     // 32 bytes
    nvenc_av1_surface_cfg_s     input_cfg;                     // defines layout of input pictures     // 32 bytes
    nvenc_av1_surface_cfg_s     outputpic_cfg;                 // defines layout of reconstructed pictures  // 32 bytes

    nvenc_av1_seq_hdr_data_s    seqhdr_data;                   // 4 bytes
    nvenc_av1_frame_hdr_data_s  framehdr_data;                 // 64 bytes
    nvenc_av1_rc_s              rate_control;                  // Rate Control information   // 96 bytes
    nvenc_av1_pic_control_s     pic_control;                   // 112 bytes
    nvenc_av1_surface_cfg_s     half_scaled_outputpic_cfg;     // defines layout of 2x2 subsampled reconstructed picture // 32 bytes
    nvenc_av1_md_control_s      md_control;                    // 160 bytes
    nvenc_me_control_s          me_control;                    // 192 bytes
    unsigned int                gpTimer_timeout_val;           // GPTimer cycle count set from driver
    unsigned char               reserved0[4];
} nvenc_av1_drv_pic_setup_s; //768 bytes

typedef nvenc_av1_drv_pic_setup_s nvenc_vp9_drv_pic_setup_s;  //TODO: Orin VP9 will share the same class as AV1. Define new one for Hopper VP9

typedef struct
{
    unsigned int   tile_top_sbx                        : 16;   // top left sbx of the tile
    unsigned int   tile_top_sby                        : 16;   // top left sby of the tile
    unsigned int   tile_bot_sbx                        : 16;   // bottom right sbx of the tile
    unsigned int   tile_bot_sby                        : 16;   // bottom right sby of the tile

    unsigned int   qp_tile_min                         : 8;    // min slice qp value
    unsigned int   qp_tile_max                         : 8;    // max slice qp value
    unsigned int   qp_avr                              : 8;    // 8bit value used by hw
    unsigned int   tile_type                           : 2;    // 0 = B, 1 = P, 2 = I
    unsigned int   force_intra                         : 1;
    unsigned int   tile_end_update_cdf                 : 1;    // enable/disable CDF update at end of tile
    unsigned int   limit_tile_top_boundary             : 1;    // limit ME to tile boundaries
    unsigned int   limit_tile_bot_boundary             : 1;    // limit ME to tile boundaries
    unsigned int   limit_tile_left_boundary            : 1;    // limit ME to tile boundaries
    unsigned int   limit_tile_right_boundary           : 1;    // limit ME to tile boundaries

    unsigned int   tile_idx                            : 8;    // tile index at picture level (potentially different from index in array of nvenc_av1_tile_control_s when doing strip encoding)
    unsigned int   disable_tile_top_filters            : 1;    //disable filters at tile top boundary
    unsigned int   disable_tile_bottom_filters         : 1;    //disable filters at tile bottom boundary
    unsigned int   disable_next_tile_top_filters       : 1;    //disable filters at next tile top boundary
    unsigned int   disable_next_tile_bottom_filters    : 1;    //disable filters at next tile bottom boundary
    unsigned int   reserved0                           : 20;

    unsigned int   next_tile_top_sbx                   : 16;   // top left sbx of next tile
    unsigned int   next_tile_top_sby                   : 16;   // top left sby of next tile
    unsigned int   next_tile_bot_sbx                   : 16;   // bottom right sbx of next tile
    unsigned int   next_tile_bot_sby                   : 16;   // bottom right sby of next tile

    unsigned short intra_refresh_cnt;                          // distance between forced-intra SBs in slice; 0 means OFF
    unsigned short intra_refresh_offs;                         // first forced-intra SB in slice

    unsigned char  reserved2[4];                               // padding to 32 bytes
} nvenc_av1_tile_control_s;                                    // 32 bytes

// AV1 not support QPP, reserved it but not used
typedef struct
{
    // QPP has not been defined for AV1
    unsigned short   qpp_run_vector_4x4;                         // cost values for 4x4 transform (16 bit total vector)
    unsigned short   qpp_run_vector_8x8[3];                      // cost values for 8x8 transform (48 bit total vector)
                                                        // 2 bits each for first 12 coefs, then 1 bit each for next 24
    unsigned int   qpp_luma8x8_cost                    : 8;    // luma 8x8 cost threshold, 0 = throw out all coefs, range 0-15
    unsigned int   qpp_luma16x16_cost                  : 8;    // luma 16x16 cost threshold, 0 = throw out all coefs, range 0-15
    unsigned int   qpp_chroma_cost                     : 8;    // chroma cost threshold, 0 = throw out all coefs, range 0-15
    unsigned int   qpp_mode                            : 2;    // 0 = OFF, 1 = 8x8, 2 = 16x16_8x8
    unsigned int   reserved0                           : 6;    // padding

    unsigned int   reserved1;                                  // padding to 16 bytes
} nvenc_av1_quant_control_s;                            // 16 bytes

// Status buffer holds for debug related information
//
// General guideline for using CRC interface debug feature:
// 1. Generate the golden CRC (e.g. using cmodel) for for all interfaces
// 2. program the golden crc to NV_CNVENC_SAFETY_CRC_GOLDEN_PART_[X]_INTERFACE_NAME, and config the SAFETY_CRC_CFG register (with values from dbg_crc_init_enable/comp_enable/addr_comp/mem_wr_dis), set SAFETY_CRC_CTRL_EN = 1 (TZ)
// 3. kick off HW and trigger HW to compare width golden by writing SAFETY_CRC_CFG_COMP_EN when finish processing the frame; compare result could read from NV_CNVENC_SAFETY_CRC_STATUS_PART_[X]
// 4. raw HW CRC values could be read from NV_CNVENC_SAFETY_CRC_HW_PART_[X]_INTERFACE_NAME
typedef struct
{
    unsigned int dbg_crc_enable     : 1;     //Global enable flag for enable/disable interface crc calculation in NVENC HW
    unsigned int dbg_crc_init_enable: 1;     //Enable crc initialization
    unsigned int dbg_crc_comp_enable: 1;     //Enable crc golden comparision
    unsigned int dbg_crc_addr_comp  : 1;     //Enable crc calculation for fbif address
    unsigned int dbg_crc_mem_wr_dis : 1;     //Disable fb memory write
    unsigned int reserved0          : 27;

    //Golden CRC values for each partition
    unsigned int dbg_crc_partb_golden[32];
    unsigned int dbg_crc_partf_golden[32];
    unsigned int dbg_crc_partg_golden[32];
    unsigned int dbg_crc_parth_golden[32];
    unsigned int dbg_crc_parti_golden[32];
    unsigned int dbg_crc_partj_golden[32];
    unsigned int dbg_crc_partk_golden[32];
    unsigned int dbg_crc_partl_golden[35];

    //Compare status register for each partition.
    //each bit corresponding to the compare status for one interface (0: pass; 1: fail)
    //The number of valid bits for each partition shall equal to the number of golden crcs for that partition
    unsigned int dbg_crc_comp_partb;
    unsigned int dbg_crc_comp_partf;
    unsigned int dbg_crc_comp_partg;
    unsigned int dbg_crc_comp_parth;
    unsigned int dbg_crc_comp_parti;
    unsigned int dbg_crc_comp_partj;
    unsigned int dbg_crc_comp_partk;
    unsigned int dbg_crc_comp_partl[2];     //split into two compare status registers


    //HW CRC values generated from each partition. the number shall match with golden CRC number
    unsigned int dbg_crc_partb_crc[32];
    unsigned int dbg_crc_partf_crc[32];
    unsigned int dbg_crc_partg_crc[32];
    unsigned int dbg_crc_parth_crc[32];
    unsigned int dbg_crc_parti_crc[32];
    unsigned int dbg_crc_partj_crc[32];
    unsigned int dbg_crc_partk_crc[32];
    unsigned int dbg_crc_partl_crc[35];


    //HW verification purpose control fields
    unsigned int lambda_ssd;              //lambda for SSD cost
    unsigned int lambda_satd;             //lambda for SATD cost
    unsigned int lambda_ld;               //lambda for laplacian cost
    unsigned char reserved1[180];
} nvenc_debug_status_s; //2304 bytes

// 2.7. Bit Stream Output Data Buffer
// ----------------------------------
//
// This buffer is used to store the encoded bit stream data at slice header
// level and below.
// The driver must ensure that there is enough free space in this buffer to
// contain the entire output produced for the current execute, including any
// headers that will be produced. Output will be written incrementally
// from the starting position relative to the 256 byte aligned start location
// specified for this buffer.
//
// 2.8. Motion Estimation Predictor Data Buffer
// --------------------------------------------
//
// This buffer is used to store by the motion estimation unit to store vector
// hints that can be used as explicit candidates or as guides for motion
// search in future pictures.
// During encode one such surface can be written and another can be read.
//
// 2.9. Rate Control Process Buffer
// --------------------------------
//
// This buffer is used to store persistent rate control information. The same
// buffer will be shared in all executes of a particular stream.
//
// This buffer is used only if picture level rate control is enabled ((rc_mode & 3) != 0)

// Macro to initialize Bavg in VBR mode
#define NVENC_VBR_INIT_BAVG(framerate, ravg)                (0)                             // zero-centered vbr fullness
// Macro to initialize Whist and Wpbi
#define NVENC_RC_INIT_COMPLEXITY(bpp_at_qs1)                ((int)((bpp_at_qs1)*16))        // use this macro to decouple from implementation-dependent precision

typedef struct
{

            int  nal_cpb_fullness;                          // 28.4 fixed point
            int  vcl_cpb_fullness;                          // 28.4 fixed point

            int  Wpbi[3];                                   // 27.4 signed fixed point
            int  Whist[3][3];                               // 27.4 signed fixed point [pbi][t]
            int  np;                                        // 27.4 signed fixed point
            int  nb;                                        // 27.4 signed fixed point
            int  t;                                         // 27.4 signed fixed point

            int  Rmean;                                     // 23.8 signed fixed point peak bitrate (bits/pixel)
            int  Bavg;                                      // 23.8 signed fixed point

          short  average_mvx;                               // average mvx info, can be used for ME hint
          short  average_mvy;                               // average mvy info, can be used for ME hint

            int  prevQS;                                    // previous second pass Qs derived from rate
          short  prevQP;                                    // previous 2nd pass qp
          short  fwdQP;                                     // QP of forward reference

  unsigned char  refQPcol[48];                              // co-located mbrow qp of the most recently coded reference
  unsigned char  Rcol[48];                                  // co-located rate (5.3 precision, relative to avg rate)
  unsigned char  ref1QP[48];                                // first-pass reference qp (when 1st pass is using its own reference frames)

  unsigned int   ext_intra_satd_Hist[2];                    // external satd History
  unsigned char  ext_scd_Hist;                              // scene change flag History
  unsigned char  scene_change;                              // last coded frame was a scene change
  unsigned char  qpp_modifier;                              // current qpp adjustment (subtracted from qpp thresholds)
  unsigned char  Ksatd;                                     // pass1 satd to pass2 complexity measure
    signed char  Werr;                                      // model deviation: Wpass2 / Wpass1 - 1.0, .8 signed fixed-point [0.5x..1.5x]
    signed char  WerrI;                                     // same as above for I-frames
  unsigned char  avgBf;                                     // 3.5 fixed point, average number of consecutive B-frames
  unsigned char  reserved;
  unsigned int   printf_offset;                             // Using for ucode print
  unsigned char  reserved2[4];

} nvenc_persistent_state_s;                                 // 256Bytes

// 2.a. Ucode Probability State
// --------------------------------
//
// This buffer is used to store probability state computed and used during the last picture.
// This is used to compute the probability updates for current picture
//

// DO NOT rearrange this structure. The alignment is crucial for dmreads
typedef struct
{
  unsigned char  coef_probs[4][8][3][11+5];                 // 1536 bytes = 4*(3*128)
                                                            // Padding cause this has to be read in parts
  unsigned char  ymode_prob[4];                             // 4 bytes
  unsigned char  uvmode_prob[3];                            // 3 bytes
  unsigned char  reserved0;                                 // Pad to word boundary
  unsigned char  mv_context[2][19];                         // 38 bytes
  unsigned char  reserved1[18];                             // Align to 32 byte boundary

}nvenc_vp8_prob_state_s;


// 2.b. Rate Control stat buffer used for 2 pass encoding
// ------------------------------------------------------
//
// This buffer is used to store rate control information for 2 pass encoding.
// The same  buffer will be shared in both the passes of the encoding.
// The first pass will write MPEC stats to the buffer and 2nd pass will read
// the stats from the same buffer for QP calculation. The layout of buffer is
// msenc_rc_pic_stat_s followed by msenc_mbrow_stat_s for each mb row.
//

typedef struct
{
  unsigned int   intra_mb_count;                            // number of intra MBs.
  unsigned int   inter_mb_count;                            // number of inter MBs.
  unsigned int   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
  unsigned int   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
  unsigned int   total_bit_count;
  unsigned int   avgQP;
  unsigned int   complexity;
  //split encoding
  unsigned int   strip_complexity;
  unsigned int   strip_intra_mb_count;
  unsigned char  reserved[220];
}nvenc_rc_pic_stat_s;                                       //256 bytes

typedef struct
{
  unsigned int   mbrow_intra_mb_count : 12;                 // number of intra MBs
  unsigned int   mbrow_inter_mb_count : 12;                 // number of inter MBs
  unsigned int   mbrow_avgQP : 8;                           // average QP
  unsigned int   mbrow_cumulative_intra_cost;               // sum of all intra SA(T)Ds
  unsigned int   mbrow_cumulative_inter_cost;               // sum of all inter SA(T)Ds
  unsigned int   mbrow_bits_per_row;
}nvenc_mbrow_stat_s;

typedef struct
{
  signed char act_qp_buf[4096];                             // Buffer to store qp modifier based on avg activity for a MB row
}nvenc_act_stat_s;

// 2.b. Generic Map Buffer
// --------------------------------
//
// This buffer is used to store qp map when rcmode==4 & lambda_map when enabled. the structure is in run/level, run==0 means end.
//
typedef struct
{
  unsigned short run;                                       // number of blocks with the same value
  unsigned short level;                                     // value
} nvenc_generic_map_s;

typedef nvenc_generic_map_s nvenc_qp_map_s;
typedef nvenc_generic_map_s nvenc_lambda_map_s;


// 2.d. Encrypt Params buffer
// --------------------------------
//
// This buffer is used by SEC / TSEC engine which will encrypt the encoded stream
// MSENC will output the # of 16 byte blocks present in the encoded bitstream to noOfInputBlocks field.
// MSENC ucode does not use any other field besides noOfInputBlocks.
// These will be populated by driver / TSEC ucode
// Currently used only for MSENC 2.0 for T114
typedef struct _hdcp_encrypt_param_s
{
    unsigned int sessionId;                            // <<in
    unsigned int noOfInputBlocks;                      // <<in
    unsigned int streamId;                             // <<in
    unsigned int encOffset;                            // <<in
    unsigned int streamCtr;                            // >>out
    unsigned int crcOffset;                            // >>out
    unsigned long long inputCtr;                       // >>out
    unsigned long long pesPriv[2];                     // >>out
    unsigned int retCode;                              // >>out
    unsigned char  reserved2[4];
} hdcp_encrypt_param_s;

// Error codes used across all codecs
typedef enum
{
    E_NONE = 0U,
    E_GPTMR_TIMEOUT,
    E_MAGIC_NUMBER_MISMATCH,
    E_PICSETUP_OFFSET,
    E_ENC_STATUS_OUTPUT_OFFSET,
    E_OUTPUT_BITSTREAM_OFFSET,
    E_INSUFFICIENT_WRDMA_BUF,
    E_MDP_ERROR,
    E_RDO_ERROR,
    E_ME_PROGRAMMING,
    E_ME_HW,
    E_TFBIF_TIMEOUT,
    E_SKIP_ENCODING,
    E_INPUT_BITSTREAM_OFFSET,
    E_INPUT_HISTORY_OFFSET,
    E_MEPRED_OFFSET,
    E_EXTHINT_OFFSET,
    E_REFPIC_OFFSET,
    E_QPMAP_OFFSET,
    E_COLLOC_OFFSET,
    E_PROB_OFFSET,
    E_ENTROPY_OFFSET,
    E_FILTER_OFFSET,
    E_SSIM_RDO_OFFSET,
    E_RC_ERROR,
    E_SMALLE_BS_OVERFLOW,
    E_SMALLE_METHODSERROR,
    E_ENTROPY_ERR,
    E_RECON_OVERFLOW,
    TOTAL_ERR_CODES
} ERROR_CODES;

// 3  -  APPLICATION SPECIFIC METHOD REGISTERS
//
// The NVENC application method registers are accessed directly by the driver.
// The range assignment is 0x400 - 0x7FC:
//
//
// 3.1 - Common Methods
// --------------------
//
// SetControlParams()
// ------------------
// This method sets the control parameters that are used in this execution
// of the encode subclass.
//
// CODEC_TYPE field should be set as
//     3: H.264                                // same codec enum as for msdec
//
// FORCE_OUTPIC indicates that the encoded picture (after deblocking if that is enabled)
//     must be written out to the buffer specified by SetOutRefPic(), even if the
//     current picture is not marked as a reference picture.
//
// FORCE_OUTCOL indicates that the colocated vector data must be written out to the
//     buffer specified by SetOutColocData(), even if the current picture is
//     not marked as a reference picture.
//
// GPTIMER_ON, this flag is used in activating an app based timer with a timeout value
//     less than watchdog timer. When this app level timer expires, various hardware
//     submodules in NVENC will be reset, the status will be informed in flowControl
//     buffer if DEBUG_MODE is OFF (0) and app will return.
//
// ME_ONLY, indicates whether to run in normal mode of operation or activate only
//     motion estimation and write out search vector results
//     0 = encode bitstream
//     1 = only ME, do not run mode decision/recon/entropy encode
//
// SLICE_STAT_ON indicates if slice level statistics dumping is enabled. If enabled,
//     following the persistent state information, slice level statistics will be
//     output in the Encoder Status buffer. It is assumed that the number of slices
//     will be limited to picture height in units of MBs. If there are more slices in the
//     picture, driver should turn off slice stat output, by resetting this flag to 0.
//
// MPEC_STAT_ON indicates if nMB level mpec statistics dumping is enabled. If SLICE_STAT_ON
//     is enabled, MPEC_STAT_ON will be automatically get disabled and all the slice level
//     mpec statistics will be dumped to nvenc_mpec_stat_s.
//
// DEBUG_MODE indicates whether the app will return non-zero error codes to OS in case
//     of an error detected by the app.
//
//     If the value is set to 1, upon detecting an error the app will NOT do a
//     soft reset of hw, the app will return appropriate error code to OS. This
//     shall be used during debugging phase.
//
//     If the value is set to 0, upon detecting an error the app will do a soft
//     reset of hw and return with zero to OS. This shall be used for product.
//
// SUBFRAME_MODE used in low latency applications where client need to read the bitstream
//     data at slice boundary before frame done.
//     0: disable
//     1: flush at slice end and report last_valid_byte_offset
//     2: report slice offsets without flushing
//     3: report slice offsets with flush at slice end
//
// RC_MODE contains the rate control mode.
//     0: Constant QP
//     1: picture level VBR
//     2: picture level CBR
//     3: picture level VBR with const QP
//     4: QP Map
//    16: (same as 0)
//    17: mbrow level VBR
//    18: mbrow level CBR (QP is updated every macroblock row)
//    19: mbrow level VBR with const QP
//    32: macroblock level activity based rate control algorithm
//
//
//  31           24       20     16 15    12      8       4       0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             |D|     |G|S|M|C|P|       | CODEC | SetControlParams
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SET_CTRL_PARAM                    0x00000700  /* -W-4R */
// #define NV_C9B7_NVENC_SET_CTRL_PARAM_CODEC_TYPE                3:0  /* -W-UF */
// #define NV_C9B7_NVENC_SET_CTRL_PARAM_CODEC_TYPE_H264    0x00000003
// #define NV_C9B7_NVENC_SET_CTRL_PARAM_FORCE_OUTPIC              8:8  /* -W-UF */
// #define NV_C9B7_NVENC_SET_CTRL_PARAM_FORCE_OUTCOL              9:9  /* -W-UF */
// #define NV_C9B7_NVENC_SET_CTRL_PARAM_ME_ONLY                 10:10  /* -W-UF */
// #define NV_C9B7_NVENC_SET_CTRL_PARAM_SLICE_STAT_ON           11:11  /* -W-UF */
// #define NV_C9B7_NVENC_SET_CTRL_PARAM_GPTIMER_ON              12:12  /* -W-UF */
// #define NV_C9B7_NVENC_SET_CTRL_PARAM_MPEC_STAT_ON            15:15  /* -W-UF */
// #define NV_C9B7_NVENC_SET_CTRL_PARAM_DEBUG_MODE              16:16  /* -W-UF */
// #define NV_C9B7_NVENC_SET_CTRL_PARAM_SUBFRAME_MODE           18:17  /* -W-UF */
// #define NV_C9B7_NVENC_SET_CTRL_PARAM_RC_MODE                 31:24  /* -W-UF */
//
//
// SetPictureIndex()
// -----------------
//
// This method sets the picture index to NVENC. For the same picture, CEA and NVENC
// should receive the same picture index. Picture index should be incremented by 1
// in each new execute. CEA will put this in the CEA Hints data buffer.
//
//  31           24 23           16 15            8 7             0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                            INDEX                               | SetPictureIndex
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SET_PICTURE_INDEX                 0x00000704  /* -W-4R */
// #define NV_C9B7_NVENC_SET_PICTURE_INDEX_INDEX                 31:0  /* -W-UF */
//
//
// SetInRcData()
// ------------------
// This method sets the location of the RCDataBuffer. It sets the
// upper bits (32 most significant bits) of the 40-bit offset where the buffer
// is located. This buffer will have RC configuration data from CEA or an app.
//
// The values of OFFSET is defined and initialized to zero during the
// instance's creation.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInDrvPicSetup
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_RC_DATA                      0x0000070c  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_RC_DATA_OFFSET                     31:0  /* -W-UF */
//
// SetInDrvPicSetup()
// ------------------
// This method sets the location of the DrvPictureSetupBuffer. It sets the
// upper bits (32 most significant bits) of the 40-bit offset where the buffer
// is located.
//
// The values of OFFSET is defined and initialized to zero during the
// instance's creation.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInDrvPicSetup
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_DRV_PIC_SETUP               0x00000710  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_DRV_PIC_SETUP_OFFSET              31:0  /* -W-UF */
//
//
// SetInCEAHintsData()
// -------------------
// This method sets the CEA hints data buffer offset. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInCEAHintsData
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_CEA_HINTS_DATA              0x00000714  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_CEA_HINTS_DATA_OFFSET             31:0  /* -W-UF */
//
//
// SetOutEncStatus()
// -----------------
// This method sets the encoder status buffer offset. It sets the upper bits
// (32 most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetOutEncStatus
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETOUT_ENC_STATUS                 0x00000718  /* -W-4R */
// #define NV_C9B7_NVENC_SETOUT_ENC_STATUS_OFFSET                31:0  /* -W-UF */
//
//
// SetOutBitstream()
// -----------------
// This method sets the bitstream output data buffer offset. It sets the upper
// bits (32 most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetOutBitstream
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETOUT_BITSTREAM                  0x0000071c  /* -W-4R */
// #define NV_C9B7_NVENC_SETOUT_BITSTREAM_OFFSET                 31:0  /* -W-UF */
//
//
// SetIOHistory()
// --------------
// This method sets the history buffer offset. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetIOHistory
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIO_HISTORY                     0x00000720  /* -W-4R */
// #define NV_C9B7_NVENC_SETIO_HISTORY_OFFSET                    31:0  /* -W-UF */
//
//
// SetIORCProcess()
// ----------------
// This method sets the RC Process buffer offset. It sets the upper  bits
// (32 most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetIORCProcess
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIO_RC_PROCESS                 0x00000724  /* -W-4R */
// #define NV_C9B7_NVENC_SETIO_PROCESS_OFFSET                   31:0  /* -W-UF */
//
//
// SetInColocData()
// ----------------
// This method sets the input colocated motion data buffer offset. It sets the upper
// bits (32 most significant bits) of the 40-bit offset where the buffer is located.
// This is used only for encoding of B pictures. If the current picture is an I or P
// picture, this method can be omitted.
//
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInColocData
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_COLOC_DATA                  0x00000728  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_COLOC_DATA_OFFSET                 31:0  /* -W-UF */
//
//
// SetOutColocData()
// -----------------
// This method sets the output colocated motion data buffer offset. It sets the upper
// bits (32 most significant bits) of the 40-bit offset where the buffer is located.
// This is used only for encoding of pictures that are marked as reference pictures.
// If the current picture is not marked as a reference picture, this method can be omitted.
//
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetOutColocData
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETOUT_COLOC_DATA                 0x0000072c  /* -W-4R */
// #define NV_C9B7_NVENC_SETOUT_COLOC_DATA_OFFSET                31:0  /* -W-UF */
//
//
// SetOutRefPic()
// --------------
// This method sets the offset for the tiled 16x16 output reference picture produced by
// the encode of the current picture. It sets the upper bits (32 most significant bits)
// of the 40-bit offset where the buffer is located. This method must be sent if the
// current picture is marked as a reference picture, or the FORCE_OUTPIC bit is set in
// the SetControlParams().
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetOutRefPic
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETOUT_REFPIC                     0x00000730  /* -W-4R */
// #define NV_C9B7_NVENC_SETOUT_REFPIC_OFFSET                    31:0  /* -W-UF */
//
//
// SetInCurPic()
// -------------
// This method sets the offset for current input picture to be encoded. It sets the upper
// bits (32 most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInCurPic
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_CURPIC                      0x00000734  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_CURPIC_OFFSET                     31:0  /* -W-UF */
//
//
// SetInMEPredData()
// -----------------
// This method sets the offset for motion estimation predictor input data buffer.
// This is the buffer containing vector hints produced by an earlier execute of NVENC.
// The method sets the upper bits (32 most significant bits) of the 40-bit offset where
// the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInMEPredData
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_MEPRED_DATA                 0x00000738  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_MEPRED_DATA_OFFSET                31:0  /* -W-UF */
//
//
// SetOutMEPredData()
// ------------------
// This method sets the offset for motion estimation predictor output data Buffer.
// It sets the upper bits (32 most significant bits) of the 40-bit offset where the
// buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetOutMEPredData
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETOUT_MEPRED_DATA                0x0000073c  /* -W-4R */
// #define NV_C9B7_NVENC_SETOUT_MEPRED_DATA_OFFSET               31:0  /* -W-UF */
//
//
// SetInRefPic0()
// --------------
// This method sets the offset for reference picture 0. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic0
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC0                     0x00000400  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC0_OFFSET                    31:0  /* -W-UF */
//
//
// SetInRefPic1()
// --------------
// This method sets the offset for reference picture 1. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic1
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC1                     0x00000404  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC1_OFFSET                    31:0  /* -W-UF */
//
//
// SetInRefPic2()
// --------------
// This method sets the offset for reference picture 2. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic2
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC2                     0x00000408  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC2_OFFSET                    31:0  /* -W-UF */
//
//
// SetInRefPic3()
// --------------
// This method sets the offset for reference picture 3. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic3
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC3                     0x0000040c  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC3_OFFSET                    31:0  /* -W-UF */
//
//
// SetInRefPic4()
// --------------
// This method sets the offset for reference picture 4. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic4
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC4                     0x00000410  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC4_OFFSET                    31:0  /* -W-UF */
//
//
// SetInRefPic5()
// --------------
// This method sets the offset for reference picture 5. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic5
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC5                     0x00000414  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC5_OFFSET                    31:0  /* -W-UF */
//
//
// SetInRefPic6()
// --------------
// This method sets the offset for reference picture 6. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic6
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC6                     0x00000418  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC6_OFFSET                    31:0  /* -W-UF */
//
//
// SetInRefPic7()
// --------------
// This method sets the offset for reference picture 7. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic7
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC7                     0x0000041c  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC7_OFFSET                    31:0  /* -W-UF */
//
//
// SetInRefPic8()
// --------------
// This method sets the offset for reference picture 8. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic8
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC8                     0x00000420  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC8_OFFSET                    31:0  /* -W-UF */
//
//
// SetInRefPic9()
// --------------
// This method sets the offset for reference picture 9. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic9
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC9                     0x00000424  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC9_OFFSET                    31:0  /* -W-UF */
//
//
// SetInRefPic10()
// ---------------
// This method sets the offset for reference picture 10. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic10
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC10                    0x00000428  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC10_OFFSET                   31:0  /* -W-UF */
//
//
// SetInRefPic11()
// ---------------
// This method sets the offset for reference picture 11. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic11
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC11                    0x0000042c  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC11_OFFSET                   31:0  /* -W-UF */
//
//
// SetInRefPic12()
// ---------------
// This method sets the offset for reference picture 12. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic12
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC12                    0x00000430  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC12_OFFSET                   31:0  /* -W-UF */
//
//
// SetInRefPic13()
// ---------------
// This method sets the offset for reference picture 13. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic13
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC13                    0x00000434  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC13_OFFSET                   31:0  /* -W-UF */
//
//
// SetInRefPic14()
// ---------------
// This method sets the offset for reference picture 14. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic14
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC14                    0x00000438  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC14_OFFSET                   31:0  /* -W-UF */
//
//
// SetInRefPic15()
// ---------------
// This method sets the offset for reference picture 15. It sets the upper bits (32
// most significant bits) of the 40-bit offset where the buffer is located.
//
//  31                            16                               0
// .-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-.
// |                             OFFSET                            | SetInRefPic15
// `-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-'
//
// #define NV_C9B7_NVENC_SETIN_REFPIC15                    0x0000043c  /* -W-4R */
// #define NV_C9B7_NVENC_SETIN_REFPIC15_OFFSET                   31:0  /* -W-UF */
//
//
//
//
// 4  -  APPLICATION SPECIFIC ERROR CODES
//
// The upper 4 bit indicate the codec type. The lower 28 bit are the error code.
//
// 5  -  Some shared defines
#define TEB_BUF_SIZE      (256 * 1024)
#define HIST_BLOCK_SIZE   192

#define H264_HIST_BLOCK_SIZE  224
#define H265_HIST_BLOCK_SIZE  H264_HIST_BLOCK_SIZE
#define VP9_HIST_BLOCK_SIZE   H265_HIST_BLOCK_SIZE

//HIST BLOCK structre has not been finalized, subject to change.
#define AV1_HIST_BLOCK_SIZE   VP9_HIST_BLOCK_SIZE

// Errors
#define NvC9B7EncErrorH264ApptimerExpired                   0x30000001
#define NvC9B7EncErrorH264InvalidInput                      0x30000002
#define NvC9B7EncErrorH264HwerrInterrupt                    0x30000003
#define NvC9B7EncErrorH264BadMagic                          0x30000004

// Appendix A  -  KEY
//
//         Read
//           ' ' = Other Information
//           '-' = Field is part of a write-only register
//           'C' = Value read is always the same, constant value line follows (C)
//           'R' = Value is read
//
//         Write
//           ' ' = Other Information
//           '-' = Must not be written (D), value ignored when written (R,A,F)
//           'W' = Can be written
//
//         Internal State
//           ' ' = Other Information
//           '-' = No internal state
//           'X' = Internal state, initial value is unknown
//           'I' = Internal state, initial value is known and follows (I)
//           'V' = Internal state, initialize at volatile reset
//           'N' = Internal state, initial value and initialize at volatile reset
//           'C' = Internal state, initial value at object creation
//
//         Declaration/Size
//           ' ' = Other Information
//           '-' = Does Not Apply
//           'V' = Type is void
//           'U' = Type is unsigned integer
//           'S' = Type is signed integer
//           'F' = Type is IEEE floating point
//           '1' = Byte size (008)
//           '2' = Short size (016)
//           '3' = Three byte size (024)
//           '4' = Word size (032)
//           '8' = Double size (064)
//
//         Define Indicator
//           ' ' = Other Information
//           'D' = Device
//           'M' = Memory
//           'R' = Register
//           'A' = Array of Registers
//           'F' = Field
//           'V' = Value
//           'C' = Clear value
//           'S' = Set value
//           'T' = Call task function


#endif
