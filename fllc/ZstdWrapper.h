#pragma once

#define COMPRESSION_RATIO 5

#include "zstd.h"

class ZstdWrapper
{
public:
    size_t GetMaxCompressedLen(size_t dataLen);
    size_t CompressData(const unsigned char* dataToCompress, size_t dataLen, unsigned char* buffer, size_t bufferLen, int ratio);
    size_t UncompressData(const unsigned char* dataToDecompress, size_t dataLen, unsigned char* buffer, size_t bufferLen);
};

