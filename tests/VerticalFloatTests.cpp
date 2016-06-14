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

            float x[5] = { 100.0f, 200.0f, 250.0f, 260.0f, 405.0f };

            VerticalFloat sd;
            sd.compress((_float*)x, 5);

            int size;
            unsigned char* zip = sd.allocate(&size);

            float* unpacked = (float*)sd.decompress(zip, size, 5);
            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(x[i], unpacked[i], 0.000001f, L"Invalid decompressed value.", LINE_INFO());
            }
        }
	};
}