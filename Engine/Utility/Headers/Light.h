#pragma once

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)

class DLL_EXPORT CLight : public CComponent
{
public:
	enum LIGHTCOLOR { WHITE, RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, CORNFLOWER_BLUE, BLACK, TEST, DIRECTIONAL_LIGHT, LIGHTTYPE_END};
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLight(const CLight& rhs);
	virtual ~CLight() = default;
public:
	HRESULT SetUp_LightPosition(const _vec3& vPos);
	HRESULT SetUp_LightInfo(D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular,  const _float& fRadius = LIGHT_RANGE_DEFAULT);
	HRESULT SetUp_LightInfo(LIGHTCOLOR eType);
	//HRESULT SetUp_LightToGraphicDev();
	const D3DLIGHT9& Get_LightInfo() { return m_Light; }
public:
	HRESULT Ready_Light(LIGHTCOLOR eType);
	HRESULT Ready_Light(const D3DLIGHT9* pLightInfo);
	void Set_ShaderLightList(LPD3DXEFFECT pEffect, LIGHT_TYPE eType, _uint iIndex = 0);
	//void Set_ShaderShadow(LPD3DXEFFECT pEffect, LIGHT_TYPE eType, _uint iIndex = 0);
private:
	D3DLIGHT9		m_Light;
public:
	virtual CComponent* Clone(void);
	static CLight* Create(LPDIRECT3DDEVICE9 pGraphicDev, LIGHTCOLOR eType);
	static CLight* Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo);
private:
	virtual _ulong Free() final;
};

END

