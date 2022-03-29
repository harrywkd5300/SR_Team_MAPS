#include "..\Headers\Scene.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDevice)
	:m_pGraphicDev(pGraphicDevice)
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pCamera_Manager(CCamera_Manager::GetInstance())
{
	m_pGraphicDev->AddRef();
	m_pObject_Manager->AddRef();
	m_pComponent_Manager->AddRef();
	m_pCamera_Manager->AddRef();
}

HRESULT CScene::Ready_Renderer(CRenderer * pRenderer)
{
	if (nullptr == pRenderer)
	{
		MSG_BOX("Renderer isn't allocated err: 68634");
		return E_FAIL;
	}

	m_pRenderer = pRenderer;
	m_pRenderer->AddRef();

	return NO_ERROR;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	_int		iExitCode = 0;

	if (iExitCode = m_pObject_Manager->Update_Object_Manager(fTimeDelta))
		return iExitCode;

	if (iExitCode = m_pCamera_Manager->Update_Camera(fTimeDelta))
		return iExitCode;

	return iExitCode;
}

_int CScene::LastUpdate_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	_int		iExitCode = 0;

	if (iExitCode = m_pObject_Manager->LastUpdate_Object_Manager(fTimeDelta))
		return iExitCode;

	if (iExitCode = m_pCamera_Manager->LastUpdate_Camera(fTimeDelta))
		return iExitCode;

	return iExitCode;
}

void CScene::Render_Scene()
{
	if (nullptr == m_pRenderer)
		return;

	m_pRenderer->Render_Renderer();

}

_ulong CScene::Free()
{
	//
	/*decrease RefCount*/
	//
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pCamera_Manager);

	return 0;
}
