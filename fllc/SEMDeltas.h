#pragma once

#include "structs.h"
#include <vector>

class SEMDeltas
{
    unsigned char* ssign;
    unsigned char* sexp;
    unsigned char* smant;

    _int<2>* earr;
    _int<4>* marr;

    //unsigned char* allocated;
    int _count;

public:

    SEMDeltas();
    ~SEMDeltas();

    void compress(_float* arr, int count);
    const unsigned char* allocate(int* size);
    _float* decompress(const unsigned char* zip, int zipSize, int pointsCount);
};

