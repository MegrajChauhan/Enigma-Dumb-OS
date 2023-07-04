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

#ifndef E_OS
#define E_OS

// this module takes a binary file and then puts it into the memory of the CPU.
// after upload is completed, the execution starts

#include "../CPU/EnigmaCPU.hpp"
#include "EnigmaLoader.hpp"

namespace Manager
{
    static std::vector<std::uint8_t> inp_bytes;
    static int data_point_st, data_point_end;
    void Load(std::string filename);
    void LoadToMemory();
    void start();
};

void Manager::Load(std::string filename)
{
    Loader loader;
    Signal rets;
    rets = loader.validate(filename);
    if (rets != Signal::OPERATION_SUCCESS)
    {
        std::cerr << "Error: " << signal_to_string(rets) << std::endl;
        std::cerr << "Exectution terminated" << std::endl;
        exit(-1);
    }
    loader.set_filename(filename);
    rets = loader.read_and_load();
    if (rets != Signal::OPERATION_SUCCESS)
    {
        std::cerr << "Error: " << signal_to_string(rets) << std::endl;
        std::cerr << "Exectution terminated" << std::endl;
        exit(-1);
    }
    inp_bytes = loader.get_bytes();
    data_point_st = loader.get_data_start();
    data_point_end = loader.get_data_end();
}

void Manager::LoadToMemory()
{
    dword __address = 0b00000001000000000000000100010000; // since the memory address actually starts at 272
    if (inp_bytes.size() - (data_point_end - data_point_st) > (1024 - 272))
    {
        auto rets = CPU::main_memory.mem_resize(1024 + (data_point_end - data_point_st)); // since we have spare 128 bytes
        if (rets != Signal::OPERATION_SUCCESS)
        {
            // failed miserably[could be either realloc failed or the pool wasn't big enough]
            rets = CPU::main_memory.mem_add_size((data_point_end - data_point_st)); // increase the pool
            if (rets != Signal::OPERATION_SUCCESS)
            {
                // we did what we could
                std::cerr << "Internal Error: Memory allocation failed" << std::endl;
                exit(-1);
            }
            rets = CPU::main_memory.mem_resize(1024 + (data_point_end - data_point_st)); // try once again
            if (rets != Signal::OPERATION_SUCCESS)
            {
                // we did what we could :(
                std::cerr << "Internal Error: Memory allocation failed" << std::endl;
                exit(-1);
            }
        }
    }
    for (int i = 0; i < inp_bytes.size(); i++)
    {
        if (i == data_point_st)
        {
            if ((data_point_end - i) > 1024)
            {
                auto rets = CPU::memory.mem_resize(1024 + (data_point_end - i) + 64); // since we have spare 128 bytes
                if (rets != Signal::OPERATION_SUCCESS)
                {
                    // failed miserably[could be either realloc failed or the pool wasn't big enough]
                    rets = CPU::memory.mem_add_size((data_point_end - i + 64)); // increase the pool
                    if (rets != Signal::OPERATION_SUCCESS)
                    {
                        // we did what we could
                        std::cerr << "Internal Error: Memory allocation failed" << std::endl;
                        exit(-1);
                    }
                    rets = CPU::memory.mem_resize(1024 + (data_point_end - i) + 64); // try once again[getting some extra in case we might need it]
                    if (rets != Signal::OPERATION_SUCCESS)
                    {
                        // we did what we could :(
                        std::cerr << "Internal Error: Memory allocation failed" << std::endl;
                        exit(-1);
                    }
                }
                // pool was increased or the realloc passed
            }
            dword addr = 0b00000001000000000000000000000000;
            while (i != data_point_end + 1)
            {
                // now we are ready to start pushing the data which starts from 0x0 address
                // the decision for different memory for instruction IObuffer and the data was to make it easier to write an assembler and the CPU
                CPU::memory.mem_write(addr, inp_bytes[i]);
                i++;
                addr++;
            }
        }
        // data writing has been handled
        CPU::main_memory.mem_write(__address, inp_bytes[i]);
        __address++;
    }
}

void Manager::start()
{
    CPU::init();
    CPU::run();
    exit(CPU::_e_registers[CPU::rega]);
}

#endif