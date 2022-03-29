#pragma once

#include "vibuffer.h"
#include "engine_defines.h"

BEGIN(Engine)

class DLL_EXPORT CCube_Color
	:public CVIBuffer
{
/*ctor dtor*/
private:
	explicit CCube_Color(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCube_Color(const CCube_Color& rhs);
	virtual ~CCube_Color() = default;

/*FUNCTION*/
public:
	HRESULT Ready_Buffer(const _float& fSizeX, const _float& fSizeY, const _float& fSizeZ);
	void	Set_Size(const _float& fSizeX, const _float& fSizeY, const _float& fSizeZ);


/*MEMBER*/
private:
	_vec3	m_vSize;

/*FUNCTION related ctor dtor*/
public:
	static CCube_Color* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fSizeX =1.f, const _float& fSizeY = 1.f, const _float& fSizeZ = 1.f);
	virtual CComponent* Clone();

private:
	virtual _ulong Free();
};

END