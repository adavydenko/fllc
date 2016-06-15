#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\fllc\structs.h"
#include "..\fllc\SEMDeltas.cpp"
//#include "..\fllc\ZlibWrapper.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
    TEST_CLASS(SEMDeltasTests)
    {
    public:
        TEST_METHOD(BasicTest)
        {
            int x[5] = { 100, 200, 250, 260, 405 };

            SEMDeltas compressor;
            compressor.compress((_float*)x, 5);

            int size;
            const unsigned char* zip = compressor.allocate(&size);

            SEMDeltas decompressor;
            int* unpacked = (int*)decompressor.decompress(zip, size, 5);
            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(x[i], unpacked[i], L"Invalid decompressed value.", LINE_INFO());
            }

            delete[] zip;
        }

        TEST_METHOD(BasicTest4200)
        {
            // TODO: Your test code here
            int x[] =
            {
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
              100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405, 100, 200, 250, 260, 405,
            };

            SEMDeltas compressor;
            compressor.compress((_float*)x, 4200);

            int size;
            const unsigned char* zip = compressor.allocate(&size);

            SEMDeltas decompressor;
            int* unpacked = (int*)decompressor.decompress(zip, size, 4200);
            for (size_t i = 0; i < 4200; i++)
            {
                Assert::AreEqual(x[i], unpacked[i], L"Invalid decompressed value.", LINE_INFO());
            }

            delete[] zip;
        }
    };
}