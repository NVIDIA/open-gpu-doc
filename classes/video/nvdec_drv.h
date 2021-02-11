/*******************************************************************************
    Copyright (c) 1993-2020, NVIDIA CORPORATION. All rights reserved.

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

#ifndef __NVDEC_DRV_H_
#define __NVDEC_DRV_H_

typedef struct _nvdec_status_hevc_s
{
    NvU32 frame_status_intra_cnt;    //Intra block counter, in unit of 8x8 block, IPCM block included
    NvU32 frame_status_inter_cnt;    //Inter block counter, in unit of 8x8 block, SKIP block included
    NvU32 frame_status_skip_cnt;     //Skip block counter, in unit of 8x8 block, blocks having NO/ZERO texture/coeff data
    NvU32 frame_status_fwd_mvx_cnt;  //ABS sum of forward  MVx, one 14bit MVx(integer) per 8x8 block
    NvU32 frame_status_fwd_mvy_cnt;  //ABS sum of forward  MVy, one 14bit MVy(integer) per 8x8 block
    NvU32 frame_status_bwd_mvx_cnt;  //ABS sum of backward MVx, one 14bit MVx(integer) per 8x8 block
    NvU32 frame_status_bwd_mvy_cnt;  //ABS sum of backward MVy, one 14bit MVy(integer) per 8x8 block
    NvU32 error_ctb_pos;             //[15:0] error ctb   position in Y direction, [31:16] error ctb   position in X direction
    NvU32 error_slice_pos;           //[15:0] error slice position in Y direction, [31:16] error slice position in X direction
} nvdec_status_hevc_s;

typedef struct _nvdec_status_vp9_s
{
    NvU32 frame_status_intra_cnt;    //Intra block counter, in unit of 8x8 block, IPCM block included
    NvU32 frame_status_inter_cnt;    //Inter block counter, in unit of 8x8 block, SKIP block included
    NvU32 frame_status_skip_cnt;     //Skip block counter, in unit of 8x8 block, blocks having NO/ZERO texture/coeff data
    NvU32 frame_status_fwd_mvx_cnt;  //ABS sum of forward  MVx, one 14bit MVx(integer) per 8x8 block
    NvU32 frame_status_fwd_mvy_cnt;  //ABS sum of forward  MVy, one 14bit MVy(integer) per 8x8 block
    NvU32 frame_status_bwd_mvx_cnt;  //ABS sum of backward MVx, one 14bit MVx(integer) per 8x8 block
    NvU32 frame_status_bwd_mvy_cnt;  //ABS sum of backward MVy, one 14bit MVy(integer) per 8x8 block
    NvU32 error_ctb_pos;             //[15:0] error ctb   position in Y direction, [31:16] error ctb   position in X direction
    NvU32 error_slice_pos;           //[15:0] error slice position in Y direction, [31:16] error slice position in X direction
} nvdec_status_vp9_s;

typedef struct _nvdec_status_s
{
    NvU32    mbs_correctly_decoded;          // total numers of correctly decoded macroblocks
    NvU32    mbs_in_error;                   // number of error macroblocks.
    NvU32    reserved;
    NvU32    error_status;                   // report error if any
    union
    {
        nvdec_status_hevc_s hevc;
        nvdec_status_vp9_s vp9;
    };
    NvU32    slice_header_error_code;        // report error in slice header

} nvdec_status_s;

// per 16x16 block, used in hevc/vp9 surface of SetExternalMVBufferOffset when error_external_mv_en = 1
typedef struct _external_mv_s
{
    NvS32    mvx     : 14;   //integrate pixel precision
    NvS32    mvy     : 14;   //integrate pixel precision
    NvU32    refidx  :  4;
} external_mv_s;

// HEVC
typedef struct _nvdec_hevc_main10_444_ext_s
{
    NvU32 transformSkipRotationEnableFlag : 1;    //sps extension for transform_skip_rotation_enabled_flag
    NvU32 transformSkipContextEnableFlag : 1;     //sps extension for transform_skip_context_enabled_flag
    NvU32 intraBlockCopyEnableFlag :1;            //sps intraBlockCopyEnableFlag, always 0 before spec define it
    NvU32 implicitRdpcmEnableFlag : 1;            //sps implicit_rdpcm_enabled_flag
    NvU32 explicitRdpcmEnableFlag : 1;            //sps explicit_rdpcm_enabled_flag
    NvU32 extendedPrecisionProcessingFlag : 1;    //sps extended_precision_processing_flag,always 0 in current profile
    NvU32 intraSmoothingDisabledFlag : 1;         //sps intra_smoothing_disabled_flag
    NvU32 highPrecisionOffsetsEnableFlag :1;      //sps high_precision_offsets_enabled_flag
    NvU32 fastRiceAdaptationEnableFlag: 1;        //sps fast_rice_adaptation_enabled_flag
    NvU32 cabacBypassAlignmentEnableFlag : 1;     //sps cabac_bypass_alignment_enabled_flag, always 0 in current profile
    NvU32 sps_444_extension_reserved : 22;        //sps reserve for future extension

    NvU32 log2MaxTransformSkipSize : 4 ;          //pps extension log2_max_transform_skip_block_size_minus2, 0...5
    NvU32 crossComponentPredictionEnableFlag: 1;  //pps cross_component_prediction_enabled_flag
    NvU32 chromaQpAdjustmentEnableFlag:1;         //pps chroma_qp_adjustment_enabled_flag
    NvU32 diffCuChromaQpAdjustmentDepth:2;        //pps diff_cu_chroma_qp_adjustment_depth, 0...3
    NvU32 chromaQpAdjustmentTableSize:3;          //pps chroma_qp_adjustment_table_size_minus1+1, 1...6
    NvU32 log2SaoOffsetScaleLuma:3;               //pps log2_sao_offset_scale_luma, max(0,bitdepth-10),maxBitdepth 16 for future.
    NvU32 log2SaoOffsetScaleChroma: 3;            //pps log2_sao_offset_scale_chroma
    NvU32 pps_444_extension_reserved : 15;        //pps reserved
    NvS8  cb_qp_adjustment[6];                    //-[12,+12]
    NvS8  cr_qp_adjustment[6];                    //-[12,+12]
    NvU32 HevcFltAboveOffset;                     // filter above offset respect to filter buffer, 256 bytes unit
    NvU32 HevcSaoAboveOffset;                     // sao    above offset respect to filter buffer, 256 bytes unit
} nvdec_hevc_main10_444_ext_s;

typedef struct _nvdec_hevc_pic_v1_s
{
    // New fields
    //hevc main10 444 extensions
    nvdec_hevc_main10_444_ext_s hevc_main10_444_ext;

    NvU32   reserved0                : 14;
    NvU32   external_ref_mem_dis     :  1;
    NvU32   error_recovery_start_pos :  2;       //0: from start of frame, 1: from start of slice segment, 2: from error detected ctb, 3: reserved
    NvU32   error_external_mv_en     :  1;
    NvU32   reserved1                : 14;
    // Reserved bits padding
} nvdec_hevc_pic_v1_s;

typedef struct _nvdec_hevc_pic_v2_s
{
// mv-hevc field
NvU32  mv_hevc_enable                     :1;
NvU32  nuh_layer_id                       :6;
NvU32  default_ref_layers_active_flag     :1;
NvU32  NumDirectRefLayers                 :6;
NvU32  max_one_active_ref_layer_flag      :1;
NvU32  NumActiveRefLayerPics              :6;
NvU32  poc_lsb_not_present_flag           :1;
NvU32  reserved0                          :10;
} nvdec_hevc_pic_v2_s;

typedef struct _nvdec_hevc_v3_s{
// slice level decoding
NvU32  slice_decoding_enable:1;//1: enable slice level decoding
NvU32  slice_ec_enable:1;      //1: enable slice error concealment. When slice_ec_enable=1,slice_decoding_enable must be 1;
NvU32  slice_ec_mv_type:2;     //0: zero mv; 1: co-located mv; 2: external mv;
NvU32  err_detected_sw:1;      //1: indicate sw/driver has detected error already in frame kick mode
NvU32  slice_ec_slice_type:2;  //0: B slice; 1: P slice ; others: reserved
NvU32  slice_strm_recfg_en:1;  //enable slice bitstream re-configure or not ;
NvU32  reserved:24;
NvU32  HevcSliceEdgeOffset;// slice edge buffer offset which repsect to filter buffer ,256 bytes as one unit
}nvdec_hevc_pic_v3_s;

typedef struct _nvdec_hevc_pic_s
{
    NvU32   reserved0[15];

    // Driver may or may not use based upon need.
    // If 0 then default value of 1<<27 = 298ms @ 450MHz will be used in ucode.
    // Driver can send this value based upon resolution using the formula:
    // gptimer_timeout_value = 3 * (cycles required for one frame)
    NvU32 gptimer_timeout_value;

    // general
    NvU8   tileformat                 : 2 ;   // 0: TBL; 1: KBL;
    NvU8   gob_height                 : 3 ;   // Set GOB height, 0: GOB_2, 1: GOB_4, 2: GOB_8, 3: GOB_16, 4: GOB_32 (NVDEC3 onwards)
    NvU8   reserverd_surface_format   : 3 ;
    NvU8   sw_start_code_e;                             // 0: stream doesn't contain start codes,1: stream contains start codes
    NvU8   disp_output_mode;                            // 0: Rec.709 8 bit, 1: Rec.709 10 bit, 2: Rec.709 10 bits -> 8 bit, 3: Rec.2020 10 bit -> 8 bit
    NvU8   reserved1;
    NvU32  framestride[2];                              // frame buffer stride for luma and chroma
    NvU32  colMvBuffersize;                             // collocated MV buffer size of one picture ,256 bytes unit
    NvU32  HevcSaoBufferOffset;                         // sao buffer offset respect to filter buffer ,256 bytes unit .
    NvU32  HevcBsdCtrlOffset;                           // bsd buffer offset respect to filter buffer ,256 bytes unit .
    // sps
    NvU16 pic_width_in_luma_samples;                      // :15, 48(?)..16384, multiple of 8 (48 is smallest width supported by NVDEC for CTU size 16x16)
    NvU16 pic_height_in_luma_samples;                     // :15, 8..16384, multiple of 8
    NvU32 chroma_format_idc                            : 4; // always 1 (=4:2:0)
    NvU32 bit_depth_luma                               : 4; // 8..12
    NvU32 bit_depth_chroma                             : 4;
    NvU32 log2_min_luma_coding_block_size              : 4; // 3..6
    NvU32 log2_max_luma_coding_block_size              : 4; // 3..6
    NvU32 log2_min_transform_block_size                : 4; // 2..5
    NvU32 log2_max_transform_block_size                : 4; // 2..5
    NvU32 reserved2                                    : 4;

    NvU32 max_transform_hierarchy_depth_inter          : 3; // 0..4
    NvU32 max_transform_hierarchy_depth_intra          : 3; // 0..4
    NvU32 scalingListEnable                            : 1; //
    NvU32 amp_enable_flag                              : 1; //
    NvU32 sample_adaptive_offset_enabled_flag          : 1; //
    NvU32 pcm_enabled_flag                             : 1; //
    NvU32 pcm_sample_bit_depth_luma                    : 4; //
    NvU32 pcm_sample_bit_depth_chroma                  : 4;
    NvU32 log2_min_pcm_luma_coding_block_size          : 4; //
    NvU32 log2_max_pcm_luma_coding_block_size          : 4; //
    NvU32 pcm_loop_filter_disabled_flag                : 1; //
    NvU32 sps_temporal_mvp_enabled_flag                : 1; //
    NvU32 strong_intra_smoothing_enabled_flag          : 1; //
    NvU32 reserved3                                    : 3;
    // pps
    NvU32 dependent_slice_segments_enabled_flag        : 1; //
    NvU32 output_flag_present_flag                     : 1; //
    NvU32 num_extra_slice_header_bits                  : 3; //  0..7 (normally 0)
    NvU32 sign_data_hiding_enabled_flag                : 1; //
    NvU32 cabac_init_present_flag                      : 1; //
    NvU32 num_ref_idx_l0_default_active                : 4; //  1..15
    NvU32 num_ref_idx_l1_default_active                : 4; //  1..15
    NvU32 init_qp                                      : 7; //  0..127, support higher bitdepth
    NvU32 constrained_intra_pred_flag                  : 1; //
    NvU32 transform_skip_enabled_flag                  : 1; //
    NvU32 cu_qp_delta_enabled_flag                     : 1; //
    NvU32 diff_cu_qp_delta_depth                       : 2; //  0..3
    NvU32 reserved4                                    : 5; //

    NvS8  pps_cb_qp_offset                             ; //  -12..12
    NvS8  pps_cr_qp_offset                             ; //  -12..12
    NvS8  pps_beta_offset                              ; //  -12..12
    NvS8  pps_tc_offset                                ; //  -12..12
    NvU32 pps_slice_chroma_qp_offsets_present_flag     : 1; //
    NvU32 weighted_pred_flag                           : 1; //
    NvU32 weighted_bipred_flag                         : 1; //
    NvU32 transquant_bypass_enabled_flag               : 1; //
    NvU32 tiles_enabled_flag                           : 1; // (redundant: = num_tile_columns_minus1!=0 || num_tile_rows_minus1!=0)
    NvU32 entropy_coding_sync_enabled_flag             : 1; //
    NvU32 num_tile_columns                             : 5; // 0..20
    NvU32 num_tile_rows                                : 5; // 0..22
    NvU32 loop_filter_across_tiles_enabled_flag        : 1; //
    NvU32 loop_filter_across_slices_enabled_flag       : 1; //
    NvU32 deblocking_filter_control_present_flag       : 1; //
    NvU32 deblocking_filter_override_enabled_flag      : 1; //
    NvU32 pps_deblocking_filter_disabled_flag          : 1; //
    NvU32 lists_modification_present_flag              : 1; //
    NvU32 log2_parallel_merge_level                    : 3; //  2..4
    NvU32 slice_segment_header_extension_present_flag  : 1; // (normally 0)
    NvU32 reserved5                                    : 6;

    // reference picture related
    NvU8  num_ref_frames;
    NvU8  reserved6;
    NvU16 longtermflag;                                // long term flag for refpiclist.bit 15 for picidx 0, bit 14 for picidx 1,...
    NvU8  initreflistidxl0[16];                        // :5, [refPicidx] 0..15
    NvU8  initreflistidxl1[16];                        // :5, [refPicidx] 0..15
    NvS16 RefDiffPicOrderCnts[16];                     // poc diff between current and reference pictures .[-128,127]
    // misc
    NvU8  IDR_picture_flag;                            // idr flag for current picture
    NvU8  RAP_picture_flag;                            // rap flag for current picture
    NvU8  curr_pic_idx;                                // current  picture store buffer index,used to derive the store addess of frame buffer and MV
    NvU8  pattern_id;                                  // used for dithering to select between 2 tables
    NvU16 sw_hdr_skip_length;                          // reference picture inititial related syntax elements(SE) bits in slice header.
                                                       // those SE only decoding once in driver,related bits will flush in HW
    NvU16 reserved7;
    NvU32 reserved8[6];

    NvU32   separate_colour_plane_flag : 1;
    NvU32   log2_max_pic_order_cnt_lsb_minus4 : 4;    //0~12
    NvU32   num_short_term_ref_pic_sets : 7 ;  //0~64
    NvU32   num_long_term_ref_pics_sps :  6;  //0~32
    NvU32   bBitParsingDisable : 1 ; //disable parsing
    NvU32   num_delta_pocs_of_rps_idx : 8;
    NvU32   long_term_ref_pics_present_flag : 1;
    NvU32   reserved_dxva : 4;
    NvU32   num_bits_short_term_ref_pics_in_slice;

    // New additions
    nvdec_hevc_pic_v1_s v1;
    nvdec_hevc_pic_v2_s v2;
    nvdec_hevc_pic_v3_s v3;
    NvU32 reserved9[3];

} nvdec_hevc_pic_s;

//hevc slice info class
typedef struct _hevc_slice_info_s {
    NvU32   first_flag    :1;//first slice(s) of frame,must valid for slice EC
    NvU32   err_flag      :1;//error slice(s) .optional info for EC
    NvU32   last_flag     :1;//last slice segment(s) of frame,this bit is must be valid when slice_strm_recfg_en==1 or slice_ec==1
    NvU32   conceal_partial_slice :1; // indicate do partial slice error conealment for packet loss case
    NvU32   available     :1; // indicate the slice bitstream is available.
    NvU32   reserved0     :7;
    NvU32   ctb_count     :20;// ctbs counter inside slice(s) .must valid for slice EC
    NvU32   bs_offset; //slice(s) bitstream offset in bitstream buffer (in byte unit)
    NvU32   bs_length; //slice(s) bitstream length. It is sum of aligned size and skip size and valid slice bitstream size.
    NvU16   start_ctbx; //slice start ctbx ,it's optional,HW can output it in previous slice decoding.
			            //but this is one check points for error
    NvU16   start_ctby; //slice start ctby
 } hevc_slice_info_s;


//hevc slice ctx class
//slice pos and next slice address
typedef struct  _slice_edge_ctb_pos_ctx_s {
    NvU32    next_slice_pos_ctbxy;         //2d address in raster scan
    NvU32    next_slice_segment_addr;      //1d address in  tile scan
}slice_edge_ctb_pos_ctx_s;

//	next slice's first ctb located tile related information
typedef struct  _slice_edge_tile_ctx_s {
    NvU32    tileInfo1;// Misc tile info includes tile width and tile height and tile col and tile row
    NvU32    tileInfo2;// Misc tile info includes tile start ctbx and start ctby and tile index
    NvU32    tileInfo3;// Misc tile info includes  ctb pos inside tile
} slice_edge_tile_ctx_s;

//frame level stats
typedef struct  _slice_edge_stats_ctx_s {
    NvU32    frame_status_intra_cnt;// frame stats for intra block count
    NvU32    frame_status_inter_cnt;// frame stats for inter block count
    NvU32    frame_status_skip_cnt;// frame stats for skip block count
    NvU32    frame_status_fwd_mvx_cnt;// frame stats for sum of  abs fwd mvx
    NvU32    frame_status_fwd_mvy_cnt;// frame stats for sum of  abs fwd mvy
    NvU32    frame_status_bwd_mvx_cnt;// frame stats for sum of  abs bwd mvx
    NvU32    frame_status_bwd_mvy_cnt;// frame stats for sum of  abs bwd mvy
    NvU32    frame_status_mv_cnt_ext;// extension bits of  sum of abs mv to keep full precision.
}slice_edge_stats_ctx_s;

//ctx of vpc_edge unit for tile left
typedef struct  _slice_vpc_edge_ctx_s {
    NvU32   reserved;
}slice_vpc_edge_ctx_s;

//ctx of vpc_main unit
typedef struct  _slice_vpc_main_ctx_s {
    NvU32   reserved;
} slice_vpc_main_ctx_s;

//hevc slice edge ctx class
typedef struct  _slice_edge_ctx_s {
//ctb pos
slice_edge_ctb_pos_ctx_s  slice_ctb_pos_ctx;
// stats
slice_edge_stats_ctx_s slice_stats_ctx;
// tile info
slice_edge_tile_ctx_s    slice_tile_ctx;
//vpc_edge
slice_vpc_edge_ctx_s  slice_vpc_edge_ctx;
//vpc_main
slice_vpc_main_ctx_s  slice_vpc_main_ctx;
} slice_edge_ctx_s;

//vp9

typedef struct _nvdec_vp9_pic_v1_s
{
    NvU32   Vp9FltAboveOffset;  // filter above offset respect to filter buffer, 256 bytes unit
    NvU32   external_ref_mem_dis :  1;
    NvU32   bit_depth            :  4;
    NvU32   error_recovery_start_pos :  2;       //0: from start of frame, 1: from start of slice segment, 2: from error detected ctb, 3: reserved
    NvU32   error_external_mv_en :  1;
    NvU32   Reserved0            : 24;
} nvdec_vp9_pic_v1_s;

enum VP9_FRAME_SFC_ID
{
    VP9_LAST_FRAME_SFC = 0,
    VP9_GOLDEN_FRAME_SFC,
    VP9_ALTREF_FRAME_SFC,
    VP9_CURR_FRAME_SFC
};

typedef struct _nvdec_vp9_pic_s
{
    NvU32   reserved0[15];

    // Driver may or may not use based upon need.
    // If 0 then default value of 1<<27 = 298ms @ 450MHz will be used in ucode.
    // Driver can send this value based upon resolution using the formula:
    // gptimer_timeout_value = 3 * (cycles required for one frame)
    NvU32 gptimer_timeout_value;

    //general
    NvU8  tileformat                 : 2 ;   // 0: TBL; 1: KBL;
    NvU8  gob_height                 : 3 ;   // Set GOB height, 0: GOB_2, 1: GOB_4, 2: GOB_8, 3: GOB_16, 4: GOB_32 (NVDEC3 onwards)
    NvU8  reserverd_surface_format   : 3 ;
    NvU8  reserved1[3];
    NvU32 Vp9BsdCtrlOffset;                           // bsd buffer offset respect to filter buffer ,256 bytes unit .


    //ref_last dimensions
    NvU16  ref0_width;    //ref_last coded width
    NvU16  ref0_height;   //ref_last coded height
    NvU16  ref0_stride[2];    //ref_last stride

    //ref_golden dimensions
    NvU16  ref1_width;    //ref_golden coded width
    NvU16  ref1_height;   //ref_golden coded height
    NvU16  ref1_stride[2];    //ref_golden stride

    //ref_alt dimensions
    NvU16  ref2_width;    //ref_alt coded width
    NvU16  ref2_height;   //ref_alt coded height
    NvU16  ref2_stride[2];    //ref_alt stride


    /* Current frame dimensions */
    NvU16  width;    //pic width
    NvU16  height;   //pic height
    NvU16  framestride[2];   // frame buffer stride for luma and chroma

    NvU8   keyFrame  :1;
    NvU8   prevIsKeyFrame:1;
    NvU8   resolutionChange:1;
    NvU8   errorResilient:1;
    NvU8   prevShowFrame:1;
    NvU8   intraOnly:1;
    NvU8   reserved2 : 2;

    NvU8   reserved3[3];

    NvU8   refFrameSignBias[4];
    NvS8   loopFilterLevel;//6 bits
    NvS8   loopFilterSharpness;//3 bits

    /* Quantization parameters */
    NvU8   qpYAc;
    NvS8   qpYDc;
    NvS8   qpChAc;
    NvS8   qpChDc;

    /* From here down, frame-to-frame persisting stuff */

    NvS8   lossless;
    NvS8   transform_mode;
    NvS8   allow_high_precision_mv;
    NvS8   mcomp_filter_type;
    NvS8   comp_pred_mode;
    NvS8   comp_fixed_ref;
    NvS8   comp_var_ref[2];
    NvS8   log2_tile_columns;
    NvS8   log2_tile_rows;

    /* Segment and macroblock specific values */
    NvU8   segmentEnabled;
    NvU8   segmentMapUpdate;
    NvU8   segmentMapTemporalUpdate;
    NvU8   segmentFeatureMode; /* ABS data or delta data */
    NvU8   segmentFeatureEnable[8][4];
    NvS16  segmentFeatureData[8][4];
    NvS8   modeRefLfEnabled;
    NvS8   mbRefLfDelta[4];
    NvS8   mbModeLfDelta[2];
    NvS8   reserved5;            // for alignment

    // New additions
    nvdec_vp9_pic_v1_s v1;
    NvU32 reserved6[3];

} nvdec_vp9_pic_s;

#define NVDEC_VP9HWPAD(x, y) NvU8 x[y]

typedef struct {
    /* last bytes of address 41 */
    NvU8 joints[3];
    NvU8 sign[2];
    /* address 42 */
    NvU8 class0[2][1];
    NvU8 fp[2][3];
    NvU8 class0_hp[2];
    NvU8 hp[2];
    NvU8 classes[2][10];
    /* address 43 */
    NvU8 class0_fp[2][2][3];
    NvU8 bits[2][10];

} nvdec_nmv_context;

typedef struct {
    NvU32 joints[4];
    NvU32 sign[2][2];
    NvU32 classes[2][11];
    NvU32 class0[2][2];
    NvU32 bits[2][10][2];
    NvU32 class0_fp[2][2][4];
    NvU32 fp[2][4];
    NvU32 class0_hp[2][2];
    NvU32 hp[2][2];

} nvdec_nmv_context_counts;

/* Adaptive entropy contexts, padding elements are added to have
 * 256 bit aligned tables for HW access.
 * Compile with TRACE_PROB_TABLES to print bases for each table. */
typedef struct nvdec_vp9AdaptiveEntropyProbs_s
{
    /* address 32 */
    NvU8 inter_mode_prob[7][4];
    NvU8 intra_inter_prob[4];

    /* address 33 */
    NvU8 uv_mode_prob[10][8];
    NvU8 tx8x8_prob[2][1];
    NvU8 tx16x16_prob[2][2];
    NvU8 tx32x32_prob[2][3];
    NvU8 sb_ymode_probB[4][1];
    NvU8 sb_ymode_prob[4][8];

    /* address 37 */
    NvU8 partition_prob[2][16][4];

    /* address 41 */
    NvU8 uv_mode_probB[10][1];
    NvU8 switchable_interp_prob[4][2];
    NvU8 comp_inter_prob[5];
    NvU8 mbskip_probs[3];
    NVDEC_VP9HWPAD(pad1, 1);

    nvdec_nmv_context nmvc;

    /* address 44 */
    NvU8 single_ref_prob[5][2];
    NvU8 comp_ref_prob[5];
    NVDEC_VP9HWPAD(pad2, 17);

    /* address 45 */
    NvU8 probCoeffs[2][2][6][6][4];
    NvU8 probCoeffs8x8[2][2][6][6][4];
    NvU8 probCoeffs16x16[2][2][6][6][4];
    NvU8 probCoeffs32x32[2][2][6][6][4];

} nvdec_vp9AdaptiveEntropyProbs_t;

/* Entropy contexts */
typedef struct nvdec_vp9EntropyProbs_s
{
    /* Default keyframe probs */
    /* Table formatted for 256b memory, probs 0to7 for all tables followed by
     * probs 8toN for all tables.
     * Compile with TRACE_PROB_TABLES to print bases for each table. */

    NvU8 kf_bmode_prob[10][10][8];

    /* Address 25 */
    NvU8 kf_bmode_probB[10][10][1];
    NvU8 ref_pred_probs[3];
    NvU8 mb_segment_tree_probs[7];
    NvU8 segment_pred_probs[3];
    NvU8 ref_scores[4];
    NvU8 prob_comppred[2];
    NVDEC_VP9HWPAD(pad1, 9);

    /* Address 29 */
    NvU8 kf_uv_mode_prob[10][8];
    NvU8 kf_uv_mode_probB[10][1];
    NVDEC_VP9HWPAD(pad2, 6);

    nvdec_vp9AdaptiveEntropyProbs_t a;    /* Probs with backward adaptation */

} nvdec_vp9EntropyProbs_t;

/* Counters for adaptive entropy contexts */
typedef struct nvdec_vp9EntropyCounts_s
{
    NvU32 inter_mode_counts[7][3][2];
    NvU32 sb_ymode_counts[4][10];
    NvU32 uv_mode_counts[10][10];
    NvU32 partition_counts[16][4];
    NvU32 switchable_interp_counts[4][3];
    NvU32 intra_inter_count[4][2];
    NvU32 comp_inter_count[5][2];
    NvU32 single_ref_count[5][2][2];
    NvU32 comp_ref_count[5][2];
    NvU32 tx32x32_count[2][4];
    NvU32 tx16x16_count[2][3];
    NvU32 tx8x8_count[2][2];
    NvU32 mbskip_count[3][2];

    nvdec_nmv_context_counts nmvcount;

    NvU32 countCoeffs[2][2][6][6][4];
    NvU32 countCoeffs8x8[2][2][6][6][4];
    NvU32 countCoeffs16x16[2][2][6][6][4];
    NvU32 countCoeffs32x32[2][2][6][6][4];

    NvU32 countEobs[4][2][2][6][6];

} nvdec_vp9EntropyCounts_t;

typedef struct _nvdec_display_param_s
{
    NvU32 enableTFOutput    : 1; //=1, enable dbfdma to output the display surface; if disable, then the following configure on tf is useless.
    //remap for VC1
    NvU32 VC1MapYFlag       : 1;
    NvU32 MapYValue         : 3;
    NvU32 VC1MapUVFlag      : 1;
    NvU32 MapUVValue        : 3;
    //tf
    NvU32 OutStride         : 8;
    NvU32 TilingFormat      : 3;
    NvU32 OutputStructure   : 1; //(0=frame, 1=field)
    NvU32 reserved0         :11;
    NvS32 OutputTop[2];                   // in units of 256
    NvS32 OutputBottom[2];                // in units of 256
    //histogram
    NvU32 enableHistogram   : 1; // enable histogram info collection.
    NvU32 HistogramStartX   :12; // start X of Histogram window
    NvU32 HistogramStartY   :12; // start Y of Histogram window
    NvU32 reserved1         : 7;
    NvU32 HistogramEndX     :12; // end X of Histogram window
    NvU32 HistogramEndY     :12; // end y of Histogram window
    NvU32 reserved2         : 8;
} nvdec_display_param_s;  // size 0x1c bytes

// H.264
typedef struct _nvdec_dpb_entry_s  // 16 bytes
{
    NvU32 index          : 7;    // uncompressed frame buffer index
    NvU32 col_idx        : 5;    // index of associated co-located motion data buffer
    NvU32 state          : 2;    // bit1(state)=1: top field used for reference, bit1(state)=1: bottom field used for reference
    NvU32 is_long_term   : 1;    // 0=short-term, 1=long-term
    NvU32 not_existing   : 1;    // 1=marked as non-existing
    NvU32 is_field       : 1;    // set if unpaired field or complementary field pair
    NvU32 top_field_marking : 4;
    NvU32 bottom_field_marking : 4;
    NvU32 output_memory_layout : 1;  // Set according to picture level output NV12/NV24 setting.
    NvU32 reserved       : 6;
    NvU32 FieldOrderCnt[2];      // : 2*32 [top/bottom]
    NvU32 FrameIdx;                       // : 16   short-term: FrameNum (16 bits), long-term: LongTermFrameIdx (4 bits)
} nvdec_dpb_entry_s;

typedef struct _nvdec_h264_pic_s
{
    NvU32 reserved0[13];
    NvU8  eos[16];
    NvU8  explicitEOSPresentFlag;
    NvU8  hint_dump_en; //enable COLOMV surface dump for all frames, which includes hints of "MV/REFIDX/QP/CBP/MBPART/MBTYPE", nvbug: 200212874
    NvU8  reserved1[2];
    NvU32 stream_len;
    NvU32 slice_count;
    NvU32 mbhist_buffer_size;     // to pass buffer size of MBHIST_BUFFER

    // Driver may or may not use based upon need.
    // If 0 then default value of 1<<27 = 298ms @ 450MHz will be used in ucode.
    // Driver can send this value based upon resolution using the formula:
    // gptimer_timeout_value = 3 * (cycles required for one frame)
    NvU32 gptimer_timeout_value;

    // Fields from msvld_h264_seq_s
    NvS32 log2_max_pic_order_cnt_lsb_minus4;
    NvS32 delta_pic_order_always_zero_flag;
    NvS32 frame_mbs_only_flag;
    NvU32 PicWidthInMbs;
    NvU32 FrameHeightInMbs;

    NvU32 tileFormat                 : 2 ;   // 0: TBL; 1: KBL;
    NvU32 gob_height                 : 3 ;   // Set GOB height, 0: GOB_2, 1: GOB_4, 2: GOB_8, 3: GOB_16, 4: GOB_32 (NVDEC3 onwards)
    NvU32 reserverd_surface_format   : 27;

    // Fields from msvld_h264_pic_s
    NvU32 entropy_coding_mode_flag;
    NvS32 pic_order_present_flag;
    NvS32 num_ref_idx_l0_active_minus1;
    NvS32 num_ref_idx_l1_active_minus1;
    NvS32 deblocking_filter_control_present_flag;
    NvS32 redundant_pic_cnt_present_flag;
    NvU32 transform_8x8_mode_flag;

    // Fields from mspdec_h264_picture_setup_s
    NvU32 pitch_luma;                    // Luma pitch
    NvU32 pitch_chroma;                  // chroma pitch

    NvU32 luma_top_offset;               // offset of luma top field in units of 256
    NvU32 luma_bot_offset;               // offset of luma bottom field in units of 256
    NvU32 luma_frame_offset;             // offset of luma frame in units of 256
    NvU32 chroma_top_offset;             // offset of chroma top field in units of 256
    NvU32 chroma_bot_offset;             // offset of chroma bottom field in units of 256
    NvU32 chroma_frame_offset;           // offset of chroma frame in units of 256
    NvU32 HistBufferSize;                // in units of 256

    NvU32 MbaffFrameFlag           : 1;  //
    NvU32 direct_8x8_inference_flag: 1;  //
    NvU32 weighted_pred_flag       : 1;  //
    NvU32 constrained_intra_pred_flag:1; //
    NvU32 ref_pic_flag             : 1;  // reference picture (nal_ref_idc != 0)
    NvU32 field_pic_flag           : 1;  //
    NvU32 bottom_field_flag        : 1;  //
    NvU32 second_field             : 1;  // second field of complementary reference field
    NvU32 log2_max_frame_num_minus4: 4;  //  (0..12)
    NvU32 chroma_format_idc        : 2;  //
    NvU32 pic_order_cnt_type       : 2;  //  (0..2)
    NvS32 pic_init_qp_minus26               : 6;  // : 6 (-26..+25)
    NvS32 chroma_qp_index_offset            : 5;  // : 5 (-12..+12)
    NvS32 second_chroma_qp_index_offset     : 5;  // : 5 (-12..+12)

    NvU32 weighted_bipred_idc      : 2;  // : 2 (0..2)
    NvU32 CurrPicIdx               : 7;  // : 7  uncompressed frame buffer index
    NvU32 CurrColIdx               : 5;  // : 5  index of associated co-located motion data buffer
    NvU32 frame_num                : 16; //
    NvU32 frame_surfaces           : 1;  // frame surfaces flag
    NvU32 output_memory_layout     : 1;  // 0: NV12; 1:NV24. Field pair must use the same setting.

    NvS32 CurrFieldOrderCnt[2];                   // : 32 [Top_Bottom], [0]=TopFieldOrderCnt, [1]=BottomFieldOrderCnt
    nvdec_dpb_entry_s dpb[16];
    NvU8  WeightScale[6][4][4];         // : 6*4*4*8 in raster scan order (not zig-zag order)
    NvU8  WeightScale8x8[2][8][8];      // : 2*8*8*8 in raster scan order (not zig-zag order)

    // mvc setup info, must be zero if not mvc
    NvU8 num_inter_view_refs_lX[2];         // number of inter-view references
    NvS8 reserved2[14];                               // reserved for alignment
    NvS8 inter_view_refidx_lX[2][16];         // DPB indices (must also be marked as long-term)

    // lossless decode (At the time of writing this manual, x264 and JM encoders, differ in Intra_8x8 reference sample filtering)
    NvU32 lossless_ipred8x8_filter_enable        : 1;       // = 0, skips Intra_8x8 reference sample filtering, for vertical and horizontal predictions (x264 encoded streams); = 1, filter Intra_8x8 reference samples (JM encoded streams)
    NvU32 qpprime_y_zero_transform_bypass_flag   : 1;       // determines the transform bypass mode
    NvU32 reserved3                              : 30;      // kept for alignment; may be used for other parameters

    nvdec_display_param_s displayPara;
    NvU32 reserved4[3];

} nvdec_h264_pic_s;

// VC-1 Scratch buffer
typedef enum _vc1_fcm_e
{
    FCM_PROGRESSIVE = 0,
    FCM_FRAME_INTERLACE = 2,
    FCM_FIELD_INTERLACE = 3
} vc1_fcm_e;

typedef enum _syntax_vc1_ptype_e
{
    PTYPE_I       = 0,
    PTYPE_P       = 1,
    PTYPE_B       = 2,
    PTYPE_BI      = 3, //PTYPE_BI is not used to config register NV_CNVDEC_VLD_PIC_INFO_COMMON. field NV_CNVDEC_VLD_PIC_INFO_COMMON_PIC_CODING_VC1 is only 2 bits. I and BI pictures are configured with same value. Please refer to manual.
    PTYPE_SKIPPED = 4
} syntax_vc1_ptype_e;

// 7.1.1.32, Table 46 etc.
enum vc1_mvmode_e
{
    MVMODE_MIXEDMV                = 0,
    MVMODE_1MV                    = 1,
    MVMODE_1MV_HALFPEL            = 2,
    MVMODE_1MV_HALFPEL_BILINEAR   = 3,
    MVMODE_INTENSITY_COMPENSATION = 4
};

// 9.1.1.42, Table 105
typedef enum _vc1_fptype_e
{
    FPTYPE_I_I = 0,
    FPTYPE_I_P,
    FPTYPE_P_I,
    FPTYPE_P_P,
    FPTYPE_B_B,
    FPTYPE_B_BI,
    FPTYPE_BI_B,
    FPTYPE_BI_BI
} vc1_fptype_e;

// Table 43 (7.1.1.31.2)
typedef enum _vc1_dqprofile_e
{
    DQPROFILE_ALL_FOUR_EDGES  = 0,
    DQPROFILE_DOUBLE_EDGE     = 1,
    DQPROFILE_SINGLE_EDGE     = 2,
    DQPROFILE_ALL_MACROBLOCKS = 3
} vc1_dqprofile_e;

typedef struct _nvdec_vc1_pic_s
{
    NvU32 reserved0[13];
    NvU8  eos[16];                    // to pass end of stream data separately if not present in bitstream surface
    NvU8  prefixStartCode[4];         // used for dxva to pass prefix start code.
    NvU32 bitstream_offset;           // offset in words from start of bitstream surface if there is gap.
    NvU8  explicitEOSPresentFlag;     // to indicate that eos[] is used for passing end of stream data.
    NvU8  reserved1[3];
    NvU32 stream_len;
    NvU32 slice_count;
    NvU32 scratch_pic_buffer_size;

    // Driver may or may not use based upon need.
    // If 0 then default value of 1<<27 = 298ms @ 450MHz will be used in ucode.
    // Driver can send this value based upon resolution using the formula:
    // gptimer_timeout_value = 3 * (cycles required for one frame)
    NvU32 gptimer_timeout_value;

    // Fields from vc1_seq_s
    NvU16 FrameWidth;     // actual frame width
    NvU16 FrameHeight;    // actual frame height

    NvU8 profile;        // 1 = SIMPLE or MAIN, 2 = ADVANCED
    NvU8 postprocflag;
    NvU8 pulldown;
    NvU8 interlace;

    NvU8 tfcntrflag;
    NvU8 finterpflag;
    NvU8 psf;
    NvU8 tileFormat                 : 2 ;   // 0: TBL; 1: KBL;
    NvU8 gob_height                 : 3 ;   // Set GOB height, 0: GOB_2, 1: GOB_4, 2: GOB_8, 3: GOB_16, 4: GOB_32 (NVDEC3 onwards)
    NvU8 reserverd_surface_format   : 3 ;

    // simple,main
    NvU8 multires;
    NvU8 syncmarker;
    NvU8 rangered;
    NvU8 maxbframes;

    // Fields from vc1_entrypoint_s
    NvU8 dquant;
    NvU8 panscan_flag;
    NvU8 refdist_flag;
    NvU8 quantizer;

    NvU8 extended_mv;
    NvU8 extended_dmv;
    NvU8 overlap;
    NvU8 vstransform;

    // Fields from vc1_scratch_s
    NvS8 refdist;
    NvS8 reserved2[3];               // for alignment

    // Fields from vld_vc1_pic_s
    vc1_fcm_e fcm;
    syntax_vc1_ptype_e ptype;
    NvS32 tfcntr;
    NvS32 rptfrm;
    NvS32 tff;
    NvS32 rndctrl;
    NvS32 pqindex;
    NvS32 halfqp;
    NvS32 pquantizer;
    NvS32 postproc;
    NvS32 condover;
    NvS32 transacfrm;
    NvS32 transacfrm2;
    NvS32 transdctab;
    NvS32 pqdiff;
    NvS32 abspq;
    NvS32 dquantfrm;
    vc1_dqprofile_e dqprofile;
    NvS32 dqsbedge;
    NvS32 dqdbedge;
    NvS32 dqbilevel;
    NvS32 mvrange;
    enum vc1_mvmode_e mvmode;
    enum vc1_mvmode_e mvmode2;
    NvS32 lumscale;
    NvS32 lumshift;
    NvS32 mvtab;
    NvS32 cbptab;
    NvS32 ttmbf;
    NvS32 ttfrm;
    NvS32 bfraction;
    vc1_fptype_e fptype;
    NvS32 numref;
    NvS32 reffield;
    NvS32 dmvrange;
    NvS32 intcompfield;
    NvS32 lumscale1;
    NvS32 lumshift1;
    NvS32 lumscale2;
    NvS32 lumshift2;
    NvS32 mbmodetab;
    NvS32 imvtab;
    NvS32 icbptab;
    NvS32 fourmvbptab;
    NvS32 fourmvswitch;
    NvS32 intcomp;
    NvS32 twomvbptab;
    // simple,main
    NvS32 rangeredfrm;

    // Fields from pdec_vc1_pic_s
    NvU32   HistBufferSize;                  // in units of 256
    // frame buffers
    NvU32   FrameStride[2];                  // [y_c]
    NvU32   luma_top_offset;                 // offset of luma top field in units of 256
    NvU32   luma_bot_offset;                 // offset of luma bottom field in units of 256
    NvU32   luma_frame_offset;               // offset of luma frame in units of 256
    NvU32   chroma_top_offset;               // offset of chroma top field in units of 256
    NvU32   chroma_bot_offset;               // offset of chroma bottom field in units of 256
    NvU32   chroma_frame_offset;             // offset of chroma frame in units of 256

    NvU16 CodedWidth;                      // entrypoint specific
    NvU16 CodedHeight;                     // entrypoint specific

    NvU8  loopfilter;                      // entrypoint specific
    NvU8  fastuvmc;                        // entrypoint specific
    NvU8  output_memory_layout;            // picture specific
    NvU8  ref_memory_layout[2];            // picture specific 0: fwd, 1: bwd
    NvU8  reserved3[3];                    // for alignment

    nvdec_display_param_s displayPara;
    NvU32 reserved4[3];

} nvdec_vc1_pic_s;

// MPEG-2
typedef struct _nvdec_mpeg2_pic_s
{
    NvU32 reserved0[13];
    NvU8  eos[16];
    NvU8  explicitEOSPresentFlag;
    NvU8  reserved1[3];
    NvU32 stream_len;
    NvU32 slice_count;

    // Driver may or may not use based upon need.
    // If 0 then default value of 1<<27 = 298ms @ 450MHz will be used in ucode.
    // Driver can send this value based upon resolution using the formula:
    // gptimer_timeout_value = 3 * (cycles required for one frame)
    NvU32 gptimer_timeout_value;

    // Fields from vld_mpeg2_seq_pic_info_s
    NvU16 FrameWidth;                   // actual frame width
    NvU16 FrameHeight;                  // actual frame height
    NvU8  picture_structure;            // 0 => Reserved, 1 => Top field, 2 => Bottom field, 3 => Frame picture. Table 6-14.
    NvU8  picture_coding_type;          // 0 => Forbidden, 1 => I, 2 => P, 3 => B, 4 => D (for MPEG-2). Table 6-12.
    NvU8  intra_dc_precision;           // 0 => 8 bits, 1=> 9 bits, 2 => 10 bits, 3 => 11 bits. Table 6-13.
    NvS8  frame_pred_frame_dct;         // as in section 6.3.10
    NvS8  concealment_motion_vectors;   // as in section 6.3.10
    NvS8  intra_vlc_format;             // as in section 6.3.10
    NvU8  tileFormat                 : 2 ;   // 0: TBL; 1: KBL;
    NvU8  gob_height                 : 3 ;   // Set GOB height, 0: GOB_2, 1: GOB_4, 2: GOB_8, 3: GOB_16, 4: GOB_32 (NVDEC3 onwards)
    NvU8  reserverd_surface_format   : 3 ;

    NvS8 reserved2;                     // always 0
    NvS8 f_code[4];                  // as in section 6.3.10

    // Fields from pdec_mpeg2_picture_setup_s
    NvU16 PicWidthInMbs;
    NvU16 FrameHeightInMbs;
    NvU32 pitch_luma;
    NvU32 pitch_chroma;
    NvU32 luma_top_offset;
    NvU32 luma_bot_offset;
    NvU32 luma_frame_offset;
    NvU32 chroma_top_offset;
    NvU32 chroma_bot_offset;
    NvU32 chroma_frame_offset;
    NvU32 HistBufferSize;
    NvU16 output_memory_layout;
    NvU16 alternate_scan;
    NvU16 secondfield;
    NvU16 rounding_type;
    NvU32 MbInfoSizeInBytes;
    NvU32 q_scale_type;
    NvU32 top_field_first;
    NvU32 full_pel_fwd_vector;
    NvU32 full_pel_bwd_vector;
    NvU8  quant_mat_8x8intra[64];
    NvU8  quant_mat_8x8nonintra[64];
    NvU32 ref_memory_layout[2]; //0:for fwd; 1:for bwd

    nvdec_display_param_s displayPara;
    NvU32 reserved3[3];

} nvdec_mpeg2_pic_s;

// MPEG-4
typedef struct _nvdec_mpeg4_pic_s
{
    NvU32 reserved1[13];
    NvU8  eos[16];
    NvU8  explicitEOSPresentFlag;
    NvU8  reserved2[3];     // for alignment
    NvU32 stream_len;
    NvU32 slice_count;
    NvU32 scratch_pic_buffer_size;

    // Driver may or may not use based upon need.
    // If 0 then default value of 1<<27 = 298ms @ 450MHz will be used in ucode.
    // Driver can send this value based upon resolution using the formula:
    // gptimer_timeout_value = 3 * (cycles required for one frame)
    NvU32 gptimer_timeout_value;

    // Fields from vld_mpeg4_seq_s
    NvS16 FrameWidth;                     // :13 video_object_layer_width
    NvS16 FrameHeight;                    // :13 video_object_layer_height
    NvU8  vop_time_increment_bitcount;    // : 5 1..16
    NvS8  resync_marker_disable;          // : 1
    NvU8  tileFormat                 : 2 ;   // 0: TBL; 1: KBL;
    NvU8  gob_height                 : 3 ;   // Set GOB height, 0: GOB_2, 1: GOB_4, 2: GOB_8, 3: GOB_16, 4: GOB_32 (NVDEC3 onwards)
    NvU8  reserverd_surface_format   : 3 ;
    NvS8  reserved3;                      // for alignment

    // Fields from pdec_mpeg4_picture_setup_s
    NvS32 width;                              // : 13
    NvS32 height;                             // : 13

    NvU32 FrameStride[2];            // [y_c]
    NvU32 luma_top_offset;           // offset of luma top field in units of 256
    NvU32 luma_bot_offset;           // offset of luma bottom field in units of 256
    NvU32 luma_frame_offset;         // offset of luma frame in units of 256
    NvU32 chroma_top_offset;         // offset of chroma top field in units of 256
    NvU32 chroma_bot_offset;         // offset of chroma bottom field in units of 256
    NvU32 chroma_frame_offset;       // offset of chroma frame in units of 256

    NvU32 HistBufferSize;            // in units of 256, History buffer size

    NvS32 trd[2];                             // : 16, temporal reference frame distance (only needed for B-VOPs)
    NvS32 trb[2];                             // : 16, temporal reference B-VOP distance from fwd reference frame (only needed for B-VOPs)

    NvS32 divx_flags;                         // : 16 (bit 0: DivX interlaced chroma rounding, bit 1: Divx 4 boundary padding, bit 2: Divx IDCT)

    NvS16 vop_fcode_forward;                // : 1...7
    NvS16 vop_fcode_backward;               // : 1...7

    NvU8 interlaced;               // : 1
    NvU8 quant_type;               // : 1
    NvU8 quarter_sample;           // : 1
    NvU8 short_video_header;       // : 1

    NvU8 curr_output_memory_layout; // : 1 0:NV12; 1:NV24
    NvU8 ptype;                    // picture type: 0 for PTYPE_I, 1 for PTYPE_P, 2 for PTYPE_B, 3 for PTYPE_BI, 4 for PTYPE_SKIPPED
    NvU8 rnd;                      // : 1, rounding mode
    NvU8 alternate_vertical_scan_flag; // : 1

    NvU8 top_field_flag;           // : 1
    NvU8 reserved0[3];             // alignment purpose

    NvU8 intra_quant_mat[64];      // : 64*8
    NvU8 nonintra_quant_mat[64];   // : 64*8
    NvU8 ref_memory_layout[2];    //0:for fwd; 1:for bwd
    NvU8 reserved1[34];            // 256 byte alignemnt till now

    nvdec_display_param_s displayPara;

} nvdec_mpeg4_pic_s;

// VP8
enum VP8_FRAME_TYPE
{
    VP8_KEYFRAME = 0,
    VP8_INTERFRAME = 1
};

enum VP8_FRAME_SFC_ID
{
    VP8_GOLDEN_FRAME_SFC = 0,
    VP8_ALTREF_FRAME_SFC,
    VP8_LAST_FRAME_SFC,
    VP8_CURR_FRAME_SFC
};

typedef struct _nvdec_vp8_pic_s
{
    NvU32 reserved0[13];

    // Driver may or may not use based upon need.
    // If 0 then default value of 1<<27 = 298ms @ 450MHz will be used in ucode.
    // Driver can send this value based upon resolution using the formula:
    // gptimer_timeout_value = 3 * (cycles required for one frame)
    NvU32 gptimer_timeout_value;

    NvU16 FrameWidth;     // actual frame width
    NvU16 FrameHeight;    // actual frame height

    NvBool keyFrame;        // 1: key frame; 0: not
    NvU8   version;
    NvU8   tileFormat                 : 2 ;   // 0: TBL; 1: KBL;
    NvU8   gob_height                 : 3 ;   // Set GOB height, 0: GOB_2, 1: GOB_4, 2: GOB_8, 3: GOB_16, 4: GOB_32 (NVDEC3 onwards)
    NvU8   reserverd_surface_format   : 3 ;
    NvU8   errorConcealOn;  // 1: error conceal on; 0: off

    NvU32  firstPartSize;   // the size of first partition(frame header and mb header partition)

    // ctx
    NvU32   HistBufferSize;                  // in units of 256
    NvU32   VLDBufferSize;                   // in units of 1
    // current frame buffers
    NvU32   FrameStride[2];                  // [y_c]
    NvU32   luma_top_offset;                 // offset of luma top field in units of 256
    NvU32   luma_bot_offset;                 // offset of luma bottom field in units of 256
    NvU32   luma_frame_offset;               // offset of luma frame in units of 256
    NvU32   chroma_top_offset;               // offset of chroma top field in units of 256
    NvU32   chroma_bot_offset;               // offset of chroma bottom field in units of 256
    NvU32   chroma_frame_offset;             // offset of chroma frame in units of 256

    nvdec_display_param_s displayPara;

    // decode picture buffere related
    NvS8 current_output_memory_layout;
    NvS8 output_memory_layout[3];  // output NV12/NV24 setting. item 0:golden; 1: altref; 2: last

    NvU8 segmentation_feature_data_update;
    NvU8 reserved1[3];

    // ucode return result
    NvU32 resultValue;
    NvU32 partition_offset[8];

    NvU32 reserved2[3];

} nvdec_vp8_pic_s; // size is 0xc0

#endif // __DRV_NVDEC_H_
