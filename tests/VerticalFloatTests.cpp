#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\fllc\structs.h"
#include "..\fllc\VerticalFloat.cpp"
#include "..\fllc\VerticalBits.cpp"
#include "..\fllc\ZlibWrapper.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{		
	TEST_CLASS(VerticalFloatTests)
	{
	public:
		TEST_METHOD(BasicTest)
        {
            // TODO: Your test code here

            int x[5] = { 100, 200, 250, 260, 405 };

            VerticalFloat sd;
            sd.compress((_float*)x, 5);

            int size;
            unsigned char* zip = sd.allocate(&size);

            int* unpacked = (int*)sd.decompress(zip, size, 5);
            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(x[i], unpacked[i], L"Invalid decompressed value.", LINE_INFO());
            }
        }
	};
}