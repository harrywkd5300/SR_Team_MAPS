#pragma once
#include "Component.h"

BEGIN(Engine)

class DLL_EXPORT CResources 
	:public CComponent
{
/*ctor dtor*/
protected:
	explicit CResources(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CResources(const CResources& rhs);
	virtual ~CResources();

/*FUNCTION*/
protected:
	virtual _ulong Free() override;
};

END