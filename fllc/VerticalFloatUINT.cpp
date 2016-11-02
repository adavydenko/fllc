#include "stdafx.h"
#include "VerticalFloatUINT.h"
#include "DeltasCalculator.h"
#include <vector>
#include "ZstdWrapper.h"

void VerticalFloatUINT::compress(_float * arr, int count)
{
    DeltasCalculator<unsigned int, DELTA_ORDER> calculator;

    unsigned int* delta = new unsigned int[count];

    calculator.getDeltas((unsigned int*)arr, count, delta);

    for (int j = 0; j < count; j++)
    {
        this->writer.write(delta[j]);
    }

    delete[] delta;
}

unsigned char * VerticalFloatUINT::allocate(int * count)
{
    //v1.insert( v1.end(), v2.begin(), v2.end() );

    std::vector<unsigned char> data = writer.allocate();
    int sizeInChars = data.size();

    ZstdWrapper zip;

    int nLenDst = zip.GetMaxCompressedLen(sizeInChars);
    unsigned char* buffer = new unsigned char[nLenDst];  // alloc dest buffer

    int nLenPacked = zip.CompressData((const unsigned char*)data.data(), sizeInChars, buffer, nLenDst, COMPRESSION_RATIO);
    if (nLenPacked == -1) return nullptr;

    *count = nLenPacked;
    return buffer;

    /*
    *count = sizeInChars;
    unsigned char* result = new unsigned char[sizeInChars];
    memcpy(result, data.data(), sizeInChars);

    return result;
    */
}

_float * VerticalFloatUINT::decompress(unsigned char * data, int dataSize, int pointsCount)
{
    int blockSizeInt = (pointsCount % 8 == 0) ? (pointsCount / 8) : (pointsCount / 8 + 1);
    int targetSize = (1 + 8 + 23) /* * 4*/ * blockSizeInt; // blockSize in ints

    unsigned char* unpacked = new unsigned char[targetSize];

    ZstdWrapper zip;
    int nLen = zip.UncompressData(data, dataSize, unpacked, targetSize);

    unsigned char* current = unpacked;

    /*
    unsigned char* current = data;
    */

    int size = 32 * blockSizeInt/* * 4*/;
    unsigned int* deltas = writer.read(current, size, pointsCount);

    //delete[] unpacked;

    unsigned int* d = new unsigned int[pointsCount]; // resulting exponent

    DeltasCalculator<unsigned int, DELTA_ORDER> calculator;
    calculator.getOriginal(deltas, pointsCount, d);

    _float* result = new _float[pointsCount];
    for (size_t i = 0; i < pointsCount; i++)
    {
        result[i]._value = d[i];
    }

    delete[] deltas;
    delete[] d;

    return result;
}
