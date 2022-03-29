#include "..\Headers\Component_Manager.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
	:m_iContainerSize(0)
{
}

HRESULT CComponent_Manager::Reserve_ComponentContainer(const _uint& iReserveSize)
{
	if (nullptr != m_pMapComponents)
	{
		MSG_BOX("Component Map Already Resereved");
		return E_FAIL;
	}

	m_pMapComponents = new MAPCOMPONENT[iReserveSize];
	m_iContainerSize = iReserveSize;
	
	return NO_ERROR;
}

HRESULT CComponent_Manager::Add_Component(const _uint & iSceneIdx, const _tchar * pComponentTag, CComponent * pComponent)
{
	if (m_pMapComponents == nullptr || iSceneIdx >= m_iContainerSize)
	{
		MSG_BOX("Map isn't Allocated or ContainerSize over");
		return E_FAIL;
	}

	CComponent* pFindComponent = Find_Component(iSceneIdx, pComponentTag);
	
	if (nullptr != pFindComponent)
	{		
		MSG_BOX("Component tag already used err:00995");
		return E_FAIL;
	}
	
	m_pMapComponents[iSceneIdx].insert (MAPCOMPONENT::value_type(pComponentTag, pComponent));

	return NOERROR;
}

CComponent * CComponent_Manager::Clone_Component(const _uint & iSceneIdx, const _tchar * pComponentTag)
{
	CComponent* pComponent = nullptr;
	pComponent = Find_Component(iSceneIdx, pComponentTag);
	if (nullptr == pComponent)
		return nullptr;

	return pComponent->Clone();
}

HRESULT CComponent_Manager::Release_TargetContainer(const _uint & iSceneIndex)
{
	if (iSceneIndex >= m_iContainerSize)
	{
		MSG_BOX("SceneIndex over containersize err: 22198");
		return E_FAIL;
	}

	for (auto& Pair : m_pMapComponents[iSceneIndex])
		Safe_Release(Pair.second);

	m_pMapComponents[iSceneIndex].clear();

	return NOERROR;
}

CComponent * CComponent_Manager::Find_Component(const _uint& iSceneIdx, const _tchar * pComponentTag)
{
	if (nullptr == m_pMapComponents || iSceneIdx >= m_iContainerSize)
		return nullptr;

	auto iter = find_if(m_pMapComponents[iSceneIdx].begin(), m_pMapComponents[iSceneIdx].end(), [&](const MAPCOMPONENT::value_type Pair) {return !lstrcmp(Pair.first, pComponentTag); });

	if (iter == m_pMapComponents[iSceneIdx].end())
		return nullptr;

	return iter->second;
}


_ulong CComponent_Manager::Free()
{
	for (size_t i = 0; i < m_iContainerSize; ++i)
	{
 		_ulong dwRefCount = 0;
 		for (auto& Pair : m_pMapComponents[i])
 		{
			if (dwRefCount = Safe_Release(Pair.second))
 				MSG_BOX("Component Release Failed err: 90767");
 		}
		m_pMapComponents[i].clear();
	}
	Safe_Delete_Array(m_pMapComponents);

	return 0;
}

