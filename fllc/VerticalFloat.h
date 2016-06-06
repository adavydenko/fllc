#pragma once
#include "structs.h"
#include "VerticalBits.h"

class VerticalFloat
{
public:
    VerticalBits sBits;     //bits for storing float's sign
    VerticalBits mBits[8];  //bits to store float's mantissa
    VerticalBits eBits[24]; //bits to store float's exponent

    void compress(_float* arr, int count);
    unsigned char* allocate(int* count); //don't use unsigned chars, just to minimize allocation
};

