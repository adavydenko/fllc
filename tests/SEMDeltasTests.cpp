#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\fllc\structs.h"
#include "..\fllc\SEMDeltas.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
    TEST_CLASS(SEMDeltasTests)
    {
    public:
        TEST_METHOD(BasicTest)
        {
            // TODO: Your test code here

            int x[5] = { 100, 200, 250, 260, 405 };

            SEMDeltas sd;
            sd.compress((_float*)x, 5);

            int size;
            const unsigned char* zip = sd.allocate(&size);

            int* unpacked = (int*)sd.decompress(zip, size, 5);
            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(x[i], unpacked[i], L"Invalid decompressed value.", LINE_INFO());
            }

            delete[] zip;
            delete[] unpacked;
        }
    };
}