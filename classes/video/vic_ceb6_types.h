/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#ifndef VIC_CEB6_TYPES_H
#define VIC_CEB6_TYPES_H

/*
 * VIC (Video Image Compositor) enum and struct type definitions.
 *
 * Hardware lineage: VIC 5.1, class 0xCEB6.
 *
 * The companion clceb6.h header documents the class methods. This header
 * documents the enums and the layout of the configuration structures that
 * those methods point to (for example the CompositorConfigStruct referenced
 * by SET_CONFIG_STRUCT_OFFSET).
 */

#include <stdint.h>

#ifdef __cplusplus
namespace nvceb6 {
#endif

/* ---------------------------------------------------------------------------
 * Enums
 * ------------------------------------------------------------------------ */

typedef enum _SPECIAL_VALUES {
    INVALID_PIXEL_DEFAULT                    = 32768,     //use 0x8000 for 4-alignment pixels
} SPECIAL_VALUES;

typedef enum _DXVAHD_FRAME_FORMAT {
    DXVAHD_FRAME_FORMAT_PROGRESSIVE          = 0,
    DXVAHD_FRAME_FORMAT_SUBPIC_PROGRESSIVE   = 5
} DXVAHD_FRAME_FORMAT;

typedef enum _DXVAHD_ALPHA_FILL_MODE {
    DXVAHD_ALPHA_FILL_MODE_OPAQUE            = 0,
    DXVAHD_ALPHA_FILL_MODE_BACKGROUND        = 1,
    DXVAHD_ALPHA_FILL_MODE_DESTINATION       = 2,
    DXVAHD_ALPHA_FILL_MODE_COMPOSITED        = 4,
    DXVAHD_ALPHA_FILL_MODE_SOURCE_ALPHA      = 5
} DXVAHD_ALPHA_FILL_MODE;

typedef enum _FILTER_LENGTH {
    FILTER_LENGTH_1TAP              = 0,
    FILTER_LENGTH_2TAP              = 1,
    FILTER_LENGTH_5TAP              = 2,
    FILTER_LENGTH_10TAP             = 3
} FILTER_LENGTH;

typedef enum _FILTER_TYPE {
    FILTER_TYPE_NORMAL              = 0,
    FILTER_TYPE_NOISE               = 1,
    FILTER_TYPE_DETAIL              = 2
} FILTER_TYPE;

typedef enum _PIXELFORMAT {
    T_A8                            = 0,
    T_L8                            = 1,
    T_A4L4                          = 2,
    T_L4A4                          = 3,
    T_R8                            = 4,
    T_A8L8                          = 5,
    T_L8A8                          = 6,
    T_R8G8                          = 7,
    T_G8R8                          = 8,
    T_B5G6R5                        = 9,
    T_R5G6B5                        = 10,
    T_B6G5R5                        = 11,
    T_R5G5B6                        = 12,
    T_A1B5G5R5                      = 13,
    T_A1R5G5B5                      = 14,
    T_B5G5R5A1                      = 15,
    T_R5G5B5A1                      = 16,
    T_A5B5G5R1                      = 17,
    T_A5R1G5B5                      = 18,
    T_B5G5R1A5                      = 19,
    T_R1G5B5A5                      = 20,
    T_X1B5G5R5                      = 21,
    T_X1R5G5B5                      = 22,
    T_B5G5R5X1                      = 23,
    T_R5G5B5X1                      = 24,
    T_A4B4G4R4                      = 25,
    T_A4R4G4B4                      = 26,
    T_B4G4R4A4                      = 27,
    T_R4G4B4A4                      = 28,
    T_B8_G8_R8                      = 29,   // reserved
    T_R8_G8_B8                      = 30,   // reserved
    T_A8B8G8R8                      = 31,
    T_A8R8G8B8                      = 32,
    T_B8G8R8A8                      = 33,
    T_R8G8B8A8                      = 34,
    T_X8B8G8R8                      = 35,
    T_X8R8G8B8                      = 36,
    T_B8G8R8X8                      = 37,
    T_R8G8B8X8                      = 38,
    T_A2B10G10R10                   = 39,
    T_A2R10G10B10                   = 40,
    T_B10G10R10A2                   = 41,
    T_R10G10B10A2                   = 42,
    T_A4P4                          = 43,  // not supported
    T_P4A4                          = 44,  // not supported
    T_P8A8                          = 45,  // not supported
    T_A8P8                          = 46,  // not supported
    T_P8                            = 47,  // not supported
    T_P1                            = 48,  // not supported
    T_U8V8                          = 49,
    T_V8U8                          = 50,
    T_A8Y8U8V8                      = 51,
    T_V8U8Y8A8                      = 52,
    T_Y8_U8_V8                      = 53,  // not supported
    T_Y8_V8_U8                      = 54,  // not supported
    T_U8_V8_Y8                      = 55,  // not supported
    T_V8_U8_Y8                      = 56,  // not supported
    T_Y8_U8__Y8_V8                  = 57,
    T_Y8_V8__Y8_U8                  = 58,
    T_U8_Y8__V8_Y8                  = 59,
    T_V8_Y8__U8_Y8                  = 60,
    T_Y8___U8V8_N444                = 61,
    T_Y8___V8U8_N444                = 62,
    T_Y8___U8V8_N422                = 63,
    T_Y8___V8U8_N422                = 64,
    T_Y8___U8V8_N422R               = 65,
    T_Y8___V8U8_N422R               = 66,
    T_Y8___U8V8_N420                = 67,
    T_Y8___V8U8_N420                = 68,
    T_Y8___U8___V8_N444             = 69,
    T_Y8___U8___V8_N422             = 70,
    T_Y8___U8___V8_N422R            = 71,
    T_Y8___U8___V8_N420             = 72,
    T_U8                            = 73,  // internal usage
    T_V8                            = 74,  // internal usage
    T_Y10___U10V10_N444             = 75,
    T_Y10___V10U10_N444             = 76,
    T_Y10___U10V10_N422             = 77,
    T_Y10___V10U10_N422             = 78,
    T_Y10___U10V10_N422R            = 79,
    T_Y10___V10U10_N422R            = 80,
    T_Y10___U10V10_N420             = 81,
    T_Y10___V10U10_N420             = 82,
    T_Y10___U10___V10_N444          = 83,
    T_Y10___U10___V10_N422          = 84,
    T_Y10___U10___V10_N422R         = 85,
    T_Y10___U10___V10_N420          = 86,
    T_U10                           = 87,  // internal usage
    T_V10                           = 88,  // internal usage
    T_L10                           = 89,
    T_U10V10                        = 90,  // internal usage
    T_V10U10                        = 91,  // internal usage
    T_Y12___U12V12_N444             = 92,
    T_Y12___V12U12_N444             = 93,
    T_Y12___U12V12_N422             = 94,
    T_Y12___V12U12_N422             = 95,
    T_Y12___U12V12_N422R            = 96,
    T_Y12___V12U12_N422R            = 97,
    T_Y12___U12V12_N420             = 98,
    T_Y12___V12U12_N420             = 99,
    T_Y12___U12___V12_N444          = 100,
    T_Y12___U12___V12_N422          = 101,
    T_Y12___U12___V12_N422R         = 102,
    T_Y12___U12___V12_N420          = 103,
    T_U12                           = 104,  // internal usage
    T_V12                           = 105,  // internal usage
    T_L12                           = 106,
    T_U12V12                        = 107,  // internal usage
    T_V12U12                        = 108,  // internal usage
    T_L16                           = 109,
    T_A16B16G16R16                  = 110,
    T_A16Y16U16V16                  = 111,
    T_R16                           = 112,
    T_Y16___V8U8_N444               = 113,  // not supported
    T_Y16___V8U8_N422               = 114,  // not supported
    T_Y16___V8U8_N420               = 115,  // not supported
    T_Y16___V16U16_N444             = 116,
    T_Y16___V16U16_N422             = 117,
    T_Y16___V16U16_N420             = 118,
    T_U16V16                        = 119,  // internal usage
    T_V16U16                        = 120,
    T_A16B16G16R16_F                = 121,
    T_X16B16G16R16_F                = 122,
    T_R8___G8___B8                  = 123,
    T_R16___G16___B16               = 124,
    T_Y16___U16___V16_N420          = 125,
    T_Y16___U16___V16_N444          = 126,
    T_R16___G16___B16_F             = 127,
} PIXELFORMAT;

typedef enum _BLK_KIND {
    BLK_KIND_PITCH_LINEAR           = 0,
    BLK_KIND_BLOCK_LINEAR_TEGRA     = 1,    // reserved
    BLK_KIND_BLOCK_LINEAR_GPU       = 2
} BLK_KIND;

typedef enum _BLEND_SRCFACTC {
    BLEND_SRCFACTC_K1               = 0,
    BLEND_SRCFACTC_K1_TIMES_DST     = 1,
    BLEND_SRCFACTC_NEG_K1_TIMES_DST = 2,
    BLEND_SRCFACTC_K1_TIMES_SRC     = 3,
    BLEND_SRCFACTC_ZERO             = 4
} BLEND_SRCFACTC;

typedef enum _BLEND_DSTFACTC {
    BLEND_DSTFACTC_K1               = 0,
    BLEND_DSTFACTC_K2               = 1,
    BLEND_DSTFACTC_K1_TIMES_DST     = 2,
    BLEND_DSTFACTC_NEG_K1_TIMES_DST = 3,
    BLEND_DSTFACTC_NEG_K1_TIMES_SRC = 4,
    BLEND_DSTFACTC_ZERO             = 5,
    BLEND_DSTFACTC_ONE              = 6
} BLEND_DSTFACTC;

typedef enum _BLEND_SRCFACTA {
    BLEND_SRCFACTA_K1               = 0,
    BLEND_SRCFACTA_K2               = 1,
    BLEND_SRCFACTA_NEG_K1_TIMES_DST = 2,
    BLEND_SRCFACTA_ZERO             = 3,
    BLEND_SRCFACTA_MAX              = 7
} BLEND_SRCFACTA;

typedef enum _BLEND_DSTFACTA {
    BLEND_DSTFACTA_K2               = 0,
    BLEND_DSTFACTA_NEG_K1_TIMES_SRC = 1,
    BLEND_DSTFACTA_ZERO             = 2,
    BLEND_DSTFACTA_ONE              = 3,
    BLEND_DSTFACTA_MAX              = 7
} BLEND_DSTFACTA;

typedef enum _BLEND_CKMODE {
    BLEND_CKMODE_DISABLED           = 0,
    BLEND_CKMODE_ENABLED            = 1
} BLEND_CKMODE;

typedef enum _PLANE_ID {
    PLANE_ID_LUMA                   = 0,
    PLANE_ID_CHROMA                 = 1,
    PLANE_ID_CHROMA_U               = 1,
    PLANE_ID_CHROMA_V               = 2
} PLANE_ID;

typedef enum _OUTHUB_SOURCE {
    OUTHUB_SOURCE_OCSC0             = 0,
    OUTHUB_SOURCE_OETF              = 2,
    OUTHUB_SOURCE_OCSC1             = 3
} OUTHUB_SOURCE;

typedef enum _ROUNDING_MODE {
    ROUND_MODE_TRUNC = 0, // Rounding towards zero
    ROUND_MODE_RHAFZ = 1, // Rounding half away from zero
    ROUND_MODE_RHTE = 2, // Rounding half to even
    ROUND_MODE_SR = 3  // Stochastic rounding
} ROUNDING_MODE;

typedef enum _BIT_PAD_MODE {
    BIT_PAD_DEFAULT = 0, // Zero-padding for YUV   MSB-replicate for RGB
    BIT_PAD_ZERO    = 1, // Always zero-padding
    BIT_PAD_MSB     = 2  // Always MSB-replicate
} BIT_PAD_MODE;

typedef enum _SLC_CLIENT_ID {
    SLC_CLIENT_OUTG_WR_CH0          = 0,
    SLC_CLIENT_OUTG_WR_CH1          = 1,
    SLC_CLIENT_OUTG_WR_CH2          = 2,
    SLC_CLIENT_SC_RD                = 3,
    SLC_CLIENT_TNR_CURFRM_WR_CH0    = 4,
    SLC_CLIENT_TNR_CURFRM_WR_CH1    = 5,
    SLC_CLIENT_TNR_CURFRM_WR_CH2    = 6,
    SLC_CLIENT_TNR_CURFRM_ALPHA_WR  = 7,
    SLC_CLIENT_TNR_NEIGHBOR_WR      = 8,
    SLC_CLIENT_TNR_PREVFRM_RD_CH0   = 9,
    SLC_CLIENT_TNR_PREVFRM_RD_CH1   = 10,
    SLC_CLIENT_TNR_PREVFRM_RD_CH2   = 11,
    SLC_CLIENT_TNR_PREVFRM_ALPHA_RD = 12,
    SLC_CLIENT_TNR_NEIGHBOR_RD      = 13,
    SLC_CLIENT_PP_XSOBEL_WR         = 14,
    SLC_CLIENT_PP_XSOBEL_DS_WR      = 15,
    SLC_CLIENT_PP_NEIGHBOR_WR       = 16,
    SLC_CLIENT_PP_NEIGHBOR_RD       = 17,
    SLC_CLIENT_POSG_WARPMAP_RD      = 18,
    SLC_CLIENT_NUM                  = 19
} SLC_CLIENT_ID;

/* ---------------------------------------------------------------------------
 * Structs
 * ------------------------------------------------------------------------ */

typedef struct _SlotConfig {
    /////////////////////////////////////////////////////////////////
    // Per-slot control bits
    /////////////////////////////////////////////////////////////////
    uint32_t    SlotEnable          : 1;    // Enable or disable bit for this slot
    uint32_t    SlotFlipX           : 1;    // Horizontal flip enable
    uint32_t    SlotFlipY           : 1;    // Vertical flip enable
    uint32_t    SlotTranspose       : 1;    // Transpose enable
    uint32_t    GeoTranMode         : 2;    // Geometry transform processing modes:
	                                        // 0: rectangle-param-derived mode(Reserved), 1: non-LDC mode, 2: IPT-only mode 3: Sparse WarpMap mode
    uint32_t    IPTMode             : 1;    // IPT mode: 0:Affine transform,  1: Perspective transform
    uint32_t    FmtMatEn            : 1;    // YUV2RGB Format Color Space Conversion matrix enable
    uint32_t    EotfEn              : 1;    // EOTF VSSLUT enable
    uint32_t    FitpCsc0MatEn       : 1;    // RGB2ITP RGB->LMS convert
    uint32_t    FitpLutEn           : 1;    // RGB2ITP PQ convert
    uint32_t    FitpCsc1MatEn       : 1;    // RGB2ITP L'M'S'->ITP convert
    uint32_t    BitpCsc0MatEn       : 1;    // ITP2RGB ITP->L'M'S' convert
    uint32_t    BitpLutEn           : 1;    // ITP2RGB inv-PQ convert
    uint32_t    BitpCsc1MatEn       : 1;    // ITP2RGB LMS->RGB convert
    uint32_t    ToneMapEn           : 1;    // ToneMap Enable
    uint32_t    SCOutputU16         : 1;    // SC output PW16E2 or U16, 0: PW16E2, 1: U16
    uint32_t    SCFp16HandleMode    : 2;    // SC handdling for fp16 special values, 0(KEEP): do nothing(i.e. keep special values);
                                            // 1(SET_ZERO): re-assign all speical values to +zero 2(CLIP): -INF->minfp16, +INF->maxfp16, NaN->0
    uint32_t    PxlgFilterBypassEn  : 1;    // 1: Skip filtering in Pxlg, used at blit test(scaling ratio = 1:1 and 1Tap filter) for power saving; 0: Do filtering in Pxlg
    uint32_t    CupsOutputClampMode : 1;    // 0:[-1.5, 2.5), 1:[0, 1)
    uint32_t    PxlgOutputClampMode : 1;    // 0:[-1.5, 2.5), 1:[0, 1)
    uint32_t    CupsCoeffOverride   : 1;    // 0: disable(use default) 1:use coefficients in CupsFilterCoeff
    uint32_t    BitDepthUpConvMode  : 2;    // 0: auto mode (RGB: MSB replicate, YUV: zero padding); 1: YUV/RGB zero padding; 2: YUV/RGB MSB replicate
    uint32_t    PxlgFilterMode      : 2;    // Pxlg filter mode 0: non-LDC polyphase filter; 1: LDC polyphase filte; 2: 2D Convolution filter; 3:reserved
    uint32_t    SrcMaskPixelFillMode : 1;   // when out of source rectangle, 0: padding with nearest boundary pixels, 1:override with register OverrideA/R/G/B in blender
    uint32_t    Cups8bClamp         : 1;    // Set 1 to enabled CUPS 8bit power saving. Only 8 MSB of CUPS output is preserved, LSBs are dropped after rounding.
    uint32_t    reserved1_cfg0      : 3;    // Reserved per-slot control configure bits

    uint32_t    ClearRectMask       : 8;    // ClearRect Slot Mask bits
    uint32_t    reserved0_cfg1      : 24;   //

    /////////////////////////////////////////////////////////////////
    // Pixel and surface format
    /////////////////////////////////////////////////////////////////
    uint32_t    SlotPixelFormat     : 10;   // Pixel format for each slot (PIXEL_FORMAT)
    uint32_t    SlotChromaLocHoriz  : 2;    // Horizontal chroma location - (0: co-located with even luma; 1: in between even and odd luma; 2: co-located with odd luma; 3: reserved)
    uint32_t    SlotChromaLocVert   : 2;    // Vertical chroma location - (0: co-located with even luma; 1: in between even and odd luma; 2: co-located with odd luma; 3: reserved)
    uint32_t    SlotBlkKind         : 2;    // Slot input Block-linear kind (BLK_KIND)
    uint32_t    SlotBlkHeight       : 4;    // Block-linear height (in gobs, log2), 0: 1 GOB; 1: 2 GOBs; 2: 4 GOBs; 3: 8 GOBs; 4: 16 GOBs; 5: 32 GOBs
    uint32_t    SlotCacheWidth      : 3;    // 0: 32Bx8, 1: 64Bx4, 2: 128Bx2, 3: 256Bx1; Other: reserved
    uint32_t    reserved0_cfg2      : 9;    //

    /////////////////////////////////////////////////////////////////
    // SourceRect -> DestRect Shape Transform
    /////////////////////////////////////////////////////////////////
    uint32_t    SourceRectLeft;             // U14.16 Source rectangle co-ordination
    uint32_t    SourceRectRight;
    uint32_t    SourceRectTop;
    uint32_t    SourceRectBottom;

    uint16_t    DestRectLeft;               // U14, Destination Rectangle, For any non-4:4:4 format all corners need to fall on a multiple of 2
    uint16_t    DestRectRight;
    uint16_t    DestRectTop;
    uint16_t    DestRectBottom;

    /////////////////////////////////////////////////////////////////////
    // Input slot Image and Surface Width and Height (in pixel units) (U14)
    // For single plane pixel format(ARGB), only use Luma surface
    /////////////////////////////////////////////////////////////////////
    uint16_t    SlotImgWidth;               // Width of Image minus 1. Any pixel data outside of this will not be used inside VIC but might still be read.
    uint16_t    SlotImgHeight;              // Height of Image minus 1. Any pixel data outside of this will not be used inside VIC but might still be read.
    uint16_t    SlotLumaSfcWPad;            // Padded luma width of surface minus 1. Any pixel data outside of this will not be read.
    uint16_t    SlotLumaSfcHPad;            // Padded luma height of surface minus 1. Any pixel data outside of this will not be read.
    uint16_t    SlotChromaSfcWPad;          // Padded chroma width of surface minus 1. Any pixel data outside of this will not be read. This value is not required for pixel interleaved surfaces such as ARGB
    uint16_t    SlotChromaSfcHPad;          // Padded chroma height of surface minus 1. Any pixel data outside of this will not be read. This value is not required for pixel interleaved surfaces such as ARGB

    //////////////////////////////////////////////////////////////////////
    // Chroma UpSampling and Scaler parameter
    // construct per-slot coefficient tables
    //////////////////////////////////////////////////////////////////////
    uint32_t    FilterLengthY       : 2;    // 0: 1-tap (Nearst);  1: 2-tap (Bilinear);  2: 5-tap(non-LDC)/4-tap(LDC)/5-tap(2D Convolution);  3: 10-taps (FILTER_LENGTH)
    uint32_t    FilterLengthX       : 2;    // 0: 1-tap (Nearst);  1: 2-tap (Bilinear);  2: 5-tap(non-LDC)/4-tap(LDC)/5-tap(2D Convolution);  3: 10-taps (FILTER_LENGTH)
    uint32_t    ChromaUpLengthY     : 2;    // Fixed constant coefficient, 1: 2-tap (Bilinear);  2: 5-tap; other reserved (FILTER_LENGTH)
    uint32_t    ChromaUpLengthX     : 2;    // Fixed constant coefficient, 1: 2-tap (Bilinear);  2: 5-tap; other reserved (FILTER_LENGTH)
    uint32_t    FrameFormat         : 4;    // 0: PROGRESSIVE;   5:SUBPIC_PROGRESSIVE;  Other reserved (DXVAHD_FRAME_FORMAT)
    uint32_t    reserved0_sclr_cfg0 : 4;    // Reserved bits
    uint32_t    DetailFltClamp      : 12;   // The maximum range allowed for the difference between the filter output and center pixel of the input filter support
    uint32_t    reserved1_sclr_cfg0 : 4;    // Reserved bits

    uint16_t    FilterNoise;                // U10, Strength of the spatial noise filter
    uint16_t    FilterDetail;               // U10, Strength of the detail filter
    uint16_t    ChromaNoise;                // U10, Strength of the chroma spatial noise filter
    uint16_t    ChromaDetail;               // U10, Strength of the chroma detail filter
    uint32_t    RatioX              : 16;   // U16, (src_w / dst_w) * 1024, program it with 2048 means 2X downSample, program it with 512 means 2X upSample, only used when GeoTranMode == 1.
    uint32_t    RatioY              : 16;   // U16, (src_h / dst_h) * 1024, program it with 2048 means 2X downSample, program it with 512 means 2X upSample, only used when GeoTranMode == 1.
    uint32_t    reserved0_sclr_cfg4;

    //////////////////////////////////////////////////////////////////////
    // Sparse Warp Map parameters
    // 1. non-fixed 4x4 region:
    //    a. Horizontal and Vertical Space factor
    //    b. Region Width and Height
    // Sum of all valid horRegionWidth and verRegionHeight should be equal to DestRect's Width and Height
    //////////////////////////////////////////////////////////////////////
    uint32_t    log2HorSpace_0      : 3;    // 0: 1 pixel(full warp map); 1: 2 pixel (half warp map); 2: 4 pixel (quad warp map)
    uint32_t    log2VerSpace_0      : 3;    // 3: 8 pixels; 4: 16 pixel; 5: 32 pixel; 6: 64 pixel; 7: 128 pixel
    uint32_t    log2HorSpace_1      : 3;
    uint32_t    log2VerSpace_1      : 3;
    uint32_t    log2HorSpace_2      : 3;
    uint32_t    log2VerSpace_2      : 3;
    uint32_t    log2HorSpace_3      : 3;
    uint32_t    log2VerSpace_3      : 3;
    uint32_t    HorRegionNum        : 2;    // Horizontal region number, 0: 1 region; 1: 2 region; 2: 3 region; 3: 4 region
    uint32_t    VerRegionNum        : 2;    // Veritcal region number,   0: 1 region; 1: 2 region; 2: 3 region; 3: 4 region
    uint32_t    reserved0_posg_cfg0 : 4;    // Reserved

    /////////////////////////////////////////////////////////////////
    // EOTF LUTs parameter
    /////////////////////////////////////////////////////////////////
    uint32_t    EotfVssLutMode      : 2;    // 0: Segmented;   1: Direct8;
    uint32_t    EotfVssLutInterp    : 1;    // Enable Interpolation
    uint32_t    EotfVssLutMirror    : 1;    // Enable Mirror Operation
    uint32_t    EotfCurveMode       : 4;    // 0-15, Curve Table index in the external surface
    uint32_t    EotfValidEntryNum   : 10;   // Valid Entry number in selected LUT table
    uint32_t    EotfDirectRound     : 1;    // Rounding mode in direct mode: 0 - floor, 1 - half up
    uint32_t    reserved1_eotf      : 13;   //

    /////////////////////////////////////////////////////////////////
    // RGB2ICtCp and ICtCp2RGB parameter
    /////////////////////////////////////////////////////////////////
    uint32_t    FitpLutInterp          : 1;    // Enable Interpolation
    uint32_t    FitpLutMirror          : 1;    // Enable Mirror Operation
    uint32_t    FitpLutTfsel           : 2;    // Transfer function selection: 0: PQ, 1: HLG 2:Direct fp16->fxpt
    uint32_t    reserved0_fitp_cfg0    : 28;   //

    uint32_t    BitpLutInterp          : 1;    // Enable Interpolation
    uint32_t    BitpLutMirror          : 1;    // Enable Mirror Operation
    uint32_t    BitpLutTfsel           : 2;    // Transfer function selection: 0: iPQ, 1: iHLG 2:Direct fxpt->fp16
    uint32_t    BitpCsc1NoiseThreshold : 6;    // Noise suppression threshold factor in Bitp csc1. U3.3
    uint32_t    reserved0_bitp_cfg0    : 6;    //
    uint32_t    BitpLutFpScale         : 16;   // FP16, scale factor of bitp lut when in direct fxpt->fp16 mode.

    /////////////////////////////////////////////////////////////////
    // ToneMap parameter
    // Color_Correction_Factor(cc_factor):
    // Mode 0: cc_factor = weight * min(I, Itmo) / Max(I, Itmo)
    // Mode 1: cc_factor = weight * Itmo / Max(I, Itmo)
    // Mode 2: cc_factor = weight * Itmo / I
    // Mode 3: cc_factor = weight
    //
    // The correction weight is a combination of linear and non-linear weight.
    //////////////////////////////////////////////////////////////////
    // LowTone + MidTone + HighTone + Transition Tone               //
    //                                                              //
    //                         mt_weight                            //
    //                      ---------------\                        //
    //                     /|              |\                       //
    //                    / |              | \   ht_weight          //
    //                   /  |              |  \-------------        //
    //      lt_weight   /   |              |  |                     //
    //  ---------------/    |              |  |                     //
    //               lt_end |              |  ht_start              //
    //                      mt_start       mt_end                   //
    //////////////////////////////////////////////////////////////////
    uint32_t    reserved0_tm_cfg0   : 2;    //
    uint32_t    ToneMapVssLutInterp : 1;    // Enable Interpolation
    uint32_t    reserved1_tm_cfg0   : 1;    //
    uint32_t    ToneMapCurveMode    : 4;    // 0-15, Curve Table index in the external surface
    uint32_t    ToneMapCcFactormode : 2;    // Color correction factor generation modes
    uint32_t    ToneMapValidEntryNum: 11;   // Valid Entry number in selected LUT table
    uint32_t    reserved1_tm        : 11;   //

    uint16_t    Tmo_lt_end;                 // LowTone Region: [0, lt_end]
    uint16_t    Tmo_mt_start;               // MidTone Region: [mt_start, mt_end]
    uint16_t    Tmo_mt_end;
    uint16_t    Tmo_ht_start;               // HighTone Region: {ht_start, 0x3FFF]

    uint32_t    Tmo_lt_cc_weight0   : 9;    // LowTone, Linear weight: U1.8
    uint32_t    Tmo_lt_cc_weight1   : 9;    // LowTone, Non-Linear weight U1.8
    uint32_t    Tmo_lt_cc_threshold : 8;    // LowTone, Threshold in Non-Linear: U0.8
    uint32_t    reserved0_tm_cfg3   : 6;    // reserved

    uint32_t    Tmo_mt_cc_weight0   : 9;    // MidTone, Linear weight: U1.8
    uint32_t    Tmo_mt_cc_weight1   : 9;    // MidTone, Non-Linear weight U1.8
    uint32_t    Tmo_mt_cc_threshold : 8;    // MidTone, Threshold in Non-Linear: U0.8
    uint32_t    reserved0_tm_cfg4   : 6;    // reserved

    uint32_t    Tmo_ht_cc_weight0   : 9;    // HighTone, Linear weight: U1.8
    uint32_t    Tmo_ht_cc_weight1   : 9;    // HighTone, Non-Linear weight U1.8
    uint32_t    Tmo_ht_cc_threshold : 8;    // HighTone, Threshold in Non-Linear: U0.8
    uint32_t    reserved0_tm_cfg5   : 6;    // reserved

    //////////////////////////////////////////////////////////////////////
    // Blender Pre-stage Per-Slot
    // src[R|G|B|A]: slot input component;  dst[R|G|B|A]: previously blended component
    // output[R|G|B] = srcFactC * src[R|G|B] + dstFratC * dst[R|G|B]
    // outputA       = srcFactA * srcA       + dstFactA * dstA
    //////////////////////////////////////////////////////////////////////
    uint16_t    AlphaK1;                    // U0.16, K1, constant alpha value
    uint16_t    AlphaK2;                    // U0.16, K2, constant alpha value

    uint16_t    OverrideA;                  // U0.16, Override color A
    uint16_t    OverrideR;                  // FP16, Override color R
    uint16_t    OverrideG;                  // FP16, Override color G
    uint16_t    OverrideB;                  // FP16, Override color B

    uint32_t    SrcFactCMatchSelect : 3;    // 0: K1;  1: K1*dstA;      2: 1-(K1*dstA);  3: K1*srcA;      4: 0
    uint32_t    DstFactCMatchSelect : 3;    // 0: K1;  1: K2;           2: K1*dstA;      3: 1-(K1*dstA)   4: 1-(K1*srcA)   5: 0    6: 1
    uint32_t    SrcFactAMatchSelect : 3;    // 0: K1;  1: K2;           2: 1-(K1*dstA)   3: 0
    uint32_t    DstFactAMatchSelect : 3;    // 0: K2;  1: 1-(K1*srcA)   2: 0             3: 1
    uint32_t    UseOverrideR        : 1;    // srcR = OverrideR
    uint32_t    UseOverrideG        : 1;    // srcG = OverrideG
    uint32_t    UseOverrideB        : 1;    // srcB = OverrideB
    uint32_t    UseOverrideA        : 1;    // srcA = OverrideA
    uint32_t    MaskR               : 1;    // outputR = dstR
    uint32_t    MaskG               : 1;    // outputG = dstG
    uint32_t    MaskB               : 1;    // outputB = dstB
    uint32_t    MaskA               : 1;    // outputA = dstA
    uint32_t    reserved_bl         : 12;   // Reserved bits

    //////////////////////////////////////////////////////////////////////
    // IEEE754 float point compliant format:  1S + 8E + 23F
    // [ M11, M12, M13 ]
    // [ M21, M22, M23 ]
    // [ M31, M32, M33 ]
    //////////////////////////////////////////////////////////////////////
    float       IPT_M[3][3];                // for on-the-fly IPT rectification

    ////////////////////////////////////////////////////////
    // YUV2RGB Format Color Space Conversion Matrix
    // out0   [ c00 c01 c02 c03 ] [ in0 ]
    // out1 = [ c10 c11 c12 c13 ] [ in1 ]
    // out2   [ c20 c21 c22 c23 ] [ in2 ]
    //                            [ 1              ]
    ////////////////////////////////////////////////////////
    int32_t     FmtMatCoeff[3][4];          // S5.16

    /////////////////////////////////////////////////////////////////
    // RGB2ICtCp and ICtCp2RGB parameter
    /////////////////////////////////////////////////////////////////
    int32_t     FitpCsc0MatCoeff[3][4];     // S5.16:  RGB2ITP: RGB->LMS conversion
    int32_t     FitpCsc1MatCoeff[3][4];     // S5.16;  RGB2ITP: L'M'S'->ITP conversion


    int32_t     BitpCsc0MatCoeff[3][4];     // S5.16:  ITP2RGB: ITP->L'M'S conversion
    int32_t     BitpCsc1MatCoeff[3][4];     // S5.16:  ITP2RGB: LMS->RGB conversion

    int16_t     CupsFilterCoeff[4][5];      // S10: Cups 5tap filter coefficient for CupsCoeffOverride


    ////////////////////////////////////////////////
    // LDC parameters
    ////////////////////////////////////////////////
    uint32_t    NonFixedPatchEn     : 1;    // Non-Fixed Patch support in the whole frame; Row and column based patch size control; 0: Disable(Normal mode); 1: Enable
    uint32_t    reserved_ldc_cfg    : 31;

    uint16_t    horRegionWidth[4];          // U14, 4x4 Horizontal Region width minus 1
    uint16_t    verRegionHeight[4];         // U14, 4x4 Vertical Region height minus 1

    uint32_t    SparseWarpMapWidth  : 14;   // Pitch Linear, Sparse warp map (0,0) is related with DestRect.top/left
    uint32_t    reserved_wm_0       : 2;
    uint32_t    SparseWarpMapHeight : 14;
    uint32_t    reserved_wm_1       : 2;
    uint32_t    SparseWarpMapSfcPad : 14;   // In point units
    uint32_t    reserved_wm_2       : 18;

    uint32_t    FitpLutFpNorm;              // u0.32: FITP_LUT Normalization factor from FP16 luminance to u0.32
    uint32_t    reserved_align[10];         // Reserved bits


} SlotConfig;


typedef struct _OutConfig {
    /////////////////////////////////////////////////////////////////
    // Output control bits
    /////////////////////////////////////////////////////////////////
    uint32_t    OutFlipX            : 1;    // Horizontal flip enable
    uint32_t    OutFlipY            : 1;    // Vertical flip enable
    uint32_t    OutTranspose        : 1;    // Transpose enable
    uint32_t    OCsc0MatrixEn       : 1;    // OCSC0 Matrix Enable
    uint32_t    OetfEn              : 1;    // OETF transfer Enable
    uint32_t    OCsc1MatrixEn       : 1;    // OCSC1 Matrix Enable
    uint32_t    FilterOverride      : 1;    // Filter Override Enable
    uint32_t    OutGInputU16        : 1;    // data type of OutG's input, 0: PW16E2, 1: U16
    uint32_t    OutGRoundMode       : 2;    // outg rounding mode, 0 - Rounding towards zero, 1: Rounding half away from zero, 2: Rounding half to even, 3: Stochastic rounding
    uint32_t    OutGFp16HandleMode  : 2;    // oug handdling for fp16 special values, 0(KEEP): do nothing(i.e. keep special values); 1(SET_ZERO): re-assign all speical values to +zero 2(CLIP): -INF->minfp16, +INF->maxfp16, NaN->0
    uint32_t    OutGClampEn         : 1;    // OutG programmable clamp enable
    uint32_t    OutGWrDisable       : 1;    // OutG write disable control
    uint32_t    SubFrameEn          : 1;    // Sub frame processing enable
    uint32_t    BlKeepOutMaskedValue: 1;    // 0(default): out-of-boundary pixels are set 0 in blending output. 1: all post-composite pixel values are sent done piepeline. Must set 1 for TNR/PP subframe process. No impact to other result.
    uint32_t    reserved0_cfg0      : 16;   // Reserved output control bits

    /////////////////////////////////////////////////////////////////
    // Pixel and surface format
    /////////////////////////////////////////////////////////////////
    uint32_t    OutPixelFormat      : 10;   // Pixel format for each slot (PIXEL_FORMAT)
    uint32_t    OutChromaLocHoriz   : 2;    // Horizontal chroma location - (0: co-located with even luma; 1: in between even and odd luma; 2: co-located with odd luma; 3: reserved)
    uint32_t    OutChromaLocVert    : 2;    // Vertical chroma location - (0: co-located with even luma; 1: in between even and odd luma; 2: co-located with odd luma; 3: reserved)
    uint32_t    OutBlkKind          : 2;    // OutG Block-linear kind (BLK_KIND)
    uint32_t    OutBlkHeight        : 4;    // OutG Block-linear height (in gobs, log2), 0: 1 GOB; 1: 2 GOBs; 2: 4 GOBs; 3: 8 GOBs; 4: 16 GOBs; 5: 32 GOBs
    uint32_t    reserved0_cfg1      : 12;   //

    uint16_t    TargetRectLeft;             // U14, Target rectangle
    uint16_t    TargetRectRight;            // U14, Pixels outside of this area are guaranteed to remain unmodified
    uint16_t    TargetRectTop;              // U14
    uint16_t    TargetRectBottom;           // U14

    uint16_t    OutImgWidth;                // U14, Width of Image minus 1.
    uint16_t    OutImgHeight;               // U14, Height of Image minus 1.
    uint16_t    OutLumaSfcWPad;             // U14, Padded luma width of surface minus 1.
    uint16_t    OutLumaSfcHPad;             // U14, Padded luma height of surface minus 1.
    uint16_t    OutChromaSfcWPad;           // U14, Padded chroma width of surface minus 1.
    uint16_t    OutChromaSfcHPad;           // U14, Padded chroma height of surface minus 1.

    /////////////////////////////////////////////////////////////////
    // Blender post stage
    // DXVAHD_ALPHA_FILL_MODE:
    // OPAQUE        0   Opaque (all alpha inside target rect will be set to 1.0).
    // BACKGROUND    1   Background (all alpha inside target rect will be set to background alpha).
    // DESTINATION   2   Destination (alpha will remain unchanged).
    // SOURCE_STREAM 3   Source stream (alpha from source stream without planar alpha).
    // COMPOSITED    4   Composited (composited alpha, starting with background). In this mode, the blend parameters specified by SrcFactA/DstFactA/SrcFactC/DstFactC etc. are used.
    // SOURCE_ALPHA  5   Source alpha (alpha from source stream with planar alpha).
    ///////////////////////////////////////////////////////////////
    uint32_t    AlphaFillMode       : 3;    // Alpha fill mode (DXVAHD_ALPHA_FILL_MODE)
    uint32_t    AlphaFillSlot       : 4;    // SlotId for when AlphaFillMode == Source stream/Source alpha
    uint32_t    reserved0_bl        : 25;   //

    /////////////////////////////////////////////////////////////////
    // For BackgroundR/G/B, in most cases they need to be configured as FP16, because per-pixel blend operation requires src & dst color value to be FP16:
    //     dstC_fp16 = (srcFactC_u1p16 * srcC_fp16) + (dstFactC_u1p16 * dstC_fp16)     // 1.0 in FP16 represent 80 nit.
    // For cases BackgroundR/G/B doesn't need to go through the blend operation as below, they can also be set as PW16E2 or U0.16 depending on whether OETF/OutGInputU16 is enabled
    //     1. Background color fill (union of destRects of all enabled slots doesn't have intersection with TargetRect).
    //        pixels inside TargetRect will be set to the exact value programmed in BackgroundA/R/G/B
    //     2. BLIT (blending settings of all enabled slots are configured as blit configuration, IAS 5.1.6.1.1).
    //        pixels inside TargetRect but outside all DestRects will be set to the exact value programmed in BackgroundA/R/G/B.
    //        for other pixels inside TargetRect, per-slot blit operation "dst = src" will be applied, no background color involved.
    // OETF and OutGInputU16 constrains:
    //     1. If OETF is enabled, BackgroundR/G/B must set in FP16 format. otherwise,
    //     2. if OutGInputU16 is enabled, BackgroundR/G/B must set in U0.16, else set in PW16E2
    /////////////////////////////////////////////////////////////////
    uint16_t    BackgroundA;                // U0.16, Background color A
    uint16_t    BackgroundR;                // FP16 or PW16E2 or U0.16, Background color R
    uint16_t    BackgroundG;                // FP16 or PW16E2 or U0.16, Background color G
    uint16_t    BackgroundB;                // FP16 or PW16E2 or U0.16, Background color B

    /////////////////////////////////////////////////////////////////
    // OCSC0 + OETF + OCSC1
    /////////////////////////////////////////////////////////////////
    uint32_t    OetfVssLutMode      : 2;    // 0: Segmented;   1: Direct8;
    uint32_t    OetfVssLutInterp    : 1;    // Enable Interpolation
    uint32_t    OetfVssLutMirror    : 1;    // Enable Mirror Operation
    uint32_t    OetfCurveMode       : 4;    // 0-15, Curve Table index in the external surface
    uint32_t    OetfDirectRound     : 1;    // Rounding mode in direct mode: 0 - floor, 1 - half up
    uint32_t    OetfNoiseThreshold  : 6;    // U3.3, up to 7.875, in 1/1024 units, FP16 noise suppression threshold, 0 value setting to disable
    uint32_t    reserved0_oetf_cfg0 : 1;    //
    uint32_t    reserved1_oetf_cfg0 : 16;   //

    uint32_t    OetfFpNorm;                 // u0.32: Normalization factor from FP16 luminance to u0.32

    int32_t     OCsc0MatCoeff[3][4];        // S5.16: Output CSC0
    int32_t     OCsc1MatCoeff[3][4];        // S5.16: Output CSC1

    uint32_t    OutGRandomSeed;             // U16: OutG random seed, used when OutGRoundMode is 3(Stochastic rounding)

    // data format of Clamp Min/Max registers depends on the output pixel format: Fixed point format -> U0.16, float point format -> FP16
    uint16_t    OutGClampRMin;              // U0.16 or FP16, min valve of R/Y component clamp.
    uint16_t    OutGClampRMax;              // U0.16 or FP16, max valve of R/Y component clamp.
    uint16_t    OutGClampGMin;              // U0.16 or FP16, min valve of G/U component clamp.
    uint16_t    OutGClampGMax;              // U0.16 or FP16, max valve of G/U component clamp.
    uint16_t    OutGClampBMin;              // U0.16 or FP16, min valve of B/V component clamp.
    uint16_t    OutGClampBMax;              // U0.16 or FP16, max valve of B/V component clamp.

    uint32_t    reserved_oc_align[16];      //

    // subframe processing setting
    uint16_t    SubFrameRectLeft;           // U14, Subframe rectangle
    uint16_t    SubFrameRectRight;          // U14, Pixels outside of this area are guaranteed to remain unmodified
    uint16_t    SubFrameRectTop;            // U14
    uint16_t    SubFrameRectBottom;         // U14

    uint32_t    reserved_oc_align2[34];

    //////////////////////////////
    // OutHub (enum OUTHUB_SOURCE)
	// Only OutG can support FP16 output via OCSC0
    //////////////////////////////
    uint32_t    OutgSrcSel          : 3;    // valid: OCSC0, OETF, OCSC1
    uint32_t    PpSrcSel            : 3;    // valid: OCSC1
    uint32_t    TnrSrcSel           : 3;    // valid: OETF,  OCSC1
    uint32_t    reserved_outhub_cfg : 23;

    ///////////////////////////////////////
    // PP: Xsobel and 4x4 DownSample
    ///////////////////////////////////////
    uint32_t    XSobelMode          : 2;   // XSobel control bit
                                           // 2'b00: disable xsobel and disable 4x4 down sample
                                           // 2'b01: Not allowed
                                           // 2'b10: enable xsobel and disable 4x4 downs sample
                                           // 2'b11: enable xsobel and enable 4x4 down sample
    uint32_t    XSobelBlkKind       : 2;   // Xsobel gradient image Block-linear kind
    uint32_t    XSobelBlkHeight     : 4;   // Block-linear height (in gobs, log2), 0: 1 GOB; 1: 2 GOBs; 2: 4 GOBs; 3: 8 GOBs; 4: 16 GOBs; 5: 32 GOBs
    uint32_t    XSobelDsBlkKind     : 2;   // Xsobel gradient 4x4 downsample image Block-linear kind
    uint32_t    XSobelDsBlkHeight   : 4;   // Block-linear height (in gobs, log2), 0: 1 GOB; 1: 2 GOBs; 2: 4 GOBs; 3: 8 GOBs; 4: 16 GOBs; 5: 32 GOBs
    uint32_t    reserved_ppcfg      : 18;  //


    uint32_t    PPPixelFormat       : 10;  // Pixel format for each slot (PIXEL_FORMAT)
    uint32_t    reserved_xs_1       : 22;  // Output pixel format supported list + add restrition

    uint32_t    XSobelSfcPad        : 14;  // Xsobel output surface stride in pixel unit
    uint32_t    reserved_xs_2       : 2;   //
    uint32_t    XSobelDsSfcPad      : 14;  // Xsobel downsample  output surface stride in pixel unit
    uint32_t    reserved_xs_3       : 2;   //

    ///////////////////////////////////////
    // OutCache control
    ///////////////////////////////////////
    uint32_t    OutCacheCoreEn             : 1;   // OutCache Core Enable
    uint32_t    OutCacheCoreBeatOptEn      : 1;   // OutCache Core Output Partial Beats Optimization Enable
    uint32_t    reserved_oc_0              : 6;  //
    uint32_t    OutCacheOutGCurFrmCH0En    : 1;   // OutCache OutG Current Frame Plane 0/1/2 Enable
    uint32_t    OutCacheOutGCurFrmCH1En    : 1;   //
    uint32_t    OutCacheOutGCurFrmCH2En    : 1;   //
    uint32_t    OutCacheTNRCurFrmCH0En     : 1;   // OutCache TNR Current Frame Plane 0/1/2 Enable
    uint32_t    OutCacheTNRCurFrmCH1En     : 1;   //
    uint32_t    OutCacheTNRCurFrmCH2En     : 1;   //
    uint32_t    OutCacheTNRCurAlphaEn      : 1;   // OutCache TNR Current alpha Enable
    uint32_t    OutCachePpXSobelEn         : 1;   // PP Xsobel
    uint32_t    OutCachePpXSobelDsEn       : 1;   // PP Xsobel 4x4 downsample
    uint32_t    reserved_oc_1              : 15;  //

    ///////////////////////////////////////
    // Weighted RoundRobin Setting
    ///////////////////////////////////////
    uint32_t    OutCacheWeightTnrNbWr      : 3;   // Weighted round-robin arbiter: Tnr neighbor write
    uint32_t    OutCacheWeightCacheCore    : 3;   // Weighted round-robin arbiter: OutCache core
    uint32_t    reserved_ocw_1             : 26;  //

    uint32_t    reserved_align[14];       //


} OutConfig;

//////////////////////////////
// TNR
//////////////////////////////
typedef struct _TNRConfig {
    uint32_t TnrEn                   : 1;   // TNR enable/disable switch
    uint32_t AlphaBlend              : 1;   // set 1 to enable temporal blending (blend previous frame into current frame). Will not work if previous frame surface is not provided.
    uint32_t BetaBlend               : 1;   // set 1 to enable spatial blending (spatial filtering result into current frame). Will not work if previous frame surface is not provided.
    uint32_t BilateralRange          : 1;   // set 1 to enable range filter of spatial filtering
    uint32_t BilateralDomain         : 1;   // set 1 to enable domain filter of spatial filtering
    uint32_t AlphaSmooth             : 1;   // set 1 to enable alpha spatial smooth from neighbor
    uint32_t AlphaTempRestrict       : 1;   // set 1 to enable alpha temporal restriction from previous frame alpha. Will not work if previous alpha surface is not provided.
    uint32_t AlphaClip               : 1;   // set 1 to enable clip with AlphaMin/AlphaMax.
    uint32_t AlphaFastRecoverLutEn   : 1;   // (alpha fast recovery) set 1 to add a lut-based cap to temporal alpha restriction.
    uint32_t LegacyModeEn            : 1;   // set 1 to enable lagecy mode, with same config result will be bit-accurate with legacy TNR3. This effectively 12bit internal precision instead of 16bit.
    uint32_t PowerSave8BEn           : 1;   // set 1 to enable tnr 8bit power saving mode. Note that tnr input after cdns is assumed to have lsbs all zero.
    uint32_t reserved_tnr_cfg        : 21;

    uint32_t TnrOutFormat            : 10;   // Pixel format of tnr output(PIXEL_FORMAT)
    uint32_t TnrOutBlkKind           : 2;    // tnr output Block-linear kind (BLK_KIND)
    uint32_t TnrOutBlkHeight         : 4;    // Block-linear height (in gobs, log2), 0: 1 GOB; 1: 2 GOBs; 2: 4 GOBs; 3: 8 GOBs; 4: 16 GOBs; 5: 32 GOBs
    uint32_t TnrBitDepthUpConvMode   : 2;    // 0: auto mode (RGB: MSB replicate, YUV: zero padding); 1: YUV/RGB zero padding; 2: YUV/RGB MSB replicate
    uint32_t TnrOutGRoundMode        : 2;    // tnr output rounding mode, 0: Rounding towards zero, 1: Rounding half away from zero 2: Rounding half to even, 3: Stochastic rounding
    uint32_t TnrOutChromaLocHoriz    : 2;    // Horizontal chroma location - (0: co-located with even luma; 1: in between even and odd luma; 2: co-located with odd luma; 3: reserved)
    uint32_t TnrOutChromaLocVert     : 2;    // Vertical chroma location - (0: co-located with even luma; 1: in between even and odd luma; 2: co-located with odd luma; 3: reserved)
    uint32_t TnrAlphaRoundMode       : 1;    // Rounding method in alpha calculation - (0:RHAFZ in alpha blending, TRUNC in alpha calculation; 1: RHTE in alpha blending, RHAFZ in alpha calculation)
    uint32_t TnrBetaRoundMode        : 1;    // Rounding method in beta calculation - (0:RHAFZ in beta blending, TRUNC in beta calculation; 1: RHTE in beta blending, RHAFZ in beta calculation)
    uint32_t reserved_tnr_out_cfg0   : 6;

    // TNR will use global image width/height (i.e. OutImgWidth/OutImgWidth)
    //     TargetRect must be the same as the output image size.
    // surface stride for TNR is defined below
    uint32_t TnrOutLumaSfcWPad       : 16;   // U14, luma stride of tnr output surface minus 1.
    uint32_t TnrOutChromaSfcWPad     : 16;   // U14, chroma stride of tnr output surface minus 1.
    uint32_t reserved_tnr_sfc_cfg0   : 32;

    // TNR alpha surface will be stored in BlockLinear height==2 layout.
    uint32_t TnrAlphaSfcStride       : 16;   // U14, stride of alpha output and prev_alpha surface. Stride should be aligned to BlockLinear layout.
    uint32_t reserved_tnr_sfc_cfg1   : 16;

    uint32_t SadWeightLuma           : 6;   // U1.5, Weight of luma channel SAD, chroma_weights = (1-luma_weight)/2. Luma weight is applied to G channel for RGB.
    uint32_t SadMultipler            : 6;   // U3.3, Sad multiply factor before converting to alpha.
    uint32_t reserved_tnr_sad_cfg    : 20;

    uint32_t AlphaTempIncCap         : 11;  // U1.10, Alpha temporal restriction capacity. Alpha value of current pixel cannot excess (alpha of the same pixel previous frame + capacity)
    uint32_t AlphaScaleIir           : 11;  // U1.10, Alpha scale factor iir to control the strength of alpha-beta blending. Will scale alpha by iir/1024 after clamping alpha into [AlphaMin, AlphaMax]
    uint32_t reserved_tnr_alpha_cfg0 : 10;

    uint32_t AlphaMaxLuma            : 11;  // U1.10 each, Alpha clamp range.
    uint32_t AlphaMinLuma            : 11;  // When AlphaClip is enabled, luma/chroma alpha conducted from SAD will be clamped into corresponding [AlphaMin, AlphaMax]
    uint32_t reserved_tnr_alpha_cfg1 : 10;
    uint32_t AlphaMaxChroma          : 11;
    uint32_t AlphaMinChroma          : 11;
    uint32_t reserved_tnr_alpha_cfg2 : 10;

    /////////////////////////////
    // Two points (BetaX1,BetaMax) (BetaX2, BetaMin) decides alpha-beta curve:
    //
    // For alpha>=BetaX1, beta = BetaMax
    // For alpha<=BetaX2, beta = BetaMin
    // For BetaX1>alpha>BetaX2, beta = (alpha-BetaX1) * BetaStep,
    //         where BetaStep = (BetaMax-BetaMin) / (BetaX1-BetaX2).
    /////////////////////////////
    uint32_t BetaMax                 : 11;  // U1.10
    uint32_t BetaMin                 : 11;  // U1.10
    uint32_t reserved_tnr_beta_cfg0  : 10;
    uint32_t BetaX1                  : 11;  // U1.10 Alpha value when Beta reaches maximum
    uint32_t BetaX2                  : 11;  // U1.10 Alpha value when Beta reaches minimum
    uint32_t reserved_tnr_beta_cfg1  : 10;
    uint32_t BetaCalcStep            : 11;  //U6.5, Beta Calculation Step. beta = maxBeta - (alpha  betaX1) * BetaStep. BetaStep value here is positive.

    uint32_t reserved_tnr_beta_cfg2  : 21;

    uint32_t reserved_tnr_0[36];            // leave [0,48) to configs, tables will follow

    /////////////////////////////
    // Domain Coefficient Table are originazied as below:
    // LumaCoefficients{c0,c1,c2,c3,c4,c5}, ChromaCoefficients{c0,c1,c2,c3,c4,c5}
    //
    // Actual filters will be constructed with c0..c5 from center:
    //   c5 c4 c3 c4 c5
    //   c4 c2 c1 c2 c4
    //   c4 c1 c0 c1 c3
    //   c4 c2 c1 c2 c4
    //   c5 c4 c5 c4 c5
    /////////////////////////////
    uint8_t  BilateralDomainCoeffTable[12]; // domain filter coeffs 6 * 7 * (luma+chroma), use 8bit each for better alignment

    /////////////////////////////
    // Range Coefficient Table are originazied as below:
    // BilateralRangeLumaTable:   LumaCoefficients{c0,c1,c2, ... c63}
    // BilateralRangeChromaTable: ChromaCoefficients{c0,c1,c2, ... c63}
    //
    // Actual filters entry are visited by diff(curr,prev) >> shift,
    //     Shift value is configured in BilateralRangeLumaShift and BilateralRangeChromaShift
    //     To comply with legacy, diff is calculated based on 12bit values.
    /////////////////////////////
    uint32_t BilateralRangeLumaShift   : 4;
    uint32_t BilateralRangeChromaShift : 4;
    uint32_t reserved_tnr_bfrange_cfg  : 24;
    uint8_t  BilateralRangeLumaTable[64];    //64*(7coeff+1alignment)
    uint8_t  BilateralRangeChromaTable[64];

    uint16_t AlphaLutValue[9];		// 9 * 11bit lut value.
    uint16_t reserved_alpha_lut_values : 16;

    uint32_t reserved_tnr_1[39];        // tables in [48,128)
} TNRConfig;

/////////////////////////////////////////////////
// PXLG Filter Config
/////////////////////////////////////////////////
typedef struct _FilterConfig{
    //non-LDC polyphase filter
    uint32_t    FilterOverride      : 1;    //non-LDC filter override enable
    uint32_t    reserved0           : 31;

    //2D convolution filter, Pout = FilterAdjustFactor * Sum(GeoFilterCoeff2D[i][j] * Pij) >> Log2CoeffSum
    uint32_t    Log2CoeffSum        : 4;    // the sum of filter coeffs must be 2^N, Log2CoeffSum = log2(Sum(GeoFilterCoeff2D))
    uint32_t    FilterAdjustFactor  : 9;    // U1.8
    uint32_t    reserved1           : 19;

    uint32_t    EdgeEnhanceEn       : 3;    // per component control; when EdgeEnhanceEn > 0, gain = FilterAdjustFactor * Sum (Cij * Pij),
                                            // Pout = Input + (abs(gain) < ThreshLow ? 0 : clip3(gain, -ThreshHigh, ThreshHigh)))
    uint32_t    FilterThreshLow     : 12;   // used when EdgeEnhanceEn = 1, 0 means disable
    uint32_t    FilterThreshHigh    : 12;   // used when EdgeEnhanceEn = 1, 0 means disable
    uint32_t    reserved2           : 5;

    uint32_t    reserved3[1];

    /////////////////////////////
    // LDC polyphase Filter Coefficients Register, 4 taps
    // C0+C1+C2+C3 = 256, only C0-3 are all assigned.
    // 32 phases symmetrical polyphase filter with 10bit precision
    // phase 0-16 are list here, and 17-31 use inverse 15-1 phase version.
    // Phase[17] = Phase[15], Phase[18] = Phase[14],
	// Filter X/Y and luma/chroma use the same coeff table
    /////////////////////////////
    int16_t     GeoFilterCoeff[17][4];      // used when PxlgFilterMode = 1

    // 2D convolution Filter Coefficients Register, 5 taps
    // 10bit precision, C[0][0] + C[0][1] + ... + C[4][4] = 2^N (N = 0, 1, 2, ...)
    // Actual filters will be constructed with c0..c5 from center:
    // c5   c4  c3  c4  c5
    // c4   c2  c1  c2  c4
    // c3   c1  c0  c1  c3
    // c4   c2  c1  c2  c4
    // c5   c4  c3  c4  c5
    int16_t     GeoFilterCoeff2D[6];        // used when PxlgFilterMode = 2

    uint32_t    reserved5[23];
}FilterConfig; //total 256 bytes

typedef struct _ClearRectStruct {
    uint16_t    ClearRectLeft;              // Clear Rectangle to improve performance
    uint16_t    ClearRectRight;             //
    uint16_t    ClearRectTop;               //
    uint16_t    ClearRectBottom;            //
} ClearRectStruct;

typedef struct _CompositorConfigStruct {
    OutConfig       outConfig;
    ClearRectStruct clearRect[8];
    SlotConfig      slotConfig[8];
    TNRConfig       tnrConfig;
    FilterConfig    filterConfig;
} CompositorConfigStruct;


///////////////////////////////////////////////////
// VSS-LUT surface structure
// R = L = Cr = Cp
// G = M = Y  = I
// B = S = Cb = Ct
///////////////////////////////////////////////////
typedef struct _VSSLUT_entry {
    uint16_t  Red;              // VSSLUT entry value (fxpt or FP16)
    uint16_t  Green;            //
    uint16_t  Blue;             //
    uint16_t  unused;           //
} VSSLUT_entry;

typedef struct _VSSLUT_header {
    uint64_t  lns_0 :    3;  // VSSLUT entry value (fxpt or FP16)
    uint64_t  lns_1 :    3;  //
    uint64_t  lns_2 :    3;  //
    uint64_t  lns_3 :    3;  //
    uint64_t  lns_4 :    3;  //
    uint64_t  lns_5 :    3;  //
    uint64_t  lns_6 :    3;  //
    uint64_t  lns_7 :    3;  //
    uint64_t  lns_8 :    3;  //
    uint64_t  lns_9 :    3;  //
    uint64_t  lns_10 :   3;  //
    uint64_t  lns_11 :   3;  //
    uint64_t  lns_12 :   3;  //
    uint64_t  lns_13 :   3;  //
    uint64_t  lns_14 :   3;  //
    uint64_t  lns_15 :   3;  //
    uint64_t  reserved : 16; //
} VSSLUT_header;

typedef struct _EOTF_VSSLUT_curve {
   VSSLUT_header curve_header[4];
   VSSLUT_entry  curve_sample[513];
   uint32_t  reserved[54];
} EOTF_VSSLUT_curve;

typedef struct _OETF_VSSLUT_curve {
   VSSLUT_header curve_header[4];
   VSSLUT_entry  curve_sample[513];
   uint32_t  reserved[54];
} OETF_VSSLUT_curve;

typedef struct _TM_VSSLUT_curve {
   VSSLUT_header curve_header[4];
   VSSLUT_entry  curve_sample[1025];
   uint32_t  reserved[54];
} TM_VSSLUT_curve;


///////////////////////////////////////////////////
// Filter Coefficient buffer
// Scale ratio:   1:1, 2:1, 4:1, 8:1, 16:1
// Filter type:   Normal, Default, Detail
// Filter length: 5 tap or 10 tap
// Phase: 0-16 phases
///////////////////////////////////////////////////
typedef struct _FilterCoeffStruct {
    uint32_t  f00                 : 10;   // 9..0
    uint32_t  f10                 : 10;   // 19..10
    uint32_t  f20                 : 10;   // 29..20
    uint32_t  reserved0           : 2;    // 31..30
    uint32_t  f01                 : 10;   // 41..32
    uint32_t  f11                 : 10;   // 51..42
    uint32_t  f21                 : 10;   // 61..52
    uint32_t  reserved1           : 2;    // 63..62
    uint32_t  f02                 : 10;   // 73..64
    uint32_t  f12                 : 10;   // 83..74
    uint32_t  f22                 : 10;   // 93..84
    uint32_t  reserved2           : 2;    // 95..94
    uint32_t  f03                 : 10;   // 105..96
    uint32_t  f13                 : 10;   // 115..106
    uint32_t  f23                 : 10;   // 125..116
    uint32_t  reserved3           : 2;    // 127..126
} FilterCoeffStruct;

typedef struct _FilterStruct {
    FilterCoeffStruct  filterCoeffStruct[520];
} FilterStruct;

///////////////////////////////////////////////////
// Surface CRC
///////////////////////////////////////////////////
typedef struct _VicSfcCrcStruct {
    uint32_t  crcEnable           : 1;    // surface crc enable control
    uint32_t  vaCmpEnable         : 1;    // 0: disable fbif va, 1: enable fbif va
    uint32_t  reserved_0          : 1;
    uint32_t  cmpMode             : 1;    // 0: HW CRC compare, 1: ucode read CRC compare
    uint32_t  reserved_cfg        : 28;
    uint32_t  cmpResult           : 6;    // 1:0 for Plane 0; 3:2 for Plane1; 5:4 for Plane2
    uint32_t  reserved_rel        : 26;
    uint32_t  sfcCrcGold[3][2];           // OutG 3 DMAs for upto 3 planes, 2 crc150 instanced
} VicSfcCrcStruct;

///////////////////////////////////////////////////
// Interface CRC
///////////////////////////////////////////////////
typedef struct _VicIntfCrcStruct {
    uint32_t  crcEnable           : 1;    // interface CRC enable control
    uint32_t  vaCmpEnable         : 1;    // 0: disable fbif va, 1: enable fbif va
    uint32_t  reserved_0          : 1;
    uint32_t  cmpMode             : 1;    // 0: HW CRC compare, 1: ucode read CRC compare
    uint32_t  reserved_cfg        : 28;
    uint32_t  cmpResultPartA      : 16;   // Compare result for Partition A
    uint32_t  cmpResultPartB      : 16;   // Compare result for Partition B
    uint32_t  cmpResultPartC      : 16;   // Compare result for Partition C
    uint32_t  reserved_rel        : 16;
    uint32_t  intfCrcGoldPartA[16];
    uint32_t  intfCrcGoldPartB[16];
    uint32_t  intfCrcGoldPartC[16];
} VicIntfCrcStruct;

///////////////////////////////////////////////////
// Fp16 invalid values status report
///////////////////////////////////////////////////
typedef struct _VicInvlidFp16WarnStruct {
    uint32_t sc_posINF_warning         : 1;
    uint32_t sc_negINF_warning         : 1;
    uint32_t sc_NaN_warning            : 1;
    uint32_t fitp_csc0_posINF_warning  : 1;
    uint32_t fitp_csc0_negINF_warning  : 1;
    uint32_t fitp_csc0_NaN_warning     : 1;
    uint32_t bitp_csc1_posINF_warning  : 1;
    uint32_t bitp_csc1_negINF_warning  : 1;
    uint32_t bitp_csc1_NaN_warning     : 1;
    uint32_t blender_posINF_warning    : 1;
    uint32_t blender_negINF_warning    : 1;
    uint32_t blender_NaN_warning       : 1;
    uint32_t ocsc0_posINF_warning      : 1;
    uint32_t ocsc0_negINF_warning      : 1;
    uint32_t ocsc0_NaN_warning         : 1;
    uint32_t outg_posINF_warning       : 1;
    uint32_t outg_negINF_warning       : 1;
    uint32_t outg_NaN_warning          : 1;
    uint32_t reserved_rel              : 14;
} VicInvlidFp16WarnStruct;

///////////////////////////////////////////////////
// vic status report from ucode to driver
///////////////////////////////////////////////////
typedef struct _vic_status_s
{
    uint32_t cycle_count;                 // total cycles taken for execute, to calculate hw and ucode performance
    uint32_t error_status;                // report error if any
    VicInvlidFp16WarnStruct fp16_invalid;

} vic_status_s;


///////////////////////////////////////////////////
// vic miscellaneous config that won't affect output result, but may affect perf, power, bandwidth etc.
///////////////////////////////////////////////////
#define VIC_SLC_CFG_FIELDS_LITE(client_en, buffer_type, axcache) ((client_en) | ((buffer_type) << 1) | ((axcache) << 3))

#define VIC_SLC_CFG_FIELDS_FULL(client_en, buffer_type, axcache, pkt_hint_override, override_buffer_type, override_axcache) ((client_en) | ((buffer_type) << 1) | ((axcache) << 3) | ((pkt_hint_override) << 7) | ((override_buffer_type) << 8) | ((override_axcache) << 10))

#define SET_SC_SURFACE_MAP_SLOT(slot_id, sfc_id)  (((sfc_id) & 0x7U) << ((slot_id) * 3))

typedef struct _VicMiscConfig
{
    // SLC Configure
    uint32_t slc_en                             : 1;      // slc hint control enable
    uint32_t default_buffer_type                : 2;      // default value for buffer_type
    uint32_t default_awcache                    : 4;      // default value for awcache
    uint32_t default_arcache                    : 4;      // default value for arcache
    uint32_t slc_rsv                            : 21;

    ///////////////////////////////////////////////////
    // SLC Configuration: for one FBIF client:
    // 0:0     ClientEn,           Client SLC hint enable control
    // 2:1     BufferType,         BufferType Setting at frame level
    // 6:3     AxCache,            AwCache for write FBIF client, and ArCache for read FBIF client
    // 7:7     PktHintOverride,    Optional, packet-level hint control, overriden by h/w logic
    // 9:8     OverrideBufferType, Optional, packet-level hint control, overriden by h/w logic with optimize case condition
    // 13:10   OverrideAxCache,    Optional, packet-level hint control, overriden by h/w logic with optimize case condition
    // 15:14   Reserved
    ///////////////////////////////////////////////////
    uint16_t slc_fbif_client_cfg[19];                     // slc per-fbif-client ctrl
    uint16_t reserved_slc[1];

    ////////////////////////////////////////////////////
    // SC surface_map configuration:
    // sc_surface_map_en: 0 - disable, 1-enable, set to 1 (and program sc_surface_map_slots properly) can boost SC performance if multiple slots use same surface as input
    // sc_surface_map_slots: slot_id to surface_id map, 3 bits per slot, usage example:
    // slot  7      6      5      4      3      2      1      0
    // bits  23:21  20:18  17:15  14:12  11:9   8:6    5:3    2:0
    // val   0d7    0d6    0d5    0d4    0d3    0d2    0d1    0d0    each slot has it's own input surface.
    // val   0d7    0d6    0d1    0d4    0d3    0d1    0d1    0d0    slot 1, slot 2 and slot 5 share the same input surface
    // val   0d1    0d6    0d5    0d4    0d0    0d0    0d1    0d0    slot 0, slot 2 and slot 3 share the same input surface, slot 1 and slot 7 shar the same input surface.
    ///////////////////////////////////////////////////
    // SC_SURFACE_MAP
    uint32_t sc_surface_map_en                  : 1;
    uint32_t sc_surface_map_slots               : 24;
    uint32_t sc_surface_map_reserved1           : 7;

    // COMP_CYA
    uint32_t sc_alloc_ack_slow_dis              : 1;
    uint32_t sc_alloc_ack_adjust_dis            : 1;
    uint32_t sc_alloc_ack_hitcnt_max            : 10;
    uint32_t sc_alloc_ack_rdat_waitnum_min      : 10;
    uint32_t comp_cya_reserved1                 : 2;
    uint32_t sc_force_pri2_zero_dis             : 1;
    uint32_t comp_cya_reserved2                 : 7;

    uint32_t reserved_align[51];
} VicMiscConfig;

/////////////////////////////////////////////////
// Macro define to align between SW and HW
/////////////////////////////////////////////////
#define ALIGN_UP(v, n)        (((v) + ((n)-1)) &~ ((n)-1))
#define VIC_ALIGN(value)      ALIGN_UP(value,256)              // Align to 256 bytes

#define VIC_TILE_WIDTH 64
#define VIC_TILE_HEIGHT 32

#define VIC_SPARSE_WARP_MAP_PAD(w)    ALIGN_UP(w, 128/8)       // 16 control points aligned


#ifdef __cplusplus
} //namespace nvceb6
#endif
#endif // VIC_CEB6_TYPES_H
