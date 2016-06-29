#include "stdafx.h"
#include "CppUnitTest.h"
#include <stdexcept>
#include "..\fllc\VerticalBitsWriter.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
    TEST_CLASS(VerticalBitsWriterTests)
    {
    public:
        TEST_METHOD(BasicEncodingTest)
        {
            // TODO: Your test code here

            char zero = '\x00'; // 00000000

            VerticalBitsWriter<8, char> writer;
            for (size_t i = 0; i < 8; i++)
            {
                writer.write(zero);
                /*

                VerticalBits are alligned to ints, so it would have a form:

                    1 2 3 4 5 6 7 8
                    _ _ _ _ _ _ _ _
                V1: 0 0 0 0 0 0 0 0 //Values
                V2: 0 0 0 0 0 0 0 0
                V3: 0 0 0 0 0 0 0 0
                V4: 0 0 0 0 0 0 0 0
                V5: 0 0 0 0 0 0 0 0
                V6: 0 0 0 0 0 0 0 0
                V7: 0 0 0 0 0 0 0 0
                V8: 0 0 0 0 0 0 0 0
                    . . . . . . . .
                V32 . . . . . . . .
                */
            }

            std::vector<unsigned int> result = writer.allocate();

            Assert::AreEqual(8, (int)result.size(), L"Invalid target buffer length.", LINE_INFO());
            Assert::AreEqual(0, (int)result[0], L"Zero is expected.", LINE_INFO());
            Assert::AreEqual(0, (int)result[1], L"Zero is expected.", LINE_INFO());
        }

        TEST_METHOD(BasicEncodingTest2)
        {
            // TODO: Your test code here

            char one = '\xFF'; // 11111111

            VerticalBitsWriter<8, char> writer;
            for (size_t i = 0; i < 32 /*number of values to write*/; i++)
            {
                writer.write(one);
                /*
                    1 2 3 4 5 6 7 8
                    _ _ _ _ _ _ _ _
                V1: 1 1 1 1 1 1 1 1 //Values
                V2: 1 1 1 1 1 1 1 1
                V3: 1 1 1 1 1 1 1 1
                V4: 1 1 1 1 1 1 1 1
                    . . . . . . . .
                V32 1 1 1 1 1 1 1 1
                */
            }

            std::vector<unsigned int> result = writer.allocate();

            Assert::AreEqual(8, (int)result.size(), L"Invalid target buffer length.", LINE_INFO());
            for (size_t i = 0; i < 8; i++)
            {
                /*
                -1: 11111111 11111111 11111111 11111111
                */
                Assert::AreEqual(-1, (int)result[i], L"Wrong value.", LINE_INFO());
            }
        }

        TEST_METHOD(BasicEncodingTest3)
        {
            // TODO: Your test code here

            char one = '\xFF'; // 11111111

            VerticalBitsWriter<8, char> writer;
            for (size_t i = 0; i < 4 /*number of values to write*/; i++)
            {
                writer.write(one);
                /*

                VerticalBits are alligned to ints, so it would have a form:

                    1 2 3 4 5 6 7 8
                    _ _ _ _ _ _ _ _
                V1: 1 1 1 1 1 1 1 1 //Values
                V2: 1 1 1 1 1 1 1 1
                V3: 1 1 1 1 1 1 1 1
                V4: 1 1 1 1 1 1 1 1
                    . . . . . . . .
                V32 0 0 0 0 0 0 0 0
                */
            }

            std::vector<unsigned int> result = writer.allocate();

            Assert::AreEqual(8, (int)result.size(), L"Invalid target buffer length.", LINE_INFO());
            for (size_t i = 0; i < 8; i++)
            {
                /*
                -268435456: 11110000 00000000 00000000 00000000
                */
                Assert::AreEqual(-268435456, (int)result[i], L"Wrong value.", LINE_INFO());
            }
        }

        TEST_METHOD(BasicDecodingTest)
        {
            // TODO: Your test code here

            char zero = '\x00'; // 00000000
            int source[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
            /*

            VerticalBits are alligned to ints, so it would have a form:

                1 2 3 4 5 6 7 8
                _ _ _ _ _ _ _ _
            V1: 0 0 0 0 0 0 0 0 //Values
            V2: 0 0 0 0 0 0 0 0
            V3: 0 0 0 0 0 0 0 0
            V4: 0 0 0 0 0 0 0 0
            V5: 0 0 0 0 0 0 0 0
            V6: 0 0 0 0 0 0 0 0
            V7: 0 0 0 0 0 0 0 0
            V8: 0 0 0 0 0 0 0 0
                . . . . . . . .
            V32 . . . . . . . .
            */

            VerticalBitsWriter<8, char> writer;
            char* decoded = writer.read((unsigned char*)source, 32 /* 8*4 chars in int*/, 5);

            for (size_t i = 0; i < 5; i++)
            {
                /*
                -268435456: 11110000 00000000 00000000 00000000
                */
                Assert::AreEqual(zero, decoded[i], L"Wrong decoded value.", LINE_INFO());
            }

            delete[] decoded;
        }

        TEST_METHOD(BasicDecodingTest2)
        {
            // TODO: Your test code here

            char one = '\xFF'; // 11111111
            int source[] = { -1, -1, -1, -1, -1, -1, -1, -1 }; //-1: 11111111 11111111 11111111 11111111
            /*
                1 2 3 4 5 6 7 8
                _ _ _ _ _ _ _ _
            V1: 1 1 1 1 1 1 1 1 //Values
            V2: 1 1 1 1 1 1 1 1
            V3: 1 1 1 1 1 1 1 1
            V4: 1 1 1 1 1 1 1 1
                . . . . . . . .
            V32 1 1 1 1 1 1 1 1
            */

            VerticalBitsWriter<8, char> writer;
            char* decoded = writer.read((unsigned char*)source, 32, 32);

            for (size_t i = 0; i < 32; i++)
            {
                Assert::AreEqual(one, decoded[i], L"Wrong decoded value.", LINE_INFO());
            }

            delete[] decoded;
        }

        TEST_METHOD(BasicDecodingTest3)
        {
            // TODO: Your test code here

            char one = '\xFF'; // 11111111
            char zero = '\x00'; // 00000000

            int source[] = { -268435456 , -268435456 , -268435456 , -268435456 ,
                             -268435456 , -268435456 , -268435456 , -268435456 }; //-268435456: 11110000 00000000 00000000 00000000
            /*

            VerticalBits are alligned to ints, so it would have a form:

                1 2 3 4 5 6 7 8
                _ _ _ _ _ _ _ _
            V1: 1 1 1 1 1 1 1 1 //Values
            V2: 1 1 1 1 1 1 1 1
            V3: 1 1 1 1 1 1 1 1
            V4: 1 1 1 1 1 1 1 1
            V5: 0 0 0 0 0 0 0 0
            V6: 0 0 0 0 0 0 0 0
                . . . . . . . .
            V32 0 0 0 0 0 0 0 0
            */

            VerticalBitsWriter<8, char> writer;
            char* decoded = writer.read((unsigned char*)source, 8 * 4, 6);

            Assert::AreEqual(one, decoded[0], L"Wrong value.", LINE_INFO());
            Assert::AreEqual(one, decoded[1], L"Wrong value.", LINE_INFO());
            Assert::AreEqual(one, decoded[2], L"Wrong value.", LINE_INFO());
            Assert::AreEqual(one, decoded[3], L"Wrong value.", LINE_INFO());

            Assert::AreEqual(zero, decoded[4], L"Wrong value.", LINE_INFO());
            Assert::AreEqual(zero, decoded[5], L"Wrong value.", LINE_INFO());

            // this should be out of range
            // Assert::AreEqual(zero, decoded[6], L"Wrong value.", LINE_INFO());
        }

        TEST_METHOD(ExceptionTest1)
        {
            // TODO: Your test code here

            int source[] = { -1, -1, -1, -1, -1, -1, -1, -1 }; //-1: 11111111 11111111 11111111 11111111
            /*
                1 2 3 4 5 6 7 8
                _ _ _ _ _ _ _ _
            V1: 1 1 1 1 1 1 1 1 //Values
            V2: 1 1 1 1 1 1 1 1
            V3: 1 1 1 1 1 1 1 1
            V4: 1 1 1 1 1 1 1 1
                . . . . . . . .
            V32 1 1 1 1 1 1 1 1
            */

            VerticalBitsWriter<8, char> writer;
            bool exceptionThrown = false;

            try
            {
                writer.read((unsigned char*)source, 32 /* 8(#of ints) * 4(int size in chars) */, 40);
            }
            catch (const std::invalid_argument&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown, L"Exception expected, becuase 40 points could fit into 8 ints");
        }

        TEST_METHOD(ExceptionTest2)
        {
            // TODO: Your test code here

            int source[] = { -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0 }; //-1: 11111111 11111111 11111111 11111111
            /*
                1 2 3 4 5 6 7 8
                _ _ _ _ _ _ _ _
            V1: 1 1 1 1 1 1 1 1 //Values
            V2: 1 1 1 1 1 1 1 1
            V3: 1 1 1 1 1 1 1 1
            V4: 1 1 1 1 1 1 1 1
                . . . . . . . .
            V32 1 1 1 1 1 1 1 1
            V33 0 0 0 0 0 0 0 0
                . . . . . . . .
            V64 0 0 0 0 0 0 0 0
            */

            VerticalBitsWriter<8, char> writer;
            bool exceptionThrown = false;

            try
            {
                writer.read((unsigned char*)source, 64 /* 16(#of ints) * 4(int size in chars) */, 32);
            }
            catch (const std::invalid_argument&)
            {
                exceptionThrown = true;
            }

            Assert::IsTrue(exceptionThrown, L"Exception expected, becuase 16 ints is too much for 48 points of char.");
        }
    };
}