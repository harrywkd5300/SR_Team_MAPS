#include "..\Headers\Light_Manager.h"
#include "..\Headers\Light.h"
#include "Component_Manager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CLight_Manager)

Engine::CLight_Manager::CLight_Manager(void)
	:m_DirectLight(nullptr)
{
	
}

//
//HRESULT Engine::CLight_Manager::Add_RenderLight(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, LIGHT_TYPE eType)
//{
//	if(pLightInfo == NULL)
//		return E_FAIL;
//	CLight*		pLight = CLight::Create(pGraphicDev, pLightInfo);
//
//	if(NULL == pLight)
//		return E_FAIL;	
//	
//	if (eType == LIGHT_TYPE::POINT_LIGHT )//|| eType == LIGHT_TYPE::SPOT_LIGHT)
//	{
//		m_LightList.push_back(pLight);
//	}
//	else if(eType == LIGHT_TYPE::DIRECTIONAL_LIGHT)
//	{
//		if (m_DirectLight != nullptr)
//		{
//			Safe_Release(m_DirectLight);
//		}
//		m_DirectLight = pLight;
//	}
//	else
//	{
//		return E_FAIL;
//	}
//	return S_OK;
//}

HRESULT CLight_Manager::Add_RenderLight(CLight* pLight, LIGHT_TYPE eType)
{
	if(pLight == NULL)
		return E_FAIL;

	if(NULL == pLight)
		return E_FAIL;	
	
	if (eType == LIGHT_TYPE::POINT_LIGHT )//|| eType == LIGHT_TYPE::SPOT_LIGHT)
	{
		m_LightList.push_back(pLight);
		pLight->AddRef();
	}
	else if(eType == LIGHT_TYPE::DIRECTIONAL_LIGHT)
	{
		if (m_DirectLight != nullptr)
		{
			Safe_Release(m_DirectLight);
		}
		m_DirectLight = pLight;
		//m_DirectLight->AddRef();
		//pLight->AddRef();
	}
	else
	{
		return E_FAIL;
	}
	return S_OK;
}


void Engine::CLight_Manager::Set_ShaderLightList(LPD3DXEFFECT pEffect)
{

	_uint index = 0;
	int iLightArrSize = m_LightList.size();
	pEffect->SetValue("g_iLightArrSize", &iLightArrSize, sizeof(iLightArrSize));

	//cout << iLightArrSize << endl;

	for (auto& LightList : m_LightList)
	{
		LightList->Set_ShaderLightList(pEffect, LIGHT_TYPE::POINT_LIGHT, index);
		++index;
	}

	if (m_DirectLight != nullptr)
	{
		m_DirectLight->Set_ShaderLightList(pEffect, LIGHT_TYPE::DIRECTIONAL_LIGHT);
	}
}

//void CLight_Manager::Set_ShaderShadow(LPD3DXEFFECT pEffect)
//{
//
//	_uint index = 0;
//	int iLightArrSize = m_LightList.size();
//	pEffect->SetValue("g_iLightArrSize", &iLightArrSize, sizeof(iLightArrSize));
//
//	cout << iLightArrSize << endl;
//
//	for (auto& LightList : m_LightList)
//	{
//		LightList->Set_ShaderLightList(pEffect, LIGHT_TYPE::POINT_LIGHT, index);
//		++index;
//	}
//
//	if (m_DirectLight != nullptr)
//	{
//		m_DirectLight->Set_ShaderLightList(pEffect, LIGHT_TYPE::DIRECTIONAL_LIGHT);
//	}
//
//}

void CLight_Manager::Clear_LightList()
{
	for (auto& LightList : m_LightList)
	{
		Safe_Release(LightList);
	}
	m_LightList.clear();

	//Safe_Release(m_DirectLight);
}

CLight * CLight_Manager::GetLight(LIGHT_TYPE eType, int iIndex)
{
	if(eType == LIGHT_TYPE::DIRECTIONAL_LIGHT)
	{
		return m_DirectLight;
	}
	else if (eType == LIGHT_TYPE::POINT_LIGHT)
	{
		int index = 0;
		for (auto pLight : m_LightList)
		{
			if (index == iIndex)
			{
				return pLight;
			}
			++index;
		}
	}
	return nullptr;
}

CLight * CLight_Manager::Get_NearLight(const _vec3& vPos)
{
	CLight* pNearLight = m_DirectLight;

	_float fMinDist = 1.0625; // 1.0f 근처의 값이면 OK

	_uint iIndex = 0;
	for (auto pLight : m_LightList)
	{
		_float fTmpDist = D3DXVec3Length(&( (_vec3)pLight->Get_LightInfo().Position - vPos) ) / pLight->Get_LightInfo().Range;
		if (fTmpDist < fMinDist)
		{
			fMinDist = fTmpDist;
			pNearLight = pLight;
		}
	}
	
	return pNearLight;
}

_ulong Engine::CLight_Manager::Free(void)
{
	_ulong dwRefCount = 0;

	for (auto& LightList : m_LightList)
	{
		Safe_Release(LightList);
	}
	m_LightList.clear();

	Safe_Release(m_DirectLight);
	//Safe_Release(m_DirectLight);

	return dwRefCount;
}
