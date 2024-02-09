/*
 * SPDX-FileCopyrightText: Copyright (c) 1993-2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#ifndef NVJPG_DRV_H
#define NVJPG_DRV_H
// 
// CLASS NV_C9D1_NVJPG
//
// NVJPG is the combination of JPEG decoder and encoder, it will support baseline sequential profile.
// In the encoder side, it support: a. 420 pitch linear format, b. programable huffman/qunat table, ... etc.
// In the decoder side, it support: a. 400/420/422/444 decoding, b. YUV2RGB, c. Power2Scale: 1/2, 1/4, 1/8, d.ChromaSumbSample ... etc.
// ===================

#ifdef NV_NVJPG_SUPPORT_ENC
typedef struct
{
    unsigned int    chroma_format       :  2;   // 0: 444; 1: 422H; 2:422V; 3:420
    unsigned int    mcu_width           : 12;   // picture width  in mcu
    unsigned int    mcu_height          : 12;   // picture height in mcu
    unsigned int    stuff_disable       :  1;   // 
    unsigned int    rsvd                :  5;   //
}ENC_TOP_CTL_s;

typedef struct
{
    unsigned int    restart_interval    :  16;  // 
    unsigned int    rsvd                :  16;  // 
}ENC_TOP_RST_INTERVAL_s;

typedef struct
{
    unsigned short  quantLumaFactor[64];        // luma quantize factor table, arranged in horizontal scan order, (1<<15)/quantLuma
    unsigned short  quantChromaFactor[64];      // chroma quantize factor table, arranged in horizontal scan order, (1<<15)/quantLuma
}ENC_FDCT_QT_WDATA_s;

typedef struct
{
    unsigned int    prequant_enable     :   1;  // pre quant trunction enabled flag
    unsigned int    prequant_index      :   8;  // pre_quant threshold index [1 ~ 63] 
    unsigned int    prequant_threshold  :  21;  // threshold magnitude
    unsigned int    rsvd                :   2;  //
}ENC_FDCT_RATECTRL_s;

// huffuman table: 
// huffuman table is organized in symbol value order, each table item include 2 field, codeWord length, and codeWord value
#define DCVALUEITEM 12
#define ACVALUEITEM 176 // in fact, only 162 items are used in baseline sequential profile.
typedef struct
{
    unsigned short length;             // 4 bit, code word length 
    unsigned short value;              // 16 bit, code word value
}huffman_symbol_s;

typedef struct
{
    // huffman tables
    huffman_symbol_s hfDcLuma[DCVALUEITEM];     // dc luma huffman table, arranged in symbol increase order, encoder can directly index and use
    huffman_symbol_s hfAcLuma[ACVALUEITEM];     // ac luma huffman table, arranged in symbol increase order, encoder can directly index and use
    huffman_symbol_s hfDcChroma[DCVALUEITEM];   // dc chroma huffman table, arranged in symbol increase order, encoder can directly index and use
    huffman_symbol_s hfAcChroma[ACVALUEITEM];   // ac chroma huffman table, arranged in symbol increase order, encoder can directly index and use
}ENC_VLE_HT_WDATA_s;

typedef struct
{
    unsigned int    rate_control        :   2;  // RC: 0:disable; 1:block-base; others: reserve
    unsigned int    luma_target_bits    :  11;  // target luma bits per block, [0 ~ (1<<11)-1]
    unsigned int    chroma_target_bits  :  11;  // target chroma bits per block, [0 ~ (1<<11)-1]
    unsigned int    rsvd                :   8;  //
}ENC_VLE_RATECTRL_s;
#endif

typedef struct
{
    unsigned int input_type             :   3;
    unsigned int chroma_mode            :   3;
    unsigned int width_in_8x8           :  12;
    unsigned int height_in_8x8          :  12;
    unsigned int power2_scale           :   2;
}PDMA_PIC_INFO_s;

typedef struct
{
    unsigned int rsvd0                  :  15;
    unsigned int tiling_mode            :   2;
    unsigned int blklinear_gob          :   3;
    unsigned int rsvd1                  :   1;
    unsigned int width_pad              :   4;
    unsigned int height_pad             :   4;
    unsigned int memory_mode            :   3;
}PDMA_PIC_INFO1_s;
    
typedef struct
{
    unsigned int luma_8x1_height        :   4;
    unsigned int chroma_8x1_height      :   4;
    unsigned int luma_block_num         :   3;
    unsigned int chroma_block_num       :   3;
    unsigned int rsvd                   :  18;
}PDMA_DEC_PACKET_INFO_s;

typedef struct 
{
    unsigned int mcu_width      : 12;
    unsigned int rsvd0          :  4;
    unsigned int mcu_height     : 12;
    unsigned int rsvd1          :  4;
}DEC_FRAME_SIZE_s;

typedef struct 
{
    unsigned int component              :  3;   // 0-400, 1-420, 2-422H, 3-422V, 4-444
    unsigned int restart_interval       : 16;   // 
    unsigned int stream_chroma_mode     :  3;   // 0-mono chrome; 1-YUV420; 2-YUV422H; 3-YUV422V; 4-YUV444;
    unsigned int motion_jpeg_type       :  1;   // 0-type A; 1-type B
    unsigned int output_type            :  3;   // 0-YUV, 1-YUY2, 2-RGBA, 3-BGRA, 4-ABGR, 5-ARGB
    unsigned int power2_downscale       :  2;   // 0-no scale; 1- 1/2; 2- 1/4; 3- 1/8
    unsigned int chroma_subsample       :  2;   // 0-disable, 1-to 420, 2-to 422H
    unsigned int rsvd                   :  2;
}DEC_FRAME_PARAM_s;

typedef struct
{
    unsigned int ac_idx                 :   2;
    unsigned int dc_idx                 :   2;
    unsigned int quant                  :   2;
    unsigned int rsvd0                  :   2;
    unsigned int hblock                 :   4;
    unsigned int vblock                 :   4;
    unsigned int rsvd1                  :  16;
}DEC_BLOCK_CTL_s;

typedef struct 
{
    unsigned int ac                     :   1;
    unsigned int comp                   :   2;
    unsigned int off                    :   4;
    unsigned int idx                    :   8;
    unsigned int val                    :  17;
}DEC_HUFFMAN_MINCODE_s;

typedef struct 
{
    unsigned int ac                     :   1;
    unsigned int comp                   :   2;
    unsigned int off                    :   8;
    unsigned int val                    :   8;
    unsigned int rsvd                   :  13;
}DEC_HUFFMAN_SYMBOL_s;

typedef struct
{
    unsigned int luma_blocks_x          :   2;
    unsigned int luma_blocks_y          :   2;
    unsigned int chroma_blocks_x        :   2;
    unsigned int chroma_blocks_y        :   2;
    unsigned int luma_block_width       :   4;
    unsigned int luma_block_height      :   4;
    unsigned int chroma_block_width     :   4;
    unsigned int chroma_block_height    :   4;
    unsigned int alpha_val              :   8;
}DEC_OUTPUT_INFO_s;

// 256 bits alignment based on cdma's requirement
typedef struct 
{
#ifdef NV_NVJPG_SUPPORT_ENC
    // encoder cfg, size = 1792 bytes
    // 256 bits
    ENC_TOP_CTL_s           enc_top_ctl;
    ENC_TOP_RST_INTERVAL_s  enc_restart_interval;
    ENC_FDCT_RATECTRL_s     enc_fdct_rc;
    ENC_VLE_RATECTRL_s      enc_vle_rc;
    unsigned int            enc_align256[4];
    // 8 * 256 bits
    ENC_FDCT_QT_WDATA_s     enc_quant_tab;
    // 47 * 256 bits
    ENC_VLE_HT_WDATA_s      enc_huff_tab;
#endif
    // pdma cfg, size = 32 bytes
    PDMA_PIC_INFO_s         pdma_pic_info;
    PDMA_PIC_INFO1_s        pdma_pic_info1;
    unsigned int            luma_stride;
    unsigned int            chroma_stride;
    PDMA_DEC_PACKET_INFO_s  pdma_dec_packet_info;
    unsigned int            pdma_align256[3];

    // sdma cfg, size = 32 bytes
    unsigned int            bitstream_offset;
    unsigned int            bitstream_size;
    unsigned int            sdma_align256[6];

    // decoder cfg, size = 2816 bytes
    // 256 bits
    DEC_FRAME_SIZE_s        dec_frame_size;
    DEC_FRAME_PARAM_s       dec_frame_param;
    DEC_BLOCK_CTL_s         dec_block_ctl[4];
    unsigned int            dec_align256_0[2];
    // 12 * 256 bits
    DEC_HUFFMAN_MINCODE_s   dec_huff_mincode[6][16];
    // 66 * 256 bits
    DEC_HUFFMAN_SYMBOL_s    dec_huff_symbol[3][174];
    unsigned int            dec_align256_1[6];
    // 256 bits
    DEC_OUTPUT_INFO_s       dec_output_info;
    unsigned int            dec_yuv2rgb_param[6];
    unsigned int            dec_align256_2;
    // 8 * 256 bits
    unsigned char           dec_quant[64][4];

}nvjpg_drv_pic_param_s;

typedef struct
{
    unsigned int bytes_offset;      //bytes consumed by HW
    unsigned int mcu_x;             //decoded mcu_x 
    unsigned int mcu_y;             //decoded mcu_y
    unsigned int cycle_count;       //total cycles taked for execute. read from NV_CNVJPG_PDMA_CYCLE register
    unsigned int error_status;      //report error if any
    unsigned char reserved[12];
}nvjpg_dec_status;

typedef struct
{
    unsigned int bitstream_size;    //exact residual part bitstram size of current image
    unsigned int mcu_x;             //encoded mcu_x 
    unsigned int mcu_y;             //encoded mcu_y
    unsigned int cycle_count;       //total cycles taked for execute. read from NV_CNVJPG_PDMA_CYCLE register
    unsigned int error_status;      //report error if any
    unsigned char reserved[12];  
}nvjpg_enc_status;

#endif

