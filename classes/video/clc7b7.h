/*
 * SPDX-FileCopyrightText: Copyright (c) 1993-2022 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#ifndef clc7b7_h_
#define clc7b7_h_

#include "nvtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NVC7B7_VIDEO_ENCODER                                                             (0x0000C7B7)

#define NVC7B7_NOP                                                              (0x00000100)
#define NVC7B7_NOP_PARAMETER                                                    31:0
#define NVC7B7_PM_TRIGGER                                                       (0x00000140)
#define NVC7B7_PM_TRIGGER_V                                                     31:0
#define NVC7B7_SET_APPLICATION_ID                                               (0x00000200)
#define NVC7B7_SET_APPLICATION_ID_ID                                            31:0
#define NVC7B7_SET_APPLICATION_ID_ID_NVENC_H264                                 (0x00000001)
#define NVC7B7_SET_APPLICATION_ID_ID_NVENC_VP8                                  (0x00000002)
#define NVC7B7_SET_APPLICATION_ID_ID_NVENC_H265                                 (0x00000003)
#define NVC7B7_SET_APPLICATION_ID_ID_NVENC_VP9                                  (0x00000004)
#define NVC7B7_SET_APPLICATION_ID_ID_NVENC_RC                                   (0x00000005)
#define NVC7B7_SET_APPLICATION_ID_ID_NVENC_OFS                                  (0x00000006)
#define NVC7B7_SET_WATCHDOG_TIMER                                               (0x00000204)
#define NVC7B7_SET_WATCHDOG_TIMER_TIMER                                         31:0
#define NVC7B7_SEMAPHORE_A                                                      (0x00000240)
#define NVC7B7_SEMAPHORE_A_UPPER                                                7:0
#define NVC7B7_SEMAPHORE_B                                                      (0x00000244)
#define NVC7B7_SEMAPHORE_B_LOWER                                                31:0
#define NVC7B7_SEMAPHORE_C                                                      (0x00000248)
#define NVC7B7_SEMAPHORE_C_PAYLOAD                                              31:0
#define NVC7B7_SET_SEMAPHORE_PAYLOAD_LOWER                                      (0x00000254)
#define NVC7B7_SET_SEMAPHORE_PAYLOAD_LOWER_PAYLOAD_LOWER                        31:0
#define NVC7B7_SET_SEMAPHORE_PAYLOAD_UPPER                                      (0x00000258)
#define NVC7B7_SET_SEMAPHORE_PAYLOAD_UPPER_PAYLOAD_UPPER                        31:0
#define NVC7B7_SET_MONITORED_FENCE_SIGNAL_ADDRESS_BASE_A                        (0x0000025C)
#define NVC7B7_SET_MONITORED_FENCE_SIGNAL_ADDRESS_BASE_A_LOWER                  31:0
#define NVC7B7_SET_MONITORED_FENCE_SIGNAL_ADDRESS_BASE_B                        (0x00000260)
#define NVC7B7_SET_MONITORED_FENCE_SIGNAL_ADDRESS_BASE_B_UPPER                  31:0
#define NVC7B7_EXECUTE                                                          (0x00000300)
#define NVC7B7_EXECUTE_NOTIFY                                                   0:0
#define NVC7B7_EXECUTE_NOTIFY_DISABLE                                           (0x00000000)
#define NVC7B7_EXECUTE_NOTIFY_ENABLE                                            (0x00000001)
#define NVC7B7_EXECUTE_NOTIFY_ON                                                1:1
#define NVC7B7_EXECUTE_NOTIFY_ON_END                                            (0x00000000)
#define NVC7B7_EXECUTE_NOTIFY_ON_BEGIN                                          (0x00000001)
#define NVC7B7_EXECUTE_PREDICATION                                              2:2
#define NVC7B7_EXECUTE_PREDICATION_DISABLE                                      (0x00000000)
#define NVC7B7_EXECUTE_PREDICATION_ENABLE                                       (0x00000001)
#define NVC7B7_EXECUTE_PREDICATION_OP                                           3:3
#define NVC7B7_EXECUTE_PREDICATION_OP_EQUAL_ZERO                                (0x00000000)
#define NVC7B7_EXECUTE_PREDICATION_OP_NOT_EQUAL_ZERO                            (0x00000001)
#define NVC7B7_EXECUTE_AWAKEN                                                   8:8
#define NVC7B7_EXECUTE_AWAKEN_DISABLE                                           (0x00000000)
#define NVC7B7_EXECUTE_AWAKEN_ENABLE                                            (0x00000001)
#define NVC7B7_SEMAPHORE_D                                                      (0x00000304)
#define NVC7B7_SEMAPHORE_D_STRUCTURE_SIZE                                       1:0
#define NVC7B7_SEMAPHORE_D_STRUCTURE_SIZE_ONE                                   (0x00000000)
#define NVC7B7_SEMAPHORE_D_STRUCTURE_SIZE_FOUR                                  (0x00000001)
#define NVC7B7_SEMAPHORE_D_STRUCTURE_SIZE_TWO                                   (0x00000002)
#define NVC7B7_SEMAPHORE_D_AWAKEN_ENABLE                                        8:8
#define NVC7B7_SEMAPHORE_D_AWAKEN_ENABLE_FALSE                                  (0x00000000)
#define NVC7B7_SEMAPHORE_D_AWAKEN_ENABLE_TRUE                                   (0x00000001)
#define NVC7B7_SEMAPHORE_D_OPERATION                                            17:16
#define NVC7B7_SEMAPHORE_D_OPERATION_RELEASE                                    (0x00000000)
#define NVC7B7_SEMAPHORE_D_OPERATION_MONITORED_FENCE                            (0x00000001)
#define NVC7B7_SEMAPHORE_D_OPERATION_MONITORED_FENCE_EXT                        (0x00000002)
#define NVC7B7_SEMAPHORE_D_OPERATION_TRAP                                       (0x00000003)
#define NVC7B7_SEMAPHORE_D_FLUSH_DISABLE                                        21:21
#define NVC7B7_SEMAPHORE_D_FLUSH_DISABLE_FALSE                                  (0x00000000)
#define NVC7B7_SEMAPHORE_D_FLUSH_DISABLE_TRUE                                   (0x00000001)
#define NVC7B7_SEMAPHORE_D_CONDITIONAL_TRAP                                     22:22
#define NVC7B7_SEMAPHORE_D_CONDITIONAL_TRAP_FALSE                               (0x00000000)
#define NVC7B7_SEMAPHORE_D_CONDITIONAL_TRAP_TRUE                                (0x00000001)
#define NVC7B7_SEMAPHORE_D_PAYLOAD_SIZE                                         23:23
#define NVC7B7_SEMAPHORE_D_PAYLOAD_SIZE_32BIT                                   (0x00000000)
#define NVC7B7_SEMAPHORE_D_PAYLOAD_SIZE_64BIT                                   (0x00000001)
#define NVC7B7_SET_PREDICATION_OFFSET_UPPER                                     (0x00000308)
#define NVC7B7_SET_PREDICATION_OFFSET_UPPER_OFFSET                              7:0
#define NVC7B7_SET_PREDICATION_OFFSET_LOWER                                     (0x0000030C)
#define NVC7B7_SET_PREDICATION_OFFSET_LOWER_OFFSET                              31:0
#define NVC7B7_SET_IN_REF_PIC0_LUMA                                             (0x00000400)
#define NVC7B7_SET_IN_REF_PIC0_LUMA_OFFSET                                      31:0
#define NVC7B7_SET_IN_REF_PIC1_LUMA                                             (0x00000404)
#define NVC7B7_SET_IN_REF_PIC1_LUMA_OFFSET                                      31:0
#define NVC7B7_SET_IN_REF_PIC2_LUMA                                             (0x00000408)
#define NVC7B7_SET_IN_REF_PIC2_LUMA_OFFSET                                      31:0
#define NVC7B7_SET_IN_REF_PIC3_LUMA                                             (0x0000040C)
#define NVC7B7_SET_IN_REF_PIC3_LUMA_OFFSET                                      31:0
#define NVC7B7_SET_IN_REF_PIC4_LUMA                                             (0x00000410)
#define NVC7B7_SET_IN_REF_PIC4_LUMA_OFFSET                                      31:0
#define NVC7B7_SET_IN_REF_PIC5_LUMA                                             (0x00000414)
#define NVC7B7_SET_IN_REF_PIC5_LUMA_OFFSET                                      31:0
#define NVC7B7_SET_IN_REF_PIC6_LUMA                                             (0x00000418)
#define NVC7B7_SET_IN_REF_PIC6_LUMA_OFFSET                                      31:0
#define NVC7B7_SET_IN_REF_PIC7_LUMA                                             (0x0000041C)
#define NVC7B7_SET_IN_REF_PIC7_LUMA_OFFSET                                      31:0
#define NVC7B7_SET_IN_REF_PIC8_LUMA                                             (0x00000420)
#define NVC7B7_SET_IN_REF_PIC8_LUMA_OFFSET                                      31:0
#define NVC7B7_SET_IN_REF_PIC9_LUMA                                             (0x00000424)
#define NVC7B7_SET_IN_REF_PIC9_LUMA_OFFSET                                      31:0
#define NVC7B7_SET_IN_REF_PIC10_LUMA                                            (0x00000428)
#define NVC7B7_SET_IN_REF_PIC10_LUMA_OFFSET                                     31:0
#define NVC7B7_SET_IN_REF_PIC11_LUMA                                            (0x0000042C)
#define NVC7B7_SET_IN_REF_PIC11_LUMA_OFFSET                                     31:0
#define NVC7B7_SET_IN_REF_PIC12_LUMA                                            (0x00000430)
#define NVC7B7_SET_IN_REF_PIC12_LUMA_OFFSET                                     31:0
#define NVC7B7_SET_IN_REF_PIC13_LUMA                                            (0x00000434)
#define NVC7B7_SET_IN_REF_PIC13_LUMA_OFFSET                                     31:0
#define NVC7B7_SET_IN_REF_PIC14_LUMA                                            (0x00000438)
#define NVC7B7_SET_IN_REF_PIC14_LUMA_OFFSET                                     31:0
#define NVC7B7_SET_IN_REF_PIC15_LUMA                                            (0x0000043C)
#define NVC7B7_SET_IN_REF_PIC15_LUMA_OFFSET                                     31:0
#define NVC7B7_SET_IN_REF_PIC0_CHROMA                                           (0x00000440)
#define NVC7B7_SET_IN_REF_PIC0_CHROMA_OFFSET                                    31:0
#define NVC7B7_SET_IN_REF_PIC1_CHROMA                                           (0x00000444)
#define NVC7B7_SET_IN_REF_PIC1_CHROMA_OFFSET                                    31:0
#define NVC7B7_SET_IN_REF_PIC2_CHROMA                                           (0x00000448)
#define NVC7B7_SET_IN_REF_PIC2_CHROMA_OFFSET                                    31:0
#define NVC7B7_SET_IN_REF_PIC3_CHROMA                                           (0x0000044C)
#define NVC7B7_SET_IN_REF_PIC3_CHROMA_OFFSET                                    31:0
#define NVC7B7_SET_IN_REF_PIC4_CHROMA                                           (0x00000450)
#define NVC7B7_SET_IN_REF_PIC4_CHROMA_OFFSET                                    31:0
#define NVC7B7_SET_IN_REF_PIC5_CHROMA                                           (0x00000454)
#define NVC7B7_SET_IN_REF_PIC5_CHROMA_OFFSET                                    31:0
#define NVC7B7_SET_IN_REF_PIC6_CHROMA                                           (0x00000458)
#define NVC7B7_SET_IN_REF_PIC6_CHROMA_OFFSET                                    31:0
#define NVC7B7_SET_IN_REF_PIC7_CHROMA                                           (0x0000045C)
#define NVC7B7_SET_IN_REF_PIC7_CHROMA_OFFSET                                    31:0
#define NVC7B7_SET_IN_REF_PIC8_CHROMA                                           (0x00000460)
#define NVC7B7_SET_IN_REF_PIC8_CHROMA_OFFSET                                    31:0
#define NVC7B7_SET_IN_REF_PIC9_CHROMA                                           (0x00000464)
#define NVC7B7_SET_IN_REF_PIC9_CHROMA_OFFSET                                    31:0
#define NVC7B7_SET_IN_REF_PIC10_CHROMA                                          (0x00000468)
#define NVC7B7_SET_IN_REF_PIC10_CHROMA_OFFSET                                   31:0
#define NVC7B7_SET_IN_REF_PIC11_CHROMA                                          (0x0000046C)
#define NVC7B7_SET_IN_REF_PIC11_CHROMA_OFFSET                                   31:0
#define NVC7B7_SET_IN_REF_PIC12_CHROMA                                          (0x00000470)
#define NVC7B7_SET_IN_REF_PIC12_CHROMA_OFFSET                                   31:0
#define NVC7B7_SET_IN_REF_PIC13_CHROMA                                          (0x00000474)
#define NVC7B7_SET_IN_REF_PIC13_CHROMA_OFFSET                                   31:0
#define NVC7B7_SET_IN_REF_PIC14_CHROMA                                          (0x00000478)
#define NVC7B7_SET_IN_REF_PIC14_CHROMA_OFFSET                                   31:0
#define NVC7B7_SET_IN_REF_PIC15_CHROMA                                          (0x0000047C)
#define NVC7B7_SET_IN_REF_PIC15_CHROMA_OFFSET                                   31:0
#define NVC7B7_SET_IN_REF_PIC_LAST_LUMA                                         (0x00000500)
#define NVC7B7_SET_IN_REF_PIC_LAST_LUMA_OFFSET                                  31:0
#define NVC7B7_SET_IN_REF_PIC_GOLDEN_LUMA                                       (0x00000504)
#define NVC7B7_SET_IN_REF_PIC_GOLDEN_LUMA_OFFSET                                31:0
#define NVC7B7_SET_IN_REF_PIC_ALTREF_LUMA                                       (0x00000508)
#define NVC7B7_SET_IN_REF_PIC_ALTREF_LUMA_OFFSET                                31:0
#define NVC7B7_SET_UCODE_STATE                                                  (0x0000050C)
#define NVC7B7_SET_UCODE_STATE_OFFSET                                           31:0
#define NVC7B7_SET_IO_VP8_ENC_STATUS                                            (0x00000510)
#define NVC7B7_SET_IO_VP8_ENC_STATUS_OFFSET                                     31:0
#define NVC7B7_SET_OUT_BITSTREAM_RES                                            (0x00000514)
#define NVC7B7_SET_OUT_BITSTREAM_RES_OFFSET                                     31:0
#define NVC7B7_SET_IN_REF_PIC_LAST_CHROMA                                       (0x00000518)
#define NVC7B7_SET_IN_REF_PIC_LAST_CHROMA_OFFSET                                31:0
#define NVC7B7_SET_IN_REF_PIC_GOLDEN_CHROMA                                     (0x0000051C)
#define NVC7B7_SET_IN_REF_PIC_GOLDEN_CHROMA_OFFSET                              31:0
#define NVC7B7_SET_IN_REF_PIC_ALTREF_CHROMA                                     (0x00000520)
#define NVC7B7_SET_IN_REF_PIC_ALTREF_CHROMA_OFFSET                              31:0
#define NVC7B7_SET_OUT_VP9_COUNTER_DATA                                         (0x00000524)
#define NVC7B7_SET_OUT_VP9_COUNTER_DATA_OFFSET                                  31:0
#define NVC7B7_SET_IN_VP9_PROBABILITY_DATA                                      (0x00000528)
#define NVC7B7_SET_IN_VP9_PROBABILITY_DATA_OFFSET                               31:0
#define NVC7B7_SET_IN_VP9_CUR_TEMPORAL_DATA                                     (0x0000052C)
#define NVC7B7_SET_IN_VP9_CUR_TEMPORAL_DATA_OFFSET                              31:0
#define NVC7B7_SET_IN_VP9_REF_TEMPORAL_DATA                                     (0x00000530)
#define NVC7B7_SET_IN_VP9_REF_TEMPORAL_DATA_OFFSET                              31:0
#define NVC7B7_SET_IN_VP9_COMBINEDLINE_BUF                                      (0x00000534)
#define NVC7B7_SET_IN_VP9_COMBINEDLINE_BUF_OFFSET                               31:0
#define NVC7B7_SET_IN_VP9_FILTERLINE_BUF                                        (0x00000538)
#define NVC7B7_SET_IN_VP9_FILTERLINE_BUF_OFFSET                                 31:0
#define NVC7B7_SET_IN_VP9_FILTERCOLLINE_BUF                                     (0x0000053C)
#define NVC7B7_SET_IN_VP9_FILTERCOLLINE_BUF_OFFSET                              31:0
#define NVC7B7_SET_IN_VP9_MOCOMP_PIC_LUMA                                       (0x00000544)
#define NVC7B7_SET_IN_VP9_MOCOMP_PIC_LUMA_OFFSET                                31:0
#define NVC7B7_SET_IN_VP9_MOCOMP_PIC_CHROMA                                     (0x00000548)
#define NVC7B7_SET_IN_VP9_MOCOMP_PIC_CHROMA_OFFSET                              31:0
#define NVC7B7_SET_CONTROL_PARAMS                                               (0x00000700)
#define NVC7B7_SET_CONTROL_PARAMS_CODEC_TYPE                                    3:0
#define NVC7B7_SET_CONTROL_PARAMS_CODEC_TYPE_H264                               (0x00000003)
#define NVC7B7_SET_CONTROL_PARAMS_FORCE_OUT_PIC                                 8:8
#define NVC7B7_SET_CONTROL_PARAMS_FORCE_OUT_COL                                 9:9
#define NVC7B7_SET_CONTROL_PARAMS_MEONLY                                        10:10
#define NVC7B7_SET_CONTROL_PARAMS_SLICE_STAT_ON                                 11:11
#define NVC7B7_SET_CONTROL_PARAMS_GPTIMER_ON                                    12:12
#define NVC7B7_SET_CONTROL_PARAMS_STOP_PROB_UPDATE                              13:13
#define NVC7B7_SET_CONTROL_PARAMS_DUMP_CYCLE_COUNT                              14:14
#define NVC7B7_SET_CONTROL_PARAMS_MPEC_STAT_ON                                  15:15
#define NVC7B7_SET_CONTROL_PARAMS_DEBUG_MODE                                    16:16
#define NVC7B7_SET_CONTROL_PARAMS_SUBFRAME_MODE                                 18:17
#define NVC7B7_SET_CONTROL_PARAMS_SUBFRAME_MODE_NONE                            (0x00000000)
#define NVC7B7_SET_CONTROL_PARAMS_SUBFRAME_MODE_SLICE_FLUSH                     (0x00000001)
#define NVC7B7_SET_CONTROL_PARAMS_SUBFRAME_MODE_SLICE_OFFSETS_WITHOUT_FLUSH     (0x00000002)
#define NVC7B7_SET_CONTROL_PARAMS_SUBFRAME_MODE_SLICE_OFFSETS_WITH_FLUSH        (0x00000003)
#define NVC7B7_SET_CONTROL_PARAMS_RCSTAT_WRITE                                  19:19
#define NVC7B7_SET_CONTROL_PARAMS_RCSTAT_READ                                   20:20
#define NVC7B7_SET_CONTROL_PARAMS_ENCRYPT_ON                                    21:21
#define NVC7B7_SET_CONTROL_PARAMS_RCMODE                                        31:24
#define NVC7B7_SET_CONTROL_PARAMS_RCMODE_CONSTQP                                (0x00000000)
#define NVC7B7_SET_PICTURE_INDEX                                                (0x00000704)
#define NVC7B7_SET_PICTURE_INDEX_INDEX                                          31:0
#define NVC7B7_SET_OUT_ENCRYPT_PARAMS                                           (0x00000708)
#define NVC7B7_SET_OUT_ENCRYPT_PARAMS_OFFSET                                    31:0
#define NVC7B7_SET_IN_RCDATA                                                    (0x0000070C)
#define NVC7B7_SET_IN_RCDATA_OFFSET                                             31:0
#define NVC7B7_SET_IN_DRV_PIC_SETUP                                             (0x00000710)
#define NVC7B7_SET_IN_DRV_PIC_SETUP_OFFSET                                      31:0
#define NVC7B7_SET_IN_CEAHINTS_DATA                                             (0x00000714)
#define NVC7B7_SET_IN_CEAHINTS_DATA_OFFSET                                      31:0
#define NVC7B7_SET_OUT_ENC_STATUS                                               (0x00000718)
#define NVC7B7_SET_OUT_ENC_STATUS_OFFSET                                        31:0
#define NVC7B7_SET_OUT_BITSTREAM                                                (0x0000071C)
#define NVC7B7_SET_OUT_BITSTREAM_OFFSET                                         31:0
#define NVC7B7_SET_IOHISTORY                                                    (0x00000720)
#define NVC7B7_SET_IOHISTORY_OFFSET                                             31:0
#define NVC7B7_SET_IO_RC_PROCESS                                                (0x00000724)
#define NVC7B7_SET_IO_RC_PROCESS_OFFSET                                         31:0
#define NVC7B7_SET_IN_COLOC_DATA                                                (0x00000728)
#define NVC7B7_SET_IN_COLOC_DATA_OFFSET                                         31:0
#define NVC7B7_SET_OUT_COLOC_DATA                                               (0x0000072C)
#define NVC7B7_SET_OUT_COLOC_DATA_OFFSET                                        31:0
#define NVC7B7_SET_OUT_REF_PIC_LUMA                                             (0x00000730)
#define NVC7B7_SET_OUT_REF_PIC_LUMA_OFFSET                                      31:0
#define NVC7B7_SET_IN_CUR_PIC                                                   (0x00000734)
#define NVC7B7_SET_IN_CUR_PIC_OFFSET                                            31:0
#define NVC7B7_SET_IN_MEPRED_DATA                                               (0x00000738)
#define NVC7B7_SET_IN_MEPRED_DATA_OFFSET                                        31:0
#define NVC7B7_SET_OUT_MEPRED_DATA                                              (0x0000073C)
#define NVC7B7_SET_OUT_MEPRED_DATA_OFFSET                                       31:0
#define NVC7B7_SET_IN_CUR_PIC_CHROMA_U                                          (0x00000740)
#define NVC7B7_SET_IN_CUR_PIC_CHROMA_U_OFFSET                                   31:0
#define NVC7B7_SET_IN_CUR_PIC_CHROMA_V                                          (0x00000744)
#define NVC7B7_SET_IN_CUR_PIC_CHROMA_V_OFFSET                                   31:0
#define NVC7B7_SET_IN_QP_MAP                                                    (0x00000748)
#define NVC7B7_SET_IN_QP_MAP_OFFSET                                             31:0
#define NVC7B7_SET_OUT_REF_PIC_CHROMA                                           (0x0000074C)
#define NVC7B7_SET_OUT_REF_PIC_CHROMA_OFFSET                                    31:0
#define NVC7B7_SET_IN_PARTITION_BUF                                             (0x00000750)
#define NVC7B7_SET_IN_PARTITION_BUF_OFFSET                                      31:0
#define NVC7B7_SET_IN_CUR_PIC_TASK_STATUS                                       (0x00000758)
#define NVC7B7_SET_IN_CUR_PIC_TASK_STATUS_OFFSET                                31:0
#define NVC7B7_SET_IN_REF_PIC_TASK_STATUS                                       (0x0000075C)
#define NVC7B7_SET_IN_REF_PIC_TASK_STATUS_OFFSET                                31:0
#define NVC7B7_SET_OUT_TASK_STATUS                                              (0x00000760)
#define NVC7B7_SET_OUT_TASK_STATUS_OFFSET                                       31:0
#define NVC7B7_SET_IN_MV_HINTS_TASK_STATUS                                      (0x00000764)
#define NVC7B7_SET_IN_MV_HINTS_TASK_STATUS_OFFSET                               31:0
#define NVC7B7_SET_OUT_SCALE_REF_PIC_LUMA                                       (0x00000768)
#define NVC7B7_SET_OUT_SCALE_REF_PIC_LUMA_OFFSET                                31:0
#define NVC7B7_SET_OUT_SCALE_REF_PIC_CHROMA                                     (0x0000076C)
#define NVC7B7_SET_OUT_SCALE_REF_PIC_CHROMA_OFFSET                              31:0
#define NVC7B7_SET_IO_OFS_ERROR_PROPAGATION                                     (0x00000770)
#define NVC7B7_SET_IO_OFS_ERROR_PROPAGATION_OFFSET                              31:0
#define NVC7B7_PM_TRIGGER_END                                                   (0x00001114)
#define NVC7B7_PM_TRIGGER_END_V                                                 31:0

#define NVC7B7_ERROR_NONE                                                       (0x00000000)
#define NVC7B7_OS_ERROR_EXECUTE_INSUFFICIENT_DATA                               (0x00000001)
#define NVC7B7_OS_ERROR_SEMAPHORE_INSUFFICIENT_DATA                             (0x00000002)
#define NVC7B7_OS_ERROR_INVALID_METHOD                                          (0x00000003)
#define NVC7B7_OS_ERROR_INVALID_DMA_PAGE                                        (0x00000004)
#define NVC7B7_OS_ERROR_UNHANDLED_INTERRUPT                                     (0x00000005)
#define NVC7B7_OS_ERROR_EXCEPTION                                               (0x00000006)
#define NVC7B7_OS_ERROR_INVALID_CTXSW_REQUEST                                   (0x00000007)
#define NVC7B7_OS_ERROR_APPLICATION                                             (0x00000008)
#define NVC7B7_OS_INTERRUPT_EXECUTE_AWAKEN                                      (0x00000100)
#define NVC7B7_OS_INTERRUPT_BACKEND_SEMAPHORE_AWAKEN                            (0x00000200)
#define NVC7B7_OS_INTERRUPT_CTX_ERROR_FBIF                                      (0x00000300)
#define NVC7B7_OS_INTERRUPT_LIMIT_VIOLATION                                     (0x00000400)
#define NVC7B7_OS_INTERRUPT_LIMIT_AND_FBIF_CTX_ERROR                            (0x00000500)
#define NVC7B7_OS_INTERRUPT_HALT_ENGINE                                         (0x00000600)
#define NVC7B7_OS_INTERRUPT_TRAP_NONSTALL                                       (0x00000700)
#define NVC7B7_OS_INTERRUPT_CTX_SAVE_DONE                                       (0x00000800)
#define NVC7B7_OS_INTERRUPT_CTX_RESTORE_DONE                                    (0x00000900)
#define NVC7B7_ENC_ERROR_H264_APPTIMER_EXPIRED                                  (0x30000001)
#define NVC7B7_ENC_ERROR_H264_INVALID_INPUT                                     (0x30000002)
#define NVC7B7_ENC_ERROR_H264_HWERR_INTERRUPT                                   (0x30000003)
#define NVC7B7_ENC_ERROR_H264_BAD_MAGIC                                         (0x30000004)

#ifdef __cplusplus
};     /* extern "C" */
#endif
#endif // clc7b7_h

