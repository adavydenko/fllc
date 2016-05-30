#pragma once
#include "structs.h"
#include "VerticalBits.h"

class VerticalFloat
{
public:
	VerticalBits sBits;
	VerticalBits mBits[8];
	VerticalBits eBits[24];

	void compress(_float* arr, int count);
	unsigned int* allocate(int* count); //don't use unsigned chars, just to minimize allocation
};

