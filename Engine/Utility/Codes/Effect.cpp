#include "..\Headers\Effect.h"
#include "Texture.h"
#include "Light.h"
#include "Light_Manager.h"
CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CVIBuffer(pGraphicDevice), m_pLight(nullptr), m_pLightMgr(nullptr)
{
	ZeroMemory(&m_tInfo, sizeof(EFTINFO));
	m_tInfo._isAlive = false;
}

CEffect::CEffect(const CEffect& rhs)
	: CVIBuffer(rhs), m_tInfo(rhs.m_tInfo), m_pLight(nullptr), m_pLightMgr(nullptr)
{

}

void CEffect::Clone_Effect(CEffect * pCopyEffect)
{
	if (pCopyEffect == nullptr)
	{
		return;
	}
	m_tInfo = pCopyEffect->m_tInfo;

}

void CEffect::Set_Light(D3DXCOLOR color, float fRange)
{
	if (m_pLightMgr == nullptr)
	{
		m_pLightMgr = CLight_Manager::GetInstance();
		m_pLightMgr->AddRef();
	}
	if (m_pLight == nullptr)
	{
		m_pLight = CLight::Create(m_pGraphicDev, CLight::LIGHTCOLOR::WHITE);
		//m_pLight->SetUp_LightInfo(color, color, color, fRange);
	}
	m_pLight->SetUp_LightInfo(color, color, color, fRange);
	m_tInfo._Lightcolor = color;
	m_tInfo._fLightRange = fRange;
	m_tInfo._isLight = true;
}

_ulong CEffect::Free()
{
	//_ulong dwRefCount = 0;

	//dwRefCount = CComponent::Free();
	Safe_Release(m_pLight);
	Safe_Release(m_pLightMgr);
	//return dwRefCount;
	return CVIBuffer::Free();
}

