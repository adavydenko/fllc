#include "stdafx.h"
#include "BitCompressor.h"

void BitCompressor::write(const unsigned char * data, size_t length)
{
}

size_t BitCompressor::bufferSize()
{
    return size_t();
}

void BitCompressor::allocate(unsigned char * buffer, size_t bufferSize)
{

}

void BitCompressor::ZerosWriter::flush(const unsigned char & data)
{
    /*
      00000000 = 0
      00000001 = 1
      00000011 = 3
      00000111 = 9
      00001111 = 15
      00011111 = 31
      00111111 = 63
      01111111 = 127
      11111111 = 255
    */

    /*
      00011111 = 31
      00010010 = 18 -> here we should write 3 bits
      --------
   &: 00010010 > 0

    */

}

void BitCompressor::ZerosWriter::write(const unsigned char & data)
{
    if (canWrite(data))
    {
        this->canWrite += 8;
    }
}

bool BitCompressor::ZerosWriter::canWrite(const unsigned char & data)
{
    return (data == zero);
}

void BitCompressor::OnesWriter::flush(const unsigned char & data)
{

}

void BitCompressor::OnesWriter::write(const unsigned char & data)
{

}

bool BitCompressor::OnesWriter::canWrite(const unsigned char & data)
{
    return false;
}

void BitCompressor::MixedWriter::flush(const unsigned char & data)
{

}

void BitCompressor::MixedWriter::write(const unsigned char & data)
{

}

bool BitCompressor::MixedWriter::canWrite(const unsigned char & data)
{
    return false;
}
