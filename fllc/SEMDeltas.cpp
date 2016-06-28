#include "stdafx.h"
#include "SEMDeltas.h"
#include "DeltasCalculator.h"
#include <vector>
#include "ZlibWrapper.h"

SEMDeltas::SEMDeltas() : ssign(nullptr), sexp(nullptr), smant(nullptr), earr(nullptr), marr(nullptr)
{
}

SEMDeltas::~SEMDeltas()
{

	if (ssign)
	{
		delete[] ssign;
	}

	if (sexp)
	{
		delete[] sexp;
	}

	if (smant)
	{
		delete[] smant;
	}

	if (earr)
	{
		delete[] earr;
	}

	if (marr)
	{
		delete[] marr;
	}

	/*
	if (allocated)
	{
		delete[] allocated;
	}*/
}

void SEMDeltas::compress(_float * nir, int count)
{
	if (earr || marr || sexp || smant || ssign) throw "Invalid operation";

	this->_count = count;

	this->ssign = new unsigned char[count];
	this->sexp = new unsigned char[count];
	this->smant = new unsigned char[count];
	this->earr = new _int<2>[count];
	this->marr = new _int<4>[count];

	int* e = new int[count];
	int* m = new int[count];

	for (int i = 0; i < count; i++)
	{
		e[i] = int(nir[i].E());
		m[i] = nir[i].M();

		ssign[i] = (unsigned char)nir[i].S();
	}

	int* deltaE = new int[count];
	int* deltaM = new int[count];

	DeltasCalculator<int, DELTA_ORDER> calculator;
	calculator.getDeltas(e, count, deltaE);
	calculator.getDeltas(m, count, deltaM);

	for (int i = 0; i < count; i++)
	{
		earr[i].SetUnsigned(deltaE[i]);
		marr[i].SetUnsigned(deltaM[i]);

		if (deltaE[i] < 0) sexp[i] = 0x1;
		else sexp[i] = 0x0;

		if (deltaM[i] < 0) smant[i] = 0x1;
		else smant[i] = 0x0;
	}

	delete[] e; e = nullptr;
	delete[] m; m = nullptr;
	delete[] deltaE; deltaE = nullptr;
	delete[] deltaM; deltaM = nullptr;
}

const unsigned char * SEMDeltas::allocate(int * size)
{
	if (/*allocated ||*/ !earr || !marr || !sexp || !smant || !ssign) throw "Invalid operation";

	//allocated = new std::vector<unsigned char>(_count* (1/*ssign*/ + 1/*sexp*/ + 1/*smant*/ + 2/*int<2>*/ + 4/*int<4>*/));
	*size = _count* (1/*ssign*/ + 1/*sexp*/ + 1/*smant*/ + 2/*int<2>*/ + 4/*int<4>*/);
	unsigned char * allocated = new unsigned char[*size];

	size_t k = 0;
	for (size_t i = 0; i < _count; i++, k++)
	{
		allocated[k] = ssign[i];
		//allocated->push_back(ssign[i]);
	}

	for (size_t i = 0; i < _count; i++, k++)
	{
		allocated[k] = sexp[i];
		//allocated->push_back(sexp[i]);
	}

	for (size_t i = 0; i < _count; i++, k++)
	{
		allocated[k] = smant[i];
		//allocated->push_back(smant[i]);
	}

	for (size_t i = 0; i < _count; i++, k++)
	{
		allocated[k++] = earr[i].bytes[0]._char;
		allocated[k] = earr[i].bytes[1]._char;
		//allocated->push_back(earr[i].bytes[0]._char);
		//allocated->push_back(earr[i].bytes[1]._char);
	}

	for (size_t i = 0; i < _count; i++, k++)
	{
		allocated[k++] = marr[i].bytes[0]._char;
		allocated[k++] = marr[i].bytes[1]._char;
		allocated[k++] = marr[i].bytes[2]._char;
		allocated[k] = marr[i].bytes[3]._char;
		//allocated->push_back(marr[i].bytes[0]._char);
		//allocated->push_back(marr[i].bytes[1]._char);
		//allocated->push_back(marr[i].bytes[2]._char);
		//allocated->push_back(marr[i].bytes[3]._char);
	}

	ZlibWrapper zip;

	int nLenDst = zip.GetMaxCompressedLen(*size);
	BYTE* buffer = new BYTE[nLenDst];  // alloc dest buffer

	int nLenPacked = zip.CompressData(allocated, *size, buffer, nLenDst);
	if (nLenPacked == -1) return nullptr;

	delete[] allocated; allocated = nullptr;

	*size = nLenPacked;
	return buffer;
}

_float * SEMDeltas::decompress(const unsigned char * data, int dataSize, int pointsCount)
{
	// unzipping ------------------------------
	ZlibWrapper zip;

	this->_count = pointsCount;
	int targetLen = pointsCount * (1/*ssign*/ + 1/*sexp*/ + 1/*smant*/ + 2/*int<2>*/ + 4/*int<4>*/);

	BYTE* unpacked = new BYTE[targetLen];

	int nLen = zip.UncompressData((BYTE*)data, dataSize, unpacked, targetLen);

	this->ssign = new unsigned char[pointsCount];
	this->sexp = new unsigned char[pointsCount];
	this->smant = new unsigned char[pointsCount];
	this->earr = new _int<2>[pointsCount];
	this->marr = new _int<4>[pointsCount];

	// converting to deltas ------------------
	size_t k = 0;
	for (size_t i = 0; i < _count; i++, k++)
	{
		ssign[i] = unpacked[k];
	}

	for (size_t i = 0; i < _count; i++, k++)
	{
		sexp[i] = unpacked[k];
	}

	for (size_t i = 0; i < _count; i++, k++)
	{
		smant[i] = unpacked[k];
	}

	for (size_t i = 0; i < _count; i++, k++)
	{
		earr[i]._value = 0;
		earr[i].bytes[0]._char = unpacked[k++];
		earr[i].bytes[1]._char = unpacked[k];

	}

	for (size_t i = 0; i < _count; i++, k++)
	{
		marr[i]._value = 0;
		marr[i].bytes[0]._char = unpacked[k++];
		marr[i].bytes[1]._char = unpacked[k++];
		marr[i].bytes[2]._char = unpacked[k++];
		marr[i].bytes[3]._char = unpacked[k];
	}

	delete[] unpacked; unpacked = nullptr;

	// from deltas to original ----------------
	int* deltaE = new int[pointsCount];
	int* deltaM = new int[pointsCount];

	for (size_t i = 0; i < pointsCount; i++)
	{
		if (!sexp[i]) deltaE[i] = earr[i]._value;
		else deltaE[i] = earr[i]._value * -1;

		if (!smant[i]) deltaM[i] = marr[i]._value;
		else deltaM[i] = marr[i]._value * -1;
	}

	delete[] sexp; sexp = nullptr;
	delete[] smant; smant = nullptr;
	delete[] earr; earr = nullptr;
	delete[] marr; marr = nullptr;

	int* e = new int[pointsCount];
	int* m = new int[pointsCount];

	DeltasCalculator<int, DELTA_ORDER> calculator;
	calculator.getOriginal(deltaE, pointsCount, e);
	calculator.getOriginal(deltaM, pointsCount, m);

	delete[] deltaE; deltaE = nullptr;
	delete[] deltaM; deltaM = nullptr;

	_float* values = new _float[pointsCount];

	for (size_t i = 0; i < pointsCount; i++)
	{
		values[i].FromSEM(ssign[i], e[i], m[i]);
	}

	delete[] ssign; ssign = nullptr;
	delete[] e; e = nullptr;
	delete[] m; m = nullptr;

	return values;
}
