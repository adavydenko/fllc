#pragma once
#include <vector>

class VerticalBits
{
    int usedBits;
    unsigned int* currentBlock;

public:
    std::vector<unsigned int> block;

    VerticalBits();
    void add(unsigned int data, int bitNum);
    void add(bool data);
    unsigned int* allocate(int* count); //don't use unsigned chars, just to minimize allocation
    void flush();
};

