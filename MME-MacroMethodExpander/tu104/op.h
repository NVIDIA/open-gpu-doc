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

namespace MME64Op {
enum OpEnum {
    ADD,
    ADDC,
    SUB,
    SUBB,
    MUL,
    MULH,
    MULU,
    EXTENDED,
    CLZ,
    SLL,
    SRL,
    SRA,
    AND,
    NAND,
    OR,
    XOR,
    MERGE,
    SLT,
    SLTU,
    SLE,
    SLEU,
    SEQ,
    STATE,
    LOOP,
    JAL,
    BLT,
    BLTU,
    BLE,
    BLEU,
    BEQ,
    DREAD,
    DWRITE,
};

    enum { count = 32 };
    enum { bits = 5 };

static const struct { std::string name; OpEnum val;} mapping[] = {
    {"ADD", ADD },
    {"ADDC", ADDC },
    {"SUB", SUB },
    {"SUBB", SUBB },
    {"MUL", MUL },
    {"MULH", MULH },
    {"MULU", MULU },
    {"EXTENDED", EXTENDED },
    {"CLZ", CLZ },
    {"SLL", SLL },
    {"SRL", SRL },
    {"SRA", SRA },
    {"AND", AND },
    {"NAND", NAND },
    {"OR", OR },
    {"XOR", XOR },
    {"MERGE", MERGE },
    {"SLT", SLT },
    {"SLTU", SLTU },
    {"SLE", SLE },
    {"SLEU", SLEU },
    {"SEQ", SEQ },
    {"STATE", STATE },
    {"LOOP", LOOP },
    {"JAL", JAL },
    {"BLT", BLT },
    {"BLTU", BLTU },
    {"BLE", BLE },
    {"BLEU", BLEU },
    {"BEQ", BEQ },
    {"DREAD", DREAD },
    {"DWRITE", DWRITE },
};
};
