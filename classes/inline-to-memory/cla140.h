/*
 * SPDX-FileCopyrightText: Copyright (c) 2003-2021 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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



#ifndef _cl_kepler_inline_to_memory_b_h_
#define _cl_kepler_inline_to_memory_b_h_

/* AUTO GENERATED FILE -- DO NOT EDIT */
/* Command: ../../../../class/bin/sw_header.pl kepler_inline_to_memory_b */

#include "nvtypes.h"

#define KEPLER_INLINE_TO_MEMORY_B    0xA140

#define NVA140_SET_OBJECT                                                                                  0x0000
#define NVA140_SET_OBJECT_CLASS_ID                                                                           15:0
#define NVA140_SET_OBJECT_ENGINE_ID                                                                         20:16

#define NVA140_NO_OPERATION                                                                                0x0100
#define NVA140_NO_OPERATION_V                                                                                31:0

#define NVA140_SET_NOTIFY_A                                                                                0x0104
#define NVA140_SET_NOTIFY_A_ADDRESS_UPPER                                                                    24:0

#define NVA140_SET_NOTIFY_B                                                                                0x0108
#define NVA140_SET_NOTIFY_B_ADDRESS_LOWER                                                                    31:0

#define NVA140_NOTIFY                                                                                      0x010c
#define NVA140_NOTIFY_TYPE                                                                                   31:0
#define NVA140_NOTIFY_TYPE_WRITE_ONLY                                                                  0x00000000
#define NVA140_NOTIFY_TYPE_WRITE_THEN_AWAKEN                                                           0x00000001

#define NVA140_WAIT_FOR_IDLE                                                                               0x0110
#define NVA140_WAIT_FOR_IDLE_V                                                                               31:0

#define NVA140_SET_GLOBAL_RENDER_ENABLE_A                                                                  0x0130
#define NVA140_SET_GLOBAL_RENDER_ENABLE_A_OFFSET_UPPER                                                        7:0

#define NVA140_SET_GLOBAL_RENDER_ENABLE_B                                                                  0x0134
#define NVA140_SET_GLOBAL_RENDER_ENABLE_B_OFFSET_LOWER                                                       31:0

#define NVA140_SET_GLOBAL_RENDER_ENABLE_C                                                                  0x0138
#define NVA140_SET_GLOBAL_RENDER_ENABLE_C_MODE                                                                2:0
#define NVA140_SET_GLOBAL_RENDER_ENABLE_C_MODE_FALSE                                                   0x00000000
#define NVA140_SET_GLOBAL_RENDER_ENABLE_C_MODE_TRUE                                                    0x00000001
#define NVA140_SET_GLOBAL_RENDER_ENABLE_C_MODE_CONDITIONAL                                             0x00000002
#define NVA140_SET_GLOBAL_RENDER_ENABLE_C_MODE_RENDER_IF_EQUAL                                         0x00000003
#define NVA140_SET_GLOBAL_RENDER_ENABLE_C_MODE_RENDER_IF_NOT_EQUAL                                     0x00000004

#define NVA140_SEND_GO_IDLE                                                                                0x013c
#define NVA140_SEND_GO_IDLE_V                                                                                31:0

#define NVA140_PM_TRIGGER                                                                                  0x0140
#define NVA140_PM_TRIGGER_V                                                                                  31:0

#define NVA140_PM_TRIGGER_WFI                                                                              0x0144
#define NVA140_PM_TRIGGER_WFI_V                                                                              31:0

#define NVA140_SET_INSTRUMENTATION_METHOD_HEADER                                                           0x0150
#define NVA140_SET_INSTRUMENTATION_METHOD_HEADER_V                                                           31:0

#define NVA140_SET_INSTRUMENTATION_METHOD_DATA                                                             0x0154
#define NVA140_SET_INSTRUMENTATION_METHOD_DATA_V                                                             31:0

#define NVA140_LINE_LENGTH_IN                                                                              0x0180
#define NVA140_LINE_LENGTH_IN_VALUE                                                                          31:0

#define NVA140_LINE_COUNT                                                                                  0x0184
#define NVA140_LINE_COUNT_VALUE                                                                              31:0

#define NVA140_OFFSET_OUT_UPPER                                                                            0x0188
#define NVA140_OFFSET_OUT_UPPER_VALUE                                                                        24:0

#define NVA140_OFFSET_OUT                                                                                  0x018c
#define NVA140_OFFSET_OUT_VALUE                                                                              31:0

#define NVA140_PITCH_OUT                                                                                   0x0190
#define NVA140_PITCH_OUT_VALUE                                                                               31:0

#define NVA140_SET_DST_BLOCK_SIZE                                                                          0x0194
#define NVA140_SET_DST_BLOCK_SIZE_WIDTH                                                                       3:0
#define NVA140_SET_DST_BLOCK_SIZE_WIDTH_ONE_GOB                                                        0x00000000
#define NVA140_SET_DST_BLOCK_SIZE_HEIGHT                                                                      7:4
#define NVA140_SET_DST_BLOCK_SIZE_HEIGHT_ONE_GOB                                                       0x00000000
#define NVA140_SET_DST_BLOCK_SIZE_HEIGHT_TWO_GOBS                                                      0x00000001
#define NVA140_SET_DST_BLOCK_SIZE_HEIGHT_FOUR_GOBS                                                     0x00000002
#define NVA140_SET_DST_BLOCK_SIZE_HEIGHT_EIGHT_GOBS                                                    0x00000003
#define NVA140_SET_DST_BLOCK_SIZE_HEIGHT_SIXTEEN_GOBS                                                  0x00000004
#define NVA140_SET_DST_BLOCK_SIZE_HEIGHT_THIRTYTWO_GOBS                                                0x00000005
#define NVA140_SET_DST_BLOCK_SIZE_DEPTH                                                                      11:8
#define NVA140_SET_DST_BLOCK_SIZE_DEPTH_ONE_GOB                                                        0x00000000
#define NVA140_SET_DST_BLOCK_SIZE_DEPTH_TWO_GOBS                                                       0x00000001
#define NVA140_SET_DST_BLOCK_SIZE_DEPTH_FOUR_GOBS                                                      0x00000002
#define NVA140_SET_DST_BLOCK_SIZE_DEPTH_EIGHT_GOBS                                                     0x00000003
#define NVA140_SET_DST_BLOCK_SIZE_DEPTH_SIXTEEN_GOBS                                                   0x00000004
#define NVA140_SET_DST_BLOCK_SIZE_DEPTH_THIRTYTWO_GOBS                                                 0x00000005

#define NVA140_SET_DST_WIDTH                                                                               0x0198
#define NVA140_SET_DST_WIDTH_V                                                                               31:0

#define NVA140_SET_DST_HEIGHT                                                                              0x019c
#define NVA140_SET_DST_HEIGHT_V                                                                              31:0

#define NVA140_SET_DST_DEPTH                                                                               0x01a0
#define NVA140_SET_DST_DEPTH_V                                                                               31:0

#define NVA140_SET_DST_LAYER                                                                               0x01a4
#define NVA140_SET_DST_LAYER_V                                                                               31:0

#define NVA140_SET_DST_ORIGIN_BYTES_X                                                                      0x01a8
#define NVA140_SET_DST_ORIGIN_BYTES_X_V                                                                      20:0

#define NVA140_SET_DST_ORIGIN_SAMPLES_Y                                                                    0x01ac
#define NVA140_SET_DST_ORIGIN_SAMPLES_Y_V                                                                    16:0

#define NVA140_LAUNCH_DMA                                                                                  0x01b0
#define NVA140_LAUNCH_DMA_DST_MEMORY_LAYOUT                                                                   0:0
#define NVA140_LAUNCH_DMA_DST_MEMORY_LAYOUT_BLOCKLINEAR                                                0x00000000
#define NVA140_LAUNCH_DMA_DST_MEMORY_LAYOUT_PITCH                                                      0x00000001
#define NVA140_LAUNCH_DMA_COMPLETION_TYPE                                                                     5:4
#define NVA140_LAUNCH_DMA_COMPLETION_TYPE_FLUSH_DISABLE                                                0x00000000
#define NVA140_LAUNCH_DMA_COMPLETION_TYPE_FLUSH_ONLY                                                   0x00000001
#define NVA140_LAUNCH_DMA_COMPLETION_TYPE_RELEASE_SEMAPHORE                                            0x00000002
#define NVA140_LAUNCH_DMA_INTERRUPT_TYPE                                                                      9:8
#define NVA140_LAUNCH_DMA_INTERRUPT_TYPE_NONE                                                          0x00000000
#define NVA140_LAUNCH_DMA_INTERRUPT_TYPE_INTERRUPT                                                     0x00000001
#define NVA140_LAUNCH_DMA_SEMAPHORE_STRUCT_SIZE                                                             12:12
#define NVA140_LAUNCH_DMA_SEMAPHORE_STRUCT_SIZE_FOUR_WORDS                                             0x00000000
#define NVA140_LAUNCH_DMA_SEMAPHORE_STRUCT_SIZE_ONE_WORD                                               0x00000001
#define NVA140_LAUNCH_DMA_REDUCTION_ENABLE                                                                    1:1
#define NVA140_LAUNCH_DMA_REDUCTION_ENABLE_FALSE                                                       0x00000000
#define NVA140_LAUNCH_DMA_REDUCTION_ENABLE_TRUE                                                        0x00000001
#define NVA140_LAUNCH_DMA_REDUCTION_OP                                                                      15:13
#define NVA140_LAUNCH_DMA_REDUCTION_OP_RED_ADD                                                         0x00000000
#define NVA140_LAUNCH_DMA_REDUCTION_OP_RED_MIN                                                         0x00000001
#define NVA140_LAUNCH_DMA_REDUCTION_OP_RED_MAX                                                         0x00000002
#define NVA140_LAUNCH_DMA_REDUCTION_OP_RED_INC                                                         0x00000003
#define NVA140_LAUNCH_DMA_REDUCTION_OP_RED_DEC                                                         0x00000004
#define NVA140_LAUNCH_DMA_REDUCTION_OP_RED_AND                                                         0x00000005
#define NVA140_LAUNCH_DMA_REDUCTION_OP_RED_OR                                                          0x00000006
#define NVA140_LAUNCH_DMA_REDUCTION_OP_RED_XOR                                                         0x00000007
#define NVA140_LAUNCH_DMA_REDUCTION_FORMAT                                                                    3:2
#define NVA140_LAUNCH_DMA_REDUCTION_FORMAT_UNSIGNED_32                                                 0x00000000
#define NVA140_LAUNCH_DMA_REDUCTION_FORMAT_SIGNED_32                                                   0x00000001
#define NVA140_LAUNCH_DMA_SYSMEMBAR_DISABLE                                                                   6:6
#define NVA140_LAUNCH_DMA_SYSMEMBAR_DISABLE_FALSE                                                      0x00000000
#define NVA140_LAUNCH_DMA_SYSMEMBAR_DISABLE_TRUE                                                       0x00000001

#define NVA140_LOAD_INLINE_DATA                                                                            0x01b4
#define NVA140_LOAD_INLINE_DATA_V                                                                            31:0

#define NVA140_SET_I2M_SEMAPHORE_A                                                                         0x01dc
#define NVA140_SET_I2M_SEMAPHORE_A_OFFSET_UPPER                                                              24:0

#define NVA140_SET_I2M_SEMAPHORE_B                                                                         0x01e0
#define NVA140_SET_I2M_SEMAPHORE_B_OFFSET_LOWER                                                              31:0

#define NVA140_SET_I2M_SEMAPHORE_C                                                                         0x01e4
#define NVA140_SET_I2M_SEMAPHORE_C_PAYLOAD                                                                   31:0

#define NVA140_SET_I2M_SPARE_NOOP00                                                                        0x01f0
#define NVA140_SET_I2M_SPARE_NOOP00_V                                                                        31:0

#define NVA140_SET_I2M_SPARE_NOOP01                                                                        0x01f4
#define NVA140_SET_I2M_SPARE_NOOP01_V                                                                        31:0

#define NVA140_SET_I2M_SPARE_NOOP02                                                                        0x01f8
#define NVA140_SET_I2M_SPARE_NOOP02_V                                                                        31:0

#define NVA140_SET_I2M_SPARE_NOOP03                                                                        0x01fc
#define NVA140_SET_I2M_SPARE_NOOP03_V                                                                        31:0

#define NVA140_SET_FALCON00                                                                                0x0200
#define NVA140_SET_FALCON00_V                                                                                31:0

#define NVA140_SET_FALCON01                                                                                0x0204
#define NVA140_SET_FALCON01_V                                                                                31:0

#define NVA140_SET_FALCON02                                                                                0x0208
#define NVA140_SET_FALCON02_V                                                                                31:0

#define NVA140_SET_FALCON03                                                                                0x020c
#define NVA140_SET_FALCON03_V                                                                                31:0

#define NVA140_SET_FALCON04                                                                                0x0210
#define NVA140_SET_FALCON04_V                                                                                31:0

#define NVA140_SET_FALCON05                                                                                0x0214
#define NVA140_SET_FALCON05_V                                                                                31:0

#define NVA140_SET_FALCON06                                                                                0x0218
#define NVA140_SET_FALCON06_V                                                                                31:0

#define NVA140_SET_FALCON07                                                                                0x021c
#define NVA140_SET_FALCON07_V                                                                                31:0

#define NVA140_SET_FALCON08                                                                                0x0220
#define NVA140_SET_FALCON08_V                                                                                31:0

#define NVA140_SET_FALCON09                                                                                0x0224
#define NVA140_SET_FALCON09_V                                                                                31:0

#define NVA140_SET_FALCON10                                                                                0x0228
#define NVA140_SET_FALCON10_V                                                                                31:0

#define NVA140_SET_FALCON11                                                                                0x022c
#define NVA140_SET_FALCON11_V                                                                                31:0

#define NVA140_SET_FALCON12                                                                                0x0230
#define NVA140_SET_FALCON12_V                                                                                31:0

#define NVA140_SET_FALCON13                                                                                0x0234
#define NVA140_SET_FALCON13_V                                                                                31:0

#define NVA140_SET_FALCON14                                                                                0x0238
#define NVA140_SET_FALCON14_V                                                                                31:0

#define NVA140_SET_FALCON15                                                                                0x023c
#define NVA140_SET_FALCON15_V                                                                                31:0

#define NVA140_SET_FALCON16                                                                                0x0240
#define NVA140_SET_FALCON16_V                                                                                31:0

#define NVA140_SET_FALCON17                                                                                0x0244
#define NVA140_SET_FALCON17_V                                                                                31:0

#define NVA140_SET_FALCON18                                                                                0x0248
#define NVA140_SET_FALCON18_V                                                                                31:0

#define NVA140_SET_FALCON19                                                                                0x024c
#define NVA140_SET_FALCON19_V                                                                                31:0

#define NVA140_SET_FALCON20                                                                                0x0250
#define NVA140_SET_FALCON20_V                                                                                31:0

#define NVA140_SET_FALCON21                                                                                0x0254
#define NVA140_SET_FALCON21_V                                                                                31:0

#define NVA140_SET_FALCON22                                                                                0x0258
#define NVA140_SET_FALCON22_V                                                                                31:0

#define NVA140_SET_FALCON23                                                                                0x025c
#define NVA140_SET_FALCON23_V                                                                                31:0

#define NVA140_SET_FALCON24                                                                                0x0260
#define NVA140_SET_FALCON24_V                                                                                31:0

#define NVA140_SET_FALCON25                                                                                0x0264
#define NVA140_SET_FALCON25_V                                                                                31:0

#define NVA140_SET_FALCON26                                                                                0x0268
#define NVA140_SET_FALCON26_V                                                                                31:0

#define NVA140_SET_FALCON27                                                                                0x026c
#define NVA140_SET_FALCON27_V                                                                                31:0

#define NVA140_SET_FALCON28                                                                                0x0270
#define NVA140_SET_FALCON28_V                                                                                31:0

#define NVA140_SET_FALCON29                                                                                0x0274
#define NVA140_SET_FALCON29_V                                                                                31:0

#define NVA140_SET_FALCON30                                                                                0x0278
#define NVA140_SET_FALCON30_V                                                                                31:0

#define NVA140_SET_FALCON31                                                                                0x027c
#define NVA140_SET_FALCON31_V                                                                                31:0

#define NVA140_SET_RENDER_ENABLE_A                                                                         0x1550
#define NVA140_SET_RENDER_ENABLE_A_OFFSET_UPPER                                                               7:0

#define NVA140_SET_RENDER_ENABLE_B                                                                         0x1554
#define NVA140_SET_RENDER_ENABLE_B_OFFSET_LOWER                                                              31:0

#define NVA140_SET_RENDER_ENABLE_C                                                                         0x1558
#define NVA140_SET_RENDER_ENABLE_C_MODE                                                                       2:0
#define NVA140_SET_RENDER_ENABLE_C_MODE_FALSE                                                          0x00000000
#define NVA140_SET_RENDER_ENABLE_C_MODE_TRUE                                                           0x00000001
#define NVA140_SET_RENDER_ENABLE_C_MODE_CONDITIONAL                                                    0x00000002
#define NVA140_SET_RENDER_ENABLE_C_MODE_RENDER_IF_EQUAL                                                0x00000003
#define NVA140_SET_RENDER_ENABLE_C_MODE_RENDER_IF_NOT_EQUAL                                            0x00000004

#define NVA140_SET_RENDER_ENABLE_OVERRIDE                                                                  0x1944
#define NVA140_SET_RENDER_ENABLE_OVERRIDE_MODE                                                                1:0
#define NVA140_SET_RENDER_ENABLE_OVERRIDE_MODE_USE_RENDER_ENABLE                                       0x00000000
#define NVA140_SET_RENDER_ENABLE_OVERRIDE_MODE_ALWAYS_RENDER                                           0x00000001
#define NVA140_SET_RENDER_ENABLE_OVERRIDE_MODE_NEVER_RENDER                                            0x00000002

#define NVA140_SET_MME_SHADOW_SCRATCH(i)                                                           (0x3400+(i)*4)
#define NVA140_SET_MME_SHADOW_SCRATCH_V                                                                      31:0

#endif /* _cl_kepler_inline_to_memory_b_h_ */
