#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

// Component들의 원본객체를 보관한다.
class CLight;
class DLL_EXPORT CLight_Manager : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)

private:
	explicit CLight_Manager();
	virtual ~CLight_Manager() = default;
public:
	//HRESULT Add_RenderLight(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, LIGHT_TYPE eType);
	HRESULT Add_RenderLight(CLight* pLight, LIGHT_TYPE eType);
	void	Set_ShaderLightList(LPD3DXEFFECT pEffect);
	//void	Set_ShaderShadow(LPD3DXEFFECT pEffect);
	void	Clear_LightList();
	CLight* GetLight(LIGHT_TYPE eType, int iIndex = 0);
	CLight* Get_NearLight(const _vec3& vPos);
private:
	CLight*			m_DirectLight = nullptr;
	list<CLight*>	m_LightList;
	typedef	list<CLight*>	LIGHTLIST;

private:
	virtual _ulong Free();
};

END
