#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class DLL_EXPORT CTimer final
	: public CBase
{
/*ctor & dtor*/
private:
	explicit CTimer(void);
	virtual ~CTimer(void) = default;
	


/*FUNCTION*/
public:
	float Get_TimeDelta(void) const {return m_fTimeDelta;}


	HRESULT			Initialize(void);
	void			Compute_Timer(void); 
	
/*MEMBER*/
private:
	LARGE_INTEGER 			m_FrameTime;
	LARGE_INTEGER 			m_FixTime;
	LARGE_INTEGER 			m_LastTime;
	LARGE_INTEGER 			m_CpuTick;
	_float					m_fTimeDelta;

/*related ctor dtor*/
public:
	static CTimer*	Create(void);

private:
	virtual _ulong	Free(void);
};

END
