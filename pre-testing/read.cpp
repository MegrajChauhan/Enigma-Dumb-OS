#include "../OS/EnigmaLoader.hpp"

int main()
{
    Loader loader;
    loader.set_filename("test.enib");
    loader.read_and_load();
}