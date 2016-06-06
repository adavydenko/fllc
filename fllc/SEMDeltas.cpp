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

    delete[] e;
    delete[] m;
    delete[] deltaE;
    delete[] deltaM;
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

    delete[] allocated;

    *size = nLenPacked;
    return buffer;
}
