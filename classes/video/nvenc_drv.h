/*******************************************************************************
    Copyright (c) 2018-2020, NVIDIA CORPORATION. All rights reserved.

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.

*******************************************************************************/

#ifndef NVENC_DRV_H
#define NVENC_DRV_H
//
// CLASS NV_90B7/ NV_A0B7/ NV_C0B7/ NV_D0B7_NVENC/ NV_C1B7_NVENC/ NV_C2B7_NVENC / NV_C3B7_NVENC / NV_B4B7_NVENC / NV_C5B7_NVENC / NV_C4B7_NVENC
// ==============================================
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
// nal_unit_header_mvc_extension( ) {                                     :
//  non_idr_flag                                                          :
//  priority_id                                                           :
//  view_id                                                               :
//  temporal_id                                                           :
//  anchor_pic_flag                                                       :
//  inter_view_flag                                                       :
//  reserved_one_bit                                                      :
// }                                                                      :
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
  NvU32   luma_log2_weight_denom              : 3;    // the base 2 logarithm of the denominator for all luma weighting factors. The value of luma_log2_weight_denom shall be in the range of 0 to 7, inclusive
  NvU32   chroma_log2_weight_denom            : 3;    // the base 2 logarithm of the denominator for all chroma weighting factors. The value of chroma_log2_weight_denom shall be in the range of 0 to 7, inclusive.
  NvU32   reversed                            : 26;

  NvU8    luma_weight_l0_flag[16];                    // equal to 1 specifies that weighting factors for the luma component of list 0 prediction are present, luma_weight_l0_flag[8~15] not used now
  NvU8    chroma_weight_l0_flag[16];                  // equal to 1 specifies that weighting factors for the chroma prediction values of list 0 prediction are present, chroma_weight_l0_flag[8~15] not used now

  NvU8    luma_weight_l1_flag[16];                    // have the same semantics as luma_weight_l0_flag, chroma_weight_l0_flag, not used now
  NvU8    chroma_weight_l1_flag[16];                  // not used now

  NvS16   luma_weight_l0[16];                         // is the weighting factor applied to the luma prediction value for list 0 prediction using RefPicList0[ i ]. When luma_weight_l0_flag is equal to 1, the value of luma_weight_l0[ i ] shall be in the range of -128 to 127, inclusive. -128 to 255 for H265
  NvS16   luma_offset_l0[16];                         // is the additive offset applied to the luma prediction value for list 0 prediction using RefPicList0[ i ]. The value of luma_offset_l0[ i ] shall be in the range of -128 to 127, inclusive. -2^9 to 2^9-1 for h265
  NvS16   luma_weight_l1[16];                         // is the weighting factor applied to the luma prediction value for list 0 prediction using RefPicList0[ i ]. When luma_weight_l0_flag is equal to 1, the value of luma_weight_l0[ i ] shall be in the range of -128 to 127, inclusive. -128 to 255 for H265
  NvS16   luma_offset_l1[16];                         // is the additive offset applied to the luma prediction value for list 0 prediction using RefPicList0[ i ]. The value of luma_offset_l0[ i ] shall be in the range of -128 to 127, inclusive. -2^9 to 2^9-1 for h265
  NvS16   chroma_weight_l0[2][16];                    // has the similar meaning with luma_weight/offset
  NvS16   chroma_offset_l0[2][16];                    // has the similar meaning with luma_weight/offset
  NvS16   chroma_weight_l1[2][16];                    // has the similar meaning with luma_weight/offset
  NvS16   chroma_offset_l1[2][16];                    // has the similar meaning with luma_weight/offset

} nvenc_pred_weight_table_s;                          // 452 bytes

typedef struct
{
  NvU16   frame_width_minus1;                        // frame width in pixels minus 1, range 0-4095
  NvU16   frame_height_minus1;                       // frame height in pixels minus 1, range 0-4095
  NvU16   sfc_pitch;                                 // pitch of luma plane
  NvU16   sfc_pitch_chroma;                          // pitch of chroma plane
  NvU16   sfc_trans_mode;                            // least 3 significient bits are used to stand for 8 modes, normal, xflip, yflip etc.
  NvU16   reserved2;                                 // pad to NvS32

  // do not use offset from cmod trace as golden, cmod behavior is different.
  NvU32   luma_top_frm_offset;                       // offset of luma top field or frame in units of 256
  NvU32   luma_bot_offset;                           // offset of luma bottom field in units of 256. Not used if frame format.
  NvU32   chroma_top_frm_offset;                     // offset of chroma top field or frame, or offset of chroma U(Cb) plane in planar mode, both in units of 256
  NvU32   chroma_bot_offset;                         // offset of chroma bottom field, not used if frame format. Or offset of chroma V(Cr) plane in planar mode. Both in units of 256.
  NvU32   block_height                       : 7;    // BL mapping block height setting
  NvU32   tiled_16x16                        : 1;    // Surface is 16x16 tiled instead of BL mapped (must be 1 for refpics)
  NvU32   memory_mode                        : 2;    // 0: semi-planar, 1: planar
  NvU32   nv21_enable                        : 1;    // the surface format is yuv or yvu
  NvU32   input_bl_mode                      : 1;    // the input block linear mode: 0~gpu bl; 1~tegra bl; this is only for cmod and cmod/plugin
  NvU32   reserved                           : 20;   // pad to NvS32
} nvenc_h264_surface_cfg_s;                          // 32 bytes

typedef struct
{
  NvU32   profile_idc                        : 8;    // seq_parameters(_ext) regs
  NvU32   level_idc                          : 8;
  NvU32   chroma_format_idc                  : 2;    // 0=monochrome, 1=yuv4:2:0, 2-3=not supported
  NvU32   pic_order_cnt_type                 : 2;    // only support values 0 and 2
  NvU32   log2_max_frame_num_minus4          : 4;
  NvU32   log2_max_pic_order_cnt_lsb_minus4  : 4;
  NvU32   frame_mbs_only                     : 1;
  //mvc
  NvU32   stereo_mvc_enable                  : 1;    //if this bit is enabled, the profile_idc should be 128
  NvU32   separate_colour_plane_flag         : 1;    //if this bit is enabled, the chroma_format_idc should be 3
  NvU32   lossless_qpprime_flag              : 1;    //if this bit is enabled and QP=0, bypass trans/q and encoded as lossless MB (hori or vert)
} nvenc_h264_sps_data_s;                             // 4 bytes

typedef struct
{
  NvU32   pic_param_set_id                   : 8;    // picture parameter set identification
  NvU32   entropy_coding_mode_flag           : 1;    // select entropy copding mode: cabac, cavlc
  NvU32   num_ref_idx_l0_active_minus1       : 5;    // number of currently active reference pictures in list 0 minus 1, range 0..30
  NvU32   num_ref_idx_l1_active_minus1       : 5;    // number of currently active reference pictures in list 1 minus 1, range 0..30
  NvU32   weighted_bipred_idc                : 2;    // weighted prediction mode for B: only 0 (default) and 2 (implicit) are supported
  NvS32   pic_init_qp_minus26                : 6;    // initial QP value, range -26..+25
  NvS32   chroma_qp_index_offset             : 5;    // offset to add to chroma QPy for QPc table indexing for Cb, range -12..+12

  NvS32   second_chroma_qp_index_offset      : 5;    // offset to add to chroma QPy for QPc table indexing for Cr, range -12..+12
  NvU32   constrained_intra_pred_flag        : 1;    // if set, intra prediction can only use pixels from macroblocks that are also intra coded
  NvU32   deblocking_filter_control_present_flag : 1;// if set, deblock filter controls syntax elements will be present/encoded in the stream
  NvU32   transform_8x8_mode_flag            : 1;    // if set, enables the use of transform size 8x8 (flags will be encoded in stream per macroblock)
  NvU32   pic_order_present_flag             : 1;    // if set, pic_order control info is encoded in the stream
  NvU32   weighted_pred_flag                 : 1;
  NvU32   reserved0                          : 22;   // pad to full NvS32
} nvenc_h264_pps_data_s;                             // 8 bytes

typedef struct
{
  NvU32   self_temporal_stamp_l0             : 3;    // stamp to use for L0 integer search in stamp based mode
  NvU32   self_temporal_stamp_l1             : 3;    // stamp to use for L1 integer search in stamp based mode
  NvU32   self_temporal_explicit             : 1;    // explicitly evaluate these vectors
  NvU32   self_temporal_search               : 1;    // integer search enable for this hint
  NvU32   self_temporal_refine               : 1;    // subpel search enable for this hint
  NvU32   self_temporal_enable               : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)

  NvU32   coloc_stamp_l0                     : 3;    // stamp to use for L0 integer search in stamp based mode
  NvU32   coloc_stamp_l1                     : 3;    // stamp to use for L1 integer search in stamp based mode
  NvU32   coloc_explicit                     : 1;    // explicitly evaluate these vectors
  NvU32   coloc_search                       : 1;    // integer search enable for this hint
  NvU32   coloc_refine                       : 1;    // subpel search enable for this hint
  NvU32   coloc_enable                       : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)

  NvU32   self_spatial_stamp_l0              : 3;    // stamp to use for L0 integer search in stamp based mode
  NvU32   self_spatial_stamp_l1              : 3;    // stamp to use for L1 integer search in stamp based mode
  NvU32   self_spatial_explicit              : 1;    // explicitly evaluate these vectors
  NvU32   self_spatial_search                : 1;    // integer search enable for this hint
  NvU32   self_spatial_refine                : 1;    // subpel search enable for this hint
  NvU32   self_spatial_enable                : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)
  NvU32   reserved0                          : 2;    // pad to full NvS32

  NvU32   external_stamp_l0_refidx0_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 0
  NvU32   external_stamp_l0_refidx1_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 1
  NvU32   external_stamp_l0_refidx2_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 2
  NvU32   external_stamp_l0_refidx3_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 3
  NvU32   external_stamp_l0_refidx4_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 4
  NvU32   external_stamp_l0_refidx5_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 5
  NvU32   external_stamp_l0_refidx6_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 6
  NvU32   external_stamp_l0_refidx7_stamp    : 3;    // Stamp to use for external L0 hint with refidx = 7
  NvU32   external_stamp_l1_refidx0_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 0
  NvU32   external_stamp_l1_refidx1_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 1
  NvU32   reserved1                          : 2;    // pad to full NvS32


  NvU32   external_stamp_l1_refidx2_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 2
  NvU32   external_stamp_l1_refidx3_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 3
  NvU32   external_stamp_l1_refidx4_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 4
  NvU32   external_stamp_l1_refidx5_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 5
  NvU32   external_stamp_l1_refidx6_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 6
  NvU32   external_stamp_l1_refidx7_stamp    : 3;    // Stamp to use for external L1 hint with refidx = 7


  NvU32   external_explicit                  : 1;    // explicitly evaluate these vectors
  NvU32   external_search                    : 1;    // integer search enable for this hint
  NvU32   external_refine                    : 1;    // subpel search enable for this hint
  NvU32   external_enable                    : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)

  NvU32   const_mv_stamp_l0                  : 3;    // stamp to use for L0 integer search in stamp based mode
  NvU32   const_mv_stamp_l1                  : 3;    // stamp to use for L1 integer search in stamp based mode
  NvU32   const_mv_explicit                  : 1;    // explicitly evaluate these vectors
  NvU32   const_mv_search                    : 1;    // integer search enable for this hint
  NvU32   const_mv_refine                    : 1;    // subpel search enable for this hint
  NvU32   const_mv_enable                    : 1;    // hint enable (enables processing + fetching of data)

  NvU32   stamp_refidx1_stamp                : 3;    // Stamp to use for multi-ref hint with refidx = 1
  NvU32   stamp_refidx2_stamp                : 3;    // Stamp to use for multi-ref hint with refidx = 2
  NvU32   stamp_refidx3_stamp                : 3;    // Stamp to use for multi-ref hint with refidx = 3
  NvU32   stamp_refidx4_stamp                : 3;    // Stamp to use for multi-ref hint with refidx = 4
  NvU32   stamp_refidx5_stamp                : 3;    // Stamp to use for multi-ref hint with refidx = 5
  NvU32   stamp_refidx6_stamp                : 3;    // Stamp to use for multi-ref hint with refidx = 6
  NvU32   stamp_refidx7_stamp 		     : 3;    // Stamp to use for multi-ref hint with refidx = 7
  NvU32   reserved2            		     : 11;   // pad to full NvS32

} nvenc_h264_me_hint_cfg_s;                          // 16 bytes

typedef struct
{
  NvU32   mvx_frac                           : 2;    // vector X component fraction
  NvS32   mvx_int                            : 12;   // vector X component integer portion
  NvU32   reserved_x                         : 2;    // padding X to 16 bit
  NvU32   mvy_frac                           : 2;    // vector Y component fraction
  NvS32   mvy_int                            : 10;   // vector Y component integer portion
  NvU32   reserved_y                         : 4;    // padding Y to 16 bit
} nvenc_h264_me_const_vec_s;                         // 4 bytes

typedef struct
{
  NvU32   bitmask[2]                         ;       // 8x8 search point pattern bitmask
  NvU32   hor_adjust                         : 1;    // shift hor based on lsb of pred
  NvU32   ver_adjust                         : 1;    // shift ver based on lsb of pred
  NvU32   reserved                           : 30;
} nvenc_h264_me_stamp_s;                             // 12 bytes

typedef struct
{
  NvU32   mv_limit_enable                    : 1;    // 0=disable, 1=enable set the absolute MV range (Cisco flip camera request)
  NvU32   left_mvx_frac                      : 2;    // vector X component fraction
  NvS32   left_mvx_int                       : 12;   // vector X component integer portion
  NvU32   reserved1                          : 1;    // padding X to 16 bit
  NvU32   top_mvy_frac                       : 2;    // vector Y component fraction
  NvS32   top_mvy_int                        : 10;   // vector Y component integer portion
  NvU32   reserved2                          : 4;    // padding Y to 16 bit

  NvU32   right_mvx_frac                     : 2;    // vector X component fraction
  NvS32   right_mvx_int                      : 12;   // vector X component integer portion
  NvU32   reserved3                          : 2;    // padding X to 16 bit
  NvU32   bottom_mvy_frac                    : 2;    // vector Y component fraction
  NvS32   bottom_mvy_int                     : 10;   // vector Y component integer portion
  NvU32   reserved4                          : 4;    // padding Y to 16 bit
} nvenc_h264_me_limit_mv_s;                          // 8 bytes

typedef struct
{
  NvU32  teb_K                               : 8;    // TEB K
  NvU32  teb_N                               : 8;    // TEB N
  NvU32  teb_P                               : 8;    // TEB P
  NvU32  teb_S                               : 4;    // TEB S
  NvU32  teb_mode                            : 4;    // TEB mode
} nvenc_me_tebif_s;

typedef struct
{
  NvU32   me_predictor_mode                  : 1;    // 0=MDP, 1=const
  NvU32   refinement_mode                    : 1;    // 0=hpel, 1=qpel
  NvU32   lambda_mode                        : 1;    // 0=MDP, 1=const
  NvU32   const_lambda                       : 11;   // U8.3 format lambda, used if lambda_mode=1
  NvU32   refine_on_search_enable            : 1;    // 0=disable, 1=enable subpel refine for search candidates
  NvU32   me_only_mode                       : 1;
  NvU32   fps_mvcost                         : 1;
  NvU32   sps_mvcost                         : 1;
  NvU32   sps_cost_func                      : 1;
  NvU32   me_icc_mode_mad                    : 1;

  NvU32   sps_filter                         : 3;
  NvU32   mc_filter                          : 3;
  NvU32   vc1_fastuv_mc                      : 1;
  NvU32   vc1_mc_rnd                         : 1;
  NvU32   mbc_ctrl_arbitor                   : 2;
  NvU32   mv_only_enable                     : 1;
  NvU32   average_mvhint_enable              : 1;

  nvenc_h264_me_limit_mv_s      limit_mv;
  nvenc_h264_me_hint_cfg_s      predsrc;             // predictor sources
  nvenc_h264_me_const_vec_s     l0_hint;             // constant MV used as L0 hint
  nvenc_h264_me_const_vec_s     l1_hint;             // constant MV used as L1 hint
  nvenc_h264_me_const_vec_s     l0_pred;             // constant MV used as L0 predictor
  nvenc_h264_me_const_vec_s     l1_pred;             // constant MV used as L1 predictor
  nvenc_h264_me_stamp_s         shape0;              // priority 0 stamping pattern
  nvenc_h264_me_stamp_s         shape1;              // priority 1 stamping pattern
  nvenc_h264_me_stamp_s         shape2;              // priority 2 stamping pattern
  nvenc_h264_me_stamp_s         shape3;              // priority 3 stamping pattern
  nvenc_h264_me_stamp_s         shape4;              // priority 4 stamping pattern
  nvenc_h264_me_stamp_s         shape5;              // priority 5 stamping pattern
  nvenc_h264_me_stamp_s         shape6;              // priority 6 stamping pattern
  nvenc_h264_me_stamp_s         shape7;              // priority 7 stamping pattern
  nvenc_me_tebif_s              teb_para;            // TEB parameter
  NvU32   mbc_mb_size                        : 9;    // MBC size in MB
  NvU32   rmvp_source_half_scaled            : 1;    // only support picture size half scale down in horz&vert
  NvU32   partDecisionMadeByFPP              : 1;    // enable CU32 FPP
  NvU32   penalty_factor_FPP                 : 6;    // MV with cost no larger than min_cost + ((min_cost * factor) >> 8) will go to SPS, not used for nvenc7
  NvU32   spatial_hint_pattern               : 4;
  NvU32   temporal_hint_pattern              : 6;
  NvU32   Cu16partDecisionMadeByFPP          : 1;    // enable CU16 making part decision
  NvU32   sps_mvp_from_fbm                   : 1;    // 1:sps use fbm's MVP and lambda;0:based on me_predictor_mode and lambda_mode to set the mvp and lambda for sps
  NvU32   reserved2                          : 3;
  NvU32   fbm_op_winner_num_p_frame          : 3;    // FBM output number of integer winners for each PU
  NvU32   fbm_op_winner_num_b_frame_l0       : 3;    // FBM output number of integer winners for each PU
  NvU32   fbm_op_winner_num_b_frame_l1       : 3;    // FBM output number of integer winners for each PU
  NvU32   fbm_select_best_cu32_parttype_num  : 3;    // cu32 select best parttype num
  NvU32   sps_evaluate_merge_cand_num        : 3;    // number of merg candidate for sps to explicit evaluate
  NvU32   fps_quad_thresh_hold               : 10;
  NvU32   external_hint_order                : 1;    //0:Insert external hint at first ref; 1:insert external hint at last ref;
  NvU32   coloc_hint_order                   : 1;    //0:Insert coloc hint at first ref; 1: insert coloc hint at last ref.
  NvU32   reserved1                          : 5;
  NvS32   ct_threshold                       : 16;   // for OFS only, threshold in Census Transform calculation
  //(1<<hint_type0)|(1<<hint_type1)|(1<<hint_type2)|(1<<hint_type3)|(1<<hint_type4) should be equal to 0x1f
  NvU32   hint_type0                         : 3;    //0:const hint; 1:spatial hint; 2:temporal hint; 3:coloc hint; 4:external hint
  NvU32   hint_type1                         : 3;    //the same as hint_type0
  NvU32   hint_type2                         : 3;    //the same as hint_type0
  NvU32   hint_type3                         : 3;    //the same as hint_type0
  NvU32   hint_type4                         : 3;    //the same as hint_type0
  NvU32   pyramidal_hint_order               : 1;    // for OFS only, used when do pyramidal ME, 1: enable, the hint order will be: if one MB has external hint, use exhint, otherwise use spatial/temproal/const hints. 0: disable.
  NvU32   left_hint_delay_N                  : 4;    // for OFS only, specify left hint delay MB number(1~8), used for high-perf mode (left_hint_delay_N need to set to 8 to achieve 4K@120FPS). set 0 for using default value, ie 3 for ofs/H265, 8 for H26
  NvU32   reserved3                          : 28;
  NvU8    reserved[32];                              // padding to make me_control=192 to allow 3 reads of 64 in ucode
} nvenc_h264_me_control_s;                           // 192 bytes

typedef struct
{
  NvU16   intra_refresh_cnt;                         // distance between forced-intra MBs in slice; 0 means OFF
  NvU16   intra_refresh_offs;                        // first forced-intra MB in slice

  NvU32   intra_luma4x4_mode_enable          : 9;    // bitmask indicating which intra luma 4x4 modes to enable
  NvU32   intra_luma8x8_mode_enable          : 9;    // bitmask indicating which intra luma 8x8 modes to enable
  NvU32   intra_luma16x16_mode_enable        : 4;    // bitmask indicating which intra luma 16x16 modes to enable
  NvU32   intra_chroma_mode_enable           : 4;    // bitmask indicating which intra chroma modes to enable
  NvU32   inter_penalty_factor_for_ip1       : 6;    // early teminate ip1 intra search if intra cost is larger than inter_cost + ((inter_cost * factor) >> 8)

  NvU32   l0_part_16x16_enable               : 1;    // enable L0 prediction for 16x16
  NvU32   l0_part_16x8_enable                : 1;    // enable L0 prediction for 16x8
  NvU32   l0_part_8x16_enable                : 1;    // enable L0 prediction for 8x16
  NvU32   l0_part_8x8_enable                 : 1;    // enable L0 prediction for 8x8
  NvU32   l0_part_8x4_enable                 : 1;    // enable L0 prediction for 8x4
  NvU32   l0_part_4x8_enable                 : 1;    // enable L0 prediction for 4x8
  NvU32   l0_part_4x4_enable                 : 1;    // enable L0 prediction for 4x4
  NvU32   l0_part_reserved                   : 1;    // reserved for future L0 prediction extensions
  NvU32   l1_part_16x16_enable               : 1;    // enable L1 prediction for 16x16
  NvU32   l1_part_16x8_enable                : 1;    // enable L1 prediction for 16x8
  NvU32   l1_part_8x16_enable                : 1;    // enable L1 prediction for 8x16
  NvU32   l1_part_8x8_enable                 : 1;    // enable L1 prediction for 8x8
  NvU32   l1_part_reserved                   : 4;    // reserved for future L1 prediction extensions
  NvU32   bi_part_16x16_enable               : 1;    // enable Bi prediction for 16x16
  NvU32   bi_part_16x8_enable                : 1;    // enable Bi prediction for 16x8
  NvU32   bi_part_8x16_enable                : 1;    // enable Bi prediction for 8x16
  NvU32   bi_part_8x8_enable                 : 1;    // enable Bi prediction for 8x8
  NvU32   bi_part_reserved                   : 4;    // reserved for future Bi prediction extensions
  NvU32   bdirect_mode                       : 2;    // 0: disable, 1: spatial, 2: temporal
  NvU32   bskip_enable                       : 1;    // enable b_skip encoding (requires b_direct encoding)
  NvU32   pskip_enable                       : 1;    // enable p_skip encoding
  NvU32   special_reserved                   : 4;    // reserved for future special prediction extensions

  NvS16   bias_inter_16x16;                          // bias for inter 16x16 (subtracts from inter 16x16 cost)
  NvS16   bias_inter_16x8;                           // bias for inter 16x8  (subtracts from inter 16x8  cost)

  NvS16   bias_inter_8x16;                           // bias for inter 8x16  (subtracts from inter 8x16  cost)
  NvS16   bias_inter_8x8;                            // bias for inter 8x8   (subtracts from inter 8x8   cost)

  NvS16   bias_pskip;                                // bias for pskip       (subtracts from pskip cost)
  NvS16   bias_bdir;                                 // bias for bdirect     (subtracts from bdirect cost)

  NvS16   bias_intra_over_inter;                     // bias for any intra   (subtracts from intra cost)
  NvS16   bias_intra_16x16;                          // bias for intra 16x16 (subtracts from intra 16x16 cost)

  NvS16   bias_intra_8x8;                            // bias for intra 8x8   (subtracts from intra 8x8 cost)
  NvS16   bias_intra_4x4;                            // bias for intra 4x4   (subtracts from intra 4x4 cost)

  NvS16   bias_intra_most_prob;                      // bias for intra most probable mode
  NvU16   mv_cost_bias;                              // bias applied to internal motion vector cost calculation

  NvU16   intra_nxn_bias_multiplier;                 // 0..255 range; used for internal bias calculation
  NvU16   intra_most_prob_bias_multiplier;           // 0..31 range; used for internal bias calculation

  NvS16   pskip_bias_multiplier;                     // S16 range; used for internal bias calculation
  NvS16   bdirect_bias_multiplier;                   // S16 range; used for internal bias calculation

  NvU16   pskip_esc_threshold;                       // if pskip cost is below this, disable full search & attempt pskip encode
  NvU16   bdirect_esc_threshold;                     // if bdirect cost is below this, disable full search & attempt bdirect encode

  NvU16   early_intra_disable_mpeb_threshold;        // if early intra cost is below this, disable full search & select intra
  NvU16   tempmv_wt_spread_threshold;                // The threshold for the sum of differences between 8x8s and 16x16 mvs. Used for temporal mv weight
  NvU32   tempmv_wt_distort_threshold        : 16;   // The threshold for the cost of the 16x16 mv. Used for temporal mv weight
  NvU32   mv_cost_predictor_control          : 1;    // 0=use 16x16 predictor only; 1=use accurate predictor (slower but better)
  NvU32   mv_cost_enable                     : 1;    // enable mv cost calculations in mode decision
  NvU32   intra_most_prob_force_on           : 1;    // always evaluate most probably 4x4 or 8x8 intra mode during full search
  NvU32   early_intra_mode_control           : 2;    // 0=inter; 1=eval_intra; 2=ext_hint; 3=none (low perf,best qual)
  NvU32   early_intra_mode_type_16x16dc      : 1;    // evaluate 16x16dc if early_intra_mode_control==1
  NvU32   early_intra_mode_type_16x16h       : 1;    // evaluate 16x16h  if early_intra_mode_control==1
  NvU32   early_intra_mode_type_16x16v       : 1;    // evaluate 16x16v  if early_intra_mode_control==1
  NvU32   early_ip_is_final                  : 1;    // if set, result of early intra is final (disable full intra search)
  NvU32   chroma_eval_mode                   : 1;    // for intra chroma search 0 = U and V; 1 = U only
  NvU32   ip_search_mode                     : 3;    // which intra sizes to search before making IP decision b0=4x4,b1=8x8,b2=16x16
  NvU32   multiply_bias_with_lambda          : 1;    // setting this to 1 will result in bias being multiplied by lambda
  NvU32   force_ipcm                         : 1;    // 0=normal encode; 1=force mpeb/mpec encode as IPCM
  NvU32   early_termination_ip1              : 1;    // enable IP1 early termination
  NvS32   bias_favor_intra_16x16_early       : 16;   // S16 range; used for internal bias calculation
  NvU32   priority_ipred_type_ip1            : 3;    // which intra type search first for early termination, must be a subset of ip_search_mode
  NvU32   intra_ssd_cnt_4x4                  : 4;    // SSD cost evaluation between N top intra modes selected by satd
  NvU32   intra_ssd_cnt_8x8                  : 4;    // SSD cost evaluation between N top intra modes selected by satd
  NvU32   intra_ssd_cnt_16x16                : 4;    // SSD cost evaluation between N top intra modes selected by satd
  NvU32   skip_evaluate_enable               : 1;    // 0: not check skip ssd cost; 1, evaluate skip ssd cost
  NvU32   rdo_level                          : 3;    // 0:mode decision by satd; 1:mode decision by ssd
  NvU32   tu_search_num                      : 3;    // tu search part num per MB
  NvU32   luma_residual_zero_eval            : 2;    // luma residual zero cost evaluate, bit0:intra; bit1:inter
  NvU32   num_1div8_lambda_ssd               : 7;    // number of 1/8 lambda ssd for lambda_coef delta. lambda_coef = (1+num_1div8_lambda_ssd*1/8)*lambda_ssd
  NvU32   ofs_mvx_range                      : 2;    // control mvx precision/range, 0: ouptput format: S10.5, range[-1024, 1023]; 1: output format: S11.4,range                                                                  [-2048, 2047]; 2: output format: S12.3, range[-4096, 4095]; 3: output format: S12.2, range[-4096, 4095]
  NvU32   reserved4                          : 15;   // pad to 32 bytes
  NvS16   bias_tu_4x4;                               // bias for TU 4x4
  NvS16   bias_tu_8x8;                               // bias for TU 8x8
  NvU32   ssim_rdo                           : 2;    // SSIM-RDO: 0 disable, 1: enable SW SSIM-RDO, 2: enable HW SSIM-RDO,
  NvU32   calc_ssim                          : 1;    // Calculate SSIM distortion: 0 disable, 1: enable. ssim_rdo = 1 => calc_ssim = 1
  NvU32   reserved5                          : 29;   // reserved
  NvU32   ssd_over_ssim_factor;                      // distortion ssd/ssim ratio, in U24.8 format
  NvU32   reserved6[13];                             // reserved
} nvenc_h264_md_control_s;                           // 128 bytes

typedef struct
{
  NvU16   qpp_run_vector_4x4;                        // cost values for 4x4 transform (16 bit total vector)
  NvU16   qpp_run_vector_8x8[3];                     // cost values for 8x8 transform (48 bit total vector)
                                                     // 2 bits each for first 12 coefs, then 1 bit each for next 24
  NvU32   qpp_luma8x8_cost                   : 8;    // luma 8x8 cost threshold, 0 = throw out all coefs, range 0-15
  NvU32   qpp_luma16x16_cost                 : 8;    // luma 16x16 cost threshold, 0 = throw out all coefs, range 0-15
  NvU32   qpp_chroma_cost                    : 8;    // chroma cost threshold, 0 = throw out all coefs, range 0-15
  NvU32   qpp_mode                           : 2;    // 0 = OFF, 1 = 8x8, 2 = 16x16_8x8
  NvU32   reserved1                          : 4;    // padding
    //add by cl
  NvU32   quant_intra_sat_flag               : 1;    // 0: no saturation is applied; 1: saturation is applied
  NvU32   quant_inter_sat_flag               : 1;    // 0: no saturation is applied; 1: saturation is applied
  NvU32   quant_intra_sat_limit              : 16;   // quantization saturation limit for intra MB
  NvU32   quant_inter_sat_limit              : 16;   // quantization saturation limit for inter MB
    //~
  NvU16   dz_4x4_YI[16];                             // deadzone for 4x4 transform of Luma Intra
  NvU16   dz_4x4_YP[16];                             // deadzone for 4x4 transform of Luma Inter
  NvU16   dz_4x4_CI;                                 // deadzone for 4x4 transform of Chroma Intra
  NvU16   dz_4x4_CP;                                 // deadzone for 4x4 transform of Chroma Inter
  NvU16   dz_8x8_YI[16];                             // deadzone for 8x8 transform of Luma Intra, 16 values are mapped to 64 coefs
  NvU16   dz_8x8_YP[16];                             // deadzone for 8x8 transform of Luma Inter, 16 values are mapped to 64 coefs
  NvU32   reserved2[11];                             // pad to 192 bytes to allow 3 reads of 64 bytes in ucode
                                                     //     this is optimal size as 160=5 reads of 32 will exceed dma queue
} nvenc_h264_quant_control_s;                        // 192 bytes

typedef struct
{
  NvU32   reorder_l0_cmd_count               : 4;
  NvU32   reorder_l1_cmd_count               : 4;
  NvU32   mmco_cmd_count                     : 4;
  NvU32   no_output_of_prior_pic_flag        : 1;
  NvU32   long_term_ref_flag                 : 1;
  NvU32   reserved                           :18;
} nvenc_h264_refpic_cmd_s;                            // 4 bytes

typedef struct
{
  // ref_pic_list_reorder_cmd includes both opcode and op arguments
  NvU32   reordering_of_pic_nums_idc         : 3;     //  Bit 31-29 (0-5  (Table 7-7)
  NvU32   abs_diff_pic_num_minus1            :17;     //  (opcode=0/1):
  NvU32   long_term_pic_num                  : 4;     //  (opcode=2):
  NvU32   abs_diff_view_idx_minus1           : 4;     //  (opcode=4/5):
  NvU32   reserved                           : 4;
} nvenc_h264_ref_pic_reorder_s;                       // 4 bytes

typedef struct
{
  NvU32   mmco_cmd_id                        : 3;     // totally 6 commands

  NvU32   difference_of_pic_nums_minus1      :17;     // 1) mark a short_term ref as long (mmco=3); 2) mark a short_term as un-used (mmco=1)
  NvU32   long_term_pic_num                  : 3;     // [0 to MaxLongTermFrameIdx ]; mark a long as "unused for ref" (mmco=2);
  NvU32   long_term_frame_idx                : 3;     // [0 to MaxLongTermFrameIdx] 1) mark a picture to long (mmco=3, mmco=6)
  NvU32   max_long_term_frame_idx_plus1      : 4;     // [0, num_ref_frames] max long term frm indexes allowed for long term pictures.
                                                      // max_long_term_frame_idx_plus1=0 -> MaxLongTermFrameIdx="no-long-term"
                                                      // max_long_term_frame_idx_plus1>0 -> MaxLongTermFrameIdx= max_long_term_frame_idx_plus1-1
  NvU32   reserved                           : 2;
} nvenc_h264_mmco_s;                                  // 4 bytes

typedef struct
{
  // int0
  NvU32   num_mb                            : 19;     // number of macroblocks in this slice, support upto 8kx8k
  NvU32   qp_avr                            :  8;     // 6bit value used by hw. Other bits can be used for rounding.
  NvU32   reserved                          :  5;
  // int1
  NvU32   slice_tgt_rate;                             // target bit rate (size) (if RC mode == slice)

  // int2
  NvS32   slice_alpha_c0_offset_div2         : 4;
  NvS32   slice_beta_offset_div2             : 4;
  NvU32   qp_slice_min                       : 8;    // min slice qp value
  NvU32   qp_slice_max                       : 8;    // max slice qp value
  NvU32   force_intra                        : 1;    // force entire slice to be intra
  NvU32   disable_deblocking_filter_idc      : 2;
  NvU32   cabac_init_idc                     : 2;
  NvU32   disable_slice_header               : 1;    // if set to 1, disables HW slice header output
  NvU32   reserved0                          : 2;

  // int3
  NvU32   num_ref_idx_active_override_flag   : 1;
  NvU32   num_ref_idx_l0_active_minus1       : 5;    // [0.31]
  NvU32   num_ref_idx_l1_active_minus1       : 5;    // [0,31]
  NvU32   reserved1                          : 9;    // padding
  NvU32   me_control_idx                     : 4;    // index in array of nvenc_h264_me_control_s to use for this slice
  NvU32   md_control_idx                     : 4;    // index in array of nvenc_h264_md_control_s to use for this slice
  NvU32   q_control_idx                      : 4;    // index in array of nvenc_h264_quant_control_s to use for this slice

  // int 4
  NvU32   limit_slice_top_boundary           : 1;    // limit ME to slice boundaries
  NvU32   limit_slice_bot_boundary           : 1;    // limit ME to slice boundaries
  NvU32   limit_slice_left_boundary          : 1;    // limit ME to slice boundaries
  NvU32   limit_slice_right_boundary         : 1;    // limit ME to slice boundaries
  NvU32   ROI_enable                         : 1;    // Enable region  of interest processing
  NvU32   ROI_me_control_idx                 : 4;
  NvU32   ROI_md_control_idx                 : 4;
  NvU32   ROI_q_control_idx                  : 4;
  NvU32   ROI_qp_delta                       : 8;    // QP delta in ROI relative to picture outside ROI
  NvU32   wp_control_idx                     : 4;    // index in array of nvenc_pred_weight_table_s to use for this slice
  NvU32   reserved2                          : 3;    // reserved for alignment

  // int 5
  NvU32   ROI_top_mbx                        : 8;    // top left mbx of the ROI
  NvU32   ROI_top_mby                        : 8;
  NvU32   ROI_bot_mbx                        : 8;    // bottom right mbx of the ROI
  NvU32   ROI_bot_mby                        : 8;

  // int 6: ref_pic_cmd_cfg
  nvenc_h264_refpic_cmd_s        ref_pic_cmd_cfg;

  // int 7-22: RefPicListReorder/Modification: Defined in Section 7.4.3.1
  nvenc_h264_ref_pic_reorder_s   ref_pic_list_reorder_cmd[2][8];     // size shall not exceed num_ref_idx_lx_active_minus1 + 1 (8+8=16)

  //int 23-30: MMCO: Defined in Section: 7.4.3.3
  nvenc_h264_mmco_s              mmco_cmd[8] ;

  // int 31
  NvU32   reserved3;

} nvenc_h264_slice_control_s;                               // 128 bytes


// Quarter-resolution modifier flags for first_pass_source_half_scaled
#define NVENC_QRES_FLAG_ENABLE          0x1U     // Set if quarter-resolution is enabled
#define NVENC_QRES_FLAG_DECIMATED       0x10U    // Set to indicate that subsampling was performed with decimation rather than low-pass
#define NVENC_QRES_FLAG_PASS2REFS       0x20U    // Set to indicate that 1st-pass uses subsampled 2nd pass references instead of self-generated references

// Lookahead macros for ab_beta(INFO1) and prev_act(INFO2)
#define NVENC_LOOKAHEAD_INFO1(Wp2Wi)        ((NvU8)(Wp2Wi))    // clamp((NvS32)(Wp*256.0/Wi), 0, 255) -> ratio of inter over intra complexity, 0=no lookahead
// Lookahead INFO2: (depth<<8)|(nextI)
// nextI: 1..255=distance to next scene change, 0=unknown or greater than lookahead depth;
// depth: 1..255=max lookahead depth (0=no lookahead)
#define NVENC_LOOKAHEAD_INFO2(nextI,depth)  (((NvU16)(nextI)) | (((NvU16)(depth))<<8))

// HRD conformance and misc RC flags
#define NVENC_RC_HRD_VCL                0x1U // vcl_cpb_size and vcl_bitrate
#define NVENC_RC_HRD_NAL                0x2U // nal_cpb_size and nal_bitrate
#define NVENC_RC_HRD_STRICTGOPTARGET    0x4U // Strict GOP target (minimizes GOP-to-GOP rate fluctuations)
#define NVENC_RC_FIXED_PBRATIO          0x8U // Disable dynamic B-frame rhopbi adjustments


 // needed only if picture level RC is enabled; as part of picture level RC, it will also perform HRD verification
 // The contents of this file is produced by the driver during sequence level operations and should not be modified.
typedef struct
{
  NvU8    hrd_type;                                  // nal (=2) and vcl (=1) type
  NvS8    QP[3];                                     // QP for 0:P picture, ,1:B picture 2; I picture
  NvS8    minQP[3];                                  // min QP for 0:P picture, ,1:B picture 2; I picture
  NvS8    maxQP[3];                                  // max QP for 0:P picture, ,1:B picture 2; I picture
  NvS8    maxQPD;                                    // max QP delta between two consecutive QP updates
  NvS8    baseQPD;                                   // initial QP delta between two consecutive QP updates
  NvU32   rhopbi[3];                                 // 23.8 unsigned fixed point quant ratios P/I, B/I, I/I(=1)

  NvU32   framerate;                                 // fps
  NvU32   buffersize;                                // total buffer size

  NvU32   nal_cpb_size;                              // size in bits
  NvU32   nal_bitrate;                               // rate in bps
  NvU32   vcl_cpb_size;                              // size in bits
  NvU32   vcl_bitrate;                               // rate in bps

  NvU32   gop_length;                                // I period, gop_length == 0xffffffff is used for infinite gop length
  NvU32   Np;                                        // 27.4 signed fixed (gop_length + num_b_frames -1)/(num_b_frames + 1)
  NvU32   Bmin;                                      // 23.8 unsigned fixed point min buffer level, updated by driver at sequence level only.
  NvU32   Ravg;                                      // 23.8 unsigned fixed point average rate, updated by driver at sequence level only.
  NvU32   R;                                         // 23.8 unsigned fixed point rate

  NvU8    ab_alpha;                                  // VBR/CBR: min/target quality level (1..51); rcmode4: weight of prev frame activity compared to part of the current picture
  NvU8    ab_beta;                                   // lookahead info1: ratio of inter over intra complexity (fix8); rcmode4: weight of current activity compared to neighbors
  NvU16   prev_act;                                  // lookahead info2: nextI, depth and valid flag; rcmode4: average activity level of the first reference (L0)

  NvU32   aqMode                             : 3;    // 0: disable adaptive quantization, 1: AQ mode fast, 2: AQ mode full
  NvU32   dump_aq_stats                      : 1;    // 1: dump aq stats
  NvU32   is_emphasis_level                  : 1;    // 1: enables emphasis level map, treats QP map as level map
  NvU32   reserve0                           : 3;    // Reserved for alignment
  NvU32   two_pass_rc                        : 8;    // 0: single pass rc , 1: first pass of 2 pass rc 2: second pass of 2 pass rc
  NvU32   rc_class                           : 8;    // reserved
  NvU32   first_pass_source_half_scaled      : 8;    // 0 : first pass on half resolution 1: first pass on full resolution (along with NVENC_QRES_FLAG_XXX)

  NvBool  single_frame_VBV;                          // 1: VBV buffer size is set to average frame size; 0: otherwise
  NvU8    iSizeRatioX;                               // ratio between I picture target size over average picture size numerator
  NvU8    iSizeRatioY;                               // ratio between I picture target size over average picture size denominator
  NvBool  ext_scene_change_flag;                     // Scene change flag set as hint by external preprocessing unit. 0: No scene change, 1: Current picture is first in scene
  NvU32   ext_intra_satd;                            // If non zero this represents the intra SATD for current picture computed by external preprocessing unit
  NvBool  ext_picture_rc_hints;                      // picture rc hints are set by external source. 0: no hints, 1: hints are avaialble for current picture
  NvS8    session_max_qp;                            // Current frame qp will never exceed this value when ext_picture_rc_hints is set
  NvU8    reserve[2];
} nvenc_h264_rc_s;                                   // 88 bytes


typedef struct
{
  NvS8    l0[8];                                     // reference picture list 0
  NvS8    l1[8];                                     // reference picture list 1
  NvS8    temp_dist_l0[8];                           // temporal distance of each ref pic in list 0 DiffPicOrderCnt( currPicOrField, pic0) )
  NvS8    temp_dist_l1[8];                           // temporal distance of each ref pic in list 1 DiffPicOrderCnt( currPicOrField, pic1) )
  NvS16   dist_scale_factor[8][8];                   // (h264 spec eq 8-203) array [refidx1][refidx0] signed 11 bit values
  NvU32   diff_pic_order_cnt_zero[2];                // This is a 2-dimensional array of booleans (1 bit flags) indicating that DiffPicOrderCnt(refidx1, refidx0) == 0
  NvU32   max_slice_size;                            // for use in slice_mode 1 only:  dynamic insertion at/before this boundary (#bytes)
  NvU32   max_byte_count_before_resid_zero;          // Maximum byte count before micrcode forces the residuals to zero.
  NvS32   delta_pic_order_cnt_bottom;                // direct value of syntax element to be put in every slice of the picture
  NvU16   frame_num;                                 // value for slice header syntax element
  NvU16   pic_order_cnt_lsb;                         // value of the corresponding syntax element
  NvU16   idr_pic_id;
  NvU16   colour_plane_id;
  NvU32   longTermFlag;                              // Bits 7:0 for pictures in RefPicList0 and bits 23:16 for pictures in RefPicList1
  NvU8    num_forced_slices_minus1;                  // number of forced slice boundary locations minus1(number of entries in h264_slice_control_s)
  NvU8    num_me_controls_minus1;                    // number of nvenc_h264_me_control_s entries in array minus1
  NvU8    num_md_controls_minus1;                    // number of nvenc_h264_md_control_s entries in array minus1
  NvU8    num_q_controls_minus1;                     // number of nvenc_h264_quant_control_s entries in array minus1
  NvU32   slice_control_offset;                      // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                     // to start of array of "nvenc_h264_slice_control_s" structures
  NvU32   me_control_offset;                         // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                     // to start of array of "nvenc_h264_me_control_s" structures
  NvU32   md_control_offset;                         // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                     // to start of array of "nvenc_h264_md_control_s" structures
  NvU32   q_control_offset;                          // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                     // to start of array of "nvenc_h264_quant_control_s" structures
  NvU32   hist_buf_size;                             // size in bytes of the buffer allocated for history
  NvU32   bitstream_buf_size;                        // size in bytes of the buffer allocated for bitstream slice/mb data
  NvU32   bitstream_start_pos;                       // start position in bitstream buffer where data should be written (byte offset)
  NvU32   pic_struct                         : 2;    // 0 = frame, 1 = top/first field, 2 = bot/second field
  NvU32   pic_type                           : 2;    // 0 = P, 1 = B, 2 = I, 3 = IDR
  NvU32   ref_pic_flag                       : 1;    // reference picture (0 = no, 1 = yes)
  NvU32   slice_mode                         : 1;    // 0 = dynamic slice insertion based on slice size
                                                     // 1 = static slice insertion based on slice control array
                                                     //     (insert slices as defined in array, if all array elements used
                                                     //      before end of picture is reached, start over with entry 0)
  NvU32   ipcm_rewind_enable                 : 1;
  NvU32   cabac_zero_word_enable             : 1;

  //mvc
  NvU32   base_view                          : 1;    // 1: current view component is a base view; 0: non-base view
  NvU32   priority_id                        : 6;    // priority identifier for the NAL unit
  NvU32   view_id                            : 10;   // view identifier for the NAL unit
  NvU32   temporal_id                        : 3;    // temporal identifier for the NAL unit
  NvU32   anchor_pic_flag                    : 1;    // equal to 1 specifies that the current access unit is an anchor access unit
  NvU32   inter_view_flag                    : 1;    // equal to 1 specifies that the current view component may be used for inter-view prediction by other view components in the current access unit
  NvU32   reserved1                          : 2;    // pad to next NvS32
  NvS32   cur_interview_ref_pic              : 8;    //current interview ref pic
  NvS32   prev_interview_ref_pic             : 8;    //previous interview ref pic to replace current interview ref pic when write the regs ME_MBC_REFLIST0_DPBLUT and ME_MBC_REFLIST1_DPBLUT
  NvU32   codec                              : 3;
  NvU32   e4byteStartCode                    : 1;    // if enable to 1, all the slices in a picture will use 4 byte start code 00000001(used in rtp mode), original is 000001
  NvU32   qpfifo                             : 1;    // 0 = Polling mode, 1 = Interrupt mode.
  NvU32   reserved3                          : 3;
  NvU32   intraRefreshCount                  : 8;    // Set to number of pictures over which intra refresh will happen. Set to 0 for non intra refresh picture
  NvU32   mpec_threshold;                            // collect mpec stats after threshold mbs
                                                     // when mpec_stat_on is enabled and mpec_threshold is not equal to picWidthInMbs,
                                                     // rc mode 0 will be used to collect stats
  NvU32   slice_stat_offset;                         // offset from start of top level "nvenc_stat_data_s" structure
                                                     // to start of array of "nvenc_slice_stat_s" structure
  NvU32   mpec_stat_offset;                          // offset from start of top level "nvenc_stat_data_s" structure
                                                     // to start of array of "nvenc_mpec_stat_s" structure
  NvU32   wp_control_offset;                         // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                     // to start of array of "nvenc_pred_weight_table_s" structure
                                                     // to start of array of "nvenc_mpec_stat_s" structure

  NvU32   num_wp_controls_minus1             : 8;    // number of nvenc_pred_weight_table_s entries in array minus 1
  NvU32   b_as_ref                           : 1;    // use b frame as reference
  NvU32   ofs_mode                           : 2;    // ofs mode. 0: OFF; 1: Optical Flow; 2: Stereo.
  NvU32   new_subframe                       : 1;
  NvU32   bit_depth_minus_8                  : 4;
  NvU32   stripID                            : 8;
  NvU32   strips_in_frame                    : 8;
  NvU16   slice_encoding_row_start;                  // start mb row for slice encoding
  NvU16   slice_encoding_row_num;                    // num of mb row for slice encoding

  NvU32   segment_enable                     : 1;    // used by ofs mode only for segment map enable flag, 0: (default) disable/ 1: enable
  NvU32   background_seg                     : 5;    // used by ofs mode only when segment map is enabled, to specify the segment id of background
  NvU32   input_sub_sampling                 : 1;    // used by PDMA 2x2 sub-sampling
  NvU32   output_sub_sampling                : 1;    // used by MPEB 2x2 sub-sampling output
  NvU32   segment_P2                         : 8;
  NvU32   segment_thr                        : 8;
  NvU32   accurate_skip_mv                   : 1;
  NvU32   intra_sel_4x4_enable               : 1;    // seletive intra4x4 , 0:disable(always do intra4x4 serach), 1:enable(enable or disable intra4x4 search based on pixel variance)
  NvU32   reserved5                          : 6;
  NvU32   chroma_skip_threshold_4x4         : 16;    // low 3 bits is the factional, to check if chroma residual is big in skip mb, can't simply set chroma residual to 0 even though luma has been decided to skip
  NvU32   intra_sel_4x4_threshold           : 16;    // disable intra4x4 search when intra_sel_4x4_enable=true and max variance of 8x8 blocks in current MB is less than intra_sel_4x4_threshold
  NvU32   aq_stat_offset;                            // to start of array of "nvenc_aq_stat_s" structure
  NvU32   act_stat_offset;                           // offset from start of "nvenc_rc_pic_stat_s" structure,
                                                     // in the RC stats buffer. This field points to the start
                                                     // of data for external (delta-)QP map.
} nvenc_h264_pic_control_s;                          // 272 bytes


// This is the top level config structure passed by driver to nvenc. It must be aligned
// to a 256B boundary. The first integer in this struct is a 'magic' number which driver
// must set to a specific value, and which the main Falcon app must check to see if it
// matches the expected value. This prevents running wrong combinations of traces/drvr
// code and falcon code. Encoding is as follows:
//  Bit 31..16 = 0xC4B7
//  Bit 15..8  = Version number
//  Bit 7..0   = Revision number
// This number changes whenever there is a change to the class. If the change is small and
// backward compatible, only the revision number is incremented. If the change is major or
// not backward compatible, the revision number is reset to zero and the version number
// incremented. Falcon app should check if the upper 24 bits match the expected value, and
// terminate with error code "NvC4B7EncErrorH264BadMagic" if there is a mismatch. The value
// for the current class is defined as:
#if defined (NV_NVENC_7_3)
#define NV_NVENC_DRV_MAGIC_VALUE  0xC7B70006U
#elif defined (NV_NVENC_7_2)
#define NV_NVENC_DRV_MAGIC_VALUE  0xC4B70006U
#elif defined(NV_NVENC_7_0)
#define NV_NVENC_DRV_MAGIC_VALUE  0xC5B70006U
#elif defined(NV_NVENC_6_6)
#define NV_NVENC_DRV_MAGIC_VALUE  0xB4B70006U
#elif defined(NV_NVENC_6_4)
#define NV_NVENC_DRV_MAGIC_VALUE  0xC3B70006U
#elif defined(NV_NVENC_6_2)
#define NV_NVENC_DRV_MAGIC_VALUE  0xC2B70006U
#elif defined(NV_NVENC_6_0)
#define NV_NVENC_DRV_MAGIC_VALUE  0xc1b70006U
#elif defined(NV_NVENC_5_0)
#define NV_NVENC_DRV_MAGIC_VALUE  0xd0b70006U
#elif defined(NV_NVENC_1_0)
#define NV_NVENC_DRV_MAGIC_VALUE  0xc0b70006U
#elif defined(NV_MSENC_2_0)
#define NV_MSENC_DRV_MAGIC_VALUE  0xa0b70006U
#else
#define NV_MSENC_DRV_MAGIC_VALUE  0x90b70006U
#endif

#define HIST_BLOCK_SIZE  192U

typedef struct
{
  // 256 aligned
  NvU32                      magic;                  // magic number, see text above              4 bytes
  nvenc_h264_surface_cfg_s   refpic_cfg;             // defines layout of reference pictures     32 bytes
  nvenc_h264_surface_cfg_s   input_cfg;              // defines layout of input pictures         32 bytes
  nvenc_h264_surface_cfg_s   outputpic_cfg;          // defines layout of reconstructed pictures 32 bytes
  nvenc_h264_sps_data_s      sps_data;               //                                           4 bytes
  nvenc_h264_pps_data_s      pps_data;               //                                           8 bytes
  nvenc_h264_rc_s            rate_control;           // Rate Control information                 88 bytes
  nvenc_h264_pic_control_s   pic_control;            //                                         272 bytes
  nvenc_h264_surface_cfg_s   half_scaled_outputpic_cfg; // defines layout of 2x2 subsampled reconstructed picture  32 bytes
  NvU32                      gpTimer_timeout_val;    // GPTimer cycle count set from driver       4 bytes
  NvU8                       reserved[4];            // Pad to 512 bytes                          4 bytes

} nvenc_h264_drv_pic_setup_s;                        // 512 bytes


// ****************************************************************************
// ****************************************************************************
// H265
// ****************************************************************************
// ****************************************************************************
typedef struct
{
  NvU16   frame_width_minus1;                        // frame width in pixels minus 1, range 0-4095
  NvU16   frame_height_minus1;                       // frame height in pixels minus 1, range 0-4095
  NvU16   sfc_pitch;                                 // pitch of luma plane
  NvU16   sfc_pitch_chroma;                          // pitch of chroma plane
  NvU16   sfc_trans_mode;                            // least 3 significient bits are used to stand for 8 modes, normal, xflip, yflip etc.
  NvU16   reserved2;                                 // pad to NvS32

  NvU32   luma_top_frm_offset;                       // offset of luma top field or frame in units of 256
  NvU32   luma_bot_offset;                           // offset of luma bottom field in units of 256. Not used if frame format.
  NvU32   chroma_top_frm_offset;                     // offset of chroma top field or frame, or offset of chroma U(Cb) plane in planar mode, both in units of 256
  NvU32   chroma_bot_offset;                         // offset of chroma bottom field, not used if frame format. Or offset of chroma V(Cr) plane in planar mode. Both in units of 256.
  NvU32   block_height                       : 7;    // BL mapping block height setting
  NvU32   tiled_16x16                        : 1;    // Surface is 16x16 tiled instead of BL mapped (must be 1 for refpics)
  NvU32   memory_mode                        : 2;    // 0: semi-planar, 1: planar
  NvU32   nv21_enable                        : 1;    // the surface format is yuv or yvu
  NvU32   input_bl_mode                      : 1;    // the input block linear mode: 0~gpu bl; 1~tegra bl; this is only for cmod and cmod/plugin
  NvU32   reserved                           : 20;   // pad to NvS32
} nvenc_h265_surface_cfg_s;                          // 32 bytes

typedef struct
{
  NvU32   chroma_format_idc                  : 2;    // 0=monochrome, 1=yuv4:2:0, 3=yuv4:4:4, 2=not supported
  NvU32   separate_colour_plane_flag         : 1;    //if this bit is enabled, the chroma_format_idc should be 3
  NvU32   log2_max_pic_order_cnt_lsb_minus4  : 4;
  NvU32   min_cu_size                        : 2;    //0-8x8, 1-16x16, 2-32x32, 3-64x64
  NvU32   max_cu_size                        : 2;    //0-8x8, 1-16x16, 2-32x32, 3-64x64
  NvU32   min_tu_size                        : 2;    //0-4x4, 1-8x8, 2-16x16, 3-32x32
  NvU32   max_tu_size                        : 2;    //0-4x4, 1-8x8, 2-16x16, 3-32x32
  NvU32   reserved0                          : 1;    // pad to full NvS16

  NvU32   max_transform_depth_inter          : 2;    // max_transform_hierarchy_depth_inter
  NvU32   max_transform_depth_intra          : 2;    // max_transform_hierarchy_depth_intra
  NvU32   pcm_enabled_flag                   : 1;    // if this bit is 1 , PCM data is present is the sequence
  NvU32   pcm_loop_filter_disable_flag       : 1;    // Disable loop filter and SAO for CU with pcm_flag to 1
  NvU32   long_term_ref_pics_present_flag    : 1;    // if set to 1 , it indicates long term pictures are used for inter prediction.
  NvU32   amp_enabled_flag                   : 1;    // if set  asymmetric motion partitions can be used for inter predictions.
  NvU32   strong_intra_smoothing_enabled_flag : 1;   // if set , bilinear interpolation is conditionally used filtering process for intra prediction.
  NvU32   num_short_term_ref_pic_sets        : 7;    // max value is 64
  NvU32   num_long_term_ref_pics_sps         : 5;
  NvU32   bit_depth_minus_8                  : 4;    // luma/chroma should have the same value
  NvU32   sample_adaptive_offset_enabled_flag : 1;   // set 1 specifies that the sample adaptive offset process is applied to the reconstructed picture after the deblocking filter process
  NvU32   sps_temporal_mvp_enabled_flag      : 1;    // if set, slice_temporal_mvp_enabled_flag is present in slice header
  NvU32   reserved1                          : 21;
} nvenc_h265_sps_data_s;                             // 8 bytes

typedef struct
{
  NvU32   pic_param_set_id                            : 8;    // picture parameter set identification
  NvU32   num_ref_idx_l0_active_minus1                : 5;    // number of currently active reference pictures in list 0 minus 1, range 0..30
  NvU32   num_ref_idx_l1_active_minus1                : 5;    // number of currently active reference pictures in list 1 minus 1, range 0..30
  NvU32   constrained_intra_pred_flag                 : 1;    // if set, intra prediction can only use pixels from macroblocks that are also intra coded
  NvU32   transquant_bypass_enabled_flag              : 1;    // if set, transform ,scaling and inloop filtering is bypassed.
  NvU32   transform_skip_enabled_flag                 : 1;    // if set , it enables transform bypass during residual coding.
  NvU32   deblocking_filter_override_enabled_flag     : 1;    // it set, the deblocking_filter_override_flag is encoded in slice segment header
  NvU32   pps_loop_filter_across_slices_enabled_flag  : 1;    // if set, it enables loop filter across slice boundary.
  NvU32   cu_qp_delta_enabled_flag                    : 1;    // if set , it enables cu_qp_delta_abs is encoded at coding unit layer.
  NvU32   lists_modification_present_flag             : 1;    // if set , it indicates reference picture list modification command might be present is slice segment header.
  NvU32   output_flag_present_flag                    : 1;    // if set, pic_output syntax element is added to slice syntax header
  NvU32   cabac_init_present_flag                     : 1;    // if set, cabac_init_flag is present in slice segment header.
  NvU32   pps_slice_chroma_qp_offsets_present         : 1;    // if set to 1, cb_qp_offset and cr_qp_offset is signalled in the slice segment header.
  NvU32   weighted_pred_flag                          : 1;    // weighted_pred_flag equal to 1 specifies that weighted prediction is applied to P slices
  NvU32   weighted_bipred_flag                        : 1;    // weighted_bipred_flag equal to 1 specifies that weighted prediction is applied to B slices
  NvU32   reserved                                    : 2;

  NvS32   pic_init_qp_minus26                : 6;    // initial QP value, range -26..+25. -38...+25 for 10bit,  -74...+25 for 16bits.
  NvS32   pps_cb_qp_offset                   : 5;
  NvS32   pps_cr_qp_offset                   : 5;
  NvU32   dependent_slice_segments_enabled_flag : 1;
  NvU32   sign_data_hiding_enabled_flag      : 1;
  NvU32   diff_cu_qp_delta_depth             : 2;
  NvU32   log2_parallel_merge_level_minus2   : 3;
  NvU32   num_extra_slice_header_bits        : 3;

  NvS32   reserved1                          : 6;
} nvenc_h265_pps_data_s;                             // 8 bytes

 // needed only if picture level RC is enabled; as part of picture level RC, it will also perform HRD verification
 // The contents of this file is produced by the driver during sequence level operations and should not be modified.
typedef struct
{
  NvU8    hrd_type;                                  // nal (=2) and vcl (=1) type
  NvS8    QP[3];                                     // QP for 0:P picture, ,1:B picture 2; I picture
  NvS8    minQP[3];                                  // min QP for 0:P picture, ,1:B picture 2; I picture
  NvS8    maxQP[3];                                  // max QP for 0:P picture, ,1:B picture 2; I picture
  NvS8    maxQPD;                                    // max QP delta between two consecutive QP updates
  NvS8    baseQPD;                                   // initial QP delta between two consecutive QP updates
  NvU32   rhopbi[3];                                 // 23.8 unsigned fixed point quant ratios P/I, B/I, I/I(=1)

  NvU32   framerate;                                 // fps
  NvU32   buffersize;                                // total buffer size

  NvU32   nal_cpb_size;                              // size in bits
  NvU32   nal_bitrate;                               // rate in bps
  NvU32   vcl_cpb_size;                              // size in bits
  NvU32   vcl_bitrate;                               // rate in bps

  NvU32   gop_length;                                // I period, gop_length == 0xffffffff is used for infinite gop length
  NvU32   Np;                                        // 27.4 unsigned fixed (gop_length + num_b_frames -1)/(num_b_frames + 1)
  NvU32   Bmin;                                      // 23.8 unsigned fixed point min buffer level, updated by driver at sequence level only.
  NvU32   Ravg;                                      // 23.8 unsigned fixed point average rate, updated by driver at sequence level only.
  NvU32   R;                                         // 23.8 unsigned fixed point rate

  NvU8    ab_alpha;                                  // VBR/CBR: min/target quality level (1..51); rcmode4: weight of prev frame activity compared to part of the current picture
  NvU8    ab_beta;                                   // lookahead info1: ratio of inter over intra complexity (fix8); rcmode4: weight of current activity compared to neighbors
  NvU16   prev_act;                                  // lookahead info2: NVENC_LOOKAHEAD_INFO2; rcmode4: average activity level of the first reference (L0)

  NvU8    reserved0;
  NvBool  single_frame_vbv;                          // 1: VBV buffer size is set to average frame size; 0: otherwise
  NvU8    two_pass_rc;                               // 0: single pass rc , 1: first pass of 2 pass rc 2: second pass of 2 pass rc
  NvU8    rc_class;                                  // reserved
  NvU8    first_pass_source_half_scaled;             // 0 : first pass on half resolution 1: first pass on full resolution (along with NVENC_QRES_FLAG_XXX)
  NvU8    i_size_ratio_x;                            // ratio between I picture target size over average picture size numerator
  NvU8    i_size_ratio_y;                            // ratio between I picture target size over average picture size denominator
  NvU8    aqMode;                                    // 0: disable adaptive quantization 1: AQ mode fast (unimplemented) 2: AQ mode full (unimplemented) 3: External qp delta map
  NvU8    dump_aq_stats;                             // 1: dump aq stats
  NvBool  ext_scene_change_flag;                     // Scene change flag set as hint by external preprocessing unit. 0: No scene change, 1: Current picture is first in scene
  NvBool  ext_picture_rc_hints;                      // picture rc hints are set by external source. 0: no hints, 1: hints are avaialble for current picture
  NvS8    session_max_qp;                            // Current frame qp will never exceed this value when ext_picture_rc_hints is set
  NvU32   ext_intra_satd;                            // If non zero this represents the intra SATD for current picture computed by external preprocessing unit
  NvU8    reserved2[8];
} nvenc_h265_rc_s;                                   // 96 bytes

typedef struct
{
  NvU32   inter_ref_pic_set_prediction_flag  : 1;
  NvS32   delta_idx_minus1                   : 6;
  NvU32   delta_rps_sign                     : 1;
  NvS32   abs_delta_rps_minus1               : 16;
  NvS32   reserved                           : 8;
  NvU16   used_by_curr_pic_flag;
  NvU16   use_delta_flag;
  NvS16   num_negative_pics;
  NvS16   num_positive_pics;
  NvS16   delta_poc_s0_minus1[16];
  NvU16   used_by_curr_pic_s0_flag;
  NvS16   delta_poc_s1_minus1[16];
  NvU16   used_by_curr_pic_s1_flag;
}nvenc_h265_strps_s;                   // 80bytes multiple of 4

typedef struct
{
  NvS32   num_long_term_sps                  : 5;
  NvS32   num_long_term_pics                 : 5;
  NvS32   reserved0                          : 22;
  NvU8    lt_idx_sps[16];
  NvU16   poc_lsb_lt[16];
  NvU16   used_by_curr_pic_lt_flag;
  NvU16   delta_poc_msb_present_flag;
  NvS32   delta_poc_msb_cycle_lt[16];
}nvenc_h265_ltrps_s;                  // 120bytes multiple of 4

typedef struct
{
  NvS32   NumPocTotalCurr                    : 5;
  NvU32   ref_pic_list_mod_flag_l0           : 1;
  NvU32   ref_pic_list_mod_flag_l1           : 1;
  NvS32   reserved0                          : 25;
  NvU8    list_entry_l0[16];
  NvU8    list_entry_l1[16];
}nvenc_h265_ref_pic_list_modification_s; //36bytes multiple of 4

typedef struct
{
  NvU32   vps_poc_lsb_not_present_flag           : 1;
  NvU32   vps_default_ref_layers_active_flag     : 1;
  NvU32   vps_max_one_active_ref_layer_flag      : 1;
  NvU32   vps_num_direct_ref_layers              : 6;
  NvU32   slh_discardable_flag                   : 1;    // mv_hevc, equal to 1 specifies that the coded picture is not used as a reference picture
  NvU32   slh_cross_layer_bla_flag               : 1;    // mv_hevc
  NvU32   slh_num_active_ref_layer_pics          : 6;    // active ref layer pics of current picture
  NvU32   slh_inter_layer_pred_enabled_flag      : 1;
  NvU32   slh_num_inter_layer_ref_pics_minus1    : 6;
  NvU32   reserved0                              : 8;
  NvU8    slh_inter_layer_pred_layer_idc[8];             //NumPicTotalCurr <= 8
}nvenc_mv_hevc_cfg_s;                   //12bytes multiple of 4

typedef struct
{
  NvS8    l0[8];                                     // reference picture list 0
  NvS8    l1[8];                                     // reference picture list 1
  NvS8    temp_dist_l0[8];                           // temporal distance of each ref pic in list 0 DiffPicOrderCnt( currPicOrField, pic0) )
  NvS8    temp_dist_l1[8];                           // temporal distance of each ref pic in list 1 DiffPicOrderCnt( currPicOrField, pic1) )
  NvS16   dist_scale_factor[8][8];                   // (h264 spec eq 8-203) array [refidx1][refidx0] signed 11 bit values
  NvU32   diff_pic_order_cnt_zero[2];                // This is a 2-dimensional array of booleans (1 bit flags) indicating that DiffPicOrderCnt(refidx1, refidx0) == 0
  NvU32   max_slice_size;                            // for use in slice_mode 1 only:  dynamic insertion at/before this boundary (#bytes)
  NvU32   max_byte_count_before_resid_zero;          // Maximum byte count before micrcode forces the residuals to zero.

  NvU32   no_output_of_prior_pics_flag       : 1;    // used to modify the DPB after decoding a IDR or BLA picture
  NvU32   pic_output_flag                    : 1;    // affects the decoded picture output and removal processes as specified in Annex C
  NvU32   mode_decision_only_flag            : 1;    // only output h265 mode and mocomp result, no recon and bitstream output
  NvU32   cu_early_terminatiom_enable        : 1;    // 0: disable CU early termination, 1: enable CU early termination
  NvU32   input_sub_sampling                 : 1;    // used by PDMA 2x2 sub-sampling
  NvU32   output_sub_sampling                : 1;    // used by MPEB 2x2 sub-sampling output
  NvU32   reserved0                          : 10;
  NvU32   pic_order_cnt_lsb                  : 16;   // value of the corresponding syntax element

  NvU8    num_forced_slices_minus1;                  // number of forced slice boundary locations minus1(number of entries in h264_slice_control_s)
  NvU8    num_me_controls_minus1;                    // number of nvenc_h264_me_control_s entries in array minus1
  NvU8    num_md_controls_minus1;                    // number of nvenc_h264_md_control_s entries in array minus1
  NvU8    num_q_controls_minus1;                     // number of nvenc_h264_quant_control_s entries in array minus1
  NvU32   slice_control_offset;                      // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                     // to start of array of "nvenc_h264_slice_control_s" structures
  NvU32   me_control_offset;                         // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                     // to start of array of "nvenc_h264_me_control_s" structures
  NvU32   md_control_offset;                         // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                     // to start of array of "nvenc_h264_md_control_s" structures
  NvU32   q_control_offset;                          // offset from start of top level "nvenc_h264_drv_pic_setup_s" structure
                                                     // to start of array of "nvenc_h264_quant_control_s" structures
  NvU32   hist_buf_size;                             // size in bytes of the buffer allocated for history
  NvU32   bitstream_buf_size;                        // size in bytes of the buffer allocated for bitstream slice/mb data
  NvU32   bitstream_start_pos;                       // start position in bitstream buffer where data should be written (byte offset)
  NvU32   pic_type                           : 2;    // 0 = P, 1 = B, 2 = I, 3 = IDR
  NvU32   ref_pic_flag                       : 1;    // reference picture (0 = no, 1 = yes):
  NvU32   slice_mode                         : 1;    // 0 = dynamic slice insertion based on slice size
                                                     // 1 = static slice insertion based on slice control array
                                                     //     (insert slices as defined in array, if all array elements used
                                                     //      before end of picture is reached, start over with entry 0)
  NvU32   cabac_zero_word_enable             : 1;
  NvU32   codec                              : 3;
  NvU32   ceil_log2_pic_size_in_ctbs         : 6;    // ceillog2(picSizeInCtbs) for HW needs too much area/time for 16kx16k case
  NvU32   sao_luma_mode                      : 5;
  NvU32   sao_chroma_mode                    : 5;
  NvU32   reserved1                          : 8;

  NvU32   mpec_threshold;                            // collect mpec stats after threshold mbs
                                                     // when mpec_stat_on is enabled and mpec_threshold is not equal to picWidthInMbs,
                                                     // rc mode 0 will be used to collect stats
  NvU32   slice_stat_offset;                         // offset from start of top level "nvenc_h265_stat_data_s" structure
                                                     // to start of array of "nvenc_h265_slice_stat_s" structure
  NvU32   mpec_stat_offset;                          // offset from start of top level "nvenc_h265_stat_data_s" structure
                                                     // to start of array of "nvenc_h265_mpec_stat_s" structure
  NvU32   aq_stat_offset;                            // to start of array of "nvenc_h265_aq_stat_s" structure
  NvU32   short_term_ref_pic_set_sps_flag    : 1;    // if set to 1 ,the short term RPS is signalled using SPS, if set to 0 the short term RPS is encoded in the current slice header
  NvU32   short_term_ref_pic_set_idx         : 6;    // used to signal index into the list of the short term RPS in the SPS.
  NvU32   e4byteStartCode                    : 1;    // if enable to 1, all the slices in a picture will use 4 byte start code 00000001(used in rtp mode), original is 000001
  NvU32   nal_unit_type                      : 6;    // specifies the type of RBSP data structure contained in the NAL unit
  NvU32   nuh_layer_id                       : 6;    // reserved and shall be equal to 0.
  NvU32   nuh_temporal_id_plus1              : 3;    // specifies the temporal_ip_plus1 of RBSP data structure contained in the NAL unit
  NvU32   qpfifo                             : 1;    // 0 = Polling mode, 1 = Interrupt mode.
  NvU32   reserved2                          : 8;

  nvenc_h265_strps_s    short_term_rps;              // if short_term_ref_pic_set_sps_flag is set to 0, the nvenc_h265_strps_s  structure is signalled in the slice  segment segment  header.
  nvenc_h265_ltrps_s    long_term_rps;               // if long_term_ref_pics_present_flag is set to 1 in SPS , nvenc_h265_ltrps_s structure is signalled in the slice segment segment header
  nvenc_h265_ref_pic_list_modification_s    ref_pic_list_modification; // if lists_modification_present_flag is set to 1, then nvenc_h265_ref_pic_list_modification_s  structure is encoded in slice segment header.
  NvU32   rc_pass2_offset;

  NvU32   wp_control_offset;                         // offset from start of top level "nvenc_h265_drv_pic_setup_s" structure
                                                     // to start of array of "nvenc_pred_weight_table_s" structure

  NvU8    num_wp_controls_minus1;                    // number of nvenc_pred_weight_table_s entries in array minus 1
  NvU8    stripID;
  NvU8    strips_in_frame;
  NvU8    slice_idx_start;
  NvU16   slice_encoding_row_start;                  // start ctb row for slice encoding
  NvU16   slice_encoding_row_num;                    // num of ctb row for slice encoding

  nvenc_mv_hevc_cfg_s mv_hevc_cfg;
  NvU32   chroma_skip_threshold_4x4          : 16;   // low 3 bits is the factional, to check if chroma residual is big in skip mb, can't simply set chroma residual to 0 even though luma has been decided to skip
  NvU32   longTermFlag                       : 16;   // Bits 7:0 for pictures in RefPicList0 and bits 15:8 for pictures in RefPicList1
  NvS8    poc_entry_l0[8];                           // ref pic list to ref pic entry map
  NvS8    poc_entry_l1[8];                           // ref pic list to ref pic entry map
  NvU32   same_poc[2];                               // set to 1 when the poc of cur ref pic and neighbor ref pic are same, otherwise set it to 0.
  NvU32 act_stat_offset;                             // offset from start of "nvenc_rc_pic_stat_s" structure, points to data for external QP map
} nvenc_h265_pic_control_s;                          // 532 bytes

typedef struct
{
  // 256 aligned
  NvU32                      magic;                  // magic number, see text above             //  4 bytes
  nvenc_h265_surface_cfg_s   refpic_cfg;             // defines layout of reference pictures     // 32 bytes
  nvenc_h265_surface_cfg_s   input_cfg;              // defines layout of input pictures         // 32 bytes
  nvenc_h265_surface_cfg_s   outputpic_cfg;          // defines layout of reconstructed pictures // 32 bytes
  nvenc_h265_sps_data_s      sps_data;                                                           //  8 bytes
  nvenc_h265_pps_data_s      pps_data;                                                           //  8 bytes
  nvenc_h265_rc_s            rate_control;           // Rate Control information                 // 96 bytes
  nvenc_h265_pic_control_s   pic_control;                                                        //532 bytes
  nvenc_h265_surface_cfg_s   half_scaled_outputpic_cfg; // defines layout of 2x2 subsampled reconstructed picture // 32 bytes
  NvU32                      gpTimer_timeout_val;    // GPTimer cycle count set from driver       4 bytes
  NvU8                       reserved[244];          // Pad to 1024 bytes

} nvenc_h265_drv_pic_setup_s;

typedef struct
{
  NvU16   intra_refresh_cnt;                         // distance between forced-intra MBs in slice; 0 means OFF
  NvU16   intra_refresh_offs;                        // first forced-intra MB in slice

  NvU32   h265_intra_luma4x4_mode_enable;            //bitmask indicate which intra luma 4x4 modes to enable in ip1 intra search
  NvU32   h265_intra_luma8x8_mode_enable;            //bitmask indicate which intra luma 8x8 modes to enable in ip1 intra search
  NvU32   h265_intra_luma16x16_mode_enable;          //bitmask indicate which intra luma 16x16 modes to enable in ip1 intra search
  NvU32   h265_intra_luma32x32_mode_enable;          //bitmask indicate which intra luma 32x32 modes to enable in ip1 intra search
  NvU32   h265_intra_luma64x64_mode_enable;          //bitmask indicate which intra luma 64x64 modes to enable in ip1 intra search
  NvU32   h265_intra_luma_mode_enable_leftbits : 30; //bitmask indicate intra luma left modes(32~34) from ip1 luma4x4 to ip2 luma64x64.
  NvU32   reserved0                          : 2;


  NvU32   cu16x16_l0_part_2Nx2N_enable       : 1;    // enable L0 prediction for 2Nx2N
  NvU32   cu16x16_l0_part_2NxN_enable        : 1;    // enable L0 prediction for 2NxN
  NvU32   cu16x16_l0_part_Nx2N_enable        : 1;    // enable L0 prediction for Nx2N
  NvU32   cu16x16_l0_part_NxN_enable         : 1;    // enable L0 prediction for NxN
  NvU32   cu16x16_l0_part_2NxnU_enable       : 1;    // enable L0 prediction for 2NxnU
  NvU32   cu16x16_l0_part_2NxnD_enable       : 1;    // enable L0 prediction for 2NxnD
  NvU32   cu16x16_l0_part_nLx2N_enable       : 1;    // enable L0 prediction for nLx2N
  NvU32   cu16x16_l0_part_nRx2N_enable       : 1;    // enable L0 prediction for nRx2N

  NvU32   cu32x32_l0_part_2Nx2N_enable       : 1;    // enable L0 prediction for 2Nx2N
  NvU32   cu32x32_l0_part_2NxN_enable        : 1;    // enable L0 prediction for 2NxN
  NvU32   cu32x32_l0_part_Nx2N_enable        : 1;    // enable L0 prediction for Nx2N
  NvU32   cu32x32_l0_part_NxN_enable         : 1;    // enable L0 prediction for NxN
  NvU32   cu32x32_l0_part_2NxnU_enable       : 1;    // enable L0 prediction for 2NxnU
  NvU32   cu32x32_l0_part_2NxnD_enable       : 1;    // enable L0 prediction for 2NxnD
  NvU32   cu32x32_l0_part_nLx2N_enable       : 1;    // enable L0 prediction for nLx2N
  NvU32   cu32x32_l0_part_nRx2N_enable       : 1;    // enable L0 prediction for nRx2N

  NvU32   cu64x64_l0_part_2Nx2N_enable       : 1;    // enable L0 prediction for 2Nx2N
  NvU32   cu64x64_l0_part_2NxN_enable        : 1;    // enable L0 prediction for 2NxN
  NvU32   cu64x64_l0_part_Nx2N_enable        : 1;    // enable L0 prediction for Nx2N
  NvU32   cu64x64_l0_part_NxN_enable         : 1;    // enable L0 prediction for NxN
  NvU32   cu64x64_l0_part_2NxnU_enable       : 1;    // enable L0 prediction for 2NxnU
  NvU32   cu64x64_l0_part_2NxnD_enable       : 1;    // enable L0 prediction for 2NxnD
  NvU32   cu64x64_l0_part_nLx2N_enable       : 1;    // enable L0 prediction for nLx2N
  NvU32   cu64x64_l0_part_nRx2N_enable       : 1;    // enable L0 prediction for nRx2N

  NvU32   intra_mode_4x4_enable              : 1;    // enable intra prediction in mdp for 4x4
  NvU32   intra_mode_8x8_enable              : 1;    // enable intra prediction in mdp for 8x8
  NvU32   intra_mode_16x16_enable            : 1;    // enable intra prediction in mdp for 16x16
  NvU32   intra_mode_32x32_enable            : 1;    // enable intra prediction in mdp for 32x32
  NvU32   intra_mode_64x64_enable            : 1;    // enable intra prediction in mdp for 32x32
  NvU32   priority_ipred_type_ip1            : 3;    // which intra type search first for early termination, must be a subset of ip_search_mode. reserved, not implemented in HW

  NvU32   h265_intra_chroma_mode_enable      : 5;    //bitmask indicate which intra_chroma_pred_mode to enable
  NvU32   inter_penalty_factor_for_ip1       : 6;    // early teminate ip1 intra search if intra cost is larger than inter_cost + ((inter_cost * factor) >> 8)
  NvU32   tusize_4x4_enable                  : 1;    //enable 4x4 transform
  NvU32   tusize_8x8_enable                  : 1;    //enable 8x8 transform
  NvU32   tusize_16x16_enable                : 1;    //enable 16x16 transform
  NvU32   tusize_32x32_enable                : 1;    //enable 32x32 transform
  NvU32   intra_tusize_decision              : 1;    //enable intra tu size decision, reserved, not implemented in HW
  NvU32   pskip_enable                       : 1;    //enable p_skip encoding
  NvU32   tu_search_basedon_pusize           : 1;    //enable tu search based on pu size
  NvU32   reserved1                          : 14;   // reserved for future special prediction extensions

  NvS16   bias_part_2Nx2N;                           //bias for 2Nx2N PU
  NvS16   bias_part_2NxN;                            //bias for 2NxN PU
  NvS16   bias_part_Nx2N;                            //bias for Nx2N PU
  NvS16   bias_part_NxN;                             //bias for NxN PU
  NvS16   bias_part_2NxnU;                           //bias for 2NxnU PU
  NvS16   bias_part_2NxnD;                           //bias for 2NxnD PU
  NvS16   bias_part_nLx2N;                           //bias for nLx2N PU
  NvS16   bias_part_nRxN;                            //bias for nRxN PU

  NvS16   bias_cusize_8x8;                           //bias for 8x8 CU
  NvS16   bias_cusize_16x16;                         //bias for 16x16 CU
  NvS16   bias_cusize_32x32;                         //bias for 32x32 CU
  NvS16   bias_cusize_64x64;                         //bias for 64x64 CU
  NvS16   bias_pskip;                                // bias for pskip       (subtracts from pskip cost)

  NvS16   bias_intra_over_inter;                     // bias for any intra   (subtracts from intra cost)
  NvS16   bias_intra_most_prob;                      // bias for intra most probable mode
  NvS16   bias_qpp;
  NvS16   bias_transform_skip;                       // bias for transform skip mode

  NvU16   mv_cost_bias;                              // bias applied to internal motion vector cost calculation
  NvU16   pskip_esc_threshold;                       // if pskip cost is below this, disable full search & attempt pskip encode

  NvU16   tempmv_wt_spread_threshold;                // The threshold for the sum of differences between 8x8s and 16x16 mvs. Used for temporal mv weight
  NvU32   tempmv_wt_distort_threshold        : 16;   // The threshold for the cost of the 16x16 mv. Used for temporal mv weight
  NvU32   mv_cost_predictor_control          : 1;    // 0=use 16x16 predictor only; 1=use accurate predictor (slower but better)
  NvU32   mv_cost_enable                     : 1;    // enable mv cost calculations in mode decision
  NvU32   chroma_eval_mode                   : 1;    // for intra chroma search 0 = U and V; 1 = U only
  NvU32   ip_search_mode                     : 4;    // which intra sizes to search before making IP decision b0=4x4,b1=8x8,b2=16x16,b3=32x32
  NvU32   multiply_bias_with_lambda          : 1;    // setting this to 1 will result in bias being multiplied by lambda
  NvU32   force_ipcm                         : 1;    // 0=normal encode; 1=force mpeb/mpec encode as IPCM
  NvU32   early_termination_ip1              : 1;    // enable IP1 early termination
  NvU32   tu_search_num_cu16                 : 3;    // tu search part num in cu16
  NvU32   tu_search_num_cu32                 : 3;    // tu search part num in cu32
  NvU32   early_termination_mode_ip1;                //enable intra mode search for early termination, 32 bits for the 35 modes other than H, V and DC, these 3 modes enabled as default
  NvS16   bias_tu_4x4;                               // bias for TU 4x4
  NvS16   bias_tu_8x8;                               // bias for TU 8x8
  NvS16   bias_tu_16x16;                             // bias for TU 16x16
  NvS16   bias_tu_32x32;                             // bias for TU 32x32
  NvU32   num_1div8_lambda_ssd               : 7;    // number of 1/8 lambda ssd for lambda_coef delta. lambda_coef = (1+num_1div8_lambda_ssd*1/8)*lambda_ssd
  NvU32   skip_evaluate_enable_cu8           : 1;    // 0: not check skip ssd cost; 1, evaluate skip ssd cost
  NvU32   skip_evaluate_enable_cu16          : 1;    // 0: not check skip ssd cost; 1, evaluate skip ssd cost
  NvU32   skip_evaluate_enable_cu32          : 1;    // 0: not check skip ssd cost; 1, evaluate skip ssd cost
  NvU32   skip_evaluate_enable_cu64          : 1;    // 0: not check skip ssd cost; 1, evaluate skip ssd cost
  NvU32   reserved2                          : 1;
  NvU32   intra_ssd_cnt_4x4                  : 4;    // SSD cost evaluation between N top intra modes selected by satd
  NvU32   intra_ssd_cnt_8x8                  : 4;    // SSD cost evaluation between N top intra modes selected by satd
  NvU32   intra_ssd_cnt_16x16                : 4;    // SSD cost evaluation between N top intra modes selected by satd
  NvU32   intra_ssd_cnt_32x32                : 4;    // SSD cost evaluation between N top intra modes selected by satd
  NvU32   rdo_level                          : 3;    // 0:cu size decided by satd in P frame  1:cu size decided by ssd
  NvU32   reserved3                          : 1;    // pad to NvS16

  NvS16   bias_skip;                                 // bias for skip mv's ssd (subtracts from skip mv's ssd cost)
  NvS16   bias_intra_32x32;                          // bias for intra 32x32 (subtracts from intra 32x32 cost)
  NvS16   bias_intra_16x16;                          // bias for intra 16x16 (subtracts from intra 16x16 cost)
  NvS16   bias_intra_8x8;                            // bias for intra 8x8   (subtracts from intra 8x8 cost)
  NvS16   bias_intra_4x4;                            // bias for intra 4x4   (subtracts from intra 4x4 cost)
  NvU8    reserved[22];
} nvenc_h265_md_control_s;                           // 128 bytes

typedef struct
{
  NvU32   num_ctus                          : 19;   // number of ctus in this slice
  NvU32   qp_avr                            : 8;    // 6bit value used by hw. Other bits can be used for rounding.
  NvU32   reserved0                         : 5;

  NvU32   dependent_slice_segment_flag      : 1;    // slice segment header syntax element present or not
  NvU32   slice_type                        : 2;    // 0 = B, 1 = P, 2 = I
  NvU32   five_minus_max_num_merge_cand     : 3;    // specifies the maximum number of merging MVP candidates supported
  NvU32   cabac_init_flag                   : 1;
  NvU32   deblocking_filter_override_flag   : 1;    // specifies the deblocking control params is preset in slice header.
  NvU32   slice_deblocking_filter_disabled_flag            : 1;    // if set to 1 , it disables deblcoking for the current slice.
                                                                          // If deblocking_filter_override_flag is 0, this is set to pps_deblocking_filter_disabled_flag
  NvU32   slice_loop_filter_across_slices_enabled_flag     : 1;    // if set to 1, it enables loop filter accross slice boundary.
  NvU32   qp_slice_min                      : 8;    // min slice qp value
  NvU32   qp_slice_max                      : 8;    // max slice qp value
  NvU32   force_intra                       : 1;
  NvU32   slice_sao_luma_flag               : 1;    // equal to 1 specifies that SAO is enabled for the luma component in the current slice
  NvU32   slice_sao_chroma_flag             : 1;    // equal to 1 specifies that SAO is enabled for the chroma component in the current slice
  NvU32   reserved1                         : 3;

  NvS32   slice_beta_offset_div2            : 4;    // if deblocking_filter_override_flag = 1 , then slice_beta_offset_div2 is signalled in slice segment header
  NvS32   slice_tc_offset_div2              : 4;    // if deblocking_filter_override_flag = 1 , then slice_tc_offset_div2 is signalled in slice segment header.
  NvS32   slice_cb_qp_offset                : 5;    // offset added to pps_cb_qp_offset. It shall be in the range -12 to +12.
  NvS32   slice_cr_qp_offset                : 5;    // offset added to pps_cr_qp_offset. It shall be in the range -12 to +12.
  NvU32   slice_temporal_mvp_enabled_flag   : 1;    // set 1, temporal motion vector predictors may be used
  NvU32   collocated_from_l0_flag           : 1;    // set 1, the collocated picture for tmvp is derived from ref. pic. list 0
  NvU32   collocated_ref_idx                : 5;    // specifies ref. index of the collocated picture for tmvp
  NvS32   reserved2                         : 7;

  NvU32   num_ref_idx_active_override_flag  : 1;
  NvU32   num_ref_idx_l0_active_minus1      : 5;    // [0.31]
  NvU32   num_ref_idx_l1_active_minus1      : 5;    // [0,31]
  NvU32   me_control_idx                    : 4;    // index in array of nvenc_h264_me_control_s to use for this slice
  NvU32   md_control_idx                    : 4;    // index in array of nvenc_h264_md_control_s to use for this slice
  NvU32   q_control_idx                     : 4;    // index in array of nvenc_h264_quant_control_s to use for this slice
  NvU32   wp_control_idx                    : 4;    // index in array of nvenc_pred_weight_table_s to use for this slice
  NvU32   reserved3                         : 5;

  NvU32   limit_slice_top_boundary          : 1;    // limit ME to slice boundaries
  NvU32   limit_slice_bot_boundary          : 1;    // limit ME to slice boundaries
  NvU32   limit_slice_left_boundary         : 1;    // limit ME to slice boundaries
  NvU32   limit_slice_right_boundary        : 1;    // limit ME to slice boundaries
  NvU32   ROI_enable                        : 1;    // Enable region  of interest processing
  NvU32   ROI_me_control_idx                : 4;
  NvU32   ROI_md_control_idx                : 4;
  NvU32   ROI_q_control_idx                 : 4;
  NvU32   ROI_qp_delta                      : 8;    // QP delta in ROI relative to picture outside ROI
  NvU32   reserved4                         : 7;    // reserved for alignment

  NvU32   ROI_top_mbx                       : 16;   // top left mbx of the ROI
  NvU32   ROI_top_mby                       : 16;   // top left mby of the ROI
  NvU32   ROI_bot_mbx                       : 16;   // bottom right mbx of the ROI
  NvU32   ROI_bot_mby                       : 16;   // bottom right mby of the ROI

  NvU8    reserved5[36];                            // padding to 64 bytes
} nvenc_h265_slice_control_s;                       // 64 bytes

// h265 not support QPP, reserved it but not used
typedef struct
{
  // QPP has not been defined for H265
  NvU16   qpp_run_vector_4x4;                        // cost values for 4x4 transform (16 bit total vector)
  NvU16   qpp_run_vector_8x8[3];                     // cost values for 8x8 transform (48 bit total vector)
                                                     // 2 bits each for first 12 coefs, then 1 bit each for next 24
  NvU32   qpp_luma8x8_cost                   : 8;    // luma 8x8 cost threshold, 0 = throw out all coefs, range 0-15
  NvU32   qpp_luma16x16_cost                 : 8;    // luma 16x16 cost threshold, 0 = throw out all coefs, range 0-15
  NvU32   qpp_chroma_cost                    : 8;    // chroma cost threshold, 0 = throw out all coefs, range 0-15
  NvU32   qpp_mode                           : 2;    // 0 = OFF, 1 = 8x8, 2 = 16x16_8x8
  NvU32   reserved0                          : 6;    // padding

  NvU32   reserved1;                                 // padding to 16 bytes
} nvenc_h265_quant_control_s;                        // 16 bytes

//*****************************************************************************
// VP9
//*****************************************************************************

//vp9 filter config
typedef struct
{
    NvU8   filter_level[8];                  // loop filter strength for each segment.

    NvS8   ref_delta[4];                     // Loop filter strength adjustments based on frame type (intra, inter)

    NvS32  mode_delta_0      : 8;
    NvS32  mode_delta_1      : 8;            // Loop filter strength adjustments based on mode (zero, new mv)
    NvU32  disable           : 1;            // disable loop-filter
    NvU32  sharpness         : 3;            // sharpness level
    NvU32  baselevel_gt32    : 1;            // filter base level greater than 32 flag
    NvU32  reserved          : 11;
}nvenc_vp9_filter_cfg_s;  //16 bytes

typedef struct
{
    NvU32  round_chroma_ac   : 3;             // round for chroma AC
    NvU32  round_chroma_dc   : 3;             // round for chroma DC
    NvU32  round_luma_ac     : 3;             // round for luma AC
    NvU32  round_luma_dc     : 3;             // round for luma DC
    NvU32  round_delta_inter : 3;             // round delta for inter
    NvU32  reserved          : 17;
}vp9_round_cfg_s;

//vp9 quant config
typedef struct
{
    NvU8   qindex_seg[8];                     // Base frame qindex (index in quant lookup table) for each segment 0...255
    NvS32  delta_y_dc       : 5;              // explicit qindex adjustment for y dccoefficient, -15...15
    NvS32  delta_uv_dc      : 5;              // qindex adjustment for uv accoefficient, -15...15
    NvS32  delta_uv_ac      : 5;              // qindex adjustment for uv dccoefficient, -15...15
    NvS32  reserved         : 17;
    //round parameters
    vp9_round_cfg_s round_seg[8];             // round config for each segment
}nvenc_vp9_quant_cfg_s;  //44 bytes

// vp9 dma config for each tile
typedef struct
{
    NvU32 tile_width       : 6;              // tile width in super blocks(64x64)
    NvU32 tile_height      : 6;              // tile height in super blocks(64x64)
    NvU32 tile_pos_x       : 6;              // tile x-position in super blocks unit
    NvU32 reserved         : 14;
}nvenc_vp9_modedma_cfg_s;

//vp9 smalle config
typedef struct
{
    NvU32 high_prec_mv_enable : 1;           // high precision mv enable
    NvU32 temporal_mv_enable  : 1;           // temporal mv enable
    NvU32 transform_mode      : 3;           // transform mode: 0: only4x4, 1: allow_8x8, 2: allow_16x16, 3: allow_32x32, 4:transform_mode_select
    NvU32 lossless_enable     : 1;           // lossless encoding enable
    NvU32 mocomp_filter_type  : 3;           // mocomp filter type:0 eight-tap smooth, 1: eight-tap, 2:eight-tap sharp, 3: bilinear, 4:switchable
    NvU32 tile_enable         : 1;           // enable tile encoding
    NvU32 segment_enable      : 1;           // enable segment
    NvU32 segment_upd_enable  : 1;           // enable explicit map update for current frame. If true, then segment_temp_enable says how to update
    NvU32 segment_temp_enable : 1;           // Type of map update
    NvU32 reserved            : 19;
}nvenc_vp9_smalle_config_s;

//vp9 frame-level parameters
typedef struct
{
    NvU32 pic_width          : 13;           // picture width in pixels
    NvU32 pic_height         : 13;           // picture height in pixels
    NvU32 rec_disable        : 1;            // disable recon output
    NvU32 type               : 1;            // picture type: 0 Intra/1 Inter
    NvU32 refframe           : 2;            // reference frame type used 1 = last 2= golden 3= alt-ref
    NvU32 reserved           : 2;

    //ref-frame param
    NvU8  ref_index[8];                      // ref-frame index for each segment
    //skip param
    NvU8  skip_mode[8];                      // skip-mode enable for each segment: skip mode implies both the use of a (0,0) motion vector and that no residual will be coded
    //filter param
    nvenc_vp9_filter_cfg_s filter_cfg;       // filter config
    //quant param
    nvenc_vp9_quant_cfg_s quant_cfg;         // quant & round config
    nvenc_vp9_smalle_config_s smalle_cfg;    // small-e config
    NvU32 num_tile_controls_minus1;          // number of nvenc_vp9_tile_control_s entries in array minus1
    NvU32 tile_controls_offset;              // offset from start of top level "nvenc_vp9_pic_control_s" structure
                                             // to start of array of "nvenc_vp9_tile_control_s" structures

    NvU32 bitstream_buf_start_pos;           // bitstream buffer offset start position byte offset
    NvU32 bitstream_buf_size;                // bitstream buffer size in bytes

    NvU32 combined_linebuf_size;             // combined line buffer size in bytes

    NvU32 filterline_linebuf_size;           // filter line line-buffer size in bytes

    NvU32 filtercol_linebuf_size;            // filter column line-buffer size in bytes
}nvenc_vp9_pic_control_s;  //112 bytes

typedef struct
{
    NvU32 col_pos      : 6;                  // tile position in Super Block of Y axis
    NvU32 bs_size      : 24;                 // tile size in Super Block
    NvU32 reserved     : 2;
    nvenc_vp9_modedma_cfg_s mode_dma_cfg;    // mode-dma config for this tile
    NvU8  tile_idx;                          // tile index in the picture
    NvU8  reserved2[55];
}nvenc_vp9_tile_control_s;  //128bytes

typedef struct
{
  NvU8    QP[3];                                     // QP for 0:P picture, ,1:B picture 2; I picture
  NvU8    minQP[3];                                  // min QP for 0:P picture, ,1:B picture 2; I picture
  NvU8    maxQP[3];                                  // max QP for 0:P picture, ,1:B picture 2; I picture
  NvU8    reserved[3];
  NvU32   rhopbi[3];                                 // 23.8 unsigned fixed point quant ratios P/I, B/I, I/I(=1)

  NvS32   framerate;                                 // fps
  NvU32   buffersize;                                // total buffer size

  NvU32   nal_cpb_size;                              // size in bits
  NvU32   nal_bitrate;                               // rate in bps

  NvS32   gop_length;                                // I period
  NvU32   Bmin;                                      // 23.8 unsigned fixed point min buffer level, updated by driver at sequence level only.
  NvS32   Ravg;                                      // 23.8 signed fixed point average rate, updated by driver at sequence level only.
  NvS32   R;                                         // 23.8 signed fixed point rate


} nvenc_vp9_rc_s;                                    // 56 bytes, currently keep the same as vp8

typedef struct
{
  // 256 aligned
  NvU32                      magic;                  // magic number, see text above             //  4 bytes
  nvenc_h265_surface_cfg_s   refpic_cfg;             // defines layout of reference pictures     // 32 bytes
  nvenc_h265_surface_cfg_s   input_cfg;              // defines layout of input pictures         // 32 bytes
  nvenc_h265_surface_cfg_s   outputpic_cfg;          // defines layout of reconstructed pictures // 32 bytes
  nvenc_h265_sps_data_s      sps_data;                                                           //  8 bytes
  nvenc_h265_pps_data_s      pps_data;                                                           //  8 bytes
  nvenc_vp9_rc_s             rate_control;           // Rate Control information                 // 56 bytes
  nvenc_h265_pic_control_s   h265_pic_control;       // h265 picture control config              //532 bytes
                                                     // it is required as vp9 uses h265 mode decision result
  nvenc_vp9_pic_control_s    vp9_pic_control;        // vp9 picture control config               //112 bytes
  NvU8                       reserved[208];          // Pad to 1024 bytes
} nvenc_vp9_drv_pic_setup_s;

typedef struct
{
  NvU32   picture_index;                             // value received from SetPictureIndex method
  NvU32   error_status                       : 2;    // report error if any
  NvU32   ucode_error_status                 : 30;   // report error status from ucode to driver
  NvU32   total_byte_count;                          // total bitstream size in bytes
  NvU32   bitstream_start_pos;                       // byte_offset where VP9 BS DMA start writing bitstream
  NvU16   pic_type;                                  // inter/key frame type
  NvS16   avgQP;                                     // average QP
  NvU32   reserved0;
  NvU32   intra_cu32x32_count;                       // number of intra 32x32 CUs.
  NvU32   inter_cu32x32_count;                       // number of inter 32x32 CUs.
  NvU32   intra_cu16x16_count;                       // number of intra 16x16 CUs.
  NvU32   inter_cu16x16_count;                       // number of inter 16x16 CUs.
  NvU32   intra_cu8x8_count;                         // number of intra 8x8   CUs.
  NvU32   inter_cu8x8_count;                         // number of inter 8x8   CUs.
  NvU32   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
  NvU32   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
  NvU32   total_intra_cost;                          // sum of all intra best mode's SATDs, each mb has one intra SATD.
  NvU32   total_inter_cost;                          // sum of all inter best mode's SATDs, each mb has one inter SATD.
} nvenc_vp9_pic_stat_s;                              // 64 bytes

// ****************************************************************************
// ****************************************************************************
// VP8
// ****************************************************************************
// ****************************************************************************

#define MAXSEGMENTS     4
#define TEB_BUF_SIZE    (256 * 1024)

typedef struct
{
  NvU16   frame_width_minus1;                        // frame width in pixels minus 1, range 0-4095
  NvU16   frame_height_minus1;                       // frame height in pixels minus 1, range 0-4095

  NvU32   sfc_pitch                          : 10;   // pitch of luma plane in macorblock units( i.e stride >>4)
  NvU32   sfc_pitch_chroma                   : 10;   // pitch of chroma plane in macroblock units
  NvU32   reserved2                          : 12;

  NvU32   sfc_trans_mode                     : 4;    // least 3 significient bits are used to stand for 8 modes, normal, xflip, yflip etc.
  NvU32   block_height                       : 7;    // BL mapping block height setting
  NvU32   tiled_16x16                        : 1;    // Surface is 16x16 tiled instead of BL mapped (must be 1 for refpics)
  NvU32   memory_mode                        : 2;    // 0: semi-planar, 1: planar
  NvU32   input_bl_mode                      : 1;    // the input block linear mode: 0~gpu bl; 1~tegra bl; this is only for cmod and cmod/plugin
  NvU32   reserved                           : 17;   // pad to NvS32

  NvU32   luma_top_frm_offset;                       // offset of luma top field or frame in units of 256
  NvU32   luma_bot_offset;                           // offset of luma bottom field in units of 256. Not used if frame format.
  NvU32   chroma_top_frm_offset;                     // offset of chroma top field or frame, or offset of chroma U(Cb) plane in planar mode, both in units of 256
  NvU32   chroma_bot_offset;                         // offset of chroma bottom field, not used if frame format. Or offset of chroma V(Cr) plane in planar mode. Both in units of 256.

} nvenc_vp8_surface_cfg_s;                           // 28 bytes

typedef struct
{
  NvU32   horiz_scale                        : 2;    // 0=1, 1=5/4, 2=5/3, 3=2
  NvU32   vert_scale                         : 2;    // 0=1, 1=5/4, 2=5/3, 3=2
  NvU32   reserved                           : 28;   // Align to word boundary
} nvenc_vp8_sequence_data_s;                         // 4 bytes

typedef struct
{
  NvU32   frame_type                         : 1;    // 0=key frame, 1=interframe
  NvU32   version                            : 3;    // VP8 version, also affects luma/chroma mocomp: 0=bicubic/bicubic, 1,2=bilinear/bilinear, 3..7=bilinear/fullpel
  NvU32   show_frame                         : 1;    // 0=don't display, 1=display
  NvU32   clr_type                           : 1;    // color space: 0=ITU-R BT.601, 1=reserved (multiplier free)
  NvU32   clamp_type                         : 1;    // 0=clamping of reconstructed samples required, 1=no clamping necessary
    //  <loop filter parameters>
  NvU32   filter_type                        : 2;    // 0=normal, 1=simple 2: disable
  NvU32   filter_level                       : 6;    // loop filter strength (1..63), 0=disabled
  NvU32   sharpness_level                    : 3;    // loop filter sharpness (0..7)
  NvU32   mode_ref_lf_delta_enabled          : 1;    // apply mb param dependent loop filter level deltas: 0=disabled, 1=enabled; if disabled the following parameters are not used
  NvU32   mode_ref_lf_delta_update           : 1;    // update ref_lf_deltas and mode_lf_deltas: 0=keep previous, 1=update
  NvU32   ref_lf_deltas_update_0             : 1;    // update ref_lf_deltas[0]: 0=keep previous, 1=update
  NvU32   ref_lf_deltas_update_1             : 1;    // update ref_lf_deltas[1]: 0=keep previous, 1=update
  NvU32   ref_lf_deltas_update_2             : 1;    // update ref_lf_deltas[2]: 0=keep previous, 1=update
  NvU32   ref_lf_deltas_update_3             : 1;    // update ref_lf_deltas[3]: 0=keep previous, 1=update
  NvU32   mode_lf_deltas_update_0            : 1;    // update mode_lf_deltas[0]: 0=keep previous, 1=update
  NvU32   mode_lf_deltas_update_1            : 1;    // update mode_lf_deltas[1]: 0=keep previous, 1=update
  NvU32   mode_lf_deltas_update_2            : 1;    // update mode_lf_deltas[2]: 0=keep previous, 1=update
  NvU32   mode_lf_deltas_update_3            : 1;    // update mode_lf_deltas[3]: 0=keep previous, 1=update
  NvU32   reserved0                          : 4;    // Align to word boundary
  NvS8    ref_lf_deltas[4];                          // reference frame dependent loop filter level delta (-63..+63)
                                                     // Index 0: macroblock is INTRA
                                                     //       1: macroblock is INTER and reference frame is LAST
                                                     //       2: macroblock is INTER and reference frame is GOLDEN
                                                     //       3: macroblock is INTER and reference frame is ALTREF
                                                     // lower 7 bits of each entry used as a signed value -63..+63, MS Bit is ignored by HW
  NvS8    mode_lf_deltas[4];                         // macroblock mode dependent loop filter level delta (-63..+63)
                                                     // Index 0: B_PRED  (macroblock is INTRA 4x4)
                                                     //       1: ZEROMV  (macroblock is INTER and mode is ZEROMV)
                                                     //       2: MV      (macroblock is INTER and mode is neither ZEROMV nor SPLITMV)
                                                     //       3: SPLITMV (macroblock is INTER and mode is SPLITMV (not 16x16))
                                                     // lower 7 bits of each entry used as a signed value -63..+63, MS Bit is ignored by HW
  // Other frame header parameters
  NvU32   multi_token_partition              : 2;    // number of tcoeff token partitions (0,1,2,3 -> 1,2,4,8): num_part = 1 << multi_token_partition
  NvU32   refresh_entropy_probs              : 1;    // 0=restore previous probs at end of frame, 1=keep updated probs for next frame
  NvU32   mb_no_coeff_skip                   : 1;    // per-mb skip flag, 0=disable 1:enable (misnomer?)
  NvU32   update_ymode_prob                  : 1;    // update ymode probs, 0=no, 1=yes
  NvU32   update_uvmode_prob                 : 1;    // update uvmode probs, 0=no, 1=yes

  NvU32   base_qindex                        : 7;    // quantizer index (0..127)

  NvU32   refresh_last_frame                 : 1;    // use decoded frame as last    reference frame
  NvU32   refresh_golden_frame               : 1;    // use decoded frame as golden  reference frame
  NvU32   refresh_alt_ref_frame              : 1;    // use decoded frame as alt_ref reference frame
  NvU32   copy_buffer_to_gf                  : 2;    // update golden frame from:  0=none (keep), 1=last_frame, 2=alt_ref_frame
  NvU32   copy_buffer_to_arf                 : 2;    // update alt_ref frame from: 0=none (keep), 1=last_frame, 2=golden_frame
  NvU32   ref_frame_sign_bias_gf             : 1;    // golden frame is future frame (invert mv sign)
  NvU32   ref_frame_sign_bias_arf            : 1;    // alt_ref frame is future frame (invert mv sign)
  NvU32   fixed_ref_frame_probs              : 1;    //use fixed ref frame probability
  NvU32   reserved1                          : 9;    // Align to word boundary


  NvS32   y1dc_delta_q                       : 5;    // quantizer delta for luma   DC (-15..+15)
  NvS32   y2dc_delta_q                       : 5;    // quantizer delta for WHT    DC (-15..+15)
  NvS32   y2ac_delta_q                       : 5;    // quantizer delta for WHT    AC (-15..+15)
  NvS32   uvdc_delta_q                       : 5;    // quantizer delta for chroma DC (-15..+15)
  NvS32   uvac_delta_q                       : 5;    // quantizer delta for chroma AC (-15..+15)

  //  <segmentation parameters>
  NvU32   segmentation_enabled               : 1;    // 0=disabled, 1=enabled; if disabled the following parameters are not used
  NvU32   update_mb_segmentation_map         : 1;    // per-mb segment ID map update: 0=keep previous, 1=update
  NvU32   update_mb_segmentation_data        : 1;    // mb_segment_abs_delta, seg_filter_level, and seg_qindex update: 0=keep previous, 1=update
  NvU32   mb_segment_abs_delta               : 1;    // seg_filter_level and seg_qindex are: 0=delta values, 1=absolute values
  NvU32   reserved2                          : 3;    // Align to word boundary
  NvS8    seg_filter_level[MAXSEGMENTS];             // per-segment loop filter levels
                                                     // if mb_segment_abs_delta==0: relative to filter_level (-63..+63)
                                                     // if mb_segment_abs_delta==1: absolute value (0..63)
  NvS8    seg_qindex[MAXSEGMENTS];                   // per-segment qindex values
                                                     // if mb_segment_abs_delta==0: relative to base_qindex (-127..+127)
                                                     // if mb_segment_abs_delta==1: absolute value (0..127)

} nvenc_vp8_picture_data_s;                          // 32 bytes

typedef struct
{
  NvU32   early_intra_mode_control           : 2;    // 0=inter; 1=eval_intra; 2=ext_hint; 3=none (low perf,best qual)
  NvU32   early_intra_mode_type_16x16dc      : 1;    // evaluate 16x16dc if early_intra_mode_control==1
  NvU32   early_intra_mode_type_16x16h       : 1;    // evaluate 16x16h  if early_intra_mode_control==1
  NvU32   early_intra_mode_type_16x16v       : 1;    // evaluate 16x16v  if early_intra_mode_control==1
  NvU32   early_ip_is_final                  : 1;    // if set, result of early intra is final (disable full intra search)
  NvU32   chroma_eval_mode                   : 1;    // for intra chroma search 0 = U and V; 1 = U only
  NvU32   nearest_mv_enable                  : 1;    // enable nearest mv mode for 16x16
  NvU32   near_mv_enable                     : 1;    // enable near mv mode for 16x16
  NvU32   zero_mv_enable                     : 1;    // enable zero mv mode 16x16
  NvU32   new_mv_enable                      : 1;    // enable new mv mode 16x16
  NvU32   above4x4_mv_enable                 : 1;    // enable above4x4 mv mode for VP8 SPLITMV modes
  NvU32   left4x4_mv_enable                  : 1;    // enable left4x4 mv mode for VP8 SPLITMV modes
  NvU32   zero4x4_mv_enable                  : 1;    // enable zero4x4 mv mode for VP8 SPLITMV modes
  NvU32   new4x4_mv_enable                   : 1;    // enable new4x4 mv mode for VP8 SPLITMV modes
  NvU32   l0_part_16x16_enable               : 1;    // enable L0 prediction for 16x16
  NvU32   l0_part_16x8_enable                : 1;    // enable L0 prediction for 16x8
  NvU32   l0_part_8x16_enable                : 1;    // enable L0 prediction for 8x16
  NvU32   l0_part_8x8_enable                 : 1;    // enable L0 prediction for 8x8
  NvU32   pskip_enable                       : 1;    // enable p_skip encoding (still used by ME for special modes, can this be removed?)
  NvU32   explicit_eval_nearestmv            : 1;    // enable explicit search vp8 nearest mv
  NvU32   explicit_eval_nearmv               : 1;    // enable explicit search vp8 near mv
  NvU32   explicit_eval_left_topmv           : 1;    // enable explicit search vp8 8x8 left_top mv
  NvU32   explicit_eval_left_bottommv        : 1;    // enable explicit search vp8 8x8 left_bottom mv
  NvU32   explicit_eval_top_leftmv           : 1;    // enable explicit search vp8 8x8 top_left mv
  NvU32   explicit_eval_top_rightmv          : 1;    // enable explicit search vp8 8x8 top_right mv
  NvU32   explicit_eval_zeromv               : 1;    // enable explicit search vp8 zero mv
  NvU32   refframe_pic                       : 2;    // ref frame type used 1=last, 2= golden, 3 = alt_ref
  NvU32   reserved0                          : 3;    // Pad to NvS32

  NvU32   intra_luma4x4_mode_enable          : 9;    // bitmask indicating which intra luma 4x4 modes to enable
  NvU32   intra_luma16x16_mode_enable        : 4;    // bitmask indicating which intra luma 16x16 modes to enable
  NvU32   intra_chroma_mode_enable           : 4;    // bitmask indicating which intra chroma modes to enable
  NvU32   intra_tm_mode_enable               : 1;    // bitmask indicating intra 4x4 true motion mode enable
  NvU32   ip_search_mode                     : 3;    // which intra sizes to search before making IP decision b0=4x4,b2=16x16                                                          // We dont need a bit for 8x8 for VP8 but it might be simpler for reg field to keep it?
  NvU32   mv_cost_enable                     : 1;    // enable mv cost calculations in mode decision
  NvU32   reserved1                          : 10;   // pad to full NvS32

  NvU16   early_intra_disable_mpeb_threshold;        // if early intra cost is below this, disable full search & select intra
  NvS16   bias_favor_intra_16x16_early;              // bias for early intra (subtracts from early intra 16x16 cost)

  NvU16   lambda_multiplier_intra[4];                // used for intra mode cost calculation  [segment_id]

  NvU32   mdpLambdaCoefMd                    :16;
  NvU32   mdpLambdaCoefMv                    :16;

  NvS16   bias_intra_16x16;                          // bias for intra 16x16 (subtracts from intra 16x16 cost)
  NvS16   bias_intra_4x4;                            // bias for intra 4x4   (subtracts from intra 4x4 cost)

  NvS16   bias_inter_16x16;                          // bias for inter 16x16 (subtracts from inter 16x16 cost)
  NvS16   bias_inter_16x8;                           // bias for inter 16x8  (subtracts from inter 16x8  cost)

  NvS16   bias_inter_8x16;                           // bias for inter 8x16  (subtracts from inter 8x16  cost)
  NvS16   bias_inter_8x8;                            // bias for inter 8x8   (subtracts from inter 8x8   cost)

  NvS16   bias_nearmv;                               // bias for nearmv      (subtracts from nearmv cost) for 16x16
  NvS16   bias_nearestmv;                            // bias for nearestmv   (subtracts from nearestmv cost) for 16x16

  NvS16   bias_zeromv;                               // bias for zeromv      (subtracts from zeromv cost) for 16x16
  NvS16   bias_newmv;                                // bias for newmv       (subtracts from newmv cost) for 16x16

  NvS16   bias_above4x4mv;                           // bias for above4x4mv  (subtracts from above4x4 cost) for VP8 SPLITMV modes
  NvS16   bias_left4x4mv;                            // bias for left4x4mv  (subtracts from left4x4 cost) for VP8 SPLITMV modes

  NvS16   bias_zero4x4mv;                            // bias for zero4x4mv  (subtracts from zero4x4 cost) for VP8 SPLITMV modes
  NvS16   bias_new4x4mv;                             // bias for new4x4mv  (subtracts from new4x4 cost) for VP8 SPLITMV modes

  NvS16   mv_cost_bias;                              // bias applied to internal motion vector cost calculation
  NvS16   bias_intra_over_inter;                     // bias for any intra   (subtracts from intra cost)
  NvU16   intra_refresh_cnt;                         // distance between forced-intra MBs; 0 means OFF
  NvU16   intra_refresh_offs;                        // first forced-intra MB

  NvU16   tempmv_wt_spread_threshold;                // The threshold for the sum of difference between 8x8 and 16x16 mvs. Used for temporal mv weight.
  NvU16   tempmv_wt_distort_threshold;               // The threshold for cost of the 16x16 mv. Used for temporal mv weight.
} nvenc_vp8_md_control_s;                            // 64 bytes

typedef struct
{
  NvU32   self_temporal_stamp_l0             : 3;    // stamp to use for L0 integer search in stamp based mode
  NvU32   self_temporal_explicit             : 1;    // explicitly evaluate these vectors
  NvU32   self_temporal_search               : 1;    // integer search enable for this hint
  NvU32   self_temporal_refine               : 1;    // subpel search enable for this hint
  NvU32   self_temporal_enable               : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)

  NvU32   coloc_stamp_l0                     : 3;    // stamp to use for L0 integer search in stamp based mode
  NvU32   coloc_explicit                     : 1;    // explicitly evaluate these vectors
  NvU32   coloc_search                       : 1;    // integer search enable for this hint
  NvU32   coloc_refine                       : 1;    // subpel search enable for this hint
  NvU32   coloc_enable                       : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)

  NvU32   self_spatial_stamp_l0              : 3;    // stamp to use for L0 integer search in stamp based mode
  NvU32   self_spatial_explicit              : 1;    // explicitly evaluate these vectors
  NvU32   self_spatial_search                : 1;    // integer search enable for this hint
  NvU32   self_spatial_refine                : 1;    // subpel search enable for this hint
  NvU32   self_spatial_enable                : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)

  NvU32   reserved0                          : 11;   // Pad to NvS32

  NvU32   external_stamp_lastref_stamp       : 3;    // Stamp to use for external L0 hint with refidx = 0
  NvU32   external_stamp_goldref_stamp       : 3;    // Stamp to use for external L0 hint with refidx = 1
  NvU32   external_stamp_altref_stamp        : 3;    // Stamp to use for external L0 hint with refidx = 2
  NvU32   external_explicit                  : 1;    // explicitly evaluate these vectors
  NvU32   external_search                    : 1;    // integer search enable for this hint
  NvU32   external_refine                    : 1;    // subpel search enable for this hint
  NvU32   external_enable                    : 1;    // hint enable (enables processing + fetching of data) (only changeable at first slice in picture)

  NvU32   const_mv_stamp_l0                  : 3;    // stamp to use for L0 integer search in stamp based mode
  NvU32   const_mv_explicit                  : 1;    // explicitly evaluate these vectors
  NvU32   const_mv_search                    : 1;    // integer search enable for this hint
  NvU32   const_mv_refine                    : 1;    // subpel search enable for this hint
  NvU32   const_mv_enable                    : 1;    // hint enable (enables processing + fetching of data)

  NvU32   reserved1                          : 12;   // pad to NvS32

} nvenc_vp8_me_hint_cfg_s;                           // 8 bytes

// Same as H264 me_limit_mv struct
typedef struct
{
  NvU32   mv_limit_enable                    : 1;    // 0=disable, 1=enable set the absolute MV range
  NvU32   left_mvx_frac                      : 2;    // vector X component fraction
  NvS32   left_mvx_int                       : 12;   // vector X component integer portion
  NvU32   reserved1                          : 1;    // padding X to 16 bit
  NvU32   top_mvy_frac                       : 2;    // vector Y component fraction
  NvS32   top_mvy_int                        : 10;   // vector Y component integer portion
  NvU32   reserved2                          : 4;    // padding Y to 16 bit

  NvU32   right_mvx_frac                     : 2;    // vector X component fraction
  NvS32   right_mvx_int                      : 12;   // vector X component integer portion
  NvU32   reserved3                          : 2;    // padding X to 16 bit
  NvU32   bottom_mvy_frac                    : 2;    // vector Y component fraction
  NvS32   bottom_mvy_int                     : 10;   // vector Y component integer portion
  NvU32   reserved4                          : 4;    // padding Y to 16 bit

} nvenc_vp8_me_limit_mv_s;                           // 8 bytes

typedef struct
{
  NvU32   mvx_frac                           : 2;    // vector X component fraction
  NvS32   mvx_int                            : 12;   // vector X component integer portion
  NvU32   reserved_x                         : 2;    // padding X to 16 bit
  NvU32   mvy_frac                           : 2;    // vector Y component fraction
  NvS32   mvy_int                            : 10;   // vector Y component integer portion
  NvU32   reserved_y                         : 4;    // padding Y to 16 bit

} nvenc_vp8_me_const_vec_s;                          // 4 bytes

typedef struct
{
  NvU32   bitmask[2]                         ;       // 8x8 search point pattern bitmask
  NvU32   hor_adjust                         : 1;    // shift hor based on lsb of pred
  NvU32   ver_adjust                         : 1;    // shift ver based on lsb of pred
  NvU32   reserved                           : 30;

} nvenc_vp8_me_stamp_s;                              // 12 bytes

typedef struct
{
  NvU32   me_predictor_mode                  : 1;    // 0=MDP, 1=const
  NvU32   refinement_mode                    : 1;    // 0=hpel, 1=qpel
  NvU32   lambda_mode                        : 1;    // 0=MDP, 1=const
  NvU32   const_lambda                       : 11;   // U8.3 format lambda, used if lambda_mode=1
  NvU32   refine_on_search_enable            : 1;    // 0=disable, 1=enable subpel refine for search candidates
  NvU32   me_only_mode                       : 1;
  NvU32   fps_mvcost                         : 1;
  NvU32   sps_mvcost                         : 1;
  NvU32   sps_cost_func                      : 1;
  NvU32   me_icc_mode_mad                    : 1;
  NvU32   sps_filter                         : 3;
  NvU32   mc_filter                          : 3;
  NvU32   mbc_ctrl_arbitor                   : 2;
  NvU32   average_mvhint_enable              : 1;
  NvU32   rmvp_source_half_scaled            : 1;    // only support picture size half scale down in horz&vert
  NvU32   reserved                           : 2;

  nvenc_vp8_me_limit_mv_s      limit_mv;
  nvenc_vp8_me_hint_cfg_s      predsrc;              // predictor sources
  nvenc_vp8_me_const_vec_s     l0_hint;              // constant MV used as L0 hint
  nvenc_vp8_me_const_vec_s     l1_hint;              // constant MV used as L1 hint
  nvenc_vp8_me_const_vec_s     l0_pred;              // constant MV used as L0 predictor
  // should this be made an array?
  nvenc_vp8_me_stamp_s         shape0;               // priority 0 stamping pattern
  nvenc_vp8_me_stamp_s         shape1;               // priority 1 stamping pattern
  nvenc_vp8_me_stamp_s         shape2;               // priority 2 stamping pattern
  nvenc_vp8_me_stamp_s         shape3;               // priority 3 stamping pattern
  nvenc_vp8_me_stamp_s         shape4;               // priority 4 stamping pattern
  nvenc_vp8_me_stamp_s         shape5;               // priority 5 stamping pattern
  nvenc_vp8_me_stamp_s         shape6;               // priority 6 stamping pattern
  nvenc_vp8_me_stamp_s         shape7;               // priority 7 stamping pattern

  nvenc_me_tebif_s             teb_para;             // teb parameters
  NvU8                         mbc_mb_size;          // MBC size in MB
  NvU8                         tempmv_scale;         // temporal mv scale
  NvU8                         reserved1[2];

} nvenc_vp8_me_control_s;                            // 136 bytes

typedef struct
{
  NvU32   qpp_run_vector_4x4                 : 16;   // cost values for 4x4 transform (16 bit total vector)
  NvU32   qpp_luma8x8_cost                   : 4;    // luma 8x8 cost threshold, 0 = throw out all coefs, range 0-15
  NvU32   qpp_luma16x16_cost                 : 4;    // luma 16x16 cost threshold, 0 = throw out all coefs, range 0-15
  NvU32   qpp_chroma_cost                    : 4;    // chroma cost threshold, 0 = throw out all coefs, range 0-15
  NvU32   qpp_mode                           : 2;    // 0 = OFF, 1 = 8x8, 2 = 16x16_8x8
  NvU32   reserved                           : 2;

  NvU16   dz_4x4_YI[16];                             // deadzone for 4x4 transform of Luma Intra
  NvU16   dz_4x4_YP[16];                             // deadzone for 4x4 transform of Luma Inter
  NvU16   dz_4x4_CI;                                 // deadzone for 4x4 transform of Chroma Intra
  NvU16   dz_4x4_CP;                                 // deadzone for 4x4 transform of Chroma Inter

} nvenc_vp8_quant_control_s;                         // 72 bytes

typedef struct
{
  NvU32   hist_buf_size;                             // size in bytes of the buffer allocated for history
  NvU32   bitstream_buf_size;                        // size in bytes of the buffer allocated for bitstream slice/mb data
  NvU32   bitstream_start_pos;                       // start position in bitstream buffer where data should be written (byte offset)
  NvU32   bitstream_residual_buf_size;               // size in bytes of the buffer allocated for residual partition bitstream slice/mb data
  NvU32   bitstream_residual_start_pos[8];           // start position in residual partition bitstream buffer where data for residual partitions should be written (byte offset)

} nvenc_vp8_pic_control_s;                           // 48 bytes

typedef struct
{
  NvU8    QP[3];                                     // QP for 0:P picture, ,1:B picture 2; I picture
  NvU8    minQP[3];                                  // min QP for 0:P picture, ,1:B picture 2; I picture
  NvU8    maxQP[3];                                  // max QP for 0:P picture, ,1:B picture 2; I picture
  NvU8    reserved[3];
  NvU32   rhopbi[3];                                 // 23.8 unsigned fixed point quant ratios P/I, B/I, I/I(=1)

  NvS32   framerate;                                 // fps
  NvU32   buffersize;                                // total buffer size

  NvU32   nal_cpb_size;                              // size in bits
  NvU32   nal_bitrate;                               // rate in bps

  NvS32   gop_length;                                // I period
  NvU32   Bmin;                                      // 23.8 unsigned fixed point min buffer level, updated by driver at sequence level only.
  NvU32   Ravg;                                      // 23.8 unsigned fixed point average rate, updated by driver at sequence level only.
  NvU32   R;                                         // 23.8 unsigned fixed point rate

} nvenc_vp8_rc_s;                                    // 56 bytes

typedef struct
{
  // 256 aligned
  NvU32                      magic;                  // magic number
  nvenc_vp8_surface_cfg_s    refpic_cfg;             // 28 bytes
  nvenc_vp8_surface_cfg_s    input_cfg;              // 28 bytes
  nvenc_vp8_surface_cfg_s    outputpic_cfg;          // 28 bytes
  nvenc_vp8_sequence_data_s  sequence_data;          // 4 bytes
  nvenc_vp8_picture_data_s   picture_data;           // 32 bytes
  nvenc_vp8_pic_control_s    pic_control;            // 48 bytes
  nvenc_vp8_me_control_s     me_control;             // 136 bytes
  nvenc_vp8_md_control_s     md_control;             // 64 bytes
  nvenc_vp8_quant_control_s  quant_control;          // 72 bytes
  nvenc_vp8_rc_s             rate_control;           // 56 bytes
  NvU8                       reserved[16];           // pad to 512 bytes

} nvenc_vp8_drv_pic_setup_s;                         // 512 bytes


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
//                                         (0=8x8, 1=8x4, 2=4x8, 3=4x4):
//     15                          0    = field mode flag
//                                 1    = intra coded
//                                 3..2 = partition size for macroblock
//                                         (0=16x16, 1=16x8, 2=8x16, 3=8x8):
//                                 5..4 = zero
//     all others                  5..0 = zero
//
// Note that motion vectors are duplicated for each 4x4 index from the top-left 4x4 block
// of each (sub)partition. Note also that since NVENC does not currently support sub-8x8
// inter partitioning, the 4 vectors within an 8x8 are replicated values.

typedef struct
{
  NvU32   mvx                                : 14;   // bit  0..13: x component of MV
  NvU32   mvy                                : 12;   // bit 14..25: y component of MV
  NvU32   e                                  : 6;    // bit 26..31: extra data, different per entry
} nvenc_h264_coloc_entry_s;

typedef struct                                       // 64 byte aligned block
{
  nvenc_h264_coloc_entry_s    coldata[16];           // col data for 1 macroblock = array of 16 entries
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
// interpreted by any other engine or by software.
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
  NvU32   cand_cnt_blk_16x16                 : 4;   // 3:0 Number of candidates per block types
  NvU32   cand_cnt_blk_16x8                  : 4;   // 7:4
  NvU32   cand_cnt_blk_8x16                  : 4;   // 11:8
  NvU32   cand_cnt_blk_8x8                   : 4;   // 15:12
  NvU32   cand_cnt_blk_8x4                   : 4;   // 19:16
  NvU32   cand_cnt_blk_4x8                   : 4;   // 23:20
  NvU32   cand_cnt_blk_4x4                   : 4;   // 27:24
  NvU32   cand_bdirect_spatial_zmv           : 1;   // 28 enable Spatial direct enable ZMV 4x4 scores
  NvU32   cand_bdirect_spatial_nzmv          : 1;   // 29 enable Spatial direct enable non-zero MV 4x4 scores
  NvU32   cand_bdirect_temporal              : 1;   // 30 enable Temporal direct mode. Only valid for Bi-type.
  NvU32   cand_pskip                         : 1;   // 31 enable p-skip mode. Only valid for L0 controls.
} cea_cand_count_blktype_s;

typedef struct
{
  // 64 aligned
  NvU32              picture_index;                  // picture index given by driver (via method)
  NvU32              data_start;                     // start offset in buffer of data section which must be 256B aligned
  // the following fields are only there right now to make it easier to implement the cmodel plugin mode
  // they might be removed at a later stage
  NvU32              cea_lambda;
  cea_cand_count_blktype_s  blk_counts[3];
  NvS32              reserved[10];                   //padded by 8 bytes to align the end of the structure to 32 bytes for dmread operations
} cea_hints_header_s;

typedef struct
{
  NvS32     mvx         : 12;                        // 11:00 x component of integer pixel MV (relative to current MB) S12.0
  NvS32     mvy         : 10;                        // 21:12 y component of integer pixel MV (relative to current MB) S10.0
  NvS32     refidx      : 5;                         // 26:22 reference index (31=invalid)
  NvU32     dir         : 1;                         // 27:27 0=L0 1=L1
  NvS32     part_type   : 2;                         // 29:28 0=16x16 1=16x8 2=8x16 3=8x8 (blocks in partition must be consecutive)
  NvU32     last_of_part: 1;                         // 30:30 last MV of (sub) partition
  NvU32     last_of_mb  : 1;                         // 31:31 last MV of macroblock
} cea_hint_s;


typedef struct
{
  NvU32     segID[4];                                //segment ID for one MB. each 4x4 block has a segID which is 8 bits, total is 8bitx16 = 128 bits. maximun segments supported is 32
} cea_segment_s;

// 2.6. Encoder Status Buffer
// ---------------------------
//
// This buffer provides some additional data about the encoded bit stream that can
// be used for rate control and multi-pass encoding as well as status information
// that can be used for debug.

typedef struct
{
  NvU32   picture_index;                             // value received from SetPictureIndex method
  NvU32   error_status                       : 2;    // report error if any
  NvU32   ucode_error_status                 : 30;   // report error status from ucode to driver
  NvU32   total_bit_count;                           // picture size in bits
  NvU32   type1_bit_count;                           // type1 bit count for the entire picture
  NvU16   pic_type;                                  // copied from pic_type in nvenc_h264_pic_control_s
  NvU16   num_slices;                                // number of slices produced
  NvU16   ave_activity;                              // reserved
  NvS16   avgQP;                                     // average QP
  NvU32   reserved0;
  NvU32   hrdFullness;
  NvU32   bitstream_start_pos;                       // byte_offset where mpec will start writing slice header
  NvU32   last_valid_byte_offset;                    // greatest offset in output buffer which is assured to have valid compressed data
  NvU16   intra_mb_count;                            // number of intra MBs.
  NvU16   inter_mb_count;                            // number of inter MBs.
  NvU32   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
  NvU32   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
  NvU32   total_intra_cost;                          // sum of all intra best mode's SATDs, each mb has one intra SATD.
  NvU32   total_inter_cost;                          // sum of all inter best mode's SATDs, each mb has one inter SATD.
  NvU32   complexity;                                // sum of mbRowBitcount * current QS
  NvS16   average_mvx;                               // average mvx info, can be used for ME hint
  NvS16   average_mvy;                               // average mvy info, can be used for ME hint
  NvS16   actual_min_qp_used;                        // actual minimum qp used in the frame
  NvS16   actual_max_qp_used;                        // actual maximum qp used in the frame
  NvU32   hw_perf_me_sps;                            // me slice per second
  NvU32   hw_perf_me_fps;                            // me frame per second
  NvU32   hw_perf_mpec_active_cnt;                   // mpec active count
  NvU32   hw_perf_mpeb_active_cnt;                   // mpeb active count
  NvU64   total_dist_ssd;                            // sum of all ssd distortion of the MB best mode
  NvU32   total_dist_ssim;                           // sum of all ssim distortion of the MB best mode
  NvU32   best_satd_l0;                              // sum of all best satd cost of L0
  NvU32   best_satd_l1;                              // sum of all best satd cost of L1
  NvU32   reserved1[5];                              // align to 128 bytes

} nvenc_pic_stat_s;                                  // 128 bytes


typedef struct
{
  NvU32   slice_offset;                              // slice offset in bytes
  NvU32   slice_size;                                // slice size in bytes
  NvU32   slh_bit_count;                             // slice header size in bits
  NvU8    reserved[4];
} nvenc_slice_stat_s;                                // 16 bytes per status


typedef struct
{
  NvU32   intra_res_bit_count;                       // #bits spent on intra residuals
  NvU32   inter_res_bit_count;                       // #bits spent on inter residuals
  NvU32   intra_mode_bit_count;                      // #bits spent on intra modes (mbtype, pred mode)
  NvU32   inter_mode_bit_count;                      // #bits spent on inter modes (mbtype, submbtype, refidx, mv)
  NvU32   ipcm_bit_count;                            // # bits spent on MB mode that caused IPCM rewind.
  NvU32   type1_bit_count;                           // count of bits in type I bitstream (excludes leading_zero_8bits,
                                                     // start_codeprefix_one_3bytes and trailing_zero_8bits)
  NvU32   total_bit_count;                           // total number of bits produced

  NvU32   total_bin_count;                           // total number of bins produced by cabac
  NvU32   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
  NvU32   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
  NvU32   cumulative_ipcm_cost;                      // sum of all inter SA(T)Ds for the MBs that caused IPCM rewind
  NvU32   cumulative_skip_cost;                      // sum of all P and B skip MB SA(T)Ds

  NvU16   intra_mb_count;                            // number of intra MBs.
  NvU16   inter_mb_count;                            // number of inter MBs.
  NvU16   skip_mb_count;                             // number of skip MBs.
  NvU16   ipcm_mb_count;                             // number of ipcm MBs.

  NvU32   total_intra_cost;                          // sum of all intra best mode's SATDs, each ctb has one intra SATD.
  NvU32   total_inter_cost;                          // sum of all inter best mode's SATDs, each ctb has one inter SATD.
} nvenc_mpec_stat_s;                                 // 64 bytes per status

typedef struct
{
    NvU32   var_min;
    NvU32   var_max;
    NvU32   var_avg;
    NvU8    reserved[4];
} nvenc_aq_stat_s;

typedef struct
{
  nvenc_pic_stat_s   pic_stat;                              // 128Bytes
  nvenc_slice_stat_s slice_stat[((4096/16)*(4096/16)) + 3]; // To account for 1mb/slice and 4kx4k, plus 3 for alignment
  nvenc_mpec_stat_s  mpec_stat[((4096/16)*(4096/16)) + 3];  // To account for stat/1mb and 4kx4k, plus 3 for alignment
  nvenc_aq_stat_s    aq_stats[((4096/16)*(4096/16)) + 3];   // AQ stats from pdma
} nvenc_stat_data_s;

typedef struct
{
  NvU32   picture_index;                             // value received from SetPictureIndex method
  NvU32   error_status                       : 2;    // report error if any
  NvU32   ucode_error_status                 : 30;   // report error status from ucode to driver
  NvU32   total_bit_count;                           // picture size in bits
  NvU32   type1_bit_count;                           // type1 bit count for the entire picture
  NvU16   pic_type;                                  // copied from pic_type in nvenc_h264_pic_control_s
  NvU16   num_slices;                                // number of slices produced
  NvU16   ave_activity;                              // report average activity if activity based RC enabled
  NvS16   avgQP;                                     // average QP
  NvU32   reserved0;
  NvU32   hrdFullness;
  NvU32   bitstream_start_pos;                       // byte_offset where mpec will start writing slice header
  NvU32   last_valid_byte_offset;                    // greatest offset in output buffer which is assured to have valid compressed data
  NvU32   intra_cu32x32_count;                       // number of intra 32x32 CUs.
  NvU32   inter_cu32x32_count;                       // number of inter 32x32 CUs.
  NvU32   intra_cu16x16_count;                       // number of intra 16x16 CUs.
  NvU32   inter_cu16x16_count;                       // number of inter 16x16 CUs.
  NvU32   intra_cu8x8_count;                         // number of intra 8x8   CUs.
  NvU32   inter_cu8x8_count;                         // number of inter 8x8   CUs.
  NvU32   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
  NvU32   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
  NvU32   total_intra_cost;                          // sum of all intra best mode's SATDs, each mb has one intra SATD.
  NvU32   total_inter_cost;                          // sum of all inter best mode's SATDs, each mb has one inter SATD.
  NvS16   average_mvx;                               // average mvx info, can be used for ME hint
  NvS16   average_mvy;                               // average mvy info, can be used for ME hint
  NvS16   actual_min_qp_used;                        // actual minimum qp used in the frame
  NvS16   actual_max_qp_used;                        // actual maximum qp used in the frame
  NvU32   hw_perf_me_sps;                            // me slice per second
  NvU32   hw_perf_me_fps;                            // me frame per second
  NvU32   hw_perf_mpec_active_cnt;                   // mpec active count
  NvU32   hw_perf_mpeb_active_cnt;                   // mpeb active count
  NvU8    reserved1[24];
} nvenc_h265_pic_stat_s;                             // 128 bytes

typedef struct
{
  NvU32   slice_offset;                              // slice offset in bytes
  NvU32   slice_size;                                // slice size in bytes
  NvU32   slh_bit_count;                             // slice header size in bits
  NvU32   slice_size_ctbs;                           // slice size in ctbs
}nvenc_h265_slice_stat_s;                            // 16 bytes per status

typedef struct
{
  NvU32   intra_res_bit_count;                       // #bits spent on intra residuals
  NvU32   inter_res_bit_count;                       // #bits spent on inter residuals
  NvU32   intra_mode_bit_count;                      // #bits spent on intra modes (mbtype, pred mode)
  NvU32   inter_mode_bit_count;                      // #bits spent on inter modes (mbtype, submbtype, refidx, mv)
  NvU32   ipcm_bit_count;                            // # bits spent on MB mode that caused IPCM rewind.
  NvU32   type1_bit_count;                           // count of bits in type I bitstream (excludes leading_zero_8bits,
                                                     // start_codeprefix_one_3bytes and trailing_zero_8bits)
  NvU32   total_bit_count;                           // total number of bits produced

  NvU32   total_bin_count;                           // total number of bins produced by cabac
  NvU32   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
  NvU32   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
  NvU32   cumulative_ipcm_cost;                      // sum of all inter SA(T)Ds for the MBs that caused IPCM rewind
  NvU32   cumulative_skip_cost;                      // sum of all P and B skip MB SA(T)Ds
  NvU32   total_intra_cost;                          // sum of all intra best mode's SATDs, each ctb has one intra SATD.
  NvU32   total_inter_cost;                          // sum of all inter best mode's SATDs, each ctb has one inter SATD.

  NvU32   intra_cu32x32_count;                       // number of intra 32x32 CUs.
  NvU32   inter_cu32x32_count;                       // number of inter 32x32 CUs.
  NvU32   skip_cu32x32_count;                        // number of skip 32x32 CUs.
  NvU32   ipcm_cu32x32_count;                        // number of ipcm 32x32 CUs.
  NvU32   intra_cu16x16_count;                       // number of intra 16x16 CUs.
  NvU32   inter_cu16x16_count;                       // number of inter 16x16 CUs.
  NvU32   skip_cu16x16_count;                        // number of skip 16x16 CUs.
  NvU32   ipcm_cu16x16_count;                        // number of ipcm 16x16 CUs.
  NvU32   intra_cu8x8_count;                         // number of intra 8x8 CUs.
  NvU32   inter_cu8x8_count;                         // number of inter 8x8 CUs.
  NvU32   skip_cu8x8_count;                          // number of skip 8x8 CUs.
  NvU32   ipcm_cu8x8_count;                          // number of ipcm 8x8 CUs.

  NvU32   total_emulated_bit_count;                  // total number of emulated bits in total_bit_count
  NvU8    reserved[20];
} nvenc_h265_mpec_stat_s;                            // 128 bytes per status

typedef struct
{
  NvU32   var_min;
  NvU32   var_max;
  NvU32   var_avg;
  NvU8    reserved[4];
} nvenc_h265_aq_stat_s;                              // 16 bytes

typedef struct
{
  nvenc_h265_pic_stat_s   pic_stat;                              // 64Bytes
  nvenc_h265_slice_stat_s slice_stat[((4096/16)*(4096/16)) + 3]; // To account for 1mb/slice and 4kx4k, plus 3 for alignment
  nvenc_h265_mpec_stat_s  mpec_stat[((4096/16)*(4096/16)) + 3];  // To account for stat/1mb and 4kx4k, plus 3 for alignment
  nvenc_h265_aq_stat_s    aq_stats [((4096/16)*(4096/16)) + 3];  // AQ stats from pdma
} nvenc_h265_stat_data_s;


typedef struct
{
  NvU32   intra_res_bit_count;                       // #bits spent on intra residuals
  NvU32   inter_res_bit_count;                       // #bits spent on inter residuals
  NvU32   intra_mode_bit_count;                      // #bits spent on intra modes
  NvU32   inter_mode_bit_count;                      // #bits spent on inter modes
  NvU32   total_bit_count;                           // total number of bits produced
  NvU32   total_bin_count;                           // total number of bins produced by bac
  NvU32   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
  NvU32   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
  NvU32   cumulative_skip_cost;                      // sum of all P skip MB SA(T)Ds
  NvU16   intra_mb_count;                            // number of intra MBs.
  NvU16   inter_mb_count;                            // number of inter MBs.
  NvU16   skip_mb_count;                             // number of skip MBs.
  NvU16   reserved0;                                 // align to a word boundary
} nvenc_vp8_segment_stat_s;                          // 44 Bytes

typedef struct
{
  NvU32   bitstream_byte_count;                      // output bytes in main partition bitstream buffer
  NvU32   bitstream_res_byte_count[8];               // output bytes in residual partition bitstream buffers
  NvU32   reserved0;
  NvU32   picture_index;                             // value received from SetPictureIndex method
  NvU32   error_status                       : 2;    // report error if any
  NvU32   ucode_error_status                 : 30;   // report error status from ucode to driver
  NvU32   total_bit_count;                           // picture size in bits (sum of header's & all the partition's bits. actually redundant)
  NvU16   pic_type;                                  // copied from pic_type in nvenc_h264_pic_control_s
  NvU16   ave_activity;                              // report average activity if activity based RC enabled
  NvU16   num_segments;                              // number of segments produced
  NvU16   base_qindex;                               // base_qindex used for the frame
  nvenc_vp8_segment_stat_s segment_stat[4];
  NvU32   total_intra_cost;                          // sum of all intra best mode's SATDs, each mb has one intra SATD.
  NvU32   total_inter_cost;                          // sum of all inter best mode's SATDs, each mb has one inter SATD.
  NvU32   reserved1[3];
} nvenc_vp8_stat_data_s;                             // 256 Bytes

// Stats stored in HW format.
typedef struct
{
  NvU32 coef_token_cnt[3][3][4][8][4];               // [value/4][ctx][value%4][band][type]    4608 bytes (4096 + 512)
  NvU32 mv_stats[20][4];
} nvenc_vp8_token_data_s;


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
#define NVENC_RC_INIT_COMPLEXITY(bpp_at_qs1)                ((NvS32)((bpp_at_qs1)*16))        // use this macro to decouple from implementation-dependent precision

typedef struct
{

  NvU32  nal_cpb_fullness;                          // 28.4 fixed point
  NvU32  vcl_cpb_fullness;                          // 28.4 fixed point

  NvU32  Wpbi[3];                                   // 27.4 unsigned fixed point
  NvU32  Whist[3][3];                               // 27.4 unsigned fixed point [pbi][t]
  NvU32  np;                                        // 27.4 unsigned fixed point
  NvU32  nb;                                        // 27.4 unsigned fixed point
  NvU32  t;                                         // 22.10 fixed point

  NvU32  Rmean;                                     // 23.8 unsigned fixed point peak bitrate (bits/pixel) (Really 16.16 Wmean in new VBR mode)
  NvS32  Bavg;                                      // 23.8 signed fixed point

  NvS16  average_mvx;                               // average mvx info, can be used for ME hint
  NvS16  average_mvy;                               // average mvy info, can be used for ME hint

  NvU32  prevQS;                                    // previous second pass Qs derived from rate
  NvS16  prevQP;                                    // previous 2nd pass qp
  NvS16  fwdQP;                                     // QP of forward reference

  NvS8   refQPcol[48];                              // co-located mbrow qp of the most recently coded reference
  NvU8   Rcol[48];                                  // co-located rate (5.3 precision, relative to avg rate)
  NvS8   ref1QP[48];                                // first-pass reference qp (when 1st pass is using its own reference frames)

  NvU32  ext_intra_satd_Hist[2];                    // external satd History
  NvU8   ext_scd_Hist;                              // scene change flag History
  NvBool scene_change;                              // last coded frame was a scene change
  NvU8   qpp_modifier;                              // current qpp adjustment (subtracted from qpp thresholds)
  NvU8   Ksatd;                                     // pass1 satd to pass2 complexity measure
  NvS8   Werr;                                      // model deviation: Wpass2 / Wpass1 - 1.0, .8 signed fixed-point [0.5x..1.5x]
  NvS8   WerrI;                                     // same as above for I-frames
  NvU8   avgBf;                                     // 3.5 fixed point, average number of consecutive B-frames
  NvU8   reserved;
  NvU32  printf_offset;                             // Using for ucode print
  NvU8   reserved2[4];
} nvenc_persistent_state_s;                         // 256Bytes

typedef struct
{
  NvU64 timestamp;                                  //  Timestamp reflecting when the status was written. If the engine supports reading the time from
                                                    //global timestamp engine, the timestamp uses GTE. Otherwise this is nanoseconds since Jan. 1,1970.

  NvU32 status_engine;                              // Additional status information of the engine state
  NvU16 subframe;                                   // Additional status information of the engine state
  NvU16 status_task;                                // Engine status. 0 if there are no errors, otherwise an engine specific error code.
} NvEnc_Status;                                     // 16 bytes


// 2.a. Ucode Probability State
// --------------------------------
//
// This buffer is used to store probability state computed and used during the last picture.
// This is used to compute the probability updates for current picture
//

// DO NOT rearrange this structure. The alignment is crucial for dmreads
typedef struct
{
  NvU8   coef_probs[4][8][3][11+5];                 // 1536 bytes = 4*(3*128)
                                                    // Padding cause this has to be read in parts
  NvU8   ymode_prob[4];                             // 4 bytes
  NvU8   uvmode_prob[3];                            // 3 bytes
  NvU8   reserved0;                                 // Pad to word boundary
  NvU8   mv_context[2][19];                         // 38 bytes
  NvU8   reserved1[18];                             // Align to 32 byte boundary

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
  NvU32   intra_mb_count;                            // number of intra MBs.
  NvU32   inter_mb_count;                            // number of inter MBs.
  NvU32   cumulative_intra_cost;                     // sum of all intra SA(T)Ds
  NvU32   cumulative_inter_cost;                     // sum of all inter SA(T)Ds
  NvU32   total_bit_count;
  NvS32   avgQP;
  NvU32   complexity;
  //split encoding
  NvU32   strip_complexity;
  NvU32   strip_intra_mb_count;
  NvU8    reserved[220];
}nvenc_rc_pic_stat_s;                                //256 bytes

typedef struct
{
  NvU32   mbrow_intra_mb_count : 12;                 // number of intra MBs
  NvU32   mbrow_inter_mb_count : 12;                 // number of inter MBs
  NvU32   mbrow_avgQP : 8;                           // average QP
  NvU32   mbrow_cumulative_intra_cost;               // sum of all intra SA(T)Ds
  NvU32   mbrow_cumulative_inter_cost;               // sum of all inter SA(T)Ds
  NvU32   mbrow_bits_per_row;
}nvenc_mbrow_stat_s;

typedef struct
{
  NvS8 act_qp_buf[256];                              // Buffer to store qp modifier based on avg activity for a MB row
}nvenc_act_stat_s;

// 2.b. Qp Map Buffer
// --------------------------------
//
// This buffer is used to store qp map when rcmode==4, the structure is in run/level, run==0 means end.
//
typedef struct
{
  NvU16 run;                                       // how many mbs to use same qp
  NvU16 level;                                     // qp value
}nvenc_qp_map_s;

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
    E_RC_ERROR,
    E_SMALLE_BS_OVERFLOW,
    E_SMALLE_METHODSERROR,
    TOTAL_ERR_CODES
} ERROR_CODES;

#endif
