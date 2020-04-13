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
void MME64Group::init(const MME64HW &hw) {
    MME64Alu newAlu;
    MME64Output newoutput;

    global.endNext = hw.extract<0, 0>().toUint() ? true : false;
    global.predMode = (MME64Pred::PredEnum)hw.extract<4, 1>().toUint();
    global.pred = (MME64Reg::RegEnum)hw.extract<9, 5>().toUint();

    newAlu.op = (MME64Op::OpEnum)hw.extract<14, 10>().toUint();
    newAlu.dst = (MME64Reg::RegEnum)hw.extract<19, 15>().toUint();
    newAlu.src[0] = (MME64Reg::RegEnum)hw.extract<24, 20>().toUint();
    newAlu.src[1] = (MME64Reg::RegEnum)hw.extract<29, 25>().toUint();
    newAlu.immed = hw.extract<45, 30>().toUint();
    alu.push_back(newAlu);

    newAlu.op = (MME64Op::OpEnum)hw.extract<50, 46>().toUint();
    newAlu.dst = (MME64Reg::RegEnum)hw.extract<55, 51>().toUint();
    newAlu.src[0] = (MME64Reg::RegEnum)hw.extract<60, 56>().toUint();
    newAlu.src[1] = (MME64Reg::RegEnum)hw.extract<65, 61>().toUint();
    newAlu.immed = hw.extract<81, 66>().toUint();
    alu.push_back(newAlu);

    newoutput.method = (MME64Out::OutEnum)hw.extract<84, 82>().toUint();
    newoutput.emit = (MME64Out::OutEnum)hw.extract<88, 85>().toUint();
    output.push_back(newoutput);

    newoutput.method = (MME64Out::OutEnum)hw.extract<91, 89>().toUint();
    newoutput.emit = (MME64Out::OutEnum)hw.extract<95, 92>().toUint();
    output.push_back(newoutput);
}

MME64Group::MME64Group(const uint32_t *instructions) {
    MME64HW bits;

    // Pack the three dwords into the instruction
    bits.insert<31, 0>(mme::bitset<32>(instructions[2]));
    bits.insert<63, 32>(mme::bitset<32>(instructions[1]));
    bits.insert<MME64GroupBits, 64>(mme::bitset<MME64GroupBits - 64 + 1>(instructions[0]));

    init(bits);
}

