#pragma once
#include "structs.h"

extern "C" {
  __declspec(dllexport) unsigned int* __cdecl compressEF(_float* nir, int count, int* compressedSize);
  __declspec(dllexport) _float* __cdecl decompressEF(unsigned int* zip, int count);

  __declspec(dllexport) unsigned char* __cdecl compressSEM(_float* nir, int count, int* compressedSize);
  __declspec(dllexport) int* __cdecl decompressSEM(const unsigned char * zip, int size, int count);

  __declspec(dllexport) unsigned char* __cdecl compressVF(_float* nir, int count, int* compressedSize);
  __declspec(dllexport) unsigned char* __cdecl decompressVF(unsigned char * zip, int size, int count);

  __declspec(dllexport) unsigned char* __cdecl zip(_float* nir, int count, int* compressedSize);
  __declspec(dllexport) unsigned char* __cdecl unzip(unsigned char * zip, int size, int count);
}
