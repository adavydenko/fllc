#include "stdafx.h"
#include "VerticalBitsWriter.h"

template<int BitsCount, class T>
void VerticalBitsWriter<BitsCount, T>::write(T & value)
{
}

template<int BitsCount, class T>
std::vector<unsigned char>& VerticalBitsWriter<BitsCount, T>::allocate()
{
    // TODO: insert return statement here
}

template<int BitsCount, class T>
T * VerticalBitsWriter<BitsCount, T>::read(unsigned char * input, int countT)
{
    return nullptr;
}
