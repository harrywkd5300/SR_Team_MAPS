#include "..\Headers\Layer.h"
#include "GameObject.h"


CLayer::CLayer()
{
}

CComponent * CLayer::Get_Component(const _uint & iLayerIndex, const _tchar * pComponentTag)
{
	if (iLayerIndex >= m_ObjList.size())
		goto exception;
	{
		OBJLIST::iterator iter = m_ObjList.begin();

		for (size_t i = 0; i < iLayerIndex; ++i)
			++iter;
		
		return (*iter)->Get_Component(pComponentTag);
	}

exception:
	MSG_BOX("Get Component Failed in CLayer err:00001");
	return nullptr;
}

HRESULT CLayer::Ready_Layer()
{
	return NO_ERROR;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	_int iExitCode = 0;

	auto& iter = m_ObjList.begin();

	for ( ; iter != m_ObjList.end() ; )
	{
		iExitCode = (*iter)->Update_GameObject(fTimeDelta);
		
		if(1 == iExitCode)	/*Object Is Dead*/
		{
			Safe_Release((*iter));
			iter = m_ObjList.erase(iter);
		}
		else if (iExitCode & 0x80000000) /* exit code < 0*/
		{
			return iExitCode;
		}
		else
			++iter;
	}

	return iExitCode;
}

_int CLayer::LastUpdate_Layer(const _float & fTimeDelta)
{
	_int iExitCode = 0;

	auto& iter = m_ObjList.begin();

	for (; iter != m_ObjList.end(); )
	{
		iExitCode = (*iter)->LastUpdate_GameObject(fTimeDelta);

		if (1 == iExitCode)	/*Object Is Dead*/
		{
			Safe_Release((*iter));
			iter = m_ObjList.erase(iter);
		}
		else if (iExitCode & 0x80000000) /* exit code < 0*/
		{
			return iExitCode;
		}
		else
			++iter;
	}

	return iExitCode;
}

void CLayer::Render_Layer()
{
	for (auto& pObject : m_ObjList)
		pObject->Render_GameObject();

}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_ObjList.push_back(pGameObject);
	return NOERROR;
}

void CLayer::Release_TargetObject(const _uint& iArrayIdx)
{
	auto& iter = m_ObjList.begin();
	for (_uint i = 0; i < iArrayIdx; ++i)
		++iter;

	(*iter)->Set_IsDead();
}

CLayer* CLayer::Create()
{
	CLayer* pInstance = new CLayer();

	if (FAILED(pInstance->Ready_Layer()))
	{
		MSG_BOX("Layer Class Creation Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CLayer::Free()
{
	_ulong dwRefCount = 0;

	for (auto& pGameObject : m_ObjList)
	{
		Safe_Release(pGameObject);
	}
	m_ObjList.clear();

	//if (0 != dwRefCount)
	//	MSG_BOX("Layer Release Failed err:99604");

	return dwRefCount;
}
