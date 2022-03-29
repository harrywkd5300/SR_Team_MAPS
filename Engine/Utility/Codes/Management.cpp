#include "Management.h"
#include "Scene.h"
#include "Object_Manager.h"

IMPLEMENT_SINGLETON(CManagement)


CManagement::CManagement()
	:m_pCurScene(nullptr), m_pNewScene(nullptr)
{

}

HRESULT CManagement::Ready_Management(const _uint& iReserveSize)
{
	if (FAILED(CObject_Manager::GetInstance()->Reserve_Container(iReserveSize)))
		return E_FAIL;
	
	if (FAILED(CComponent_Manager::GetInstance()->Reserve_ComponentContainer(iReserveSize)))
		return E_FAIL;
	
	return NO_ERROR;
}

_int CManagement::Update_Management(const _float& fTimeDelta)
{
	if (nullptr == m_pCurScene)
		return -1;

	/*scene change*/
	if (nullptr != m_pNewScene)
	{
		if (Safe_Release(m_pCurScene))
		{
			MSG_BOX("scene change failed: Old Scene Release Fail err:84192");
			return -1;
		}
		m_pCurScene = m_pNewScene;
		m_pNewScene = nullptr;
	}

	_int iExitCode = 0;

	if (iExitCode = m_pCurScene->Update_Scene(fTimeDelta))
		return iExitCode;

	if (iExitCode = m_pCurScene->LastUpdate_Scene(fTimeDelta))
		return iExitCode;

	return iExitCode;
}

void CManagement::Render_Management()
{
	if (nullptr == m_pCurScene)
		return;

	m_pCurScene->Render_Scene();
}

HRESULT CManagement::Set_CurScene(CScene* pNewScene,CRenderer* pRenderer)
{
	if (nullptr == pNewScene || nullptr == pRenderer)
		return E_FAIL;

	if (nullptr != m_pCurScene)
		m_pNewScene = pNewScene;
	else
		m_pCurScene = pNewScene;

	if (FAILED(pNewScene->Ready_Renderer(pRenderer)))
		return E_FAIL;
	
	return NOERROR;
}

_ulong CManagement::Free()
{
	_ulong dwRefCount = 0;

	if (dwRefCount = Safe_Release(m_pCurScene))
		MSG_BOX("Scene Release Failed err: 27477");


	return dwRefCount;
}
