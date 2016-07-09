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
            Assert::AreEqual(1, (int)zip.bufferSize(), L"Incorrect buffer size");

            unsigned char result;
            zip.allocate(&result, 1);

            // Totally 8 zeros was written,
            // result = [00]001000
            Assert::AreEqual('\x08', (char)result, L"Incorrect resulting data");

        }
    };
}