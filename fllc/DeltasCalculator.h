#pragma once

#define DELTA_ORDER 10

template <class T, int Order = DELTA_ORDER>
class DeltasCalculator
{
public:
    void getDeltas(T* originalArray, int count, T* targetBuffer);
    void getOriginal(T* deltasArray, int count, T* targetBuffer);
};

