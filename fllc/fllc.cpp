// fllc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <vector>

#include "structs.h"
#include "EngelsonFritzson.h"
#include "DeltasCalculator.h"
#include "VerticalBits.h"
#include "VerticalFloat.h"
#include "SEMDeltas.h"
#include "fllc.h"

void printUsage(const char* cmd)
{
    printf("%s -method -c|-d -l <points count> -file <file name> [-save]\n", cmd);
    printf("-method          : defines compression algorithm, should be one of:\n", cmd);
    printf("                   -ef - EngelsonFritzson\n", cmd);
    printf("                   -sem - Separated Sign/Exponent/Mantissa\n", cmd);
    printf("                   -vb - Vartical Bits technique (=vf)\n", cmd);
    printf("-c (default), -d : defines whether file should be (c)ompressed of (d)ecompressed\n", cmd);
    printf("-l <points count>: specifies number of floats stored in the file\n", cmd);
    printf("-file <file name>: specifies file name with uncompressed (should contain text) or compressed data (should be binary)\n", cmd);
    printf("[-save]          : optional, specified whether output data should be saved on disk or not\n", cmd);

    printf("\nExample (of compression): fllc -ef -c -l 4200 -f \"da1650_nir.txt\" -save\n", cmd);
    printf("Example (of decompression): fllc -ef -d -l 4200 -f \"da1650_nir.compressed\" -save\n", cmd);
}

//TODO: preallocate buffer for results
_float* readFile(const char* fileName, int pointsCount)
{
    FILE *f;
    errno_t err = fopen_s(&f, fileName, "r");

    if (err)
    {
        return NULL;
    }

    _float* results = new _float[pointsCount];
    int i = 0;
    while (EOF != fscanf_s(f, "%f", &results[i].fvalue))
    {
        i++;
    }

    fclose(f);
    return results;
}

//TODO: preallocate buffer for results
std::vector<unsigned char>* readFile(const char* fileName)
{
    FILE *f;
    errno_t err = fopen_s(&f, fileName, "r");

    if (err)
    {
        return NULL;
    }

    std::vector<unsigned char>* results = new std::vector<unsigned char>();
    unsigned char buf;

    while ((buf = (unsigned char)fgetc(f)) && !feof(f))
    {
        results->push_back(*(new unsigned char(buf)));
    }

    fclose(f);
    return results;
}

//TODO: preallocate buffer for results
std::vector<char>* readEfFile(const char* fileName, int &count)
{
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char>* buffer = new std::vector<char>(size);
    if (file.read(buffer->data(), size))
    {
        return buffer;
    }

    count = size;

    return nullptr;
}

void PrintBinary(_float &f)
{
    for (int j = 0; j < 32; j++)
    {
        std::cout << f[31 - j];
        if (j == 0 || j == 8) std::cout << " ";
    }
}

void saveBinary(const char* sourceFile, _float* nir, int count)
{
    char binaryFile[200];
    strncpy_s(binaryFile, strlen(sourceFile) + 1, sourceFile, _TRUNCATE);
    strncat_s(binaryFile, sizeof binaryFile, ".binary", _TRUNCATE);

    std::ofstream binary(binaryFile, std::ofstream::binary);
    binary.write((const char*)nir, 4 * count); //4 bytes=chars per int

    binary.close();
}

void saveCompressed(const char* sourceFile, const unsigned char* data, int count, const char* extension)
{
    char binaryFile[256];
    strncpy_s(binaryFile, strlen(sourceFile) + 1, sourceFile, _TRUNCATE);
    strncat_s(binaryFile, sizeof binaryFile, extension, _TRUNCATE);

    std::cout << "Writing " << binaryFile << std::endl;

    std::ofstream compressed(binaryFile, std::ofstream::binary);
    compressed.write((const char*)data, count); //1 bytes=chars per float sign
    compressed.close();
}

int main(int argc, char* argv[])
{
    int pointsCount;
    char* sourceFile = NULL;
    bool saveResults = false;
    bool compress = true;

    bool useEF = false;
    bool useVB = false;
    bool useSEM = false;

    if (argc < 7)
    {
        printUsage(argv[0]);
        return 0;
    }

    for (int i = 1; i < argc; i++) { /* We will iterate over argv[] to get the parameters stored inside.
                                     * Note that we're starting on 1 because we don't need to know the
                                     * path of the program, which is stored in argv[0] */


                                     //if (i + 1 != argc) // Check that we haven't finished parsing already
                                     //{
        if (!strcmp(argv[i], "-c")) {
            compress = true;
        }
        else if (!strcmp(argv[i], "-d")) {
            compress = false;
        }
        else if (!strcmp(argv[i], "-ef")) {
            useEF = true;
        }
        else if (!strcmp(argv[i], "-vb")) {
            useVB = true;
        }
        else if (!strcmp(argv[i], "-sem")) {
            useSEM = true;
        }
        else if (!strcmp(argv[i], "-l")) {
            // We know the next argument *should* be the filename:
            pointsCount = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "-file")) {
            sourceFile = argv[++i];
        }
        else if (!strcmp(argv[i], "-save"))
        {
            saveResults = true;
        }
        else {
            std::cout << "Not enough or invalid arguments, please try again.\n";
            printUsage(argv[0]);
            return 0;
        }
    }

    if (compress)
    {
        _float* nir = readFile(sourceFile, pointsCount);

        if (saveResults)
        {
            saveBinary(sourceFile, nir, pointsCount);
        }

        if (useEF)
        {
            EngelsonFritzson zip;
            zip.compress(nir, pointsCount);

            int totalBlocks = 0;
            unsigned int* data = zip.allocate(&totalBlocks);

            saveCompressed(sourceFile, (unsigned char*)data, totalBlocks * 4, ".compressed.ef");
            delete data;
        }

        if (useSEM)
        {
            SEMDeltas sem;
            sem.compress(nir, pointsCount);

            int size(0);
            const unsigned char* data = sem.allocate(&size);

            saveCompressed(sourceFile, data, size, ".compressed.sem");
        }

        if (useVB)
        {
            VerticalFloat zip;
            zip.compress(nir, pointsCount);

            int size(0);
            unsigned char* data = zip.allocate(&size);
            saveCompressed(sourceFile, data, size, ".compressed.vf");

            delete[] data;
        }

        delete nir;
    }
    else //we are decompressing data
    {
        if (useEF)
        {
            int size;
            std::vector<char>* zip = readEfFile(sourceFile, size);

            EngelsonFritzson compressor;
            _float* results = compressor.decompress((unsigned int*)zip->data(), pointsCount);

            saveCompressed(sourceFile, (unsigned char*)results, pointsCount * 4, ".decompressed");

            delete zip;
            delete[] results;
        }

        if (useSEM)
        {
            int size;
            std::vector<char>* zip = readEfFile(sourceFile, size);

            VerticalFloat compressor;
            _float* results = compressor.decompress((unsigned char*)zip->data(), zip->size(), pointsCount);

            saveCompressed(sourceFile, (unsigned char*)results, pointsCount * 4, ".decompressed");

            delete zip;
            delete[] results;
        }

        if (useVB)
        {
            int size;
            std::vector<char>* zip = readEfFile(sourceFile, size);

            VerticalFloat compressor;
            _float* results = compressor.decompress((unsigned char*)zip->data(), size, pointsCount);

            saveCompressed(sourceFile, (unsigned char*)results, pointsCount * 4, ".decompressed");

            delete zip;
            delete[] results;
        }
    }

    return 0;
}

extern unsigned int* __cdecl compressEF(_float* nir, int count, int* compressedSize)
{
    EngelsonFritzson ef;
    ef.compress(nir, count);

    int size;
    unsigned int* zip = ef.allocate(&size);
    *compressedSize = size;

    return zip;
}

extern _float* __cdecl decompressEF(unsigned int* zip, int count)
{
    EngelsonFritzson compressor;
    _float* results = compressor.decompress(zip, count);

    return results;
}

extern unsigned char* __cdecl compressSEM(_float * nir, int count, int * compressedSize)
{
    SEMDeltas sem;
    sem.compress(nir, count);

    int size(0);
    unsigned char* data = (unsigned char*)sem.allocate(&size);
    *compressedSize = size;

    return (unsigned char*)data;
}

extern int* __cdecl decompressSEM(const unsigned char * zip, int size, int count)
{
    SEMDeltas sd;
    int* unpacked = (int*)sd.decompress(zip, size, count);

    return unpacked;
}

extern unsigned char * compressVF(_float * nir, int count, int * compressedSize)
{
    VerticalFloat zip;
    zip.compress(nir, count);

    int size(0);
    unsigned char* data = zip.allocate(&size);
    *compressedSize = size;

    return (unsigned char*)data;
}

extern unsigned char* decompressVF(unsigned char * zip, int size, int count)
{
    VerticalFloat vf;

    int* unpacked = (int*)vf.decompress(zip, size, count);

    return (unsigned char*)unpacked;
}