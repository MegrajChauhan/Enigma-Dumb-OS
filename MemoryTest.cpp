#include "../Memory/EnigmaMemory.hpp"

int main()
{
    Memory mem1; //we are using default size here
    Memory mem2;
    mem2.mem_resize(16); // we will resize the memory to size lesser than the actual size
    int res = mem1.mem_resize(100000000); //intentional error
    if(res == Signal::MEM_REQUEST_INVALID)
    {
        std::cerr<<"Invalid memory resizing request"<<std::endl;
    }else if(res == Signal::MEM_REALLOC_FAILED){
     //this error might come, there is a chance
     std::cerr<<"Memory allocation failed."<<std::endl;
     return 0;
    }
    res = mem1.mem_add_size(12);
    if(res == Signal::MEM_SBRK_FAILED)
    {
        std::cerr<<"Error: Memory increase was rejected"<<std::endl;
    }
    // 00000010 00000000 00000001 101111001 = 67109753 [2 bytes write]
    // 00001111 11110000 = 4080
    mem1.mem_write(0b00000001000000000000001101111001,240UL); //write one byte to mem1
    mem2.mem_resize(1024);
    mem2.mem_write(0b00000001000000000000001101111001,15UL); //write one byte to mem2
    auto p = mem1.mem_read(0b00000001000000000000001101111001);
    auto q = mem2.mem_read(0b00000001000000000000001101111001);
    std::cout<<"In mem1: "<<p<<std::endl;
    std::cout<<"In mem2: "<<q<<std::endl;
    std::cout<<"Together: "<<((q << 8) | p)<<std::endl;
}