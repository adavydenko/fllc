#include "stdafx.h"
#include "BitCompressor.h"

const unsigned char BitCompressor::ZerosOnesWriter::zeros = 0x00;
const unsigned char BitCompressor::ZerosOnesWriter::transitionsZerosOnes[8] = { zeros, 1, 3, 7, 15, 31, 63, 127 };
/*
00000000 = 0
00000001 = 1
00000011 = 3
00000111 = 7
00001111 = 15
00011111 = 31
00111111 = 63
01111111 = 127
*/

const unsigned char BitCompressor::ZerosOnesWriter::ones = 0xFF;
const unsigned char BitCompressor::ZerosOnesWriter::transitionsOnesZeros[8] = { ones, 254, 252, 248, 240, 224, 192, 128 };
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
        else if (!currentWriter->canWrite(data[i]))
        {
            currentWriter->flush(data[i]);
            writers.push(currentWriter);

            currentWriter = getWriter(&data[i], length - i);
        }

        currentWriter->write(data[i]);
    }
}

void BitCompressor::flush()
{
    if (currentWriter)
    {
        writers.push(currentWriter);
    }

    currentWriter = nullptr;
}

size_t BitCompressor::bufferSize()
{
    size_t result(0);

    size_t size = writers.size();
    for (size_t i = 0; i < size; i++)
    {
        Writer* start = writers.front();
        writers.pop();

        result += start->bufferSize();
        writers.push(start);
    }

    if (currentWriter)
    {
        result += currentWriter->bufferSize();
    }

    return result;
}

void BitCompressor::allocate(unsigned char * buffer, size_t bufferSize)
{
    if (currentWriter)
    {
        writers.push(currentWriter);
        currentWriter = nullptr;
    }

    while (!writers.empty())
    {
        Writer* writer = writers.front();
        //writer->flush();

        size_t bytesWritten = writer->allocate(buffer, bufferSize);

        buffer += bytesWritten;
        bufferSize -= bytesWritten;

        writers.pop();
    }
}

int BitCompressor::ZerosOnesWriter::bufferSize()
{
    size_t bytes(0);

    for (std::vector<BitInfo>::iterator i = dataToWrite.begin(); i != dataToWrite.end(); i++)
    {
        int e = 1;
        while (i->bits >= powf(2, 6 * e))
        {
            e++;
        }

        bytes += e;
    }

    return bytes;
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

    // if it is empty => create first entry
    if (dataToWrite.size() == 0)
    {
        // Possible cases:
        // 00000000
        // 11111111
        // 00111111
        // 11100000

        for (size_t i = 0; i < 8; i++)
        {
            if (data == transitionsOnesZeros[i])
            {
                BitInfo bits;

                bits.symbol = &ones;
                bits.bits = 8 - i;

                dataToWrite.push_back(bits);

                if (i > 0)
                {
                    BitInfo mixedBit;
                    mixedBit.symbol = &zeros;
                    mixedBit.bits = i;

                    dataToWrite.push_back(mixedBit);
                }

                break;
            }

            if (data == transitionsZerosOnes[i])
            {
                BitInfo bits;

                bits.symbol = &zeros;
                bits.bits = 8 - i;

                dataToWrite.push_back(bits);

                if (i > 0)
                {
                    BitInfo mixedBit;
                    mixedBit.symbol = &ones;
                    mixedBit.bits = i;

                    dataToWrite.push_back(mixedBit);
                }

                break;
            }
        }

    }
    else // we need to check last character or switch 0->1, 1->0
    {
        BitCompressor::ZerosOnesWriter::BitInfo& lastBits = dataToWrite.back();
        if (*lastBits.symbol == data) // ==0x00 or ==0xFF
        {
            lastBits.bits += 8;
            return;
        }
        else
        {
            for (size_t i = 0; i < 8; i++)
            {
                if (data == transitionsOnesZeros[i])
                {
                    if (*lastBits.symbol == ones)
                    {
                        lastBits.bits += 8 - i;
                    }
                    else
                    {
                        BitInfo begin;
                        begin.symbol = &ones;
                        begin.bits = 8 - i;
                        dataToWrite.push_back(begin);
                    }

                    if (i > 0)
                    {
                        BitInfo ending;
                        ending.symbol = &zeros;
                        ending.bits = i;

                        dataToWrite.push_back(ending);
                    }

                    break;
                }

                if (data == transitionsZerosOnes[i])
                {
                    if (*lastBits.symbol == zeros)
                    {
                        lastBits.bits += 8 - i;
                    }
                    else
                    {
                        BitInfo begin;
                        begin.symbol = &zeros;
                        begin.bits = 8 - i;
                        dataToWrite.push_back(begin);
                    }

                    if (i > 0)
                    {
                        BitInfo ending;
                        ending.symbol = &ones;
                        ending.bits = i;

                        dataToWrite.push_back(ending);
                    }

                    break;
                }
            }
        }

    }
}

void BitCompressor::ZerosOnesWriter::flush(const unsigned char & data)
{
    this->write(data); // it does flush itself
    /*
      00011111 = 31
      00010010 = 18 -> here we should write 3 bits
      --------
   &: 00010010 > 0

    */

}

size_t BitCompressor::ZerosOnesWriter::allocate(unsigned char * buffer, size_t size)
{
    size_t totalBytes(0);

    for (std::vector<BitInfo>::iterator i = dataToWrite.begin(); i != dataToWrite.end(); i++)
    {
        int e = 0;
        while (i->bits >= powf(2, 6 * (e + 1))) //TODO: propagate to constants
        {
            e++;
        }

        unsigned char mask = (3 << 6); //11000000
        unsigned char pattern = *(i->symbol) & mask; // 00000000 or 11111111 -> 00.000000 or 11.000000

        while (e >= 0)
        {
            unsigned char portion = unsigned char(i->bits >> (6 * e));
            *buffer = ~mask & portion;
            *buffer |= pattern;

            buffer++;
            e--;
            totalBytes++;
        }

        //totalBytes += e;
    }

    return totalBytes;
}


int BitCompressor::MixedWriter::bufferSize()
{
    size_t bytes = dataToWrite.size();

    int e = 1;
    while (bytes >= powf(2, 6 * e))
    {
        e++;
    }

    return (bytes + e);
}

bool BitCompressor::MixedWriter::canWrite(const unsigned char & data)
{
    return !BitCompressor::ZerosOnesWriter().canWrite(data);
    /* mixed writer can
       write anything what cannot plain writer

    return true;*/
}

void BitCompressor::MixedWriter::write(const unsigned char & data)
{
    this->dataToWrite.push_back(data);
}

void BitCompressor::MixedWriter::flush(const unsigned char & data)
{
    // don't flush, becuase no need to write bytes by portions
}

size_t BitCompressor::MixedWriter::allocate(unsigned char * buffer, size_t size)
{
    size_t totalBytes(0);
    size_t myBytes = dataToWrite.size();

    int e = 0;
    while (myBytes >= powf(2, 6 * (e + 1))) //TODO: propagate to constants
    {
        e++;
    }

    unsigned char mask = '\x3F'; //00111111
    unsigned char pattern = (1 << 6); // 01.000000

    while (e >= 0)
    {
        unsigned char portion = unsigned char((int)myBytes >> (6 * e));
        *buffer = mask & portion;
        *buffer |= pattern;

        buffer++;
        e--;
        totalBytes++;
    }


    for (std::vector<unsigned char>::iterator i = dataToWrite.begin(); i != dataToWrite.end(); i++)
    {
        *buffer = *i;
        buffer++;
        totalBytes++;
    }

    return totalBytes;
}
