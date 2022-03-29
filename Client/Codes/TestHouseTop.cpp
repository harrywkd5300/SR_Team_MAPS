#include "stdafx.h"
#include "..\Headers\TestHouseTop.h"

#include "Cube.h"

CTestHouseTop::CTestHouseTop(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pRendererCom(nullptr)
	, m_pBufferCom(nullptr)
{
}


HRESULT CTestHouseTop::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_CubeLoad()))
		return E_FAIL;

	return NOERROR;
}

_int CTestHouseTop::Update_GameObject(const _float & fTimeDelta)
{
	if (true == m_bIsDead)
		return 1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (m_pCubeObj.empty())
		return 0;

	auto iter = m_pCubeObj.begin();
	for (; iter != m_pCubeObj.end(); ++iter)
	{
		dynamic_cast<CCube*>((*iter))->Update_GameObject(fTimeDelta);
	}

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);

	return 0;
}

void CTestHouseTop::Render_GameObject(void)
{
	if (nullptr == m_pRendererCom)
		return;

	if (m_pCubeObj.empty())
		return;

	auto iter = m_pCubeObj.begin();
	for (; iter != m_pCubeObj.end(); ++iter)
	{
		dynamic_cast<CCube*>((*iter))->Render_GameObject();
	}
}

HRESULT CTestHouseTop::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;

	/*Add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();

	/*add Cube buffer*/
	pComponent = m_pBufferCom = dynamic_cast<CCube_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeTex"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Buffer_Cube", pComponent));
	if (nullptr == m_pBufferCom)
		MSG_BOX("Buffer_Cube Clone Failed err:50624");
	m_pBufferCom->AddRef();

	return NOERROR;

exception:
	MSG_BOX("Effect Component Ready Failed err: 13115");
	return E_FAIL;
}

HRESULT CTestHouseTop::Ready_CubeLoad()
{
	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../../Data/MainMapTop.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,		//기본값으로 파일을 생성하라는옵션.
		NULL
	);

	auto& iter = m_pCubeObj.begin();
	for (; iter != m_pCubeObj.end(); ++iter)
	{
		Safe_Release((*iter));
	}
	m_pCubeObj.clear();

	while (true)
	{
		STROBJECT* pObjectData = new STROBJECT;

		ZeroMemory(pObjectData, sizeof(STROBJECT));

		ReadFile(hFile, pObjectData, sizeof(STROBJECT), &dwByte, NULL);

		if (dwByte == 0)
		{
			Safe_Delete(pObjectData);
			break;
		}
		/*Create Cube*/
		CGameObject* pObject = CCube::Create(m_pGraphicDev, pObjectData->m_vecPos, pObjectData->m_vecRot, pObjectData->m_vecScale,
			pObjectData->m_iTextureNum, pObjectData->m_strKey, m_pBufferCom);
		if (nullptr == pObject)
		{
			MSG_BOX("Add_GameObject failed on Scene_Logo err:50620");
			return E_FAIL;
		}
		m_pCubeObj.push_back(pObject);
	}
	CloseHandle(hFile);
	return NOERROR;
}

CTestHouseTop * CTestHouseTop::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestHouseTop*	pInstance = new CTestHouseTop(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CTestHouseTop Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CTestHouseTop::Free(void)
{
	_ulong		dwRefCnt = 0;

	auto& iter = m_pCubeObj.begin();
	
	for (; iter != m_pCubeObj.end(); ++iter)
	{
		dwRefCnt += Safe_Release((*iter));
	}

	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
