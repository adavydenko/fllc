#pragma once
#include "VerticalBits.h"
#include <vector>

template <int BitsCount, class T>
class VerticalBitsWriter
{
    VerticalBits bitContainer[BitsCount];
    std::vector<unsigned int> buffer;

public:
    void write(T& value);
    std::vector<unsigned int>& allocate();
    T* read(unsigned char* input, int len, int pointsCount);
};

template<int BitsCount, class T>
void VerticalBitsWriter<BitsCount, T>::write(T& value)
{
    for (size_t i = 0; i < BitsCount; i++)
    {
        this->bitContainer[i].add((unsigned int)value, i);
    }
}

template<int BitsCount, class T>
std::vector<unsigned int>& VerticalBitsWriter<BitsCount, T>::allocate()
{
    buffer.clear();

    for (int i = BitsCount - 1; i >= 0; i--)
    {
        bitContainer[i].flush();
        buffer.insert(buffer.end(),
            bitContainer[i].block.begin(), bitContainer[i].block.end());
    }

    return buffer;
}

template<int BitsCount, class T>
T * VerticalBitsWriter<BitsCount, T>::read(unsigned char * input, int len, int pointsCount)
{
    /*
    int bitsPerBlock = (sizeof T) * 8;
    int countBlocks = len % ((sizeof T) * 8) == 0
        ? len / ((sizeof T) * 8)
        : len / ((sizeof T) * 8) + 1;

    int targetSize =

    int* eBlock = (unpackedBits + blockSize);
    int eSize = blockSize * 8; // 8 - bits per 1 exponent
    int* eDeltas = new int[pointsCount];
    */

    /*
    T* results = new T[pointsCount];

    for (size_t i = 0; i < pointsCount; i++)
    {
        int blockNum = i / 32;
        int bitNum = i % 32;

        T value = 0;

        for (size_t j = 0; j < BitsCount; j++)
        {
            unsigned int currentBlock = input[j*blockSize + blockNum];
            currentBlock &= (1 << (32 - bitNum - 1));
            currentBlock >>= (32 - bitNum - 1);
            currentBlock <<= j;

            value |= currentBlock;
        }

        results[i] = value;
    }

    return results;
    */

    return nullptr;
}