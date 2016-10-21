#pragma once
#include <vector>

template <typename T>
class VerticalBits
{
    int usedBits;
    unsigned char* currentBlock;

public:
    std::vector<unsigned char> block;

    VerticalBits();
    void add(std::make_unsigned_t<T> data, int bitNum); //~= void add(unsigned T& value, int bitNum);
    void add(bool data);
    unsigned char* allocate(int* count); //don't use unsigned chars, just to minimize allocation
    void flush();
};

template <typename T>
VerticalBits<T>::VerticalBits() : currentBlock(nullptr), usedBits(0)
{ }

template <typename T>
void VerticalBits<T>::add(std::make_unsigned_t<T> data, int bitNum)
{
    // before start
    // bitNum 'X' = 11 (starts with 0)
    // 00000011.00000000.0000X000.01100000

    data <<= (sizeof(data) * 8 - bitNum - 1);
    // X0000110.00000000.00000000.0000.0000

    data >>= (sizeof(data) * 8 - 1);
    // 00000000.00000000.00000000.0000000X

    if (!currentBlock)
    {
        currentBlock = new unsigned char(0);
    }

    *currentBlock <<= 1;
    *currentBlock |= data;
    usedBits++;

    if (usedBits == 8)
        flush();
}

template <typename T>
void VerticalBits<T>::add(bool data)
{
    if (!currentBlock)
    {
        currentBlock = new unsigned char(data);
        usedBits++;
        return;
    }

    *currentBlock <<= 1;
    *currentBlock |= data;
    usedBits++;

    if (usedBits == 8)
        flush();
}

template <typename T>
unsigned char * VerticalBits<T>::allocate(int * count)
{
    flush();

    int total = this->block.size();
    if (!total)
        return nullptr;

    *count = total;
    return this->block.data();
}

template <typename T>
void VerticalBits<T>::flush()
{
    if (currentBlock == nullptr)
    {
        return;
    }

    if (usedBits < 8 && currentBlock)
    {
        *currentBlock = *currentBlock << (8 - usedBits); //e.g. 0000...0000 00111101 -> 11110100 ... 0000 (usedBits = 6)
    }

    block.push_back(unsigned char(*currentBlock));

    currentBlock = nullptr;
    usedBits = 0;
}

