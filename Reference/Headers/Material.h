#pragma once

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)

class DLL_EXPORT CMaterial : public CComponent
{
public:
	enum MATTYPE { MATTYPE_NORMAL, MATTYPE_DULL, MATTYPE_SHINY, MATTYPE_TEST, MATTYPE_END };
private:
	explicit CMaterial(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMaterial(const CMaterial& rhs);
	virtual ~CMaterial() = default;
public:
	HRESULT SetUp_MaterialInfo(D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, D3DXCOLOR Emissive, const _float& fPower);
	HRESULT SetUp_MaterialInfo(MATTYPE eType);
	HRESULT SetUp_MaterialToGraphicDev();
	const D3DMATERIAL9& Get_MaterialInfo() { return m_Material; }
public:
	HRESULT Ready_Material(MATTYPE eType);
private:
	D3DMATERIAL9		m_Material;
public:
	virtual CComponent* Clone(void);
	static CMaterial* Create(LPDIRECT3DDEVICE9 pGraphicDev, MATTYPE eType);
private:
	virtual _ulong Free() final;
};

END

