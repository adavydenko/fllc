#pragma once
#include "structs.h"
#include "VerticalBits.h"
#include "VerticalBitsWriter.h"

class VerticalFloat
{
public:
    VerticalBits sBits;     //bits for storing float's sign
    VerticalBits eBits[8];  //bits to store float's exponent
    VerticalBits mBits[24]; //bits to store float's mantissa

    VerticalBitsWriter<1, char> signWriter;
    VerticalBitsWriter<8, int> exponentWriter;
	VerticalBitsWriter<23, int> mantissaWriter;

    void compress(_float* arr, int count);
    unsigned char* allocate(int* count); //don't use unsigned chars, just to minimize allocation
    _float* decompress(unsigned char* data, int dataSize, int pointsCount);
};

