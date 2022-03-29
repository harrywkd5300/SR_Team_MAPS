#pragma once

#include "Engine_Defines.h"
#include "base.h"
#include "frame.h"

BEGIN(Engine)

class DLL_EXPORT CFrame_Manager final
	: public CBase
{
	DECLARE_SINGLETON(CFrame_Manager)

/*ctor dtor*/
private:
	explicit CFrame_Manager(void);
	virtual ~CFrame_Manager(void) = default;

/*FUNCTION*/
public:
	HRESULT		Add_Frame(const TCHAR* pFrameTag, const _float& fCallCnt);
	_bool		Permit_Call(const TCHAR* pFrameTag, const _float& fTimeDelta);

private:
	CFrame* Find_Frame(const TCHAR* pFrameTag);


/*MEMBER*/
private:
	map<const TCHAR*, CFrame*>			m_mapFrame;
	typedef map<const TCHAR*, CFrame*>	MAPFRAME;


/*related ctor dtor*/
private:
	virtual _ulong Free() final;
};

END
