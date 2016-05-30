#include "stdafx.h"
#include "VerticalBits.h"


VerticalBits::VerticalBits() : currentBlock(nullptr), usedBits(0)
{
}


VerticalBits::~VerticalBits()
{
}

void VerticalBits::add(unsigned int data, int bitNum)
{
	// before start
	// 0000.0011 0000.0000 0000._X_000 0110.0000, bitNum = 11

	data <<= (32 - bitNum - 1);
	// _X_000.0110 0000.0000 0000.0000 0000.0000

	data >>= (32 - bitNum);
	// 0000.0000 0000.0000 0000.0000 0000.000_X_

	if (!currentBlock)
	{
		currentBlock = new unsigned int(data);
		usedBits++;
		return;
	}

	*currentBlock <<= 1;
	*currentBlock |= data;
	usedBits++;

	if (usedBits == 32)
		flush();
}

/*void VerticalBits::add(unsigned char data, int bitNum)
{
	data <<= (8 - bitNum - 1);
	data >>= (8 - bitNum);

	if (!currentBlock)
	{
		currentBlock = new unsigned int(data);
		usedBits++;
		return;
	}

	*currentBlock <<= 1;
	*currentBlock |= data;
	usedBits++;

	if (usedBits == 32)
		flush();
}*/

void VerticalBits::add(bool data)
{
	if (!currentBlock)
	{
		currentBlock = new unsigned int(data);
		usedBits++;
		return;
	}

	*currentBlock <<= 1;
	*currentBlock |= data;
	usedBits++;

	if (usedBits == 32)
		flush();
}

unsigned int * VerticalBits::allocate(int * count)
{
	flush();

	int total = this->block.size();

	if (!total)
		return nullptr;

	unsigned int* result = new unsigned int[total];
	for (size_t i = 0; i < total; i++)
	{
		result[i] = this->block[i];
	}

	*count = total;
	return result;
}

void VerticalBits::flush()
{
	if (currentBlock == nullptr)
	{
		return;
	}

	if (usedBits < 32 && currentBlock)
	{
		*currentBlock = *currentBlock << (32 - usedBits); //e.g. 0000...0000 00111101 -> 11110100 ... 0000 (usedBits = 6)
	}

	block.push_back(unsigned int(*currentBlock));

	currentBlock = nullptr;
	usedBits = 0;
}
