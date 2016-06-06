#include "stdafx.h"
#include "ZlibWrapper.h"


int ZlibWrapper::GetMaxCompressedLen(int nLenSrc)
{
    int n16kBlocks = (nLenSrc + 16383) / 16384; // round up any fraction of a block
    return (nLenSrc + 6 + (n16kBlocks * 5));
}

int ZlibWrapper::CompressData(const BYTE * abSrc, int nLenSrc, BYTE * abDst, int nLenDst)
{
    z_stream zInfo = { 0 };
    zInfo.total_in = zInfo.avail_in = nLenSrc;
    zInfo.total_out = zInfo.avail_out = nLenDst;
    zInfo.next_in = (BYTE*)abSrc;
    zInfo.next_out = abDst;

    int nErr, nRet = -1;
    nErr = deflateInit(&zInfo, Z_BEST_COMPRESSION/*Z_DEFAULT_COMPRESSION*/); // zlib function
    if (nErr == Z_OK) {
        nErr = deflate(&zInfo, Z_FINISH);              // zlib function
        if (nErr == Z_STREAM_END) {
            nRet = zInfo.total_out;
        }
    }
    deflateEnd(&zInfo);    // zlib function
    return(nRet);
}

int ZlibWrapper::UncompressData(const BYTE * abSrc, int nLenSrc, BYTE * abDst, int nLenDst)
{
    z_stream zInfo = { 0 };
    zInfo.total_in = zInfo.avail_in = nLenSrc;
    zInfo.total_out = zInfo.avail_out = nLenDst;
    zInfo.next_in = (BYTE*)abSrc;
    zInfo.next_out = abDst;

    int nErr, nRet = -1;
    nErr = inflateInit(&zInfo);               // zlib function
    if (nErr == Z_OK) {
        nErr = inflate(&zInfo, Z_FINISH);     // zlib function
        if (nErr == Z_STREAM_END) {
            nRet = zInfo.total_out;
        }
    }
    inflateEnd(&zInfo);   // zlib function
    return(nRet); // -1 or len of output
}
