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

#ifndef E_LOADER
#define E_LOADER

// instead of being a loader, it actually reads the binary file,
// of the final data to the OS[More like a manager that manages the Machine and not an OS]

// since this reads a singular file and then loads the data on the file, we need a different way something like a linker
//  to merge two files into one so that this can load it successfully

// The thing that the user of this component must make sure of is that, the headers are compulsory if you do not
// want to destroy your real PC since the random bit of data that was read could actually corrupt your system
// Since there are no[a little] protection measures in place, please be careful
// currently there aren't many headers but in the future, we might add some more headers for more metadata purposes
#include <fstream>
#include <filesystem>
#include <vector>
#include "../utils/EnigmaTypedefs.hpp"
#include "../utils/Esignals.hpp"
#include <iostream>

class Loader
{
public:
    Loader() = default;

    void set_filename(std::string filename);

    Signal read_and_load();

    std::vector<std::uint8_t> get_bytes();
    int get_data_start();
    int get_data_end();
    Signal validate(std::string filename);

private:
    std::ifstream read;
    std::vector<std::uint8_t> out;
    int data_st;
    int data_end;
    std::string format();
    int data__addr();

    std::string read_line();

    Signal read_hexadecimal();
    Signal read_decimal();
    Signal read_binary();
};

int Loader::get_data_end()
{
    return data_end;
}

int Loader::get_data_start()
{
    return data_st;
}

std::vector<std::uint8_t> Loader::get_bytes()
{
    return out;
}

Signal Loader::validate(std::string filename)
{
    std::filesystem::path p = std::filesystem::current_path() / filename;
    std::string ext = filename.substr(filename.length() - 5);
    if (ext != ".enib")
    {
        return Signal::LOADER_INVALID_BIN_FILE;
    }
    if (!std::filesystem::exists(p))
    {
        return Signal::LOADER_FILE_NOT_EXISTS;
    }
    if (std::filesystem::is_directory(p))
    {
        return Signal::LOADER_FILE_A_DIRECTORY;
    }
    return Signal::OPERATION_SUCCESS;
}

void Loader::set_filename(std::string filename)
{
    read.open(filename, std::ios::in);
}

std::string Loader::read_line()
{
    std::string line;
    std::getline(read, line);
    return line;
}

std::string Loader::format()
{
    return read_line();
}

int Loader::data__addr()
{
    return std::stoi(read_line());
}

Signal Loader::read_hexadecimal()
{
    // 4 characters can represent all that there is or 0x00 in hex
    char *buff = new char[4];
    while (!read.eof())
    {
        read.read(buff, 4); // if it is binary we read 8 characters and since we do not check for spaces and the stuff
        try
        {
            out.push_back(std::stoi(std::string{buff}, 0, 16));
        }
        catch (const std::invalid_argument &arg)
        {
            delete[] buff;
            return Signal::LOADER_LOADED_FILE_FORMAT_INVALID;
        }
        if (read.peek() == (int)' ') // the data can have spaces between them for readability. This USED to be mandatory and now it's not!
        {
            read.ignore();
        }
    }
    delete[] buff;
    return Signal::OPERATION_SUCCESS;
}

Signal Loader::read_binary()
{
    char *buff = new char[8];
    while (!read.eof())
    {
        read.read(buff, 8); // if it is binary we read 8 characters and since we do not check for spaces and the stuff
        try
        {
            out.push_back(std::stoi(std::string{buff}, 0, 2));
        }
        catch (const std::invalid_argument &arg)
        {
            delete[] buff;
            return Signal::LOADER_LOADED_FILE_FORMAT_INVALID;
        }
        if (read.peek() == (int)' ') // the data can have spaces between them for readability. This USED to be mandatory and now it's not!
        {
            read.ignore();
        }
    }
    delete[] buff;
    return Signal::OPERATION_SUCCESS;
}

Signal Loader::read_decimal()
{
    // decimal is a bit of a pain. Mostly the instructions are 2 numbers long but there are some that are 3 numbers long
    char *buff = new char[3];
    int i = 0;
    while (!read.eof())
    {
        // since decimals might have 3 numbers, we now read 3 numbers at once
        read.read(buff, 3);
        try
        {
            out.push_back(std::stoi(std::string{buff}, 0, 10));
        }
        catch (const std::invalid_argument &arg)
        {
            delete[] buff;
            return Signal::LOADER_LOADED_FILE_FORMAT_INVALID;
        }
        if (read.peek() == (int)' ') // the data can have spaces between them for readability. This USED to be mandatory and now it's not!
        {
            read.ignore();
        }
    }
    // even if the number is more than 255[which is invalid], the way that the numbers are stored in the memory that is prevented
    return Signal::OPERATION_SUCCESS;
}

Signal Loader::read_and_load()
{
    // now we have to read the file. 4 bytes or 4 8 bit binary numbers are read at once
    // because the file may be either binary or decimal or hexadecimal that is the user choice or the assemblers choice
    // the bin file must have headers
    // first line in the file[must be the format of the file binary, hex, or decimal?]
    auto __format = format();
    // second line the point where we begin to get the data bytes
    data_st = data__addr();
    // third line: the point where the data bytes end
    data_end = data__addr();
    if (__format != "decimal" && __format != "binary" && __format != "hexadecimal")
    {
        return Signal::LOADER_FILE_FORMAT_INVALID;
    }
    Signal sig = (__format == "decimal") ? read_decimal() : (__format == "hexadecimal") ? read_hexadecimal()
                                                                                        : read_binary();
    if (data_end == -1 && data_st == -1)
    {
        return Signal::OPERATION_SUCCESS;
    }
    if (data_st > out.size() || data_end < data_st || data_end > out.size() || (data_end > -1 && data_st <= -1) || (data_end <= -1 && data_st > -1)) // if the points are -1 then we assume that there are no data at all
    {
        return Signal::LOADER_DATA_POINTS_NOT_CLEAR;
    }
    return sig;
}

#endif