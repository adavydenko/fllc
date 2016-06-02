#include "stdafx.h"
#include "DeltasCalculator.h"

template<class T, int Order>
inline void DeltasCalculator<T, Order>::getDeltas(T * originalArray, int count, T * targetBuffer)
{
    T deltas[Order]; //Order=0 => the original value itself

    for (size_t i = 0; i < count; i++)
    {
        targetBuffer[i] = originalArray[i];
        for (size_t k = 0; (k < Order) && (k < i); k++) //targetBuffer[i] = originalArray[i] - lowOrderSum;
        {
            targetBuffer[i] -= deltas[k];
        }

        // update deltas for further steps
        T x = originalArray[i];
        for (size_t l = 0; (l < Order) && (l <= i); l++)
        {
            T newDelta = x - deltas[l];
            deltas[l] = x;
            x = newDelta;
        }
    }
}

template<class T, int Order>
inline void DeltasCalculator<T, Order>::getOriginal(T * deltasArray, int count, T * targetBuffer)
{
    T restored[Order]; //Order=0 => the original value itself
    T deltas[Order];

    for (size_t i = 0; i < count; i++)
    {
        targetBuffer[i] = deltasArray[i];
        for (size_t k = 0; (k < Order) && (k < i); k++) //targetBuffer[i] = deltasArray[i] - lowOrderSum;
        {
            targetBuffer[i] += restored[k];
        }

        // update deltas for further steps
        T x = targetBuffer[i];
        for (size_t l = 0; (l < Order) && (l <= i); l++)
        {
            T newDelta = x - deltas[l];
            deltas[l] = x;
            x = newDelta;
        }

        for (size_t l = 0; (l < Order); l++)
        {
            restored[l] = deltas[l];
        }
    }
}
