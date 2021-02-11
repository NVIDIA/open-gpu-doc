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

#ifndef _cle7d0_h_
#define _cle7d0_h_

#ifdef __cplusplus
extern "C" {
#endif

#define NVE7D0_VIDEO_NVJPG                                                               (0x0000E7D0)

#define NVE7D0_NOP                                                              (0x00000100)
#define NVE7D0_NOP_PARAMETER                                                    31:0
#define NVE7D0_SET_APPLICATION_ID                                               (0x00000200)
#define NVE7D0_SET_APPLICATION_ID_ID                                            31:0
#define NVE7D0_SET_APPLICATION_ID_ID_NVJPG_DECODER                              (0x00000001)
#define NVE7D0_SET_APPLICATION_ID_ID_NVJPG_ENCODER                              (0x00000002)
#define NVE7D0_SET_WATCHDOG_TIMER                                               (0x00000204)
#define NVE7D0_SET_WATCHDOG_TIMER_TIMER                                         31:0
#define NVE7D0_SEMAPHORE_A                                                      (0x00000240)
#define NVE7D0_SEMAPHORE_A_UPPER                                                7:0
#define NVE7D0_SEMAPHORE_B                                                      (0x00000244)
#define NVE7D0_SEMAPHORE_B_LOWER                                                31:0
#define NVE7D0_SEMAPHORE_C                                                      (0x00000248)
#define NVE7D0_SEMAPHORE_C_PAYLOAD                                              31:0
#define NVE7D0_CTX_SAVE_AREA                                                    (0x0000024C)
#define NVE7D0_CTX_SAVE_AREA_OFFSET                                             27:0
#define NVE7D0_CTX_SAVE_AREA_CTX_VALID                                          31:28
#define NVE7D0_CTX_SWITCH                                                       (0x00000250)
#define NVE7D0_CTX_SWITCH_RESTORE                                               0:0
#define NVE7D0_CTX_SWITCH_RESTORE_FALSE                                         (0x00000000)
#define NVE7D0_CTX_SWITCH_RESTORE_TRUE                                          (0x00000001)
#define NVE7D0_CTX_SWITCH_RST_NOTIFY                                            1:1
#define NVE7D0_CTX_SWITCH_RST_NOTIFY_FALSE                                      (0x00000000)
#define NVE7D0_CTX_SWITCH_RST_NOTIFY_TRUE                                       (0x00000001)
#define NVE7D0_CTX_SWITCH_RESERVED                                              7:2
#define NVE7D0_CTX_SWITCH_ASID                                                  23:8
#define NVE7D0_EXECUTE                                                          (0x00000300)
#define NVE7D0_EXECUTE_NOTIFY                                                   0:0
#define NVE7D0_EXECUTE_NOTIFY_DISABLE                                           (0x00000000)
#define NVE7D0_EXECUTE_NOTIFY_ENABLE                                            (0x00000001)
#define NVE7D0_EXECUTE_NOTIFY_ON                                                1:1
#define NVE7D0_EXECUTE_NOTIFY_ON_END                                            (0x00000000)
#define NVE7D0_EXECUTE_NOTIFY_ON_BEGIN                                          (0x00000001)
#define NVE7D0_EXECUTE_AWAKEN                                                   8:8
#define NVE7D0_EXECUTE_AWAKEN_DISABLE                                           (0x00000000)
#define NVE7D0_EXECUTE_AWAKEN_ENABLE                                            (0x00000001)
#define NVE7D0_SEMAPHORE_D                                                      (0x00000304)
#define NVE7D0_SEMAPHORE_D_STRUCTURE_SIZE                                       0:0
#define NVE7D0_SEMAPHORE_D_STRUCTURE_SIZE_ONE                                   (0x00000000)
#define NVE7D0_SEMAPHORE_D_STRUCTURE_SIZE_FOUR                                  (0x00000001)
#define NVE7D0_SEMAPHORE_D_AWAKEN_ENABLE                                        8:8
#define NVE7D0_SEMAPHORE_D_AWAKEN_ENABLE_FALSE                                  (0x00000000)
#define NVE7D0_SEMAPHORE_D_AWAKEN_ENABLE_TRUE                                   (0x00000001)
#define NVE7D0_SEMAPHORE_D_OPERATION                                            17:16
#define NVE7D0_SEMAPHORE_D_OPERATION_RELEASE                                    (0x00000000)
#define NVE7D0_SEMAPHORE_D_OPERATION_RESERVED0                                  (0x00000001)
#define NVE7D0_SEMAPHORE_D_OPERATION_RESERVED1                                  (0x00000002)
#define NVE7D0_SEMAPHORE_D_OPERATION_TRAP                                       (0x00000003)
#define NVE7D0_SEMAPHORE_D_FLUSH_DISABLE                                        21:21
#define NVE7D0_SEMAPHORE_D_FLUSH_DISABLE_FALSE                                  (0x00000000)
#define NVE7D0_SEMAPHORE_D_FLUSH_DISABLE_TRUE                                   (0x00000001)
#define NVE7D0_SET_CONTROL_PARAMS                                               (0x00000700)
#define NVE7D0_SET_CONTROL_PARAMS_GPTIMER_ON                                    0:0
#define NVE7D0_SET_CONTROL_PARAMS_DUMP_CYCLE_COUNT                              1:1
#define NVE7D0_SET_CONTROL_PARAMS_DEBUG_MODE                                    2:2
#define NVE7D0_SET_PICTURE_INDEX                                                (0x00000704)
#define NVE7D0_SET_PICTURE_INDEX_INDEX                                          31:0
#define NVE7D0_SET_IN_DRV_PIC_SETUP                                             (0x00000708)
#define NVE7D0_SET_IN_DRV_PIC_SETUP_OFFSET                                      31:0
#define NVE7D0_SET_OUT_STATUS                                                   (0x0000070C)
#define NVE7D0_SET_OUT_STATUS_OFFSET                                            31:0
#define NVE7D0_SET_BITSTREAM                                                    (0x00000710)
#define NVE7D0_SET_BITSTREAM_OFFSET                                             31:0
#define NVE7D0_SET_CUR_PIC                                                      (0x00000714)
#define NVE7D0_SET_CUR_PIC_OFFSET                                               31:0
#define NVE7D0_SET_CUR_PIC_CHROMA_U                                             (0x00000718)
#define NVE7D0_SET_CUR_PIC_CHROMA_U_OFFSET                                      31:0
#define NVE7D0_SET_CUR_PIC_CHROMA_V                                             (0x0000071C)
#define NVE7D0_SET_CUR_PIC_CHROMA_V_OFFSET                                      31:0

#define NVE7D0_ERROR_NONE                                                       (0x00000000)
#define NVE7D0_OS_ERROR_EXECUTE_INSUFFICIENT_DATA                               (0x00000001)
#define NVE7D0_OS_ERROR_SEMAPHORE_INSUFFICIENT_DATA                             (0x00000002)
#define NVE7D0_OS_ERROR_INVALID_METHOD                                          (0x00000003)
#define NVE7D0_OS_ERROR_INVALID_DMA_PAGE                                        (0x00000004)
#define NVE7D0_OS_ERROR_UNHANDLED_INTERRUPT                                     (0x00000005)
#define NVE7D0_OS_ERROR_EXCEPTION                                               (0x00000006)
#define NVE7D0_OS_ERROR_INVALID_CTXSW_REQUEST                                   (0x00000007)
#define NVE7D0_OS_ERROR_APPLICATION                                             (0x00000008)
#define NVE7D0_OS_INTERRUPT_EXECUTE_AWAKEN                                      (0x00000100)
#define NVE7D0_OS_INTERRUPT_BACKEND_SEMAPHORE_AWAKEN                            (0x00000200)
#define NVE7D0_OS_INTERRUPT_CTX_ERROR_FBIF                                      (0x00000300)
#define NVE7D0_OS_INTERRUPT_LIMIT_VIOLATION                                     (0x00000400)
#define NVE7D0_OS_INTERRUPT_LIMIT_AND_FBIF_CTX_ERROR                            (0x00000500)
#define NVE7D0_OS_INTERRUPT_HALT_ENGINE                                         (0x00000600)
#define NVE7D0_OS_INTERRUPT_TRAP_NONSTALL                                       (0x00000700)
#define NVE7D0_OS_INTERRUPT_CTX_SAVE_DONE                                       (0x00000800)
#define NVE7D0_OS_INTERRUPT_CTX_RESTORE_DONE                                    (0x00000900)
#define NVE7D0_ERROR_JPGAPPTIMER_EXPIRED                                        (0x30000001)
#define NVE7D0_ERROR_JPGINVALID_INPUT                                           (0x30000002)
#define NVE7D0_ERROR_JPGHWERR_INTERRUPT                                         (0x30000003)
#define NVE7D0_ERROR_JPGBAD_MAGIC                                               (0x30000004)

#ifdef __cplusplus
};     /* extern "C" */
#endif
#endif // _cle7d0_h

