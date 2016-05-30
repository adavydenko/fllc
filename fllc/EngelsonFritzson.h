#pragma once
#include <vector>

class EngelsonFritzson
{
	const int bitsPerSize = 5;
	std::vector<unsigned int> block;
	int usedBits;
	unsigned int* currentBlock;

	void add(unsigned int data, int bitsToCopy);
	void add(int value);

public:
	EngelsonFritzson();

	void compress(_float* arr, int count);
	unsigned int* allocate(int* count); //don't use unsigned chars, just to minimize allocation
	void flush();
};

