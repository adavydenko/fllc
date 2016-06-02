#pragma once

template <class T, int Order = 1>
class DeltasCalculator
{
public:
    void getDeltas(T* originalArray, int count, T* targetBuffer);
    void getOriginal(T* deltasArray, int count, T* targetBuffer);
};

