// VerticalFloat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VerticalFloat.h"
#include "structs.h"
#include <vector>
#include "ZlibWrapper.h"

void VerticalFloat::compress(_float * nir, int count)
{
    int deltaE = 0;
    int ddeltaE = 0;
    int dddeltaE = 0;
    int deltaM = 0;
    int ddeltaM = 0;
    int dddeltaM = 0;

    for (int i = 0; i < count; i++)
    {
        int s = int(nir[i].S());
        int e = int(nir[i].E());
        int m = nir[i].M();

        if (i > 0)
        {
            deltaE = e - (int)nir[i - 1].E();
            deltaM = m - nir[i - 1].M();
        }

        if (i > 1)
        {
            ddeltaE = deltaE - ((int)nir[i - 1].E() - (int)nir[i - 2].E());
            ddeltaM = deltaM - (nir[i - 1].M() - nir[i - 2].M());
        }

        if (i > 2)
        {
            int ddeltaE_1 = deltaE - ddeltaE - ((int)nir[i - 2].E() - (int)nir[i - 3].E());
            int ddeltaM_1 = deltaM - ddeltaM - (nir[i - 2].M() - nir[i - 3].M());

            dddeltaE = ddeltaE - ddeltaE_1;
            dddeltaM = ddeltaM - ddeltaM_1;

            this->sBits.add(s > 0);

            for (size_t i = 0; i < 8; i++)
            {
                this->eBits[i].add(dddeltaE, i);
            }

            for (size_t i = 0; i < 24; i++)
            {
                this->mBits[i].add(dddeltaM, i);
            }
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

    for (size_t i = 0; i < 8; i++)
    {
        this->eBits[i].flush();
        noncompressed.insert(noncompressed.end(),
            this->eBits[i].block.begin(), this->eBits[i].block.end());
    }

    for (size_t i = 0; i < 24; i++)
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