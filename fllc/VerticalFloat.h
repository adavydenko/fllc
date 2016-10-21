#pragma once
#include "structs.h"
#include "VerticalBits.hpp"
#include "VerticalBitsWriter.hpp"

class VerticalFloat
{
public:
    VerticalBits<char> sBits;     //bits for storing float's sign
    VerticalBits<int> eBits[8];  //bits to store float's exponent
    VerticalBits<int> mBits[24]; //bits to store float's mantissa

    VerticalBitsWriter<1, char> signWriter;
    VerticalBitsWriter<8, int> exponentWriter;
    VerticalBitsWriter<23, int> mantissaWriter;

    void compress(_float* arr, int count);
    unsigned char* allocate(int* count); //don't use unsigned chars, just to minimize allocation
    _float* decompress(unsigned char* data, int dataSize, int pointsCount);
};

