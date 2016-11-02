#include "stdafx.h"
#include "CppUnitTest.h"
//#include "..\fllc\zstd.h"
#include "..\fllc\ZstdWrapper.h"
#include "..\fllc\ZstdWrapper.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
    TEST_CLASS(ZstdWrapperTests)
    {
        const unsigned char zero = '\x00';
        const unsigned char ones = '\xFF';

    public:
        TEST_METHOD(HelloWorldTest)
        {
            // TODO: Your test code here

            const char* hello = "Hello, world! This is an example of Zstd compression.";
            int len = strlen(hello);

            ZstdWrapper zstd;

            int maxSize = zstd.GetMaxCompressedLen(len);

            unsigned char* buffer = new unsigned char[maxSize];

            size_t compressedSize = zstd.CompressData((const unsigned char*)hello, len, buffer, maxSize);



            unsigned char* decompressed = new unsigned char[len];
            zstd.UncompressData(buffer, compressedSize, decompressed, len);

            Assert::IsTrue(strncmp(hello, (const char*)decompressed, len) == 0, L"Incorrect buffer size");

            delete[] buffer;
            delete[] decompressed;
        }
    };
}