#pragma once
#include <vector>

class VerticalBits
{
	std::vector<unsigned int> block;

	int usedBits;
	unsigned int* currentBlock;

public:
	VerticalBits();
	~VerticalBits();

	void add(unsigned int data, int bitNum);
	//void add(unsigned char data, int bitNum);
	void add(bool data);
	unsigned int* allocate(int* count); //don't use unsigned chars, just to minimize allocation
	void flush();
};

