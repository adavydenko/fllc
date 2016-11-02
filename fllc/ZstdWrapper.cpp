#include "stdafx.h"
#include "ZstdWrapper.h"

size_t ZstdWrapper::GetMaxCompressedLen(size_t dataLen)
{
    size_t estimatedSize = ZSTD_compressBound(dataLen);
    return estimatedSize;
}

size_t ZstdWrapper::CompressData(const unsigned char * dataToCompress, size_t dataLen, unsigned char * buffer, size_t bufferLen, int ratio = COMPRESSION_RATIO)
{
    size_t actualSize = ZSTD_compress(
        buffer, bufferLen,
        dataToCompress, dataLen, ratio);

    return actualSize;
}

size_t ZstdWrapper::UncompressData(const unsigned char * dataToDecompress, size_t dataLen, unsigned char * buffer, size_t bufferLen)
{
    size_t actualSize = ZSTD_decompress(
        buffer, bufferLen,
        dataToDecompress, dataLen);

    return actualSize;
}
