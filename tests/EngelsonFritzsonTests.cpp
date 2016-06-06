#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\fllc\EngelsonFritzson.cpp"
#include "..\fllc\structs.h"

#define PI 3.14159265

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
    TEST_CLASS(EngelsonFritzsonTests)
    {
    public:
        TEST_METHOD(BasicTest)
        {
            // TODO: Your test code here

            int x[5] = { 100, 200, 250, 260, 405 };

            EngelsonFritzson ef;
            ef.compress((_float*)x, 5);

            int size;
            unsigned int* zip = ef.allocate(&size);

            int* unpacked = (int*)ef.decompress(zip, 5);
            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(x[i], unpacked[i], L"Invalid decompressed value.", LINE_INFO());
            }

            delete[] zip;
            delete[] unpacked;
        }

        TEST_METHOD(Basic2Test)
        {
            // TODO: Your test code here

            int x[9] = { 777, 788, 798, 809, 819, 829, 838, 848, 857 };

            EngelsonFritzson ef;
            ef.compress((_float*)x, 9);

            int size;
            unsigned int* zip = ef.allocate(&size);

            int* unpacked = (int*)ef.decompress(zip, 9);
            for (size_t i = 0; i < 9; i++)
            {
                Assert::AreEqual(x[i], unpacked[i], L"Invalid decompressed value.", LINE_INFO());
            }

            delete[] zip;
            delete[] unpacked;

        }

        TEST_METHOD(Basic3Test)
        {
            // TODO: Your test code here

            int x[16] = { 406, 422, 438, 453, 469, 484, 499, 515, 529, 544, 559, 573, 587, 601, 615, 629 };

            EngelsonFritzson ef;
            ef.compress((_float*)x, 16);

            int size;
            unsigned int* zip = ef.allocate(&size);

            int* unpacked = (int*)ef.decompress(zip, 16);
            for (size_t i = 0; i < 16; i++)
            {
                Assert::AreEqual(x[i], unpacked[i], L"Invalid decompressed value.", LINE_INFO());
            }

            delete[] zip;
            delete[] unpacked;

        }

        TEST_METHOD(SinOrderTest)
        {
            const int SIZE = 361;
            //TODO: it is not good to keep float deltas, as it cannot be reconstructed losslessly, so ints are used
            int values[SIZE];

            for (size_t degree = 0; degree < SIZE; degree++)
            {
                values[degree] = (int)(1000 * sin(degree*PI / 180));
            }

            EngelsonFritzson ef;

            ef.compress((_float*)values, SIZE);

            int bytesAllocated;
            unsigned int* zip = ef.allocate(&bytesAllocated);

            int* restored = (int*)ef.decompress(zip, SIZE);

            for (size_t i = 0; i < SIZE; i++)
            {
                Assert::AreEqual(values[i], restored[i], L"Invalid restored value.", LINE_INFO());
            }
        }

    };
}