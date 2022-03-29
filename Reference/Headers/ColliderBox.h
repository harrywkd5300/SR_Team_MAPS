#pragma once

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)

class DLL_EXPORT CColliderBox : public CComponent
{
public:
	enum COLTYPE { COL_AABB, COL_BILLBOARD, COL_END };
private:
	explicit CColliderBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CColliderBox(const CColliderBox& rhs);
	virtual ~CColliderBox() = default;

public:
	HRESULT Ready_ColliderBox(COLTYPE eType);

private:
	_float fSizeX = 0.f;
	_float fSIzeY = 0.f;

	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;

public:
	virtual CComponent* Clone(void);
	static CColliderBox* Create(LPDIRECT3DDEVICE9 pGraphicDev, COLTYPE eType,_float fSizeX, _float fSizeY);
private:
	virtual _ulong Free() final;
};

END

