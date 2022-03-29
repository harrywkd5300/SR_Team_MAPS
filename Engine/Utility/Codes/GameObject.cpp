#include "..\Headers\GameObject.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
	, m_bIsDead(false)
	, m_pLightMgr(CLight_Manager::GetInstance())
{
	m_pGraphicDev->AddRef();
	m_pLightMgr->AddRef();
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag)
{
	for (_int i = 0; i < CComponent::UPDATETYPE_END; ++i)
	{
		CComponent* pFindComponent = Find_Component(i, pComponentTag);
		
		if (nullptr != pFindComponent)
			return pFindComponent;
	}

	MSG_BOX("Get_Component failed in CGameObject err:00003");
	return nullptr;	/*Component is not founded*/

}

HRESULT CGameObject::Ready_GameObject()
{
	return NOERROR;
}

_int CGameObject::Update_GameObject(const _float & fTimeDelta)
{
	return 0;
}

_int CGameObject::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return 0;
}

void CGameObject::Render_GameObject()
{
}

void CGameObject::Render_Shadow(_matrix CameraWorldMatrix)
{

}

CComponent * CGameObject::Find_Component(const _uint & UpdateTypeIndex, const _tchar * pComponentTag)
{

	auto iter = find_if(m_mapComponent[UpdateTypeIndex].begin(), m_mapComponent[UpdateTypeIndex].end(), [&](const MAPCOMPONENT::value_type Pair) {return !lstrcmp(Pair.first, pComponentTag);  });

	if (iter == m_mapComponent[UpdateTypeIndex].end())
		return nullptr;

	return iter->second;
}

_ulong CGameObject::Free()
{
	for (size_t i = 0; i < CComponent::UPDATETYPE_END; ++i)
	{
		for (auto& Pair : m_mapComponent[i])
		{
			Safe_Release(Pair.second);
		}
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pLightMgr);

	return 0;
}