#pragma once

#include "vibuffer.h"
#include "engine_defines.h"

BEGIN(Engine)

class CGameObject;

class DLL_EXPORT CBillboard_Buffer
	:public CVIBuffer
{
/*ctor dtor*/
private:
	explicit CBillboard_Buffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBillboard_Buffer(const CBillboard_Buffer& rhs);
	virtual ~CBillboard_Buffer() = default;

/*FUNCTION*/
public:
	HRESULT Ready_Buffer(const _float & fSizeX, const _float & fSizeY, const _float & fSizeZ);
	void SetTarget(CGameObject* pTarget);
	virtual void Set_Size(const _float & fSizeX, const _float & fSizeY, const _float & fSizeZ);
	void Set_Position(const _matrix& matWorld);
/*MEMBER*/
private:
	CGameObject* m_pTarget = nullptr;

/*FUNCTION related ctor dtor*/
public:
	static CBillboard_Buffer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float & fSizeX, const _float & fSizeY, const _float & fSizeZ);
	virtual CComponent* Clone();

private:
	virtual _ulong Free();
};

END