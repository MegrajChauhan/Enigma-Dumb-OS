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

#ifndef E_MEMORY
#define E_MEMORY

#include <cstdint>
#include <memory.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <cmath>

#include "../utils/Esignals.hpp"

typedef std::uint8_t byte;
typedef std::uint16_t word;
typedef std::uint32_t dword;
typedef std::uint64_t qword;

#define MEM_START_SIZE 1024 // the memory starts with 1MB for each program, this is by default

/*
What we first need is a manager of the memory itself.
*/

class Memory
{
public:
    Memory();

    int mem_resize(dword new_size);
    int mem_add_size(dword __size_to_add);

    qword mem_read(dword address); // this one function should handle all 4 lengths of read

    int mem_write(dword address, qword value);

    ~Memory();

private:
    qword *memory;        // the memory
    dword __current_size; // the current memory size
    dword __current_memory_size;

    byte get_size(dword addr);
    dword get_addr(dword addr);
};

Memory::Memory()
{
    auto x = sbrk(MEM_START_SIZE + 128); // for extra measures
    if (x == (void *)-1)
    {
        std::cerr << "Error: The memory allocation failed." << std::endl;
        exit(-1);
    }
    memory = (qword *)malloc(MEM_START_SIZE); // this is the default and it can be left to this.
    if (memory == nullptr)
    {
        // since we cannot send a signal and memory is not allocated so, we can exit the program
        std::cerr << "Internal Error: Memory allocation failed." << std::endl;
        exit(-1);
    }
    __current_size = MEM_START_SIZE + 128; // this keeps track of the current size of the pool we have requested
    __current_memory_size = MEM_START_SIZE;
}

Memory::~Memory()
{
    free(memory); // delete the memory
}

// if the memory size is not acceptable, this can be used to increase the size
int Memory::mem_resize(dword new_size)
{
    if (new_size > __current_size)
    {
        return Signal::MEM_REQUEST_INVALID;
    }
    if (new_size < __current_memory_size)
    {
        qword *temp = (qword *)malloc(new_size);
        memcpy(temp, memory, new_size);
        memory = temp;
        return Signal::OPERATION_SUCCESS;
    }
    memory = (qword *)realloc(memory, new_size); // the new_size must be a full and complete size
    if (memory == nullptr)
    {
        // realloc failed: send a signal and let the caller handle it
        return Signal::MEM_REALLOC_FAILED;
    }
    __current_memory_size = new_size;
    return Signal::OPERATION_SUCCESS; // reallocation was successful
}

int Memory::mem_add_size(dword __size_to_add)
{
    // unlike mem_resize this is used to add a little bit of size to the original size of the memory pool we have
    // it uses sbrk to ask for more memory
    auto x = sbrk(__size_to_add);
    if(x == (void*)-1)
    {
        return Signal::MEM_SBRK_FAILED;
    }
    return Signal::OPERATION_SUCCESS;
}

// this function will be used to get the number of bytes we have to read
//  from the given addr
byte Memory::get_size(dword addr)
{
    // since the first byte of the address are shifted and 0's are added automatically added to the begining
    //  we do not have to perform any & operation
    return (addr >> 24);
}

// this function will be used to get the actual address from addr removing the size
dword Memory::get_addr(dword addr)
{
    return (addr & 16777215);
}

qword Memory::mem_read(dword address)
{
    auto s = get_size(address);
    auto i = get_addr(address);
    qword res;
    if (i < 0 && (i + s) > __current_size)
    {
        return Signal::MEM_ADDRESS_OUT_OF_BOUNDS;
    }
    if (s != 1 && s != 2 && s != 4 && s != 8)
    {
        return Signal::MEM_INVALID_SIZE;
    }
    switch (s)
    {
    case 1:
        return memory[i]; // just directly send it
    case 2:
    {
        // now we need to read two bytes and send it as one
        res = memory[i];                  // first get the first byte specified by the address
        res = (res << 8) & memory[i + 1]; // now ge the next byte and add it to the end of res
        break;
    }
    case 4:
    {
        // read 4 bytes just like reading 2 bytes
        for (int x = 0; i < 4; i++)
        {
            res = (res << (x * 8)) & memory[i + x];
        }
        break;
    }
    case 8:
    {
        // same as reading 4 bytes but this time we read 8 bytes
        for (int x = 0; i < 8; i++)
        {
            res = (res << (x * 8)) & memory[i + x];
        }
        break;
    }
    }
    return res;
}

// this function will write value to address
// if there are errors, signals will be sent
// here too, the format must be followed of addressing
int Memory::mem_write(dword address, qword value)
{
    auto s = get_size(address);
    auto i = get_addr(address);
    if (i < 0 && (i + s) > __current_size)
    {
        return Signal::MEM_ADDRESS_OUT_OF_BOUNDS;
    }
    if (s != 1 && s != 2 && s != 4 && s != 8)
    {
        return Signal::MEM_INVALID_SIZE;
    }
    switch (s)
    {
    case 1:
        memory[i] = (value & 255); // just save the 1 byte from the last it doesn't matter how long the value is
        break;
    case 2:
    {
        // we now need to write to two bytes
        memory[i] = (value >> 8) & 255;
        memory[i + 1] = (value & 255);
        break;
    }
    case 4:
    {
        // now loop and write
        for (int x = 0; i < 4; i++)
        {
            memory[i + x] = (value >> (x * 8)) & 255;
        }
        break;
    }
    case 8:
    {
        //same as 4 bytes but loop 8 times
        for (int x = 0; i < 8; i++)
        {
            memory[i + x] = (value >> (x * 8)) & 255;
        }
        break;
    }
    }
    return Signal::OPERATION_SUCCESS;
}

#endif