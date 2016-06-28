// VerticalFloat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DeltasCalculator.h"
#include "VerticalFloat.h"
#include "structs.h"
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

        this->sBits.add(s);

        for (size_t i = 0; i < 8; i++)
        {
            this->eBits[i].add(eDelta[j], i);
        }

        for (size_t i = 0; i < 24; i++)
        {
            this->mBits[i].add(mDelta[j], i);
        }
    }
}

unsigned char * VerticalFloat::allocate(int * count)
{
    //v1.insert( v1.end(), v2.begin(), v2.end() );

    std::vector<unsigned int> noncompressed;

    this->sBits.flush();
    noncompressed.insert(noncompressed.end(),
        this->sBits.block.begin(), this->sBits.block.end());

    for (int i = 7; i >= 0; i--)
    {
        this->eBits[i].flush();
        noncompressed.insert(noncompressed.end(),
            this->eBits[i].block.begin(), this->eBits[i].block.end());
    }

    for (int i = 23; i >= 0; i--)
    {
        this->mBits[i].flush();
        noncompressed.insert(noncompressed.end(),
            this->mBits[i].block.begin(), this->mBits[i].block.end());
    }

    int sizeInChars = noncompressed.size() * 4;

    ZlibWrapper zip;

    int nLenDst = zip.GetMaxCompressedLen(sizeInChars);
    BYTE* buffer = new BYTE[nLenDst];  // alloc dest buffer

    int nLenPacked = zip.CompressData((const BYTE*)noncompressed.data(), sizeInChars, buffer, nLenDst);
    if (nLenPacked == -1) return nullptr;

    *count = nLenPacked;
    return buffer;
}

_float * VerticalFloat::decompress(unsigned char * data, int dataSize, int pointsCount)
{
    int blockSize = (pointsCount % 32 == 0) ? (pointsCount / 32) : (pointsCount / 32 + 1);
    int targetSize = (1 + 8 + 24) * 4 * blockSize; // blockSize in ints
    BYTE* unpacked = new BYTE[targetSize];

    ZlibWrapper zip;
    int nLen = zip.UncompressData(data, dataSize, unpacked, targetSize);

    int* unpackedBits = (int*)unpacked;
    int size = targetSize / 4; // char -> int

    int* signBlock = unpackedBits;

    int* eBlock = (unpackedBits + blockSize);
    int eSize = blockSize * 8; // 8 - bits per 1 exponent
    int* eDeltas = new int[pointsCount];

    int* mBlock = (eBlock + eSize);
    int mSize = blockSize * 24;
    int* mDeltas = new int[pointsCount];

    for (size_t i = 0; i < pointsCount; i++)
    {
        int blockNum = i / 32;
        int bitNum = i % 32;

        int eValue = 0;
        int mValue = 0;

        for (size_t e = 0; e < 8; e++)
        {
            unsigned int currentBlock = eBlock[e*blockSize + blockNum];
            currentBlock &= (1 << (32 - bitNum - 1));
            currentBlock >>= (32 - bitNum - 1);
            currentBlock <<= e;

            eValue |= currentBlock;
        }

        for (size_t m = 0; m < 24; m++)
        {
            unsigned int currentBlock = eBlock[m*blockSize + blockNum];
            currentBlock &= (1 << (32 - bitNum - 1));
            currentBlock >>= (32 - bitNum - 1);
            currentBlock <<= m;

            mValue |= currentBlock;
        }

        eDeltas[i] = eValue;
        mDeltas[i] = mValue;
    }

    delete[] unpacked;

    int* e = new int[pointsCount]; // resulting exponent
    int* m = new int[pointsCount]; // resulting mantissa

    DeltasCalculator<int, DELTA_ORDER> calculator;
    calculator.getOriginal(eDeltas, pointsCount, e);
    calculator.getOriginal(mDeltas, pointsCount, m);

    delete[] eDeltas;
    delete[] mDeltas;

    _float* result = new _float[pointsCount];
    for (size_t i = 0; i < pointsCount; i++)
    {
        int blockNum = i / 32;
        int bitNum = i % 32;
        int shift = 32 - bitNum - 1; // start reading from left to right

        char s = ((signBlock[blockNum] & (1 << shift)) >> shift);

        result[i].FromSEM(s, e[i], m[i]);
    }
    delete[] e;
    delete[] m;

    return result;
}

/*

void saveVerticalBits(std::ofstream &out, VerticalBits &bits)
{
int totalBlocks = 0;
unsigned int* data = bits.allocate(&totalBlocks);

out.write((const char*)data, totalBlocks * 4); //1 bytes=chars per float sign
//delete data;
}

void saveCompressed(const char* sourceFile, VerticalFloat &data, const char* extension)
{
char binaryFile[200];
strncpy_s(binaryFile, strlen(sourceFile) + 1, sourceFile, _TRUNCATE);
strncat_s(binaryFile, sizeof binaryFile, extension, _TRUNCATE);

std::cout << "Writing " << binaryFile << std::endl;

std::ofstream compressed(binaryFile, std::ofstream::binary);

saveVerticalBits(compressed, data.sBits);

for (size_t i = 0; i < 8; i++)
{
saveVerticalBits(compressed, data.eBits[i]);
}

for (size_t i = 0; i < 24; i++)
{
saveVerticalBits(compressed, data.mBits[i]);
}
compressed.close();
}

*/