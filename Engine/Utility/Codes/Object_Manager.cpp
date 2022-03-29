#include "..\Headers\Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"


IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
	:m_iContainerSize(0), m_pMapLayer(nullptr)
{
}


CComponent * CObject_Manager::Get_Component(const _uint & iSceneIndex, const _tchar * pLayerTag, const _uint & iLayerIndex, const _tchar * pComponentTag)
{
	if (iSceneIndex >= m_iContainerSize)
		goto exception;

	CLayer* pLayer = Find_Layer(iSceneIndex, pLayerTag);
	
	if (nullptr == pLayer)
		goto exception;

	return pLayer->Get_Component(iLayerIndex,pComponentTag);

exception:
	MSG_BOX("Get_Component Failed in CObject_Manager err: 00000");
	return nullptr;
}

CLayer * CObject_Manager::Get_Layer(const _uint & iSceneIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pMapLayer || iSceneIndex >= m_iContainerSize)
		return nullptr;

	auto iter = find_if(m_pMapLayer[iSceneIndex].begin(), m_pMapLayer[iSceneIndex].end(), [&](const MAPLAYER::value_type Pair) {return !lstrcmp(Pair.first, pLayerTag);  });

	if (iter == m_pMapLayer[iSceneIndex].end())
		return nullptr;

	return iter->second;
}

HRESULT CObject_Manager::Reserve_Container(const _uint& iContainerSize)
{
	if (nullptr != m_pMapLayer)
	{
		MSG_BOX("LayerMap Already Reserved");
		return E_FAIL;
	}

	m_pMapLayer = new MAPLAYER[iContainerSize];
	m_iContainerSize = iContainerSize;

	return NO_ERROR;
}

HRESULT CObject_Manager::Add_GameObject(const _uint & iSceneIdx, const _tchar * pLayerTag, CGameObject * pGameObject)
{
 	if (iSceneIdx >= m_iContainerSize || m_pMapLayer == nullptr)
 	{
 		MSG_BOX("map isnt Allocated or ContainerSize over");
 		return E_FAIL;
 	}
 
 	CLayer* pFindLayer = Find_Layer(iSceneIdx, pLayerTag);
 	
	if (nullptr != pFindLayer) /*used layertag*/
	{
		pFindLayer->Add_GameObject(pGameObject);
	}
	else /*New LayerTag*/
	{
		CLayer* pLayer = CLayer::Create();

		if (nullptr == pLayer)
			return E_FAIL;

		pLayer->Add_GameObject(pGameObject);
		m_pMapLayer[iSceneIdx].insert(MAPLAYER::value_type(pLayerTag, pLayer));
	}

	return NOERROR;
}

HRESULT CObject_Manager::Ready_Object_Manager()
{
	return NO_ERROR;
}

_int CObject_Manager::Update_Object_Manager(const _float & fTimeDelta)
{
	_int iExitCode = 0;

	for (_uint i = 0; i < m_iContainerSize; ++i)
	{
		for (auto& Pair : m_pMapLayer[i])
		{
			iExitCode = Pair.second->Update_Layer(fTimeDelta);
			
			if (iExitCode & 0x80000000)
				return iExitCode;
		}
	}

	return iExitCode;
}

_int CObject_Manager::LastUpdate_Object_Manager(const _float & fTimeDelta)
{
	_int iExitCode = 0;

	for (_uint i = 0; i < m_iContainerSize; ++i)
	{
		for (auto& Pair : m_pMapLayer[i])
		{
			iExitCode = Pair.second->LastUpdate_Layer(fTimeDelta);

			if (iExitCode & 0x80000000)
				return iExitCode;
		}
	}

	return iExitCode;
}

void CObject_Manager::Render_Object_Manager()
{
	for (_uint i = 0; i < m_iContainerSize; ++i)
	{
		for (auto& Pair : m_pMapLayer[i])
			Pair.second->Render_Layer();
	}
}

HRESULT CObject_Manager::Release_TargetContainer(const _uint & iSceneIndex)
{
	if (iSceneIndex >= m_iContainerSize)
	{
		MSG_BOX("SceneIndex over containersize err: 22196");
		return E_FAIL;
	}
	
	for (auto& Pair : m_pMapLayer[iSceneIndex])
		Safe_Release(Pair.second);

	m_pMapLayer[iSceneIndex].clear();

	return NOERROR;
}

HRESULT CObject_Manager::Release_TargetLayer(const _uint & iSceneIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pMapLayer || iSceneIndex >= m_iContainerSize)
		return E_FAIL;

	auto iter = find_if(m_pMapLayer[iSceneIndex].begin(), m_pMapLayer[iSceneIndex].end(), [&](const MAPLAYER::value_type Pair) {return !lstrcmp(Pair.first, pLayerTag);  });

	if (iter == m_pMapLayer[iSceneIndex].end())
		return E_FAIL;

	Safe_Release(iter->second);
	m_pMapLayer[iSceneIndex].erase(iter);

	return NOERROR;
}


CLayer * CObject_Manager::Find_Layer(const _uint& iSceneIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pMapLayer || iSceneIndex >= m_iContainerSize)
		return nullptr;

	auto iter = find_if(m_pMapLayer[iSceneIndex].begin(), m_pMapLayer[iSceneIndex].end(), [&](const MAPLAYER::value_type Pair) {return !lstrcmp(Pair.first, pLayerTag);  });

	if (iter == m_pMapLayer[iSceneIndex].end())
		return nullptr;

	return iter->second;
}

_ulong CObject_Manager::Free()
{
	_ulong dwRefCount = 0;

	for (size_t i = 0; i < m_iContainerSize; i++)
	{
		for_each(m_pMapLayer[i].begin(), m_pMapLayer[i].end(), CReleaseMap());
		m_pMapLayer[i].clear();
	}

	return dwRefCount;
}
