#pragma once

//#define ZLIB_WINAPI   // actually actually needed (for linkage)

#include "windows.h"  // get BYTE et al.
#include "zlib.h"     // declare the external fns -- uses zconf.h, too

//#pragma comment(lib, "zlibwapi.lib") // for access to the DLL


// see https://www.experts-exchange.com/articles/3189/In-Memory-Compression-and-Decompression-Using-ZLIB.html
class ZlibWrapper
{
public:

    int GetMaxCompressedLen(int nLenSrc);
    int CompressData(const BYTE* abSrc, int nLenSrc, BYTE* abDst, int nLenDst);
    int UncompressData(const BYTE* abSrc, int nLenSrc, BYTE* abDst, int nLenDst);
};

/*

int main()
{
BYTE pbSrc[]="hello hello hello hello there";

//-------------- compress (save the original length)

int nLenOrig= strlen( (char*)pbSrc )+1; // include terminating NULL
int nLenDst= GetMaxCompressedLen( nLenOrig );
BYTE* pbDst= new BYTE [nLenDst];  // alloc dest buffer

int nLenPacked= CompressData( pbSrc, nLenOrig, pbDst, nLenDst );
if ( nLenPacked == -1 ) return(1);  // error

//-------------- uncompress (uses the saved original length)

BYTE* pbPacked=   pbDst;
BYTE* pbUnpacked= new BYTE[ nLenOrig ];

int nLen= UncompressData( pbPacked, nLenPacked, pbUnpacked, nLenOrig );

// breakpoint here and view pbUnpacked to confirm
delete pbDst;            // do some cleanup
delete pbUnpacked;
return 0;
}

*/
