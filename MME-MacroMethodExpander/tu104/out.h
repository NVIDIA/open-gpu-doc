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

namespace MME64Out {
enum OutEnum {
    NONE,
    ALU0,
    ALU1,
    LOAD0,
    LOAD1,
    IMMED0,
    IMMED1,
    RESERVED,
    IMMEDHIGH0,
    IMMEDHIGH1,
    IMMED32_0,
};

    enum { count = 11 };
    enum { bits = 4 };

static const struct { std::string name; OutEnum val;} mapping[] = {
    {"NONE", NONE },
    {"ALU0", ALU0 },
    {"ALU1", ALU1 },
    {"LOAD0", LOAD0 },
    {"LOAD1", LOAD1 },
    {"IMMED0", IMMED0 },
    {"IMMED1", IMMED1 },
    {"RESERVED", RESERVED },
    {"IMMEDHIGH0", IMMEDHIGH0 },
    {"IMMEDHIGH1", IMMEDHIGH1 },
    {"IMMED32_0", IMMED32_0 },
};
};
