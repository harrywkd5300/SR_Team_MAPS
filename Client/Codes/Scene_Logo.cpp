#include "stdafx.h"
#include "Scene_Logo.h"
#include "Back_Logo.h"
#include "logo_skybox.h"
#include "Logo_UI.h"
#include "Logo_UIQuit.h"
#include "Camera_Dynamic.h"
#include "Scene_Stage.h"
#include "management.h"
#include "Input_Device.h"
#include "Logo_SkyBox.h"
#include "Layer.h"
#include "Sound_Manager.h"

CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
	, m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
}


HRESULT CScene_Logo::Ready_Scene()
{
	if (FAILED(Add_Logo_BGM()))
		return E_FAIL;

	if (FAILED(Add_Component_ForLogo()))
		return E_FAIL;

	if (FAILED(Add_GameObject_ForLogo()))
		return E_FAIL;

	if (FAILED(Add_Camera_ForLogo()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Logo::Add_GameObject_ForLogo()
{
	if (nullptr == m_pObject_Manager)
		goto exception;

	/*Create Back_Logo & Add on ObjectManager*/
	CGameObject* pObject = CBack_Logo::Create(m_pGraphicDev);

	if (nullptr == pObject)
		goto exception;

	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_LOGO, L"Layer_BackGround", pObject)))
		goto exception;

	pObject = CLogo_UI::Create(m_pGraphicDev, 0);

	if (nullptr == pObject)
		goto exception;

	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_LOGO, L"Layer_LogoUI", pObject)))
		goto exception;

	pObject = CLogo_UIQuit::Create(m_pGraphicDev, 2);

	if (nullptr == pObject)
		goto exception;

	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_LOGO, L"Layer_LogoUI_Quit", pObject)))
		goto exception;

	pObject = CLogo_SkyBox::Create(m_pGraphicDev);

	if (nullptr == pObject)
		goto exception;

	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_LOGO, L"Layer_SkyBox", pObject)))
		goto exception;

	return NOERROR;

exception:
	MSG_BOX("Add_GameObject failed on Scene_Logo err:50624");
	return E_FAIL;
}

HRESULT CScene_Logo::Add_Component_ForLogo()
{
	if (nullptr == m_pComponent_Manager)
		goto exception;

	CComponent* pComponent = CTexture::Create(m_pGraphicDev,CTexture::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/BackLogo/Logo2.png");

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_LOGO, L"Component_Texture_BackLogo", pComponent)))
		goto exception;

	pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/BackLogo/MenuButton%d.png", 4);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_LOGO, L"Component_Texture_LogoUI", pComponent)))
		goto exception;

	/*Create&Add Texture_SkyBox*/
	pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_CUBE, L"../Bin/Resources/Textures/BackLogo/MenuSkyBox.dds");

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_LOGO, L"Component_Texture_SkyBox", pComponent)))
		goto exception;


	return NO_ERROR;

exception:
	MSG_BOX("Add Component_ForLogo failed err:12854");
	return E_FAIL;
}

HRESULT CScene_Logo::Add_Camera_ForLogo()
{
	CGameObject* pObject = CCamera_Dynamic::Create(m_pGraphicDev, _vec3(0.f, 0.f, -8.f), _vec3(0.f, 1.f, 0.f), _vec3(0.f, 1.f, 0.f));

	if (nullptr == pObject)
		goto exception;

	if (FAILED(m_pCamera_Manager->Add_Camera(L"Camera_Dynamic",pObject)))
		goto exception;

	m_pCamera_Manager->Change_Camera(L"Camera_Dynamic");

	dynamic_cast<CCamera_Dynamic*>(m_pCamera_Manager->Get_Camera(L"Camera_Dynamic"))->Set_CameraMove(false);

	return NOERROR;

exception:
	return E_FAIL;
}

HRESULT CScene_Logo::Add_Logo_BGM()
{
	CSound_Manager::GetInstance()->PlayBGM(L"Logo.mp3", CSound_Manager::Channel_BGM, 1.f);

	return NOERROR;
}

_int CScene_Logo::Update_Scene(const _float & fTimeDelta)
{
	if (m_pInput_Device->Get_DIMouseState(m_pInput_Device->DIM_LBUTTON) & 0x80)
	{
		auto iter = m_pObject_Manager->Get_Layer(SCENE_LOGO, L"Layer_LogoUI")->Get_ObjList().begin();
		if (dynamic_cast<CLogo_UI*>(*iter)->Get_ClickButton())
		{
			Engine::CScene*	pNewScene = CScene_Stage::Create(m_pGraphicDev);
			if (nullptr == pNewScene)
				return 0;
			CManagement::GetInstance()->Set_CurScene(pNewScene, m_pRenderer);
		}
	}


	_int iExitCode = CScene::Update_Scene(fTimeDelta);

	return iExitCode;
}

void CScene_Logo::Render_Scene()
{
	CScene::Render_Scene();
}

CScene_Logo * CScene_Logo::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CScene_Logo* pInstance = new CScene_Logo(pGraphicDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Logo creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CScene_Logo::Free()
{
	/*Decrease RefCount*/

 	m_pComponent_Manager->Release_TargetContainer(SCENE_LOGO);
 	m_pObject_Manager->Release_TargetContainer(SCENE_LOGO);


	Safe_Release(m_pInput_Device);

	return CScene::Free();
}


void CScene_Logo::Fix_MousePointer(const _uint & iX, const _uint & iY)
{
	POINT			pt = { LONG(iX), LONG(iY) };

	ClientToScreen(g_hWnd, &pt);

	SetCursorPos(pt.x, pt.y);
}
