#include "stdafx.h"
#include "..\Headers\MainApp.h"
#include "scene_logo.h"
#include "camera_manager.h"
#include "Light_Manager.h"
#include "Target_Manager.h"
#include "Particle_Renderer.h"
#include "Effect_Manager.h"
#include "Collider_Manager.h"
#include "WELL512a.h"
#include "Wave_Manager.h"
#include "Sound_Manager.h"

CMainApp::CMainApp()
	:m_pDevice(CGraphic_Device::GetInstance())
	, m_pGraphicDev(nullptr)
	, m_pManagement(CManagement::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pFont(nullptr)
	, m_pParticle_Renderer(CParticle_Renderer::GetInstance())
	, m_pEffect_Manager(CEffect_Manager::GetInstance())
	, m_pRandomMgr(CRandomWell512::GetInstance())
	, m_pSound_Manager(CSound_Manager::GetInstance())
	
{
	m_pDevice->AddRef();
	m_pComponent_Manager->AddRef();
	m_pManagement->AddRef();
	m_pParticle_Renderer->AddRef();
	m_pEffect_Manager->AddRef();
	m_pSound_Manager->AddRef();
}

HRESULT CMainApp::Ready_MainApp()
{
	if (FAILED(Ready_DeviceSetting(g_hWnd, Engine::CGraphic_Device::MODE_WIN, BACKCX, BACKCY)))
		goto exception;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	/*reserve container*/
	if (FAILED(m_pManagement->Ready_Management(SCENE_END)))
		goto exception;

	if (FAILED(Ready_StaticComponent()))
		goto exception;

	if (FAILED(Ready_Sound()))
		goto exception;

	if (FAILED(Ready_StartScene()))
		goto exception;

	if (FAILED(Ready_Singleton()))
		goto exception;

	return NOERROR;

exception:
	MSG_BOX("Ready_MainApp failed err:04933");
	return E_FAIL;
}

_int CMainApp::Update_MainApp(const _float fTimeDelta)
{
	if (nullptr == m_pManagement)
		return -1;

	m_fTimer += fTimeDelta;
	++m_fFPS;

	m_pParticle_Renderer->Update_Particle(fTimeDelta);
	m_pEffect_Manager->Update_Effect(fTimeDelta);
	m_pSound_Manager->UpdateSound();

	return m_pManagement->Update_Management(fTimeDelta);
}

void CMainApp::Render_MainApp()
{
	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	m_pGraphicDev->BeginScene();

	m_pManagement->Render_Management();
	
	this->Render_Font();

	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(nullptr, nullptr, nullptr, nullptr);
}

HRESULT CMainApp::Ready_DeviceSetting(HWND hWnd, CGraphic_Device::WINMODE eMode, const _uint & iBackCX, const _uint & iBackCY)
{
	if (nullptr == m_pDevice)
		goto exception;

	if (FAILED(m_pDevice->Ready_GraphicDev(&m_pGraphicDev, hWnd, eMode, iBackCX, iBackCY)))
		goto exception;

	/*Add Font*/
	m_pFont = m_pDevice->Get_Font();
	if (nullptr == m_pFont)
		return E_FAIL;

	m_pFont->AddRef();

	return NOERROR;

exception:
	MSG_BOX("Ready_DeviceSetting failed err:04934");
	return E_FAIL;
}

HRESULT CMainApp::Ready_StaticComponent()
{
	if (nullptr == m_pComponent_Manager)
	{
		MSG_BOX("Component_Manager isn't allocated");
		return E_FAIL;
	}

	/*Create Triangle_Color*/
	CComponent* pComponent = nullptr;
	pComponent = CTriangle_Color::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_TriCol", pComponent)))
		goto exception;


	/*create Rect_Color*/
	pComponent = CRect_Color::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_RectCol", pComponent)))
		goto exception;

	
	/*Create Rect_Texture*/
	pComponent = CRect_Texture::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_RectTex", pComponent)))
		goto exception;

	/*Create Rect_Texture*/
	pComponent = CRectUI_Texture::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_RectUITex", pComponent)))
		goto exception;

	/*Create Rect_Texture*/
	pComponent = CRectUI_Texture::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_RectUITexButton1", pComponent)))
		goto exception;

	/*Create Rect_Texture*/
	pComponent = CRectUI_Texture::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_RectUITexButton2", pComponent)))
		goto exception;

	/*Create Rect_Texture*/
	pComponent = CRectUI_Texture::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_RectStageUI1", pComponent)))
		goto exception;

	/*Create Rect_Texture*/
	pComponent = CRectUI_Texture::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_RectStageUI2", pComponent)))
		goto exception;

	/*Create Rect_Texture*/
	pComponent = CRectUI_Texture::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_RectHITUI", pComponent)))
		goto exception;

	/*Create Rect_Texture*/
	pComponent = CRectUI_Texture::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_RectCross", pComponent)))
		goto exception;


	/*Create ColliderBox_AABB*/
	pComponent = CAABB_Buffer::Create(m_pGraphicDev,1.0f,1.0f,1.0f);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_ColliderBox_AABB", pComponent)))
		goto exception;

	/*Create ColliderBox_Billboard*/
	pComponent = CBillboard_Buffer::Create(m_pGraphicDev, 1.0f, 1.0f, 1.0f);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_ColliderBox_Billboard", pComponent)))
		goto exception;

	/*create Renderer & reference*/
	pComponent = m_pRenderer = CRenderer::Create(m_pGraphicDev);
	m_pRenderer->AddRef();

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Renderer", pComponent)))
		goto exception;

	pComponent = Engine::CPicking::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Picking", pComponent)))
		return E_FAIL;

	// ---------------------------------- Test ------------------------------
	/*create Particle_Renderer & reference*/
	//pComponent = m_pParticle_Renderer = CParticle_Renderer::Create(m_pGraphicDev);
	//m_pParticle_Renderer->AddRef();
	//if (nullptr == pComponent)
	//	goto exception;
	//if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Particle_Renderer", pComponent)))
	//	goto exception;

	CCollider_Manager::GetInstance()->Ready_CCollider_Manager(m_pGraphicDev);
	m_pParticle_Renderer->Ready_Particle_Renderer(m_pGraphicDev);
	m_pEffect_Manager->Ready_Effect_Renderer(m_pGraphicDev);

	/*create Particle_Bullet */
	pComponent = CParticle_Bullet::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		goto exception;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Particle_Bullet", pComponent)))
		goto exception;

	/*create Particle_Effect */
	pComponent = CParticle_Effect::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		goto exception;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Particle_Effect", pComponent)))
		goto exception;

	/*create Particle_Bullet */
	pComponent = CEffect_Bullet::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		goto exception;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Effect_Bullet", pComponent)))
		goto exception;

	/*create CEffect_Fixed */
	pComponent = CEffect_Fixed::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		goto exception;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Effect_Fixed", pComponent)))
		goto exception;

	// For.Texture_Particle_Bullet
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Particle/Bullet/Flare/Flare%d.bmp", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Texture_Bullet_Flare", pComponent)))
		return E_FAIL;

	// For.Texture_Bullet_Pistol
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Particle/Bullet/Pistol/Pistol%d.bmp", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Texture_Bullet_Pistol", pComponent)))
		return E_FAIL;

	// For.Texture_Bullet_Rifle
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Particle/Bullet/Rifle/Rifle%d.bmp", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Texture_Bullet_Rifle", pComponent)))
		return E_FAIL;

	// For.Texture_Bullet_Shotgun
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Particle/Bullet/Shotgun/Shotgun%d.bmp", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Texture_Bullet_Shotgun", pComponent)))
		return E_FAIL;

	// For.Texture_Bullet_Chaingun
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Particle/Bullet/Chaingun/Chaingun%d.bmp", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Texture_Bullet_Chaingun", pComponent)))
		return E_FAIL;

	// For.Texture_Effect_Muzzle
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Particle/Muzzle/Muzzle%d.bmp", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Add_Component(SCENE_STATIC, L"Component_Texture_Particle_Muzzle", pComponent)))
		return E_FAIL;

	// For.Texture_Effect_Dust
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Particle/Dust/Dust%d.bmp", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Add_Component(SCENE_STATIC, L"Component_Texture_Effect_Dust", pComponent)))
		return E_FAIL;

	// For.Texture_Effect_Bullet_Rifle
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Effect/Rifle/Rifle%d.png", 1);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Texture_Effect_Bullet_Rifle", pComponent)))
		return E_FAIL;

	//// For.Texture_Effect_Muzzle
	//pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Effect/MuzzleFlash/MuzzleFlash%d.png", 7);
	//if (nullptr == pComponent)
	//	return E_FAIL;
	//if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Texture_Effect_Muzzle", pComponent)))
	//	return E_FAIL;
	// ---------------------------------- TestEnd ------------------------------
	
	

	/*create transform*/
	pComponent = CTransform::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Transform", pComponent)))
		goto exception;

	/*create Temp_Cube*/
	pComponent = CTemp_Cube::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_Cube", pComponent)))
		goto exception;

	/*create Cube_texture*/
	pComponent = CCube_Texture::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_CubeTex", pComponent)))
		goto exception;


	/*create Cube_Color*/
	pComponent = CCube_Color::Create(m_pGraphicDev,0.1f,0.1f,0.1f);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_CubeCol", pComponent)))
		goto exception;

	/*create Jang_Cube*/
	pComponent = CJang_Cube::Create(m_pGraphicDev,1.0f,1.0f,1.0f);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_Jang_Cube", pComponent)))
		goto exception;

	/*create Material*/
	pComponent = CMaterial::Create(m_pGraphicDev,Engine::CMaterial::MATTYPE::MATTYPE_TEST);
	if (nullptr == pComponent)
		goto exception;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Material", pComponent)))
		goto exception;

	/*create Light*/
	pComponent = CLight::Create(m_pGraphicDev, Engine::CLight::LIGHTCOLOR::TEST);
	if (nullptr == pComponent)
		goto exception;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Light", pComponent)))
		goto exception;

	/*create Directional_Light*/
	pComponent = CLight::Create(m_pGraphicDev, Engine::CLight::LIGHTCOLOR::DIRECTIONAL_LIGHT);
	if (nullptr == pComponent)
		goto exception;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Direct_Light", pComponent)))
		goto exception;
	CLight_Manager::GetInstance()->Add_RenderLight(dynamic_cast<CLight*>(pComponent), LIGHT_TYPE::DIRECTIONAL_LIGHT);
	
	//CLight_Manager::GetInstance()->Add_RenderLight(CLight::Create(m_pGraphicDev, Engine::CLight::LIGHTCOLOR::DIRECTIONAL_LIGHT), LIGHT_TYPE::DIRECTIONAL_LIGHT);

	// ------------------------ Shader -------------------------------------
	/*Create&Add Shader_Cube */
	pComponent = CShader::Create(m_pGraphicDev, CShader::SDTYPE_NORMAL, L"../Bin/Resources/Shader/Cube_%d.fx", 1);
	if (nullptr == pComponent)
		goto exception;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Shader_Cube", pComponent)))
		goto exception;

	///*Create&Add Shader_UI */
	//pComponent = CShader::Create(m_pGraphicDev, CShader::SDTYPE_NORMAL, L"../Bin/Resources/Shader/UI_%d.fx", 1);
	//if (nullptr == pComponent)
	//	goto exception;
	//if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Shader_UI", pComponent)))
	//	goto exception;
	// ------------------------ Shader -------------------------------------
	return NO_ERROR;

exception:
	MSG_BOX("Ready_StaticComponent failed err:95965");
	return E_FAIL;
}

HRESULT CMainApp::Ready_StartScene()
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	CScene* pScene = CScene_Logo::Create(m_pGraphicDev);

	if (nullptr == pScene)
		return E_FAIL;

	if (FAILED(m_pManagement->Set_CurScene(pScene, m_pRenderer)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainApp::Ready_Singleton()
{
	if (FAILED(CWave_Manager::GetInstance()->Ready_Wave_Manager(m_pGraphicDev)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainApp::Ready_Sound()
{
	if (FAILED(m_pSound_Manager->Ready_Sound_Manager()))
	{
		MSG_BOX("Sound_Manager Load Failed!!");
		return E_FAIL;
	}
	return NOERROR;
}

void CMainApp::Render_Font()
{
	if (m_fTimer > 1.f)
	{
		sprintf_s(m_szFPS, "FPS : %d", _int(m_fFPS));

		m_fTimer = 0.f;
		m_fFPS = 0.f;
	}

	m_pFont->DrawTextW(10, 10, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), m_szFPS);
}

CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("MainApp Creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CMainApp::Free()
{
	_ulong dwRefCnt = 0;

	/*Decrease RefCount*/	
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDevice);
	Safe_Release(m_pManagement);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pParticle_Renderer);
	Safe_Release(m_pEffect_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pFont);
	Safe_Release(m_pRandomMgr);
	Safe_Release(m_pSound_Manager);

// 	/* Release */
// 	if(dwRefCnt = Safe_Release(m_pFont))
// 		MSG_BOX("Font Release Failed");

	/*Destroy Instance*/
	if(dwRefCnt = CWave_Manager::GetInstance()->DestroyInstance())
		//MSG_BOX("Wave_Manager not destroyted");

	if (dwRefCnt = CManagement::GetInstance()->DestroyInstance())
		//MSG_BOX("Management not destroyed");

	if (dwRefCnt = CCamera_Manager::GetInstance()->DestroyInstance())
		//MSG_BOX("Camera_Manager not destroyed");

	if (dwRefCnt = CObject_Manager::GetInstance()->DestroyInstance())
		//MSG_BOX("CObject_Manager not destroyed");

	if (dwRefCnt = CSound_Manager::GetInstance()->DestroyInstance())
		//MSG_BOX("CSound_Manager not destroyed");

	if (dwRefCnt = CComponent_Manager::GetInstance()->DestroyInstance())
		//MSG_BOX("Component_Manager not destroyed");

	if (dwRefCnt = CEffect_Manager::GetInstance()->DestroyInstance())
		//MSG_BOX("CEffect_Manager not destroyed");

	if (dwRefCnt = CParticle_Renderer::GetInstance()->DestroyInstance())
		//MSG_BOX("CParticle_Renderer not destroyed");


	if (dwRefCnt = CCollider_Manager::GetInstance()->DestroyInstance())
		//MSG_BOX("CCollider_Manager not destroyed");

	//if (dwRefCnt = CObject_Manager::GetInstance()->DestroyInstance())
	//	MSG_BOX("CObject_Manager not destroyed");


	//if (dwRefCnt = CRandomWell512::GetInstance()->DestroyInstance())
	//	MSG_BOX("CRandomWell512 not destroyed");

	if (dwRefCnt = CGraphic_Device::GetInstance()->DestroyInstance())
		//MSG_BOX("Device not destroyed");



	return 0;
}
