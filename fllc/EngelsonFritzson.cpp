// EngelsonFritzson.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "structs.h"
#include "EngelsonFritzson.h"
#include "DeltasCalculator.cpp"

int EngelsonFritzson::lowLimits[32];
int EngelsonFritzson::highLimits[32];
EngelsonFritzson::Initializer EngelsonFritzson::initializer;

EngelsonFritzson::Initializer::Initializer()
{
    for (size_t i = 0; i < 32; i++)
    {
        highLimits[i] = 1 << i; //<
    }

    for (size_t i = 0; i < 32; i++)
    {
        lowLimits[i] = ~highLimits[i] + 1; //>
    }
}

EngelsonFritzson::EngelsonFritzson() : currentBlock(nullptr), usedBits(0)
{
}

unsigned char EngelsonFritzson::meaningfulBits(const int &value)
{
    if (value >= 0)
    {
        for (size_t i = 0; i < 32; i++)
        {
            if (value < highLimits[i])
                return (i + 1);
        }
    }
    else
    {
        for (size_t i = 0; i < 32; i++)
        {
            if (value > lowLimits[i])
                return (i + 1);
        }
    }

    return 0;
}

void EngelsonFritzson::compress(_float * nir, int count)
{
    int* data = (int*)nir;
    int* deltas = new int[count];

    DeltasCalculator<int, 3> calculator;
    calculator.getDeltas(data, count, deltas);

    for (int i = 0; i < count; i++)
    {
        add(deltas[i]);
    }

    delete[] deltas;
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

_float * EngelsonFritzson::decompress(int * data, int count)
{
    return nullptr;
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
    int bitsToStore = meaningfulBits(value);
    add(bitsToStore, bitsPerSize);
    add(value, bitsToStore);
}

