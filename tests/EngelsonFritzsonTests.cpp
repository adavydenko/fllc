#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\fllc\EngelsonFritzson.cpp"
#include "..\fllc\structs.h"

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
	};
}