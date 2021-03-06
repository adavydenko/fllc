#pragma once
#include "VerticalBits.hpp"
#include <vector>

template <int BitsCount, typename T>
class VerticalBitsWriter
{
    VerticalBits<T> bitContainer[BitsCount];
    std::vector<unsigned char> buffer;

public:
    void write(T value);
    std::vector<unsigned char>& allocate();
    T* read(unsigned char* input, int len, int pointsCount);
};

template<int BitsCount, typename T>
void VerticalBitsWriter<BitsCount, T>::write(T value)
{
    for (size_t i = 0; i < BitsCount; i++)
    {
        this->bitContainer[i].add(value, i);
    }
}

template<int BitsCount, typename T>
std::vector<unsigned char>& VerticalBitsWriter<BitsCount, T>::allocate()
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

template<int BitsCount, typename T>
T * VerticalBitsWriter<BitsCount, T>::read(unsigned char * input, int len, int pointsCount)
{
    //all sizes are in bits
    int sectors = BitsCount;
    int blockSize = 8; // sizeof(unsigned char)
    int blocksInSector = len * 8 / (blockSize * sectors);

    int blocksInSector_verified = (pointsCount % blockSize == 0)
        ? pointsCount / blockSize
        : 1 + pointsCount / blockSize;

    if (blocksInSector != blocksInSector_verified)
    {
        throw std::invalid_argument("pointsCount");
    }

    T* results = new T[pointsCount];
    unsigned char * blocks = input;

    for (size_t i = 0; i < pointsCount; i++)
    {
        int blockNum = i / blockSize;
        int bitNum = i % blockSize;

        int shift = (8 - bitNum - 1);

        T value;

        // this is an exception case for size_t sectorNum = 0
        // thus no if() is required on each for loop
        if (blocks[blockNum] & (1 << shift))
        {
            value = -1;
        }
        else
        {
            value = 0;
        }

        for (size_t sectorNum = 1/*0*/; sectorNum < BitsCount; sectorNum++)
        {
            unsigned char currentBlock = blocks[sectorNum*blocksInSector + blockNum];
            currentBlock &= (1 << shift /*(8 - bitNum - 1)*/);
            currentBlock >>= shift /*(8 - bitNum - 1)*/;
            //currentBlock <<= (BitsCount - sectorNum - 1); //currentBlock cannot be shifted on T size

            /*
            if (sectorNum == 0 && currentBlock) // first symbol == 1
            {
                value |= (-1 << BitsCount); // in case BitsCount < sizeof(T), then fill 1with leading 1's
            }
            */

            value <<= 1;
            value |= currentBlock;
        }

        results[i] = value;
    }

    return results;
}