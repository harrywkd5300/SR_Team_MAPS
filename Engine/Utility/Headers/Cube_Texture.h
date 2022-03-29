#pragma once

#include "vibuffer.h"
#include "engine_defines.h"

BEGIN(Engine)

class DLL_EXPORT CCube_Texture
	:public CVIBuffer
{

/*ctor dtor*/
private:
	explicit CCube_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCube_Texture(const CCube_Texture& rhs);
	virtual ~CCube_Texture() = default;

/*FUNCTION*/
public:

public:
	HRESULT Ready_Buffer();
	void Set_Position(const _matrix& matWorld);

private:
	void CalcTangentVector(VTXCUBETEX& pVertex1, VTXCUBETEX& pVertex2, VTXCUBETEX& pVertex3, _vec3 vNormal);


/*MEMBER*/
private:


/*FUNCTION related ctor dtor*/
public:
	static CCube_Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual _ulong Free();
};

END