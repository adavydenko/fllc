#pragma once
#include "structs.h"
#include "VerticalBits.hpp"
#include "VerticalBitsWriter.hpp"

class VerticalFloatUINT
{
public:
    VerticalBits<unsigned int> bits[32]; //bits to store float's mantissa

    VerticalBitsWriter<32, unsigned int> writer;

    void compress(_float* arr, int count);
    unsigned char* allocate(int* count); //don't use unsigned chars, just to minimize allocation
    _float* decompress(unsigned char* data, int dataSize, int pointsCount);
};

