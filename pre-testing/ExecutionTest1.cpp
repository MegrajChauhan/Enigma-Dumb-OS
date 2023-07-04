#include "../OS/EnigmaOS.hpp"

//Tests done here:
// Program read from file -- success
// Program loaded to memory -- success
// Program executed -- success
// Stack protection check -- success

int main()
{
    Manager::Load("test.enib");
    Manager::LoadToMemory();
    Manager::start(); 
}