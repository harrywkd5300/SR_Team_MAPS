#pragma once

#include "Engine_Defines.h"
#include "Base.h"
#include "timer.h"

BEGIN(Engine)

class DLL_EXPORT CTimer_Manager final 
	: public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)

/*ctor dtor*/
private:
	explicit CTimer_Manager(void);
	virtual ~CTimer_Manager(void) = default;


/*FUNCTION*/
public:
	_float Get_TimeDelta(const TCHAR* pTimerTag) const;

	HRESULT Add_Timer(const TCHAR* pTimerTag, CTimer* pTimer);
	void Compute_Timer(const TCHAR* pTimerTag);


private:
	CTimer* Find_Timer(const TCHAR* pTimerTag) const;



/*MEMBER*/
private:
	map<const TCHAR*, CTimer*>			m_mapTimer;
	typedef map<const TCHAR*, CTimer*>	MAPTIMER;


/*related ctor dtor*/
private:
	virtual _ulong Free(void) final;
};

END