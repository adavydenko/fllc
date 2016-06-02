#pragma once

#include "structs.h"

class SEMDeltas
{
public:
    void compress(_float* arr, int count);
    const char* allocate(int* size);
};

