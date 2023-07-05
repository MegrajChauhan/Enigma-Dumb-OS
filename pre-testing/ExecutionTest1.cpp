#include "../OS/EnigmaOS.hpp"

//Tests done here:
// Program read from file -- success
// Program loaded to memory -- success
// Program executed -- success
// Stack protection check -- success
// Instruction execution check -- success
// Memory reading -- success
// Memory writing -- success
// Loading of different formats -- success
// Execution in different formats -- success
// Instructions executing -- as intended
// Other tests in the Tests folder of this same folder
// test.enib moved inside the Tests folder

int main()
{
    Manager::Load("test.enib");
    Manager::LoadToMemory();
    Manager::start(); 
}