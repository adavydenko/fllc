// VerticalFloat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DeltasCalculator.h"
#include "VerticalFloat.h"
#include <vector>
#include "ZlibWrapper.h"

void VerticalFloat::compress(_float * nir, int count)
{
    DeltasCalculator<int, DELTA_ORDER> calculator;

    int* e = new int[count];
    int* m = new int[count];

    for (int i = 0; i < count; i++)
    {
        e[i] = int(nir[i].E());
        m[i] = nir[i].M();
    }

    int* eDelta = new int[count];
    int* mDelta = new int[count];

    calculator.getDeltas(e, count, eDelta);
    calculator.getDeltas(m, count, mDelta);

    for (int j = 0; j < count; j++)
    {
        char s = nir[j].S();
        this->signWriter.write(s);
        this->exponentWriter.write(eDelta[j]);
        this->mantissaWriter.write(mDelta[j]);
    }

    delete[] e;
    delete[] m;

    delete[] eDelta;
    delete[] mDelta;
}

unsigned char * VerticalFloat::allocate(int * count)
{
    //v1.insert( v1.end(), v2.begin(), v2.end() );

    std::vector<unsigned char> noncompressed;

    std::vector<unsigned char> signs = signWriter.allocate();
    noncompressed.insert(noncompressed.end(),
        signs.begin(), signs.end());

    std::vector<unsigned char> exponents = exponentWriter.allocate();
    noncompressed.insert(noncompressed.end(),
        exponents.begin(), exponents.end());

    std::vector<unsigned char> mantissas = mantissaWriter.allocate();
    noncompressed.insert(noncompressed.end(),
        mantissas.begin(), mantissas.end());

    int sizeInChars = noncompressed.size();

	/*
    ZlibWrapper zip;

    int nLenDst = zip.GetMaxCompressedLen(sizeInChars);
    BYTE* buffer = new BYTE[nLenDst];  // alloc dest buffer

    int nLenPacked = zip.CompressData((const BYTE*)noncompressed.data(), sizeInChars, buffer, nLenDst);
    if (nLenPacked == -1) return nullptr;

    *count = nLenPacked;
    return buffer;
	*/

    
    *count = sizeInChars;
    BYTE* result = new BYTE[sizeInChars];
    memcpy(result, noncompressed.data(), sizeInChars);
    return result;
    
}

_float * VerticalFloat::decompress(unsigned char * data, int dataSize, int pointsCount)
{
    int blockSizeInt = (pointsCount % 8 == 0) ? (pointsCount / 8) : (pointsCount / 8 + 1);
    int targetSize = (1 + 8 + 23) /* * 4*/ * blockSizeInt; // blockSize in ints
    
	/*
	BYTE* unpacked = new BYTE[targetSize];

    ZlibWrapper zip;
    int nLen = zip.UncompressData(data, dataSize, unpacked, targetSize);

    BYTE* current = unpacked;
	*/

	BYTE* current = data;

    int signBlockSize = 1 * blockSizeInt/* * 4*/;
    char* signs = signWriter.read(current, signBlockSize, pointsCount);
    current += signBlockSize;

    int eSize = 8 * blockSizeInt/* * 4*/; // 8 - bits per 1 exponent
    int* eDeltas = exponentWriter.read(current, eSize, pointsCount);
    current += eSize;

    int mSize = 23 * blockSizeInt/* * 4*/;
    int* mDeltas = mantissaWriter.read(current, mSize, pointsCount);

    //delete[] unpacked;

    int* e = new int[pointsCount]; // resulting exponent
    int* m = new int[pointsCount]; // resulting mantissa

    DeltasCalculator<int, DELTA_ORDER> calculator;
    calculator.getOriginal(eDeltas, pointsCount, e);
    calculator.getOriginal(mDeltas, pointsCount, m);

    _float* result = new _float[pointsCount];
    for (size_t i = 0; i < pointsCount; i++)
    {
        int blockNum = i / 8;
        int bitNum = i % 8;
        int shift = 8 - bitNum - 1; // start reading from left to right

        char s = ((signs[blockNum] & (1 << shift)) >> shift);

        result[i].FromSEM(s, e[i], m[i]);
    }

    delete[] signs;
    delete[] eDeltas;
    delete[] mDeltas;
    delete[] e;
    delete[] m;

    return result;
}