#pragma once
#include <iostream>

struct _byte {
	unsigned char a : 1;
	unsigned char b : 1;
	unsigned char c : 1;
	unsigned char d : 1;
	unsigned char e : 1;
	unsigned char f : 1;
	unsigned char g : 1;
	unsigned char h : 1;

	/* e.g. for 64:
	0 1 0 0 0 0 0 0
	h g f e d c b a
	*/
};

union byte {
	unsigned char _char;
	_byte _byte;

	const unsigned char &operator [] (int position)
	{
		int bitNum = position % 8;
		switch (bitNum)
		{
		case 0: return unsigned char(_byte.a == 0 ? '0' : '1');
		case 1: return unsigned char(_byte.b == 0 ? '0' : '1');
		case 2: return unsigned char(_byte.c == 0 ? '0' : '1');
		case 3: return unsigned char(_byte.d == 0 ? '0' : '1');
		case 4: return unsigned char(_byte.e == 0 ? '0' : '1');
		case 5: return unsigned char(_byte.f == 0 ? '0' : '1');
		case 6: return unsigned char(_byte.g == 0 ? '0' : '1');
		case 7: return unsigned char(_byte.h == 0 ? '0' : '1');
		}
	}
};

union _float
{
	float fvalue;
	unsigned int _value;
	byte bytes[4];

	const unsigned char &operator [] (int position)
	{
		int byteNum = position / 8;
		int bitNum = position % 8;

		return bytes[byteNum][bitNum];
	}

	char S()
	{
		return bytes[3]._byte.h;
	}

	char E()
	{
		char result = (char)((_value << 1 /*erase sign 'S'*/) >> 24);

		if (result == 0) return result;
		return result - 127; //exponent is shifted + 127
	}

	int M()
	{
		return (int)((_value << 9) >> 9);
	}

	void FromSEM(char s, char e, int m)
	{
		_value = 0;
		_value <<= 32; //erase
		_value |= s;

		_value <<= 8;
		//_value |= (e + 127);
		_value |= e;

		_value <<= 23;
		_value |= m;
	}
};

template <int B>
union _int
{
	unsigned int _value;
	byte bytes[B];

	const unsigned char &operator [] (int position)
	{
		int byteNum = position / 8;
		int bitNum = position % 8;

		return bytes[byteNum][bitNum];
	}

	void PrintBinary()
	{
		int bitsCount = 8 * B;

		for (int j = 0; j < bitsCount; j++)
		{
			cout << (*this)[bitsCount - 1 - j];
			if ((j + 1) % 8 == 0) cout << ".";
		}
	}

	void SetUnsigned(int value)
	{
		_value = 0;
		if (value < 0)
		{
			_value = -value;
			//bytes[B - 1]._byte.h = 1;
		}
		else
		{
			_value = value;
		}
	}
};