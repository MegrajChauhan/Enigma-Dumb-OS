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

#ifndef E_CPU
#define E_CPU

#include "../Memory/EnigmaMemory.hpp"
#include "../OS/EnigmaOS.hpp"

namespace CPU
{
    // all of these registers accept regsp, regbp, regpc can be used for anything
    enum Register : qword
    {
        rega,
        regb,
        regc,
        regd,
        rege,
        regf,
        reg1,
        reg2,
        reg3,
        reg4,
        reg5,
        reg6,
        regsp,
        regbp,
        regpc,
        REGR_COUNT
    };

    enum Instructions : byte
    {
        NOP,
        ADD, // for arithmetic operations
        SUB,
        MUL,
        DIV,
        SYSCALL,
        HALT,
    };

    qword _e_registers[REGR_COUNT];
    static Memory main_memory; // the instruction memory[it's size can only be increased by the OS]
    static Memory memory;      // the data memory[it's size is changeable by the user]
    static Memory IObuffer;    // this is also changeable but by only the OS

    static qword __current_memory_input;
    static byte __current_instruction;

    static bool running = true;

    void init();

    void fetch();
    void decode();

    void execute();
    void run();
};

void CPU::init()
{
    // the first 256 bytes of the memory is for the stack
    _e_registers[regsp] = 0;
    _e_registers[regpc] = 0b00001000000000000000000100010000; // 16 bytes have been reserved for any future memory mapped register
    // base pointer can be willingly modified for functions
    // for those who think that that value is large, this is what it looks like in binary 0b00001000000000000000000100010000. so if we follow the order, it becomes this
}

void CPU::fetch()
{
    __current_memory_input = main_memory.mem_read(_e_registers[regpc]);
}

void CPU::decode()
{
    __current_instruction = (__current_memory_input >> 56);
}

#include "EnigmaInstruction.hpp"

void CPU::execute()
{
    switch (__current_instruction)
    {
    case NOP:
        break;
    case ADD:
        Add();
        break;
    case SUB:
        Sub();
        break;
    case MUL:
        Mul();
        break;
    case DIV:
        Div();
        break;
    case HALT:
        std::cout << "Halt" << std::endl;
        running = false;
        break;
    default:
        std::cerr << "Unknown Instructions. Bad Instruction[Terminating Execution]" << std::endl;
        _e_registers[rega] = -1;
        running = false;
    }
}

void CPU::run()
{
    while (running)
    {
        fetch();
        decode();
        execute();
        _e_registers[regpc] += 8;
    }
}

#endif