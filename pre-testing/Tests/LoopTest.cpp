#include "../../OS/EnigmaOS.hpp"

// TEST 1: LOOPING 1 MILLION TIMES[TIME CHECK]
// 013 128 000 000 015 066 064 000 ; mov rega, 1000000
// 013 128 000 000 000 000 000 001 ; mov regb, 0
// 000 000 000 000 000 000 000 000 ; nop           <--
// 005 000 000 000 000 000 000 001 ; inc regb        |
// 024 000 000 000 000 000 000 001 ; cmp rega, regb  |
// 035 000 000 000 008 000 001 032 ; jne -------------
// 013 128 000 000 000 000 000 000 ; mov rega, 0
// 047 000 000 000 000 000 000 000 ; halt
// we will print to check if regb is equal to 1 million

//0.170 0.173 0.191 0.190 0.191 | 0.188 0.193 0.191 0.196 0.193 | 0.193 0.194 0.172 0.188 0.192
//0.195 0.174 0.195 0.169 0.190 | 0.175 0.172 0.194 0.197 0.195 | 0.175 0.173 0.180 0.172 0.195
// Using the old reading method:
// AVG1 => 0.1828
// AVG2 => 0.1922                TOTAL AVG => 0.1876
// AVG3 => 0.18779999999999997
// Using new reading method:
// AVG1 => 0.18460000000000001
// AVG2 => 0.18660000000000002   TOTAL AVG => 0.1834
// AVG3 => 0.179

// The all of this time is taken to execute because the OS has to read the file then parse it and write to memory
// without that the average time to execute this instruction was 0.099

int main()
{
    Manager::Load("test.enib");
    Manager::LoadToMemory();
    Manager::start();
    // CPU::init();
    // dword addr = 0b00000001000000000000000100010000;
    // //doing this manually instead of writing 64 bits at once, but i wanna see what effects this has on speed and effeciency
    // CPU::main_memory.mem_write(addr, 13);
    // CPU::main_memory.mem_write(addr+1, 128);
    // CPU::main_memory.mem_write(addr+2, 0);
    // CPU::main_memory.mem_write(addr+3, 0);
    // CPU::main_memory.mem_write(addr+4, 15);
    // CPU::main_memory.mem_write(addr+5, 66);
    // CPU::main_memory.mem_write(addr+6, 64);
    // CPU::main_memory.mem_write(addr+7, 00);

    // CPU::main_memory.mem_write(addr+8, 13);
    // CPU::main_memory.mem_write(addr+9, 128);
    // CPU::main_memory.mem_write(addr+10, 0);
    // CPU::main_memory.mem_write(addr+11, 0);
    // CPU::main_memory.mem_write(addr+12, 0);
    // CPU::main_memory.mem_write(addr+13, 0);
    // CPU::main_memory.mem_write(addr+14, 0);
    // CPU::main_memory.mem_write(addr+15, 1);

    // CPU::main_memory.mem_write(addr+16, 0);
    // CPU::main_memory.mem_write(addr+17, 0);
    // CPU::main_memory.mem_write(addr+18, 0);
    // CPU::main_memory.mem_write(addr+19, 0);
    // CPU::main_memory.mem_write(addr+20, 0);
    // CPU::main_memory.mem_write(addr+21, 0);
    // CPU::main_memory.mem_write(addr+22, 0);
    // CPU::main_memory.mem_write(addr+23, 0);

    // CPU::main_memory.mem_write(addr+24, 5);
    // CPU::main_memory.mem_write(addr+25, 0);
    // CPU::main_memory.mem_write(addr+26, 0);
    // CPU::main_memory.mem_write(addr+27, 0);
    // CPU::main_memory.mem_write(addr+28, 0);
    // CPU::main_memory.mem_write(addr+29, 0);
    // CPU::main_memory.mem_write(addr+30, 0);
    // CPU::main_memory.mem_write(addr+31, 1);

    // CPU::main_memory.mem_write(addr+32, 24);
    // CPU::main_memory.mem_write(addr+33, 0);
    // CPU::main_memory.mem_write(addr+34, 0);
    // CPU::main_memory.mem_write(addr+35, 0);
    // CPU::main_memory.mem_write(addr+36, 0);
    // CPU::main_memory.mem_write(addr+37, 0);
    // CPU::main_memory.mem_write(addr+38, 0);
    // CPU::main_memory.mem_write(addr+39, 1);

    // CPU::main_memory.mem_write(addr+40, 35);
    // CPU::main_memory.mem_write(addr+41, 0);
    // CPU::main_memory.mem_write(addr+42, 0);
    // CPU::main_memory.mem_write(addr+43, 0);
    // CPU::main_memory.mem_write(addr+44, 8);
    // CPU::main_memory.mem_write(addr+45, 0);
    // CPU::main_memory.mem_write(addr+46, 1);
    // CPU::main_memory.mem_write(addr+47, 32);

    // CPU::main_memory.mem_write(addr+48, 13);
    // CPU::main_memory.mem_write(addr+49, 128);
    // CPU::main_memory.mem_write(addr+50, 0);
    // CPU::main_memory.mem_write(addr+51, 0);
    // CPU::main_memory.mem_write(addr+52, 0);
    // CPU::main_memory.mem_write(addr+53, 0);
    // CPU::main_memory.mem_write(addr+54, 0);
    // CPU::main_memory.mem_write(addr+55, 0);

    // CPU::main_memory.mem_write(addr+56, 47);
    // CPU::main_memory.mem_write(addr+57, 0);
    // CPU::main_memory.mem_write(addr+58, 0);
    // CPU::main_memory.mem_write(addr+59, 0);
    // CPU::main_memory.mem_write(addr+60, 0);
    // CPU::main_memory.mem_write(addr+61, 0);
    // CPU::main_memory.mem_write(addr+62, 0);
    // CPU::main_memory.mem_write(addr+63, 0);
    // CPU::run();
}