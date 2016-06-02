#pragma once
#include <vector>

class EngelsonFritzson
{
    const int bitsPerSize = 5;
    std::vector<unsigned int> block;
    int usedBits;
    unsigned int* currentBlock;

    void add(unsigned int data, int bitsToCopy);
    void add(int value);

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
    _float* decompress(int* data, int count);

};

