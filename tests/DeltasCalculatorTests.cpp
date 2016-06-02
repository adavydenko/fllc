#include "stdafx.h"
#include "CppUnitTest.h"
#include <Windows.h>
#include "..\fllc\DeltasCalculator.cpp"

#define PI 3.14159265

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
    TEST_CLASS(DeltasCalculatorTests)
    {
        void printDeviation(int* arr, int count)
        {
            int min = arr[0];
            int max = arr[0];

            for (size_t i = 0; i < count; i++)
            {
                if (arr[i] < min) min = arr[i];
                if (arr[i] > max) max = arr[i];
            }

            char buffer[200];
            sprintf(buffer, "Max deviation: %d\nMin deviation: %d\n", max, min);
            OutputDebugStringA(buffer);
        }

        void printDeviation(float* arr, int count)
        {
            float min = arr[0];
            float max = arr[0];

            for (size_t i = 0; i < count; i++)
            {
                if (arr[i] < min) min = arr[i];
                if (arr[i] > max) max = arr[i];
            }

            char buffer[200];
            sprintf(buffer, "Max deviation: %f\nMin deviation: %f\n", max, min);
            OutputDebugStringA(buffer);
        }


    public:
        TEST_METHOD(BasicZerosTest)
        {
            // TODO: Your test code here
            int values[5] = { 0, 0, 0, 0, 0 };
            int deltas[5];

            DeltasCalculator<int> calculator;
            calculator.getDeltas(values, 5, deltas);

            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(0, deltas[i], L"Delta values should be zero.", LINE_INFO());
            }

            int restored[5];
            calculator.getOriginal(deltas, 5, restored);
            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(values[i], restored[i], L"Invalid restored value.", LINE_INFO());
            }
        }

        TEST_METHOD(BasicIntTest)
        {
            // TODO: Your test code here
            int values[5] = { 0, 1, 2, 3, 4 };
            int deltas[5];

            DeltasCalculator<int> calculator;
            calculator.getDeltas(values, 5, deltas);

            for (size_t i = 0; i < 5; i++)
            {
                if (i == 0)
                    Assert::AreEqual(0, deltas[i], L"First value should be unchanged.", LINE_INFO());
                else
                    Assert::AreEqual(1, deltas[i], L"Delta value should be 1.", LINE_INFO());
            }

            int restored[5];
            calculator.getOriginal(deltas, 5, restored);
            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(values[i], restored[i], L"Invalid restored value.", LINE_INFO());
            }
        }

        TEST_METHOD(BasicFloatTest)
        {
            // TODO: Your test code here
            float values[5] = { 0.00000f, 1.00000f, 1.10000f, 1.10001, 0.99999f };
            float deltas[5];

            DeltasCalculator<float> calculator;
            calculator.getDeltas(values, 5, deltas);

            Assert::AreEqual(0.00000f, deltas[0], 0.000001f, L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(1.00000f, deltas[1], 0.000001f, L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0.10000f, deltas[2], 0.000001f, L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0.00001f, deltas[3], 0.000001f, L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(-0.10002f, deltas[4], 0.000001f, L"Invalid delta value.", LINE_INFO());

            float restored[5];
            calculator.getOriginal(deltas, 5, restored);
            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(values[i], restored[i], 0.000001f, L"Invalid restored value.", LINE_INFO());
            }
        }

        TEST_METHOD(Order2IntTest)
        {
            // TODO: Your test code here
            int values[5] = { 0, 1, 2, 3, 4 };
            // d_0:|0  1  2  3  4 .. - initial array
            // d_1: 0|+1 +1 +1 +1 .. - first value is unchanged for deltas of 1st order
            // d_2: 0 +1| 0  0  0 .. - first and second values are kept from previous delta unchanged

            int deltas[5];

            DeltasCalculator<int, 2> calculator;
            calculator.getDeltas(values, 5, deltas);


            Assert::AreEqual(0, deltas[0], L"First value should be unchanged.", LINE_INFO());
            Assert::AreEqual(1, deltas[1], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(1, deltas[1], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(1, deltas[1], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(1, deltas[1], L"Invalid delta value.", LINE_INFO());


            int restored[5];
            calculator.getOriginal(deltas, 5, restored);
            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(values[i], restored[i], L"Invalid restored value.", LINE_INFO());
            }
        }

        TEST_METHOD(Order3FloatTest)
        {
            // NOTE: Those floats are not precise, so it's ok to use deltas bewteen them
            float values[8] = { 1.20000f, 1.44000f, 1.20020f, 0.98065, 0.99000f, 1.60500f, 1.61600f, 2.00000f };

            // d_0: |1.20000  1.44000  1.20020  0.98065  0.99000  1.60500  1.61600  2.00000 .. - array as it given
            // d_1:  1.20000|+0.24000 -0.23980 -0.21955 +0.00935 +0.61500 +0.01100 +0.38400 .. - d_1(i) = d_0(i) - d_0(i-1)
            // d_2:  1.20000 +0.24000|-0.47980 +0.02025 +0.22890 +0.60565 -0.60400 +0.37300
            // d_3:  1.20000 +0.24000 -0.47980|+0.50005 +0.20865 +0.37675 -1.20965 +0.97700
            //                                ^ d_3 values are started here

            float deltas[8];

            DeltasCalculator<float, 3> calculator;
            calculator.getDeltas(values, 8, deltas);

            Assert::AreEqual(+1.20000f, deltas[0], 0.000001f, L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(+0.24000f, deltas[1], 0.000001f, L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(-0.47980f, deltas[2], 0.000001f, L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(+0.50005f, deltas[3], 0.000001f, L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(+0.20865f, deltas[4], 0.000001f, L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(+0.37675f, deltas[5], 0.000001f, L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(-1.20965f, deltas[6], 0.000001f, L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(+0.97700f, deltas[7], 0.000001f, L"Invalid delta value.", LINE_INFO());

            float restored[8];
            calculator.getOriginal(deltas, 8, restored);
            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(values[i], restored[i], 0.000001f, L"Invalid restored value.", LINE_INFO());
            }
        }

        TEST_METHOD(SinOrder1Test)
        {
            const int SIZE = 361;
            //TODO: it is not good to keep float deltas, as it cannot be reconstructed losslessly, so ints are used
            int values[SIZE];

            for (size_t degree = 0; degree < SIZE; degree++)
            {
                values[degree] = (int)(1000 * sin(degree*PI / 180));
            }

            int deltas[SIZE];
            DeltasCalculator<int, 1> calculator;
            calculator.getDeltas(values, SIZE, deltas);

            printDeviation(deltas, SIZE);

            int restored[SIZE];
            calculator.getOriginal(deltas, SIZE, restored);

            Assert::AreEqual(0, restored[0], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0, restored[180], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0, restored[360], L"Invalid delta value.", LINE_INFO());

            for (size_t i = 0; i < SIZE; i++)
            {
                Assert::AreEqual(values[i], restored[i], L"Invalid restored value.", LINE_INFO());
            }
        }

        TEST_METHOD(SinOrder2Test)
        {
            const int SIZE = 361;
            //TODO: it is not good to keep float deltas, as it cannot be reconstructed losslessly, so ints are used
            int values[SIZE];

            for (size_t degree = 0; degree < SIZE; degree++)
            {
                values[degree] = (int)(1000 * sin(degree*PI / 180));
            }

            int deltas[SIZE];
            DeltasCalculator<int, 2> calculator;
            calculator.getDeltas(values, SIZE, deltas);

            printDeviation(deltas, SIZE);

            int restored[SIZE];
            calculator.getOriginal(deltas, SIZE, restored);

            Assert::AreEqual(0, restored[0], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0, restored[180], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0, restored[360], L"Invalid delta value.", LINE_INFO());

            for (size_t i = 0; i < SIZE; i++)
            {
                Assert::AreEqual(values[i], restored[i], L"Invalid restored value.", LINE_INFO());
            }
        }

        TEST_METHOD(SinOrder3Test)
        {
            const int SIZE = 361;
            //TODO: it is not good to keep float deltas, as it cannot be reconstructed losslessly, so ints are used
            int values[SIZE];

            for (size_t degree = 0; degree < SIZE; degree++)
            {
                values[degree] = (int)(1000 * sin(degree*PI / 180));
            }

            int deltas[SIZE];
            DeltasCalculator<int, 3> calculator;
            calculator.getDeltas(values, SIZE, deltas);

            printDeviation(deltas, SIZE);

            int restored[SIZE];
            calculator.getOriginal(deltas, SIZE, restored);

            Assert::AreEqual(0, restored[0], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0, restored[180], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0, restored[360], L"Invalid delta value.", LINE_INFO());

            for (size_t i = 0; i < SIZE; i++)
            {
                Assert::AreEqual(values[i], restored[i], L"Invalid restored value.", LINE_INFO());
            }
        }

        TEST_METHOD(SinOrder4Test)
        {
            const int SIZE = 361;
            //TODO: it is not good to keep float deltas, as it cannot be reconstructed losslessly, so ints are used
            int values[SIZE];

            for (size_t degree = 0; degree < SIZE; degree++)
            {
                values[degree] = (int)(1000 * sin(degree*PI / 180));
            }

            int deltas[SIZE];
            DeltasCalculator<int, 4> calculator;
            calculator.getDeltas(values, SIZE, deltas);

            printDeviation(deltas, SIZE);

            int restored[SIZE];
            calculator.getOriginal(deltas, SIZE, restored);

            Assert::AreEqual(0, restored[0], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0, restored[180], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0, restored[360], L"Invalid delta value.", LINE_INFO());

            for (size_t i = 0; i < SIZE; i++)
            {
                Assert::AreEqual(values[i], restored[i], L"Invalid restored value.", LINE_INFO());
            }
        }

        TEST_METHOD(SinOrder7Test)
        {
            const int SIZE = 361;
            //TODO: it is not good to keep float deltas, as it cannot be reconstructed losslessly, so ints are used
            int values[SIZE];

            for (size_t degree = 0; degree < SIZE; degree++)
            {
                values[degree] = (int)(1000 * sin(degree*PI / 180));
            }

            int deltas[SIZE];
            DeltasCalculator<int, 7> calculator;
            calculator.getDeltas(values, SIZE, deltas);

            printDeviation(deltas, SIZE);

            int restored[SIZE];
            calculator.getOriginal(deltas, SIZE, restored);

            Assert::AreEqual(0, restored[0], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0, restored[180], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0, restored[360], L"Invalid delta value.", LINE_INFO());

            for (size_t i = 0; i < SIZE; i++)
            {
                Assert::AreEqual(values[i], restored[i], L"Invalid restored value.", LINE_INFO());
            }
        }

        TEST_METHOD(SinOrder10Test)
        {
            const int SIZE = 361;
            //TODO: it is not good to keep float deltas, as it cannot be reconstructed losslessly, so ints are used
            int values[SIZE];

            for (size_t degree = 0; degree < SIZE; degree++)
            {
                values[degree] = (int)(1000 * sin(degree*PI / 180));
            }

            int deltas[SIZE];
            DeltasCalculator<int, 10> calculator;
            calculator.getDeltas(values, SIZE, deltas);

            printDeviation(deltas, SIZE);

            int restored[SIZE];
            calculator.getOriginal(deltas, SIZE, restored);

            Assert::AreEqual(0, restored[0], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0, restored[180], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0, restored[360], L"Invalid delta value.", LINE_INFO());

            for (size_t i = 0; i < SIZE; i++)
            {
                Assert::AreEqual(values[i], restored[i], L"Invalid restored value.", LINE_INFO());
            }
        }

        TEST_METHOD(SinOrder100Test)
        {
            const int SIZE = 361;
            //TODO: it is not good to keep float deltas, as it cannot be reconstructed losslessly, so ints are used
            int values[SIZE];

            for (size_t degree = 0; degree < SIZE; degree++)
            {
                values[degree] = (int)(1000 * sin(degree*PI / 180));
            }

            int deltas[SIZE];
            DeltasCalculator<int, 100> calculator;
            calculator.getDeltas(values, SIZE, deltas);

            printDeviation(deltas, SIZE);

            int restored[SIZE];
            calculator.getOriginal(deltas, SIZE, restored);

            Assert::AreEqual(0, restored[0], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0, restored[180], L"Invalid delta value.", LINE_INFO());
            Assert::AreEqual(0, restored[360], L"Invalid delta value.", LINE_INFO());

            for (size_t i = 0; i < SIZE; i++)
            {
                Assert::AreEqual(values[i], restored[i], L"Invalid restored value.", LINE_INFO());
            }
        }
    };
}