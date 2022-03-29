#include "stdafx.h"
#include "../Headers/Scene_ATool.h"
#include "tool_camera_dynamic.h"
#include "Camera_Manager.h"
#include "Creature_Manager.h"

CScene_ATool::CScene_ATool(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
	,m_pCamera_Manager(CCamera_Manager::GetInstance())
	, m_pCreature_Manager(CCreature_Manager::GetInstance())
{

}

HRESULT CScene_ATool::Ready_Scene()
{
	if (FAILED(Add_GameObject_ATool()))
		return E_FAIL;

	if (FAILED(Add_Component_ATool()))
		return E_FAIL;

	if (FAILED(Add_Camera_ATool()))
		return E_FAIL;

	return NOERROR;
}

_int CScene_ATool::Update_Scene(const _float & fTimeDelta)
{
	m_pCreature_Manager->Update_Creature(fTimeDelta);
	m_pCamera_Manager->Update_Camera(fTimeDelta);
	//return CScene::Update_Scene(fTimeDelta);;
	return 0;
}

void CScene_ATool::Render_Scene()
{
	m_pCreature_Manager->Render_Creature();


	//CScene::Render_Scene();
}

HRESULT CScene_ATool::Add_GameObject_ATool()
{
	return NOERROR;
}

HRESULT CScene_ATool::Add_Component_ATool()
{
	CComponent*	pComponent = nullptr;

	pComponent = CCube_Color::Create(m_pGraphicDev,0.1f,0.1f,0.1f);
	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_TOOL, L"Component_BoneCube", pComponent)))
		goto exception;

	/*Add Bone*/
	pComponent = CBone::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_TOOL, L"Component_Bone", pComponent)))
		goto exception;

	/*Add Cube_Texture*/
	pComponent = CCube_Texture::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_TOOL, L"Component_Buffer_CubeTex", pComponent)))
		goto exception;

	/*Cube_TextureComponent*/
	//pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_CUBE, L"../Bin/Resources/Textures/CubeTexture/CubeTile_%d.dds", 104);

	//if (nullptr == pComponent)
	//	goto exception;

	//if (FAILED(m_pComponent_Manager->Add_Component(SCENE_TOOL, L"Component_Texture_Cube", pComponent)))
	//	goto exception;

	/*Cube_TextureComponent*/
	pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_CUBE, L"../Bin/Resources/Textures/Monster/Monster_%d.dds", 25);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_TOOL, L"Component_Texture_Zombie", pComponent)))
		goto exception;

	return NOERROR;


exception:
	AfxMessageBox(L"AddComponent Failed");
	return E_FAIL;
}

HRESULT CScene_ATool::Add_Camera_ATool()
{
	CGameObject* pObject = CTool_Camera_Dynamic::Create(m_pGraphicDev, _vec3(0.f, 0.f, -5.f), _vec3(0.f, 0.f, 0.f), _vec3(0.f, 1.f, 0.f));

	if (nullptr == pObject)
		goto exception;

	if (FAILED(m_pCamera_Manager->Add_Camera(L"Camera_Dynamic", pObject)))
		goto exception;

	m_pCamera_Manager->Change_Camera(L"Camera_Dynamic");

	return NOERROR;

exception:
	AfxMessageBox(L"AddCamera Failed");
	return E_FAIL;
}

CScene_ATool * CScene_ATool::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CScene_ATool* pInstance = new CScene_ATool(pGraphicDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Scene_ATool creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CScene_ATool::Free()
{
	/*Decrease RefCount*/

	m_pComponent_Manager->Release_TargetContainer(SCENE_TOOL);
	m_pObject_Manager->Release_TargetContainer(SCENE_TOOL);

	return CScene::Free();
}


