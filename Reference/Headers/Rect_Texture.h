#pragma once

#include "engine_defines.h"
#include "vibuffer.h"

BEGIN(Engine)

class DLL_EXPORT CRect_Texture final
	:public CVIBuffer
{
/*ctor dtor*/
private:
	explicit CRect_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRect_Texture(const CRect_Texture& rhs);
	virtual ~CRect_Texture() =default;

/*FUNCTION*/
public:
	HRESULT Ready_Buffer();


/*FUNCTION related ctor dtor*/
public:
	static CRect_Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual _ulong Free();
};

END
