#pragma once

#include "Engine_Defines.h"
#include "base.h"

///* ***************************************************************************** */
///* Copyright:      Francois Panneton and Pierre L'Ecuyer, University of Montreal */
///*                 Makoto Matsumoto, Hiroshima University                        */
///* Notice:         This code can be used freely for personal, academic,          */
///*                 or non-commercial purposes. For commercial purposes,          */
///*                 please contact P. L'Ecuyer at: lecuyer@iro.UMontreal.ca       */
///* ***************************************************************************** */

//void InitWELLRNG512a(unsigned int *init);
//double WELLRNG512a(void);

BEGIN(Engine)


class DLL_EXPORT CRandomWell512 :public CBase
{
	DECLARE_SINGLETON(CRandomWell512)
private:
	explicit CRandomWell512();
	virtual ~CRandomWell512() = default;

protected:
	unsigned long m_states[16];
	unsigned int m_idxState;
	unsigned int SeedDiffer();

public:
	//void InitSeedStatic();
	void InitSeedRandomize();
	void InitSeedRandomize(unsigned long *state);
	unsigned int GetValue();
	unsigned int GetValue(unsigned int minValue, unsigned int maxValue);
	unsigned int GetValue(unsigned int maxValue);
	float GetValueF();
	float GetValueF(float minValue, float maxValue);
	float GetValueF(float maxValue);
private:
	virtual _ulong Free();
};

END