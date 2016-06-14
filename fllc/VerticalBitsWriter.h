#pragma once
#include "VerticalBits.h"
#include <vector>

template <int BitsCount, typedef T>
class VerticalBitsWriter
{
    VerticalBits bitContainer[BitsCount];
    std::vector<unsigned char> buffer;

public:

    void write(T& value);
    std::vector<unsigned char>& allocate();
    T* read(unsigned char* input, int countT);

    VerticalBitsWriter();
    ~VerticalBitsWriter();
};

