// VerticalFloat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VerticalFloat.h"
#include "structs.h"

void VerticalFloat::compress(_float * nir, int count)
{
	int deltaE = 0;
	int ddeltaE = 0;
	int dddeltaE = 0;
	int deltaM = 0;
	int ddeltaM = 0;
	int dddeltaM = 0;

	for (int i = 0; i < count; i++)
	{
		int s = int(nir[i].S());
		int e = int(nir[i].E());
		int m = nir[i].M();

		if (i > 0)
		{
			deltaE = e - (int)nir[i - 1].E();
			deltaM = m - nir[i - 1].M();
		}

		if (i > 1)
		{
			ddeltaE = deltaE - ((int)nir[i - 1].E() - (int)nir[i - 2].E());
			ddeltaM = deltaM - (nir[i - 1].M() - nir[i - 2].M());
		}

		if (i > 2)
		{
			int ddeltaE_1 = deltaE - ddeltaE - ((int)nir[i - 2].E() - (int)nir[i - 3].E());
			int ddeltaM_1 = deltaM - ddeltaM - (nir[i - 2].M() - nir[i - 3].M());

			dddeltaE = ddeltaE - ddeltaE_1;
			dddeltaM = ddeltaM - ddeltaM_1;

			this->sBits.add(s > 0);

			for (size_t i = 0; i < 8; i++)
			{
				this->eBits[i].add(dddeltaE, i);
			}

			for (size_t i = 0; i < 24; i++)
			{
				this->mBits[i].add(dddeltaM, i);
			}
		}
	}
}

unsigned int * VerticalFloat::allocate(int * count)
{
	return nullptr;
}