// EngelsonFritzson.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "structs.h"
#include "EngelsonFritzson.h"

using namespace std;

int highLimits[32];
int lowLimits[32];

unsigned char DropWithSign(const int &delta)
{
	if (delta >= 0)
	{
		for (size_t i = 0; i < 32; i++)
		{
			if (delta < highLimits[i])
				return i + 1;
		}
	}
	else
	{
		for (size_t i = 0; i < 32; i++)
		{
			if (delta > lowLimits[i])
				return i + 1;
		}
	}

	return 0;
}


EngelsonFritzson::EngelsonFritzson() : currentBlock(nullptr), usedBits(0)
{
}

void EngelsonFritzson::compress(_float * nir, int count)
{
	int delta = 0;
	int ddelta = 0;
	int dddelta = 0;

	for (int i = 0; i < count; i++)
	{
		if (i == 0)
		{
			add(nir[i]._value);
		}

		if (i > 0)
		{
			delta = nir[i]._value - nir[i - 1]._value;
		}

		if (i == 1)
		{
			add(delta);
		}

		if (i > 1)
		{
			ddelta = delta - (nir[i - 1]._value - nir[i - 2]._value);
		}

		if (i == 2)
		{
			add(ddelta);
		}

		if (i > 2)
		{
			int ddelta_1 = delta - ddelta - (nir[i - 2]._value - nir[i - 3]._value);

			dddelta = ddelta - ddelta_1;

			add(dddelta);
		}
	}
}

unsigned int * EngelsonFritzson::allocate(int * bytesCount)
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

	*bytesCount = total;
	return result;
}

void EngelsonFritzson::flush()
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

void EngelsonFritzson::add(unsigned int data, int bitsToCopy)
{
	if (!currentBlock)
	{
		currentBlock = new unsigned int(0x0);
	}

	data = (data << (32 - bitsToCopy)) >> (32 - bitsToCopy); // erase what was at left
	int freeBits = 32 - usedBits;

	if (bitsToCopy <= freeBits)
	{
		*currentBlock = *currentBlock << bitsToCopy;
		*currentBlock = *currentBlock | data;
		usedBits = usedBits + bitsToCopy;
	}
	else
	{
		unsigned int left = data >> (bitsToCopy - freeBits);
		add(left, freeBits);

		data = (data << (32 + freeBits - bitsToCopy)) >> (32 + freeBits - bitsToCopy);
		add(data, (bitsToCopy - freeBits));
	}

	if (usedBits == 32)
		flush();
}

void EngelsonFritzson::add(int value)
{
	int bitsToStore = DropWithSign(value);
	add(bitsToStore, bitsPerSize);
	add(value, bitsToStore);
}
