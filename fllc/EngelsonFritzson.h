#pragma once
#include <vector>

#define DELTA_ORDER 3

class EngelsonFritzson
{
    const int bitsPerSize = 5;
    std::vector<unsigned int> block;
    int usedBits;
    unsigned int* currentBlock;

    void write(unsigned int data, int bitsToCopy);
    void write(int value);
    unsigned int read(unsigned int * &data, int &lastPosition, int bitsToRead, bool fillNegative = false);

    void flush();

    static int highLimits[32];
    static int lowLimits[32];
    static unsigned char meaningfulBits(const int &value);

    class Initializer
    {
    public: Initializer();
    };
    friend class Initializer;

    static Initializer initializer;

public:
    EngelsonFritzson();

    void compress(_float* arr, int count);
    unsigned int* allocate(int* count); //don't use unsigned chars, just to minimize allocation
    _float* decompress(unsigned int* data, int pointsCount);

};

