/*******************************************************************************
    Copyright (c) 2016-2020, NVIDIA CORPORATION. All rights reserved.

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

#ifndef NVJPG_DRV_H
#define NVJPG_DRV_H
//
// CLASS NV_E7D0_NVJPG
//
// NVJPG is the combination of JPEG decoder and encoder, it will support baseline sequential profile.
// In the encoder side, it support: a. 420 pitch linear format, b. programable huffman/qunat table, ... etc.
// In the decoder side, it support: a. 400/420/422/444 decoding, b. YUV2RGB, c. Power2Scale: 1/2, 1/4, 1/8, d.ChromaSumbSample ... etc.
// ===================


// huffuman table:
// huffuman table is organized in symbol value order, each table item include 2 field, codeWord length, and codeWord value
#define DCVALUEITEM 12
#define ACVALUEITEM 256 // in fact, only 162 items are used in baseline sequential profile.
typedef struct
{
    unsigned short length;             // 4 bit, code word length
    unsigned short value;              // 16 bit, code word value
}huffman_symbol_s;


typedef struct
{
    // surface related
    unsigned int   bitstream_start_off;// start offset position in bitstream buffer where data should be written (byte offset)
    unsigned int   bitstream_buf_size; // size in bytes of the buffer allocated for bitstream slice/mb data
    unsigned int   luma_stride;        // 64 bytes align;
    unsigned int   chroma_stride;      // 64 bytes align;
    unsigned int   inputType    : 4;   // 0: YUV; 1: RGB, 2: BGR, 3:RGBA, 4: BGRA, 5: ABGR, 6: ARGB
    unsigned int   chromaFormat : 2;   // chroma format: 0: 444; 1: 422H; 2:422V; 3:420
    unsigned int   tilingMode   : 2;   // 0: linear; 1: GPU_blkLinear; 2: Tegra_blkLinear
    unsigned int   gobHeight    : 3;   // used for blkLinear, 0: 2; 1: 4; ... 4: 32
    unsigned int   yuvMemoryMode: 3;   // 0-semi planar nv12; 1-semi planar nv21; 2-plane(yuy2); 3-planar
    unsigned int   reserved_0   : 18;
    // control para
    unsigned short imageWidth;         // real image width, up to 16K
    unsigned short imageHeight;        // real image height, up to 16K
    unsigned short jpegWidth;          // image width align to 8 or 16 pixel
    unsigned short jpegHeight;         // image height align to 8 or 16 pixel
    unsigned int   totalMcu;
    unsigned int   widthMcu;
    unsigned int   heightMcu;
    unsigned int   restartInterval;    // restart interval, 0 means disable the restart feature

    // rate control related
    unsigned int   rateControl   : 2;  // RC: 0:disable; 1:block-base; others: reserve
    unsigned int   rcTargetYBits : 11; // target luma bits per block, [0 ~ (1<<11)-1]
    unsigned int   rcTargetCBits : 11; // target chroma bits per block, [0 ~ (1<<11)-1]
    unsigned int   reserved_1    : 8;
    unsigned int   preQuant      : 1;  // pre quant trunction enabled flag
    unsigned int   rcThreshIdx   : 8;  // pre_quant threshold index [1 ~ 63]
    unsigned int   rcThreshMag   : 21; // threshold magnitude
    // mjpeg-typeB
    unsigned int   isMjpgTypeB   : 1;  // a flag indicate mjpg type B format, which control HW no stuff byte.
    unsigned int   reserved_2    : 1;
    // huffman tables
    huffman_symbol_s hfDcLuma[DCVALUEITEM];   //dc luma huffman table, arranged in symbol increase order, encoder can directly index and use
    huffman_symbol_s hfAcLuma[ACVALUEITEM];   //ac luma huffman table, arranged in symbol increase order, encoder can directly index and use
    huffman_symbol_s hfDcChroma[DCVALUEITEM]; //dc chroma huffman table, arranged in symbol increase order, encoder can directly index and use
    huffman_symbol_s hfAcChroma[ACVALUEITEM]; //ac chroma huffman table, arranged in symbol increase order, encoder can directly index and use
    // quantization tables
    unsigned short  quantLumaFactor[64];        //luma quantize factor table, arranged in horizontal scan order, (1<<15)/quantLuma
    unsigned short  quantChromaFactor[64];      //chroma quantize factor table, arranged in horizontal scan order, (1<<15)/quantLuma

    unsigned char  reserve[0x6c];
}nvjpg_enc_drv_pic_setup_s;

typedef struct
{
    unsigned int bitstream_size; //exact residual part bitstram size of current image
    unsigned int mcu_x;          //encoded mcu_x
    unsigned int mcu_y;          //encoded mcu_y
    unsigned int reserved0;
    unsigned int error_status;   //report error if any
    unsigned char reserved1[12];
}nvjpg_enc_status;

struct ctrl_param_s
{
    union
    {
        struct
        {
            unsigned int gptimer_on         :1;
            unsigned int dump_cycle         :1;
            unsigned int debug_mode         :1;
            unsigned int reserved           :29;
        }bits;
        unsigned int data;
    };
};


//NVJPG Decoder class interface
typedef struct
{
    int codeNum[16]; //the number of huffman code with length i
    unsigned char minCodeIdx[16]; //the index of the min huffman code with length i
    int minCode[16];  //the min huffman code with length i
    unsigned char symbol[162]; // symbol need to be coded.
    unsigned char reserved[2]; // alignment
}huffman_tab_s;

typedef struct
{
    unsigned char hblock;
    unsigned char vblock;
    unsigned char quant;
    unsigned char ac;
    unsigned char dc;
    unsigned char reserved[3]; //alignment
} block_parameter_s;

typedef struct
{
    huffman_tab_s  huffTab[2][4];
    block_parameter_s blkPar[4];
    unsigned char quant[4][64]; //quant table
    int restart_interval;
    int frame_width;
    int frame_height;
    int mcu_width;
    int mcu_height;
    int comp;
    int bitstream_offset;
    int bitstream_size;
    int stream_chroma_mode;  //0-mono chrome; 1-yuv420; 2-yuv422H; 3-yuv422V; 4-yuv444;
    int output_chroma_mode;  //0-mono chrome; 1-yuv420; 2-yuv422H; 3-yuv422V; 4-yuv444;
    int output_pixel_format; //0-yuv; 1-RGB; 2-BGR; 3-RGBA; 4-BGRA; 5-ABGR; 6-ARGB
    int output_stride_luma;   //64 bytes align
    int output_stride_chroma; //64 bytes align
    int alpha_value;
    int yuv2rgb_param[6]; //K0, K1, K2, K3, K4, C
    int tile_mode; //0-pitch linear; 1-gpu block linear; 2-tegra block linear
    int block_linear_height;
    int memory_mode; //0-semi planar nv12; 1-semi planar nv21; 2-plane(yuy2); 3-planar
    int power2_downscale; //0-no scale; 1- 1/2; 2- 1/4; 3- 1/8
    int motion_jpeg_type; //0-type A; 1-type B
    int start_mcu_x;      //set start mcu x for error robust
    int start_mcu_y;      //set start mcu y for error robust
}nvjpg_dec_drv_pic_setup_s;

typedef struct
{
    unsigned int bytes_offset; //bytes consumed by HW
    unsigned int mcu_x;        //decoded mcu_x
    unsigned int mcu_y;        //decoded mcu_y
    unsigned int reserved0;
    unsigned int error_status; //report error if any
    unsigned char reserved1[12];
}nvjpg_dec_status;
#endif

