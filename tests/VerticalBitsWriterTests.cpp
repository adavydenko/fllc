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

                --VerticalBits are alligned to ints, so it would have a form:
                VerticalBits are alligned to chars, so it would have a form:

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

            std::vector<unsigned char> result = writer.allocate();

            Assert::AreEqual(8, (int)result.size(), L"Invalid target buffer length.", LINE_INFO());
            Assert::AreEqual(0, (int)result[0], L"Zero is expected.", LINE_INFO());
            Assert::AreEqual(0, (int)result[1], L"Zero is expected.", LINE_INFO());
        }

        TEST_METHOD(BasicEncodingTest2)
        {
            // TODO: Your test code here

            char one = '\xFF'; // 11111111
            const int totalBytes = 32;

            VerticalBitsWriter<8, char> writer;
            for (size_t i = 0; i < totalBytes /*number of values to write*/; i++)
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

            std::vector<unsigned char> result = writer.allocate();

            Assert::AreEqual(totalBytes, (int)result.size(), L"Invalid target buffer length.", LINE_INFO());
            for (size_t i = 0; i < totalBytes; i++)
            {
                /*
                255: 11111111 11111111 11111111 11111111
                */
                Assert::AreEqual((char)255, (char)result[i], L"Wrong value.", LINE_INFO());
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
                V8: 0 0 0 0 0 0 0 0 //last byte in Writer
                */
            }

            std::vector<unsigned char> result = writer.allocate();

            Assert::AreEqual(8, (int)result.size(), L"Invalid target buffer length.", LINE_INFO());
            for (size_t i = 0; i < 8; i++)
            {
                /*
                240: 11110000
                */
                Assert::AreEqual((unsigned char)240, result[i], L"Wrong value.", LINE_INFO());
            }
        }

        TEST_METHOD(BasicDecodingTest)
        {
            // TODO: Your test code here

            char zero = '\x00'; // 00000000
            int source[] = { 0, 0 };
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
            char* decoded = writer.read((unsigned char*)source, 8, 5);

            for (size_t i = 0; i < 5; i++)
            {
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

            char eightyEight = '\x88'; // 10001000
            char zero = '\x00'; // 00000000

            //int source[] = { -268435456 , -268435456 }; //-268435456: 11110000 00000000 00000000 00000000
            // in case of low endian, ints are stored in reverse order, so substitute them with chars for simplicity:
            char source[] = { '\xF0', '\x00', '\x00', '\x00', '\xF0', '\x00', '\x00', '\x00' };
            /*

            VerticalBits are alligned to ints, so it would have a form:

                1 2 3 4 5 6 7 8
                _ _ _ _ _ _ _ _
            V1: 1 0 0 0 1 0 0 0 //Values
            V2: 1 0 0 0 1 0 0 0
            V3: 1 0 0 0 1 0 0 0
            V4: 1 0 0 0 1 0 0 0
            V5: 0 0 0 0 0 0 0 0
            V6: 0 0 0 0 0 0 0 0
                . . . . . . . .
            V8: 0 0 0 0 0 0 0 0
            */

            VerticalBitsWriter<8, char> writer;
            char* decoded = writer.read((unsigned char*)source, 8, 6);

            Assert::AreEqual(eightyEight, decoded[0], L"Wrong value.", LINE_INFO());
            Assert::AreEqual(eightyEight, decoded[1], L"Wrong value.", LINE_INFO());
            Assert::AreEqual(eightyEight, decoded[2], L"Wrong value.", LINE_INFO());
            Assert::AreEqual(eightyEight, decoded[3], L"Wrong value.", LINE_INFO());

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

            const int source[] = { -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0 }; //-1: 11111111 11111111 11111111 11111111
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

            auto func = [&] {writer.read((unsigned char*)source, 64 /* 16(#of ints) * 4(int size in chars) */, 32); };
            Assert::ExpectException<std::invalid_argument>(
                func,
                L"Exception expected, becuase 16 ints is too much for 48 points of char.");

        }

        TEST_METHOD(EncodingDecodingTest)
        {
            // TODO: Your test code here

            const unsigned char one = '\xFF';  // 11111111
            const unsigned char zero = '\x00'; // 00000000
            const unsigned char four = '\x0F'; // 00001111
            const unsigned char five = '\x1F'; // 00011111

            unsigned char data[] = { one, zero, four, five };

            VerticalBitsWriter<6 /*encode only 6 bits at right*/, unsigned char> writer;
            for (size_t i = 0; i < 4 /*number of values to write*/; i++)
            {
                writer.write(data[i]);
                /*

                VerticalBits are alligned to chars (in test), so it would have a form:

                    1 2 | 3 4 5 6 7 8
                    _ _ | _ _ _ _ _ _
                V1: 1 1 | 1 1 1 1 1 1 //Values
                V2: 0 0 | 0 0 0 0 0 0
                V3: 0 0 | 0 0 1 1 1 1
                V4: 0 0 | 0 1 1 1 1 1
                    . . | . . . . . .
                V8  0 0 | 0 0 0 0 0 0
                ---------------------
                   none | encoded values = 6
                */
            }

            std::vector<unsigned char> result = writer.allocate();

            Assert::AreEqual(6, (int)result.size(), L"Invalid target buffer length.", LINE_INFO());
            /*
            128: 10000000
            144: 10010000
            176: 10110000
            */
            Assert::AreEqual((unsigned char)128, result[0], L"Wrong encoded value.", LINE_INFO());
            Assert::AreEqual((unsigned char)144, result[1], L"Wrong encoded value.", LINE_INFO());
            Assert::AreEqual((unsigned char)176, result[2], L"Wrong encoded value.", LINE_INFO());
            Assert::AreEqual((unsigned char)176, result[3], L"Wrong encoded value.", LINE_INFO());
            Assert::AreEqual((unsigned char)176, result[4], L"Wrong encoded value.", LINE_INFO());
            Assert::AreEqual((unsigned char)176, result[5], L"Wrong encoded value.", LINE_INFO());

            // we coded lossy, so the resulting array should be different

            VerticalBitsWriter<6 /*encode only 6 bits at right*/, unsigned char> reader;
            unsigned char* decoded = reader.read(result.data(), 8, 4);

            Assert::AreEqual(one, decoded[0], L"Wrong decoded value.", LINE_INFO());
            Assert::AreEqual(zero, decoded[1], L"Wrong decoded value.", LINE_INFO());
            Assert::AreEqual(four, decoded[2], L"Wrong decoded value.", LINE_INFO());
            Assert::AreEqual(five, decoded[3], L"Wrong decoded value.", LINE_INFO());

            // just to test algorithm, since rest part are zeros, we can convert them to zeros as well
            VerticalBitsWriter<6 /*encode only 6 bits at right*/, unsigned char> reader2;
            unsigned char* decoded2 = reader2.read(result.data(), 8, 8);

            int i = 0;
            Assert::AreEqual(one, decoded2[i++], L"Wrong decoded value.", LINE_INFO());
            Assert::AreEqual(zero, decoded2[i++], L"Wrong decoded value.", LINE_INFO());
            Assert::AreEqual(four, decoded2[i++], L"Wrong decoded value.", LINE_INFO());
            Assert::AreEqual(five, decoded2[i++], L"Wrong decoded value.", LINE_INFO());
            for (; i < 8; i++)
                Assert::AreEqual(zero, decoded2[i], L"Wrong decoded value.", LINE_INFO());
        }

        TEST_METHOD(EncodingDecodingTest2)
        {
            // TODO: Your test code here

            const unsigned char one_zero = '\xDF';  // 11011111
            const unsigned char zero = '\x00';      // 00000000
            const unsigned char four = '\x0F';      // 00001111
            const unsigned char five = '\x1F';      // 00011111

            char data[] = { one_zero, zero, four, five };

            VerticalBitsWriter<6 /*encode only 6 bits at right*/, char> writer;
            for (size_t i = 0; i < 4 /*number of values to write*/; i++)
            {
                writer.write(data[i]);
                /*

                VerticalBits are alligned to ints, so it would have a form:

                    1 2 | 3 4 5 6 7 8
                    _ _ | _ _ _ _ _ _
                V1: 1 1 | 0 1 1 1 1 1 //Values
                V2: 0 0 | 0 0 0 0 0 0
                V3: 0 0 | 0 0 1 1 1 1
                V4: 0 0 | 0 1 1 1 1 1
                    . . | . . . . . .
                V8: 0 0 | 0 0 0 0 0 0
                ---------------------
                   none | encoded values = 6
                */
            }

            std::vector<unsigned char> result = writer.allocate();

            Assert::AreEqual(6, (int)result.size(), L"Invalid target buffer length.", LINE_INFO());
            /*
              0: 00000000
            144: 10010000
            176: 10110000
            */
            Assert::AreEqual(0, (int)result[0], L"Wrong encoded value.", LINE_INFO());
            Assert::AreEqual(144, (int)result[1], L"Wrong encoded value.", LINE_INFO());
            Assert::AreEqual(176, (int)result[2], L"Wrong encoded value.", LINE_INFO());
            Assert::AreEqual(176, (int)result[3], L"Wrong encoded value.", LINE_INFO());
            Assert::AreEqual(176, (int)result[4], L"Wrong encoded value.", LINE_INFO());
            Assert::AreEqual(176, (int)result[5], L"Wrong encoded value.", LINE_INFO());

            // we coded lossy, so the resulting array should be different

            VerticalBitsWriter<6 /*encode only 6 bits at right*/, char> reader;
            char* decoded = reader.read((unsigned char*)result.data(), 6, 4);

            Assert::AreEqual(five, (unsigned char)decoded[0], L"Wrong decoded value.", LINE_INFO());
            Assert::AreEqual(zero, (unsigned char)decoded[1], L"Wrong decoded value.", LINE_INFO());
            Assert::AreEqual(four, (unsigned char)decoded[2], L"Wrong decoded value.", LINE_INFO());
            Assert::AreEqual(five, (unsigned char)decoded[3], L"Wrong decoded value.", LINE_INFO());

            // just to test algorithm, since rest part are zeros, we can convert them to zeros as well
            VerticalBitsWriter<6 /*encode only 6 bits at right*/, char> reader2;
            char* decoded2 = reader2.read((unsigned char*)result.data(), 6, 8);

            int i = 0;
            Assert::AreEqual(five, (unsigned char)decoded2[i++], L"Wrong decoded value.", LINE_INFO());
            Assert::AreEqual(zero, (unsigned char)decoded2[i++], L"Wrong decoded value.", LINE_INFO());
            Assert::AreEqual(four, (unsigned char)decoded2[i++], L"Wrong decoded value.", LINE_INFO());
            Assert::AreEqual(five, (unsigned char)decoded2[i++], L"Wrong decoded value.", LINE_INFO());
            for (; i < 8; i++)
                Assert::AreEqual(zero, (unsigned char)decoded2[i], L"Wrong decoded value.", LINE_INFO());
        }

        TEST_METHOD(MantissaWriterSignedTest)
        {
            // TODO: Your test code here

            int data[] = { +100, +100, -50, +10, +175 };

            VerticalBitsWriter<23 /*encode only 6 bits at right*/, int> writer;
            for (size_t i = 0; i < 5 /*number of values to write*/; i++)
            {
                writer.write(data[i]);
            }

            std::vector<unsigned char> result = writer.allocate();

            int* decoded = writer.read(result.data(), result.size(), 5);

            for (int i = 0; i < 5; i++)
                Assert::AreEqual(data[i], decoded[i], L"Wrong decoded value.", LINE_INFO());
        }

        TEST_METHOD(MantissaWriterUnsignedTest)
        {
            // TODO: Your test code here

            unsigned int data[] = { +100u, +100u, -50u, +10u, +175u };

            VerticalBitsWriter<23 /*encode only 6 bits at right*/, unsigned int> writer;
            for (size_t i = 0; i < 5 /*number of values to write*/; i++)
            {
                writer.write(data[i]);
            }

            std::vector<unsigned char> result = writer.allocate();

            unsigned int* decoded = writer.read(result.data(), result.size(), 5);

            for (int i = 0; i < 5; i++)
                Assert::AreEqual(data[i], decoded[i], L"Wrong decoded value.", LINE_INFO());
        }
    };
}