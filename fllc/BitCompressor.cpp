#include "stdafx.h"
#include "BitCompressor.h"

const unsigned char BitCompressor::ZerosOnesWriter::zeros = '0x00';
const unsigned char BitCompressor::ZerosOnesWriter::transitionsZerosOnes[8] = { zeros, 1, 3, 9, 15, 31, 63, 127 };
/*
00000000 = 0
00000001 = 1
00000011 = 3
00000111 = 9
00001111 = 15
00011111 = 31
00111111 = 63
01111111 = 127
*/

const unsigned char BitCompressor::ZerosOnesWriter::ones = '0xFF';
const unsigned char BitCompressor::ZerosOnesWriter::transitionsOnesZeros[8] = { ones, 254, 252, 248, 224, 192, 128 };
/*
11111111 = 255
11111110 = 254
11111100 = 252
11111000 = 248
11110000 = 240
11100000 = 224
11000000 = 192
10000000 = 128
*/

BitCompressor::Writer * BitCompressor::getWriter(const unsigned char * data, size_t length)
{
    BitCompressor::Writer * plain = new BitCompressor::ZerosOnesWriter();
    if (plain->canWrite(*data))
    {
        return plain;
    }
    delete plain;

    return new BitCompressor::MixedWriter();
}

void BitCompressor::write(const unsigned char * data, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        if (!currentWriter)
        {
            currentWriter = getWriter(&data[i], length - i);
        }

        if (!currentWriter->canWrite(data[i]))
        {
            currentWriter->flush(data[i]);
            writers.push(currentWriter);

            currentWriter = getWriter(&data[i], length - i);
        }

        currentWriter->write(data[i]);
    }

    if (writers.back() != currentWriter)
    {
        writers.push(currentWriter);
    }
}

size_t BitCompressor::bufferSize()
{
    return size_t();
}

void BitCompressor::allocate(unsigned char * buffer, size_t bufferSize)
{

}

bool BitCompressor::ZerosOnesWriter::canWrite(const unsigned char & data)
{
    for (size_t i = 0; i < 8; i++)
    {
        if (transitionsOnesZeros[i] == data || transitionsZerosOnes[i] == data)
        {
            return true;
        }
    }

    return false;
}

void BitCompressor::ZerosOnesWriter::write(const unsigned char & data)
{
    if (!canWrite(data)) //just to be on a safe side
    {
        return;
    }

    // if it i clear => create first entry
    if (dataToWrite.size() == 0)
    {

        // Possible cases:
        // 00000000
        // 11111111
        // 00111111
        // 11100000
    }
    else // we need to checl last character or switch 0->1, 1->0
    {
        BitCompressor::ZerosOnesWriter::BitInfo& lastBits = dataToWrite.back();
        if (lastBits.symbol == data) // ==0x00 or ==0xFF
        {
            lastBits.bits += 8;
            return;
        }

    }
}

void BitCompressor::ZerosOnesWriter::flush(const unsigned char & data)
{

    /*
      00011111 = 31
      00010010 = 18 -> here we should write 3 bits
      --------
   &: 00010010 > 0

    */

}



bool BitCompressor::MixedWriter::canWrite(const unsigned char & data)
{
    /* mixed writer can write anything */
    return true;
}

void BitCompressor::MixedWriter::write(const unsigned char & data)
{

}

void BitCompressor::MixedWriter::flush(const unsigned char & data)
{

}