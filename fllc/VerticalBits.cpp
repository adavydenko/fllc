#include "stdafx.h"
#include "VerticalBits.h"

VerticalBits::VerticalBits() : currentBlock(nullptr), usedBits(0)
{ }

void VerticalBits::add(unsigned int data, int bitNum)
{
    // before start
    // 00000011.00000000.0000X000.01100000, bitNum = 11 (starts with 0)

    data <<= (sizeof(data)*8/*32*/ - bitNum - 1);
    // X0000110.00000000.00000000.0000.0000

    data >>= (sizeof(data)*8/*32*/ - 1);
    // 00000000.00000000.00000000.0000000X

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

void VerticalBits::add(bool data)
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

unsigned char * VerticalBits::allocate(int * count)
{
    flush();

    int total = this->block.size();
    if (!total)
        return nullptr;

    *count = total;
    return this->block.data();
}

void VerticalBits::flush()
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
