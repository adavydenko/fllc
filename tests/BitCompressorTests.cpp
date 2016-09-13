#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\fllc\BitCompressor.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
    TEST_CLASS(BitCompressorTests)
    {
        const unsigned char zero = '\x00';
        const unsigned char ones = '\xFF';

    public:
        TEST_METHOD(ZerosWriting1)
        {
            // TODO: Your test code here
            BitCompressor zip;

            zip.write(&zero, 1);
            Assert::AreEqual(1, (int)zip.bufferSize(), L"Incorrect buffer size");

            zip.write(&zero, 1);
            zip.write(&zero, 1);
            Assert::AreEqual(1, (int)zip.bufferSize(), L"Incorrect buffer size");

            unsigned char data[] = { zero, zero, zero, zero, zero };
            zip.write(data, 5);
            Assert::AreEqual(2, (int)zip.bufferSize(), L"Incorrect buffer size");

            unsigned char result[2];
            zip.allocate(result, 2);

            // Totally 8 zero bytes were written => 64 bits,
            // result = [00]000001 + [00]000000 ( = 000001.000000 = 64)
            Assert::AreEqual('\x01', (char)result[0], L"Incorrect resulting data");
            Assert::AreEqual('\x00', (char)result[1], L"Incorrect resulting data");

        }

        TEST_METHOD(ZerosWriting2)
        {
            // TODO: Your test code here
            BitCompressor zip;

            for (size_t i = 1; i <= 65; i++)
            {
                zip.write(&zero, 1);
            }

            Assert::AreEqual(2, (int)zip.bufferSize(), L"Incorrect buffer size");

            unsigned char * result = new unsigned char[2];
            zip.allocate(result, 2);

            // Totally 520 zeros were written,
            // result = [00]001000 [00]001000
            Assert::AreEqual('\x08', (char)result[0], L"Incorrect resulting data");
            Assert::AreEqual('\x08', (char)result[1], L"Incorrect resulting data");

            delete[] result;

        }

        TEST_METHOD(OnesWriting)
        {
            // TODO: Your test code here
            BitCompressor zip;

            for (size_t i = 1; i <= 10; i++)
            {
                zip.write(&ones, 1);
            }

            Assert::AreEqual(2, (int)zip.bufferSize(), L"Incorrect buffer size");

            unsigned char * result = new unsigned char[2];
            zip.allocate(result, 2);

            // Totally 80 one-bits were written,
            // result = [11]000001 [11]010000
            Assert::AreEqual('\xC1', (char)result[0], L"Incorrect resulting data");
            Assert::AreEqual('\xD0', (char)result[1], L"Incorrect resulting data");

            delete[] result;
        }

        TEST_METHOD(ZeroOnesWriting1)
        {
            const unsigned char fiveZerosThreeOnes = '\x07';

            // TODO: 00000000 00000000 00000000 00000111 11111111 11111111
            BitCompressor zip;
            zip.write(&zero, 1);
            zip.write(&zero, 1);
            zip.write(&zero, 1);
            zip.write(&fiveZerosThreeOnes, 1); // = 00000111
            zip.write(&ones, 1);
            zip.write(&ones, 1);

            Assert::AreEqual(2, (int)zip.bufferSize(), L"Incorrect buffer size");

            unsigned char result[2];
            zip.allocate(result, 2);

            // Totally 29 zero-bits & 19 one-bits were written,
            // result = [00]011101 [11]010011
            Assert::AreEqual('\x1D', (char)result[0], L"Incorrect resulting data");
            Assert::AreEqual('\xD3', (char)result[1], L"Incorrect resulting data");

        }

        TEST_METHOD(MixedWriting1)
        {
            const unsigned char fourMixed = '\x06'; // 00000110

            // TODO: 00000110 00000110 00000110
            BitCompressor zip;
            zip.write(&fourMixed, 1);
            zip.write(&fourMixed, 1);

            Assert::AreEqual(3, (int)zip.bufferSize(), L"Incorrect buffer size");

            unsigned char result[3];
            zip.allocate(result, 3);

            // Totally 2 plain bytes were saved + 1 byte for size,
            // result = [01]000010 00000110 00000110
            Assert::AreEqual('\x42', (char)result[0], L"Incorrect resulting data");
            Assert::AreEqual('\x06', (char)result[1], L"Incorrect resulting data");
            Assert::AreEqual('\x06', (char)result[2], L"Incorrect resulting data");

        }

        TEST_METHOD(MixedWritingSizeTest)
        {
            const unsigned char four = '\x06'; // 00000110
            const unsigned char ff = '\x55';   // 01010101

            BitCompressor zip;
            for (size_t i = 0; i < 32; i++)
            {
                zip.write(&four, 1);
            }

            for (size_t i = 0; i < 31; i++)
            {
                zip.write(&ff, 1);
            }

            Assert::AreEqual((1 + 32 + 31), (int)zip.bufferSize(), L"Incorrect buffer size");

            zip.write(&ff, 1);
            Assert::AreEqual((2 + 32 + 31 + 1), (int)zip.bufferSize(), L"Incorrect buffer size");

            unsigned char result[66];
            zip.allocate(result, 66);

            // Totally 64 plain bytes were saved,
            // result = [01]000001 [01]000000 ..
            Assert::AreEqual('\x41', (char)result[0], L"Incorrect resulting data");
            Assert::AreEqual('\x40', (char)result[1], L"Incorrect resulting data");

            for (size_t i = 2; i < 32 + 2; i++)
            {
                Assert::AreEqual(four, (unsigned char)result[i], L"Incorrect resulting data");
            }

            for (size_t i = 34; i < 64; i++)
            {
                Assert::AreEqual(ff, (unsigned char)result[i], L"Incorrect resulting data");
            }
        }

        TEST_METHOD(PlainMixed2xWritingTest)
        {
            //zeros
            //ones
            const unsigned char transition = '\x07'; // 00000111
            const unsigned char ff = '\x55';		 // 01010101

            //Wire: 00000000 00000111 11111111 01010101 11111111 11111111 00000111 01010101

            BitCompressor zip;

            zip.write(&zero, 1);
            Assert::AreEqual(1, (int)zip.bufferSize(), L"Incorrect buffer size", LINE_INFO());

            zip.write(&transition, 1);
            Assert::AreEqual(2, (int)zip.bufferSize(), L"Incorrect buffer size", LINE_INFO());

            zip.write(&ones, 1);
            Assert::AreEqual(2, (int)zip.bufferSize(), L"Incorrect buffer size", LINE_INFO());

            zip.write(&ff, 1);
            Assert::AreEqual(4, (int)zip.bufferSize(), L"Incorrect buffer size", LINE_INFO());

            zip.write(&ones, 1);
            Assert::AreEqual(5, (int)zip.bufferSize(), L"Incorrect buffer size", LINE_INFO());

            zip.write(&ones, 1);
            Assert::AreEqual(5, (int)zip.bufferSize(), L"Incorrect buffer size", LINE_INFO());

            zip.write(&transition, 1);
            Assert::AreEqual(7, (int)zip.bufferSize(), L"Incorrect buffer size", LINE_INFO());

            zip.write(&ff, 1);
            Assert::AreEqual(9, (int)zip.bufferSize(), L"Incorrect buffer size", LINE_INFO());

            unsigned char result[9];
            zip.allocate(result, 9);

            //Wire: 00000000 00000111 11111111 01010101 11111111 11111111 00000111 01010101
            /*Zip : [00]001101 [11]001011 [01]000001 01010101 [11]010000 [00]000101 [11]000011 [01]000001 01010101
                     13 of 0s   11 of 1s   1 mixed    raw      16 of 1s   5 of 0s    3 of 1s    1 mixed    raw
            */

            Assert::AreEqual('\x0D', (char)result[0], L"Incorrect resulting data [0]");
            Assert::AreEqual('\xCB', (char)result[1], L"Incorrect resulting data [1]");
            Assert::AreEqual('\x41', (char)result[2], L"Incorrect resulting data [2]");
            Assert::AreEqual((char)ff, (char)result[3], L"Incorrect resulting data [3]");
            Assert::AreEqual('\xD0', (char)result[4], L"Incorrect resulting data [4]");
            Assert::AreEqual('\x05', (char)result[5], L"Incorrect resulting data [5]");
            Assert::AreEqual('\xC3', (char)result[6], L"Incorrect resulting data [6]");
            Assert::AreEqual('\x41', (char)result[7], L"Incorrect resulting data [7]");
            Assert::AreEqual((char)ff, (char)result[8], L"Incorrect resulting data [8]");
        }
    };
}