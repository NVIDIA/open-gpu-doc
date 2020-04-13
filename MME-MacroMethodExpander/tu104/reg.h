/*******************************************************************************
    Copyright (c) 2020, NVIDIA CORPORATION. All rights reserved.

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
#include <string>

namespace MME64Reg {
enum RegEnum {
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,
    R16,
    R17,
    R18,
    R19,
    R20,
    R21,
    R22,
    R23,
    ZERO,
    IMMED,
    IMMEDPAIR,
    IMMED32,
    LOAD0,
    LOAD1,
    VIRTUAL0 = 32,
};

    enum { count = 30 };
    enum { bits = 5 };

static const struct { std::string name; RegEnum val;} mapping[] = {
    {"R0", R0 },
    {"R1", R1 },
    {"R2", R2 },
    {"R3", R3 },
    {"R4", R4 },
    {"R5", R5 },
    {"R6", R6 },
    {"R7", R7 },
    {"R8", R8 },
    {"R9", R9 },
    {"R10", R10 },
    {"R11", R11 },
    {"R12", R12 },
    {"R13", R13 },
    {"R14", R14 },
    {"R15", R15 },
    {"R16", R16 },
    {"R17", R17 },
    {"R18", R18 },
    {"R19", R19 },
    {"R20", R20 },
    {"R21", R21 },
    {"R22", R22 },
    {"R23", R23 },
    {"ZERO", ZERO },
    {"IMMED", IMMED },
    {"IMMEDPAIR", IMMEDPAIR },
    {"IMMED32", IMMED32 },
    {"LOAD0", LOAD0 },
    {"LOAD1", LOAD1 },
};
};
