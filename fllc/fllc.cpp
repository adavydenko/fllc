// fllc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <vector>

#include "structs.h"
#include "EngelsonFritzson.h"

using namespace std;

void printUsage(const char* cmd)
{
    printf("%s -method -c|-d -l <points count> -file <file name> [-save]\n", cmd);
    printf("-method          : defines compression algorithm, should be one of:\n", cmd);
    printf("                   -ef - EngelsonFritzson\n", cmd);
    printf("                   -sem - Separated Sign/Exponent/Mantissa\n", cmd);
    printf("                   -vb - Vartical Bits technique (=vf)\n", cmd);
    printf("-c (default), -d : defines whether file should be (c)ompressed of (d)ecompressed\n", cmd);
    printf("-l <points count>: specifies number of floats stored in the file\n", cmd);
    printf("-file <file name>: specifies file name with uncompressed (should contain text) or compressed data (should be binary)\n", cmd);
    printf("[-save]          : optional, specified whether output data should be saved on disk or not\n", cmd);

    printf("\nExample (of compression): fllc -ef -c -l 4200 -f \"da1650_nir.txt\" -save\n", cmd);
    printf("Example (of decompression): fllc -ef -d -l 4200 -f \"da1650_nir.compressed\" -save\n", cmd);
}

//TODO: preallocate buffer for results
_float* readFile(const char* fileName, int pointsCount)
{
    FILE *f;
    errno_t err = fopen_s(&f, fileName, "r");

    if (err)
    {
        return NULL;
    }

    _float* results = new _float[pointsCount];
    int i = 0;
    while (EOF != fscanf_s(f, "%f", &results[i].fvalue))
    {
        i++;
    }

    fclose(f);
    return results;
}

//TODO: preallocate buffer for results
vector<unsigned char>* readFile(const char* fileName)
{
    FILE *f;
    errno_t err = fopen_s(&f, fileName, "r");

    if (err)
    {
        return NULL;
    }

    vector<unsigned char>* results = new vector<unsigned char>();
    unsigned char buf;

    while (EOF != (buf = (unsigned char)fgetc(f)))
    {
        results->push_back(*(new unsigned char(buf)));
    }

    fclose(f);
    return results;
}

void PrintBinary(_float &f)
{
    for (int j = 0; j < 32; j++)
    {
        std::cout << f[31 - j];
        if (j == 0 || j == 8) cout << " ";
    }
}

//VB -----------------------------------------------------------
//
//void saveBinary(const char* sourceFile, _float* nir, int count)
//{
//char binaryFile[200];
//strncpy_s(binaryFile, strlen(sourceFile) + 1, sourceFile, _TRUNCATE);
//strncat_s(binaryFile, 200/*sizeof binaryFile*/, ".bin", _TRUNCATE);
//
//cout << "Writing " << binaryFile << endl;
//
//ofstream binary(binaryFile, ofstream::binary);
//
//binary.write((const char*)nir, 4 * count); //4 bytes=chars per int
//binary.close();
//}
//
//void saveVerticalBits(ofstream &out, VerticalBits &bits)
//{
//	int totalBlocks = 0;
//	unsigned int* data = bits.allocate(&totalBlocks);
//
//	out.write((const char*)data, totalBlocks * 4); //1 bytes=chars per float sign
//	delete data;
//}
//
//void saveCompressed(const char* sourceFile, VerticalFloat &data)
//{
//	char binaryFile[200];
//	strncpy_s(binaryFile, strlen(sourceFile) + 1, sourceFile, _TRUNCATE);
//	strncat_s(binaryFile, sizeof binaryFile, ".vf.compressed", _TRUNCATE);
//
//	cout << "Writing " << binaryFile << endl;
//
//	ofstream compressed(binaryFile, ofstream::binary);
//
//	saveVerticalBits(compressed, data.sBits);
//
//	for (size_t i = 0; i < 8; i++)
//	{
//		saveVerticalBits(compressed, data.eBits[i]);
//	}
//
//	for (size_t i = 0; i < 24; i++)
//	{
//		saveVerticalBits(compressed, data.mBits[i]);
//	}
//	compressed.close();
//}


void saveBinary(const char* sourceFile, _float* nir, int count)
{
    char binaryFile[200];
    strncpy_s(binaryFile, strlen(sourceFile) + 1, sourceFile, _TRUNCATE);
    strncat_s(binaryFile, sizeof binaryFile, ".binary", _TRUNCATE);

    ofstream binary(binaryFile, ofstream::binary);
    binary.write((const char*)nir, 4 * count); //4 bytes=chars per int

    binary.close();
}

void saveCompressed(const char* sourceFile, _int<2>* e, _int<4>* m, unsigned char* ss, unsigned char* se, unsigned char* sm, int count)
{
    char binaryFile[200];
    strncpy_s(binaryFile, strlen(sourceFile) + 1, sourceFile, _TRUNCATE);
    strncat_s(binaryFile, sizeof binaryFile, ".compressed", _TRUNCATE);

    ofstream compressed(binaryFile, ofstream::binary);
    compressed.write((const char*)ss, count); //1 bytes=chars per float sign
    compressed.write((const char*)se, count); //1 bytes=chars per exponent sign
    compressed.write((const char*)sm, count); //4 bytes=chars per mantissa sign

    compressed.write((const char*)e, 2 * count); //4 bytes=chars per int
    compressed.write((const char*)m, 4 * count); //4 bytes=chars per int

    compressed.close();
}

void saveCompressed(const char* sourceFile, unsigned char* data, int count, const char* extension)
{
    char binaryFile[256];
    strncpy_s(binaryFile, strlen(sourceFile) + 1, sourceFile, _TRUNCATE);
    strncat_s(binaryFile, sizeof binaryFile, extension, _TRUNCATE);

    cout << "Writing " << binaryFile << endl;

    ofstream compressed(binaryFile, ofstream::binary);
    compressed.write((const char*)data, count); //1 bytes=chars per float sign
    compressed.close();
}

int main(int argc, char* argv[])
{
    int pointsCount;
    char* sourceFile = NULL;
    bool saveResults = false;
    bool compress = true;

    bool useEF = false;
    bool useVB = false;
    bool useSEM = false;

    if (argc < 7)
    {
        printUsage(argv[0]);
        return 0;
    }

    for (int i = 1; i < argc; i++) { /* We will iterate over argv[] to get the parameters stored inside.
                                     * Note that we're starting on 1 because we don't need to know the
                                     * path of the program, which is stored in argv[0] */


                                     //if (i + 1 != argc) // Check that we haven't finished parsing already
                                     //{
        if (!strcmp(argv[i], "-c")) {
            compress = true;
        }
        else if (!strcmp(argv[i], "-d")) {
            compress = false;
        }
        else if (!strcmp(argv[i], "-ef")) {
            useEF = true;
        }
        else if (!strcmp(argv[i], "-vb")) {
            useVB = true;
        }
        else if (!strcmp(argv[i], "-sem")) {
            useSEM = true;
        }
        else if (!strcmp(argv[i], "-l")) {
            // We know the next argument *should* be the filename:
            pointsCount = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "-file")) {
            sourceFile = argv[++i];
        }
        else if (!strcmp(argv[i], "-save"))
        {
            saveResults = true;
        }
        else {
            std::cout << "Not enough or invalid arguments, please try again.\n";
            printUsage(argv[0]);
            return 0;
        }
    }

    if (compress)
    {
        _float* nir = readFile(sourceFile, pointsCount);

        if (saveResults)
        {
            saveBinary(sourceFile, nir, pointsCount);
        }

        if (useEF)
        {
            EngelsonFritzson zip;
            zip.compress(nir, pointsCount);

            int totalBlocks = 0;
            unsigned int* data = zip.allocate(&totalBlocks);

            saveCompressed(sourceFile, (unsigned char*)data, totalBlocks * 4, ".compressed.ef");
            delete data;
        }

        delete nir;
    }
    else //we are decompressing data
    {
        vector<unsigned char>* zip = readFile(sourceFile);

        if (useEF)
        {
            unsigned char* data = &(*zip)[0];
            int size = zip->size() / 4; // unsigned char -> int

            int* compressedDeltas = (int*)data;
            _float* results = new _float[pointsCount];
        }

        if (useSEM)
        {
            _float* results = nullptr;
        }

        if (useVB)
        {
            _float* results = nullptr;

        }

        delete zip;

    }


    //
    // VerticalFloat

    //VerticalFloat zip;
    //zip.compress(nir, pointsCount);
    //
    //saveBinary(sourceFile, nir, pointsCount);
    //saveCompressed(sourceFile, zip);

    //delete nir;


    // ------------------------------
    //bool d1 = false;
    //bool d2 = false;
    //bool d3 = false;
    //bool print = false;
    //bool saveResults = false;
    //

    //
    //_float* nir = readFile(sourceFile, pointsCount);
    //
    //_int<2>* earr = new _int<2>[pointsCount];
    //_int<4>* marr = new _int<4>[pointsCount];
    //unsigned char* ssign = new unsigned char[pointsCount];
    //unsigned char* sexp = new unsigned char[pointsCount];
    //unsigned char* smant = new unsigned char[pointsCount];
    //_float* outfloats = new _float[pointsCount];
    //
    //int deltaE = 0;
    //int ddeltaE = 0;
    //int dddeltaE = 0;
    //int deltaM = 0;
    //int ddeltaM = 0;
    //int dddeltaM = 0;
    //
    //int maxDeltaE = deltaE;
    //int maxDDeltaE = ddeltaE;
    //int maxDDDeltaE = dddeltaE;
    //int maxDeltaM = deltaM;
    //int maxDDeltaM = ddeltaM;
    //int maxDDDeltaM = dddeltaM;
    //
    //for (int i = 0; i < pointsCount; i++)
    //{
    //	if (print)
    //	{
    //		printf("%*f = ", 10, nir[i].fvalue);    //13 symbols
    //	}
    //
    //	int s = int(nir[i].S());
    //	int e = int(nir[i].E());
    //	int m = nir[i].M();
    //
    //	ssign[i] = (unsigned char)s;
    //
    //	if (print)
    //	{
    //		printf("%1d %*d %*d | ", s, 2, e, 8, m); //18 symbols
    //		PrintBinary(nir[i]);                    //34 symbols
    //	}
    //
    //	if (d1 && i > 0)
    //	{
    //		//ddeltaE = -deltaE; /*ddelta = deltaCurrent - deltaPrev(=deltaE)*/
    //		//ddeltaE = -deltaM;
    //
    //		deltaE = e - (int)nir[i - 1].E();
    //		deltaM = m - nir[i - 1].M();
    //
    //		if (abs(deltaE) > maxDeltaE) maxDeltaE = abs(deltaE);
    //		if (abs(deltaM) > maxDeltaM) maxDeltaM = abs(deltaM);
    //
    //		earr[i].SetUnsigned(deltaE);
    //		marr[i].SetUnsigned(deltaM);
    //
    //		if (deltaE < 0) sexp[i] = 0x1;
    //		else sexp[i] = 0x0;
    //
    //		if (deltaM < 0) smant[i] = 0x1;
    //		else smant[i] = 0x0;
    //	}
    //
    //	if (d2 && i > 1)
    //	{
    //		//ddeltaE += deltaE; /*ddelta = deltaCurrent(=deltaE) - deltaPrev*/
    //		//ddeltaM += deltaM;
    //
    //		ddeltaE = deltaE - ((int)nir[i - 1].E() - (int)nir[i - 2].E());
    //		ddeltaM = deltaM - (nir[i - 1].M() - nir[i - 2].M());
    //
    //		if (abs(ddeltaE) > maxDDeltaE) maxDDeltaE = abs(ddeltaE);
    //		if (abs(ddeltaM) > maxDDeltaM) maxDDeltaM = abs(ddeltaM);
    //
    //		earr[i].SetUnsigned(ddeltaE);
    //		marr[i].SetUnsigned(ddeltaM);
    //
    //		if (ddeltaE < 0) sexp[i] = 0x1;
    //		else sexp[i] = 0x0;
    //
    //		if (ddeltaM < 0) smant[i] = 0x1;
    //		else smant[i] = 0x0;
    //	}
    //
    //	if (d3 && i > 2)
    //	{
    //		//ddeltaE += deltaE; /*ddelta = deltaCurrent(=deltaE) - deltaPrev*/
    //		//ddeltaM += deltaM;
    //
    //		int ddeltaE_1 = deltaE - ddeltaE - ((int)nir[i - 2].E() - (int)nir[i - 3].E());
    //		int ddeltaM_1 = deltaM - ddeltaM - (nir[i - 2].M() - nir[i - 3].M());
    //
    //		dddeltaE = ddeltaE - ddeltaE_1;
    //		dddeltaM = ddeltaM - ddeltaM_1;
    //
    //		if (abs(dddeltaE) > maxDDDeltaE) maxDDDeltaE = abs(dddeltaE);
    //		if (abs(dddeltaM) > maxDDDeltaM) maxDDDeltaM = abs(dddeltaM);
    //
    //		earr[i].SetUnsigned(dddeltaE);
    //		marr[i].SetUnsigned(dddeltaM);
    //
    //		if (dddeltaE < 0) sexp[i] = 0x1;
    //		else sexp[i] = 0x0;
    //
    //		if (dddeltaM < 0) smant[i] = 0x1;
    //		else smant[i] = 0x0;
    //	}
    //
    //	if (print && d1)
    //	{
    //		printf("\n             %1d %+*d %+*d | ", s, 2, deltaE, 10, deltaM);
    //		cout << s << " ";
    //		earr[i].PrintBinary();
    //		cout << " ";
    //		marr[i].PrintBinary();
    //		cout << " ";
    //	}
    //
    //	if (print && d2)
    //	{
    //		printf("\n             %1d %+*d %+*d   |- ", s, 2, ddeltaE, 10, ddeltaM);
    //		cout << s << " ";
    //		earr[i].PrintBinary();
    //		cout << " ";
    //		marr[i].PrintBinary();
    //		cout << " ";
    //	}
    //
    //	if (print && d3)
    //	{
    //		printf("\n             %1d %+*d %+*d     |- ", s, 2, dddeltaE, 10, dddeltaM);
    //		cout << s << " ";
    //		earr[i].PrintBinary();
    //		cout << " ";
    //		marr[i].PrintBinary();
    //		cout << " ";
    //	}
    //	//if (i>)
    //
    //	cout << endl;
    //}
    //
    //if (saveResults)
    //{
    //	saveBinary(sourceFile, nir, pointsCount);
    //	saveCompressed(sourceFile, earr, marr, ssign, sexp, smant, pointsCount);
    //}
    //
    //if (print)
    //{
    //	printf("\n%+d %+d %+d | %+d %+d %+d", maxDeltaE, maxDDeltaE, maxDDDeltaE, maxDeltaM, maxDDeltaM, maxDDDeltaM);
    //	cout << endl;
    //}
    //
    ////system("pause");

    return 0;
}

