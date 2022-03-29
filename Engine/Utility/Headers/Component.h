#pragma once

#include "base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class DLL_EXPORT CComponent abstract 
	:public CBase
{
public:
	enum COM_UPDATETYPE {UPDATETYPE_NOTIFY, UPDATETYPE_SUSTAIN, UPDATETYPE_END};

/*ctor dtor*/
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();

/*FUNCTION*/


/*MEMBER*/
protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
	_bool				m_bIsClone = false;


/* FUNCTION related ctor/dtor */
public:
	virtual CComponent*		Clone() = 0;
protected:
	virtual _ulong			Free() override;

};

END