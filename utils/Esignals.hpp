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

#ifndef E_SIGNALS
#define E_SIGNALS

//this module defines the signals that the different components can transfer between themselves
enum Signal
{
    MEM_REALLOC_FAILED, //it was requested to increase the current size of the memory but the request failed
    MEM_REQUEST_INVALID, //it was requested to increase memory but the current pool size was in the way
    MEM_INVALID_SIZE, //it was requested to access number of bytes other than 1, 2, 4 and 8.
    MEM_ADDRESS_OUT_OF_BOUNDS, //data was requested from a memory address but the address is out of bounds
    MEM_SBRK_FAILED, //the OS was requested to increase the current memory pool but the request was rejected
    OPERATION_SUCCESS,
};

#endif