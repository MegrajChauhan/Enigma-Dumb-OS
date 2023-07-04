#include "../OS/EnigmaOS.hpp"

int main()
{
    Manager::Load("test.enib");
    Manager::LoadToMemory();
    Manager::start();
}