#pragma once

#include "engine_defines.h"
#include "vibuffer.h"

BEGIN(Engine)

class DLL_EXPORT CRectUI_Texture final
	:public CVIBuffer
{
/*ctor dtor*/
private:
	explicit CRectUI_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRectUI_Texture(const CRectUI_Texture& rhs);
	virtual ~CRectUI_Texture() =default;

/*FUNCTION*/
public:
	HRESULT Ready_Buffer();

	void	Set_Parameter(const _float& fPosX, const _float& fPosY, const _float& fSizeX, const _float& fSizeY);


/*FUNCTION related ctor dtor*/
public:
	static CRectUI_Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual _ulong Free();
};

END
