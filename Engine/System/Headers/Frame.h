#pragma once

#include "Engine_defines.h"
#include "base.h"

BEGIN(Engine)

class DLL_EXPORT CFrame final
	:public CBase
{
/*ctor dtor*/
private:
	explicit CFrame(void);
	virtual ~CFrame(void) = default;


/*FUNCTION*/
public:
	HRESULT Initialize(const _float& fCallCnt);
	_bool Permit_Call(const _float& fTimeDelta);


/*MEMBER*/
private:
	_float		m_fTimeAcc = 0.f;
	_float		m_fTimePerSec = 0.f;



/*related ctor dtor*/
public: 
	static CFrame* Create(const _float& fCallCnt);

private:
	virtual _ulong Free() final;
};

END