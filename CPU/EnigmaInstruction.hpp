/*
Copyright (c) 2023 MegrajChauhan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef E_INSTRUCTIONS
#define E_INSTRUCTIONS

#include "EnigmaCPU.hpp"

void Add()
{
    // 0xxxxxx7 8 xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // bits 0 to 7 = instruction
    // bit 8 = instruction mode
    // 0 => register-register, 1 => register-immediate
    byte mode = ((CPU::__current_memory_input >> 55) & 1UL);
    switch (mode)
    {
    case 0:
    {
        // 0xxxxxx7 8 xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx dest = [xxxx] src = [xxxx]
        CPU::_e_registers[(CPU::__current_memory_input >> 3) & 15] += CPU::_e_registers[CPU::__current_memory_input & 15];
        break;
    }
    case 1:
    {
        // 0xxxxxx7 8 src = [xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxx] dest = [xxxx]
        CPU::_e_registers[CPU::__current_memory_input & 15] += (CPU::__current_memory_input)&0x7FFFFFFFFFFFF;
        break;
    }
    }
}

void Sub()
{
    // 0xxxxxx7 8 xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // bits 0 to 7 = instruction
    // bit 8 = instruction mode
    // 0 => register-register, 1 => register-immediate
    byte mode = ((CPU::__current_memory_input >> 55) & 1UL);
    switch (mode)
    {
    case 0:
    {
        // 0xxxxxx7 8 xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx dest = [xxxx] src = [xxxx]
        CPU::_e_registers[(CPU::__current_memory_input >> 3) & 15] -= CPU::_e_registers[CPU::__current_memory_input & 15];
        break;
    }
    case 1:
    {
        // 0xxxxxx7 8 src = [xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxx] dest = [xxxx]
        CPU::_e_registers[CPU::__current_memory_input & 15] -= (CPU::__current_memory_input)&0x7FFFFFFFFFFFF;
        break;
    }
    }
}

void Mul()
{
    // 0xxxxxx7 8 xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // bits 0 to 7 = instruction
    // bit 8 = instruction mode
    // 0 => register-register, 1 => register-immediate
    byte mode = ((CPU::__current_memory_input >> 55) & 1UL);
    switch (mode)
    {
    case 0:
    {
        // 0xxxxxx7 8 xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx dest = [xxxx] src = [xxxx]
        CPU::_e_registers[(CPU::__current_memory_input >> 3) & 15] *= CPU::_e_registers[CPU::__current_memory_input & 15];
        break;
    }
    case 1:
    {
        // 0xxxxxx7 8 src = [xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxx] dest = [xxxx]
        CPU::_e_registers[CPU::__current_memory_input & 15] *= (CPU::__current_memory_input)&0x7FFFFFFFFFFFF;
        break;
    }
    }
}

void Div()
{
    // 0xxxxxx7 8 xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // bits 0 to 7 = instruction
    // bit 8 = instruction mode
    // 0 => register-register, 1 => register-immediate
    byte mode = ((CPU::__current_memory_input >> 55) & 1UL);
    switch (mode)
    {
    case 0:
    {
        // 0xxxxxx7 8 xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx dest = [xxxx] src = [xxxx]
        CPU::_e_registers[(CPU::__current_memory_input >> 3) & 15] /= CPU::_e_registers[CPU::__current_memory_input & 15];
        break;
    }
    case 1:
    {
        // 0xxxxxx7 8 src = [xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxx] dest = [xxxx]
        CPU::_e_registers[CPU::__current_memory_input & 15] /= (CPU::__current_memory_input)&0x7FFFFFFFFFFFF;
        break;
    }
    }
}

#endif