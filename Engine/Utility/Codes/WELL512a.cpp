/* ***************************************************************************** */
/* Copyright:      Francois Panneton and Pierre L'Ecuyer, University of Montreal */
/*                 Makoto Matsumoto, Hiroshima University                        */
/* Notice:         This code can be used freely for personal, academic,          */
/*                 or non-commercial purposes. For commercial purposes,          */
/*                 please contact P. L'Ecuyer at: lecuyer@iro.UMontreal.ca       */
/* ***************************************************************************** */
#include "WELL512a.h"

// double형 랜덤 값 반환
//#define W 32
//#define R 16
//#define P 0
//#define M1 13
//#define M2 9
//#define M3 5
//
//#define MAT0POS(t,v) (v^(v>>t))
//#define MAT0NEG(t,v) (v^(v<<(-(t))))
//#define MAT3NEG(t,v) (v<<(-(t)))
//#define MAT4NEG(t,b,v) (v ^ ((v<<(-(t))) & b))
//
//#define V0            STATE[state_i                   ]
//#define VM1           STATE[(state_i+M1) & 0x0000000fU]
//#define VM2           STATE[(state_i+M2) & 0x0000000fU]
//#define VM3           STATE[(state_i+M3) & 0x0000000fU]
//#define VRm1          STATE[(state_i+15) & 0x0000000fU]
//#define VRm2          STATE[(state_i+14) & 0x0000000fU]
//#define newV0         STATE[(state_i+15) & 0x0000000fU]
//#define newV1         STATE[state_i                 ]
//#define newVRm1       STATE[(state_i+14) & 0x0000000fU]
//
//#define FACT 2.32830643653869628906e-10
//
//static unsigned int state_i = 0;
//static unsigned int STATE[R];
//static unsigned int z0, z1, z2;
//
//void InitWELLRNG512a(unsigned int *init) {
//	int j;
//	state_i = 0;
//	for (j = 0; j < R; j++)
//		STATE[j] = init[j];
//}
//
//double WELLRNG512a(void) {
//	z0 = VRm1;
//	z1 = MAT0NEG(-16, V0) ^ MAT0NEG(-15, VM1);
//	z2 = MAT0POS(11, VM2);
//	newV1 = z1                  ^ z2;
//	newV0 = MAT0NEG(-2, z0) ^ MAT0NEG(-18, z1) ^ MAT3NEG(-28, z2) ^ MAT4NEG(-5, 0xda442d24U, newV1);
//	state_i = (state_i + 15) & 0x0000000fU;
//	return ((double)STATE[state_i]) * FACT;
//}


IMPLEMENT_SINGLETON(CRandomWell512)


//CRandomWell512& rand_512(){
// static CRandomWell512 r;
//
// return r;
//}
unsigned int CRandomWell512::SeedDiffer()
{
	return 5 + (rand() % 500);
}

CRandomWell512::CRandomWell512()
{
	//InitSeedStatic();
	InitSeedRandomize();
}

//CRandomWell512::CRandomWell512(unsigned long * state)
//{
//	//InitSeedStatic();
//	InitSeedRandomize(state);
//}


//void CRandomWell512::InitSeedStatic()
//{
//	m_idxState = 0;
//	unsigned int differ = SeedDiffer();
//	unsigned int timecurrent = static_cast<unsigned int>(time(NULL));
//	for (int i = 0; i < 16; ++i)
//	{
//		m_states[i] = timecurrent + (i * differ);
//	}
//}

void CRandomWell512::InitSeedRandomize()
{
	m_idxState = 0;
	unsigned int randseed = 0;
	for (int i = 0; i < 16; ++i)
	{
		randseed = SeedDiffer();
		bool soleness = true;
		while (1)
		{
			soleness = true;
			for (int j = 0; j < i; ++j)
			{
				if (randseed == m_states[j])
				{
					soleness = false;
					break;
				}
			}
			if (false == soleness) { randseed = SeedDiffer(); }
			else { break; }
		}

		m_states[i] = randseed;
	}
}

void CRandomWell512::InitSeedRandomize(unsigned long * state)
{
	for (int i = 0; i < 16; ++i)
	{
		m_states[i] = state[i];
	}

}

unsigned int CRandomWell512::GetValue()
{
	unsigned int a, b, c, d;

	a = m_states[m_idxState];
	c = m_states[(m_idxState + 13) & 15];
	b = a ^ c ^ (a << 16) ^ (c << 15);
	c = m_states[(m_idxState + 9) & 15];
	c = c ^ (c >> 11);
	a = m_states[m_idxState] = b ^ c;
	d = a ^ ((a << 15) & 0xda442d24U);
	m_idxState = (m_idxState + 15) & 15;
	a = m_states[m_idxState];
	m_states[m_idxState] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);

	return m_states[m_idxState];
}

unsigned int CRandomWell512::GetValue(unsigned int minValue, unsigned int maxValue)
{
	return minValue + (GetValue() % (maxValue - minValue));
}

unsigned int CRandomWell512::GetValue(unsigned int maxValue)
{
	return GetValue() % maxValue;
}

float CRandomWell512::GetValueF()
{
	unsigned int randValue = GetValue();

	union { unsigned long ul; float f; } p;
	p.ul = (((randValue = randValue * 16807) & 0x007fffff) - 1) | 0x3f800000;

	return p.f - 1.f;
}

float CRandomWell512::GetValueF(float minValue, float maxValue)
{
	return minValue + ((maxValue - minValue) * GetValueF());
}

float CRandomWell512::GetValueF(float maxValue)
{
	return GetValueF()* maxValue;
}

_ulong CRandomWell512::Free()
{
	_ulong dwRefCount = 0;


	return dwRefCount;
}
