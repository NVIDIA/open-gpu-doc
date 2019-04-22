/*
 * Copyright (c) 1993-2014, NVIDIA CORPORATION. All rights reserved.
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */


#ifndef _cl907b_h_
#define _cl907b_h_

#ifdef __cplusplus
extern "C" {
#endif

#define NV907B_OVERLAY_IMM_CHANNEL_PIO                                          (0x0000907B)

typedef volatile struct {
    NvV32 Reserved00[0x2];
    NvV32 Free;                                                                 // 0x00000008 - 0x0000000B
    NvV32 Reserved01[0x1D];
    NvV32 Update;                                                               // 0x00000080 - 0x00000083
    NvV32 SetPointOut;                                                          // 0x00000084 - 0x00000087
    NvV32 AwakenOnceFlippedTo;                                                  // 0x00000088 - 0x0000008B
    NvV32 Reserved02[0x3DD];
} GF110DispOverlayImmControlPio;

#define NV907B_FREE                                                             (0x00000008)
#define NV907B_FREE_COUNT                                                       5:0
#define NV907B_UPDATE                                                           (0x00000080)
#define NV907B_UPDATE_INTERLOCK_WITH_CORE                                       0:0
#define NV907B_UPDATE_INTERLOCK_WITH_CORE_DISABLE                               (0x00000000)
#define NV907B_UPDATE_INTERLOCK_WITH_CORE_ENABLE                                (0x00000001)
#define NV907B_SET_POINT_OUT                                                    (0x00000084)
#define NV907B_SET_POINT_OUT_X                                                  15:0
#define NV907B_SET_POINT_OUT_Y                                                  31:16
#define NV907B_AWAKEN_ONCE_FLIPPED_TO                                           (0x00000088)
#define NV907B_AWAKEN_ONCE_FLIPPED_TO_AWAKEN_COUNT                              11:0

#ifdef __cplusplus
};     /* extern "C" */
#endif
#endif // _cl907b_h

