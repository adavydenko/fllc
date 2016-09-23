#pragma once
#include <vector>

class VerticalBits
{
    int usedBits;
    unsigned char* currentBlock;

public:
    std::vector<unsigned char> block;

    VerticalBits();
    void add(unsigned int data, int bitNum);
    void add(bool data);
    unsigned char* allocate(int* count); //don't use unsigned chars, just to minimize allocation
    void flush();
};

