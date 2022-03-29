#include "stdafx.h"
#include "..\Headers\Tool_Object.h"


CTool_Object::CTool_Object(LPDIRECT3DDEVICE9 pGraphicDev, CString strName)
	:CGameObject(pGraphicDev)
{
	m_strName = strName;
}


HRESULT CTool_Object::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CTool_Object::Update_GameObject(const _float & fTimeDelta)
{
	if (true == m_bIsDead)
		return 1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (m_pCubeObj.empty())
		return 0;

	auto iter = m_pCubeObj.begin();
	for (; iter != m_pCubeObj.end();)
	{
		int iExitCode = dynamic_cast<CTool_Cube*>((*iter))->Update_GameObject(fTimeDelta);
		if (1 == iExitCode)	/*Object Is Dead*/
		{
			Safe_Release((*iter));
			iter = m_pCubeObj.erase(iter);
		}
		else
			++iter;
	}
	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);

	return 0;
}

void CTool_Object::Render_GameObject(void)
{
	if (nullptr == m_pRendererCom)
		return;

	if (m_pCubeObj.empty())
		return;

	auto iter = m_pCubeObj.begin();
	for (; iter != m_pCubeObj.end(); ++iter)
	{
		dynamic_cast<CTool_Cube*>((*iter))->Render_GameObject();
	}
}

void CTool_Object::Set_NotPicking()
{
	if (m_pCubeObj.empty())
	{
		return;
	}

	auto& iter = m_pCubeObj.begin();
	for (; iter != m_pCubeObj.end(); ++iter)
	{
		int ia = m_pCubeObj.size();
		dynamic_cast<CTool_Cube*>(*iter)->Set_PickingCube(false);
	}
}

HRESULT CTool_Object::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;

	/*Add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();

	return NOERROR;

exception:
	MSG_BOX("Effect Component Ready Failed err: 13115");
	return E_FAIL;
}

HRESULT CTool_Object::Create_Cube(_vec3 vPos, _vec3 vRota, _vec3 vScalse, const _uint & iTextureNum, const TCHAR * pKey)
{
	Set_NotPicking();

	/*Create Cube*/
	CGameObject* pObject = CTool_Cube::Create(m_pGraphicDev, vPos, vRota, vScalse, iTextureNum, pKey);

	if (nullptr == pObject)
		goto CreateTool;

	m_pCubeObj.push_back(pObject);

	return NOERROR;

CreateTool:
	MSG_BOX("Add_GameObject failed on Scene_Logo err:50620");
	return E_FAIL;
}

CTool_Object* CTool_Object::Create(LPDIRECT3DDEVICE9 pGraphicDev, CString strName)
{
	CTool_Object*	pInstance = new CTool_Object(pGraphicDev, strName);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CTool_Object Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CTool_Object::Free(void)
{
	_ulong		dwRefCnt = 0;

	auto iter = m_pCubeObj.begin();
	for (; iter != m_pCubeObj.end(); ++iter)
	{
		dwRefCnt += Safe_Release((*iter));
	}
	m_pCubeObj.clear();

	Engine::Safe_Release(m_pRendererCom);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
