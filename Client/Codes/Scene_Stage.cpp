#include "stdafx.h"
#include "..\Headers\Scene_Stage.h"
#include "Camera_Dynamic.h"
#include "camera_Player.h"
#include "Terrain.h"
#include "player.h"
#include "layer.h"
#include "LightObj.h"
#include "Input_Device.h"
#include "skybox.h"
#include "LightEffectObj.h"
#include "Effect_Manager.h"

#include "House.h"
#include "HouseRoof.h"

#include "BaseMonster.h"
#include "Ghost.h"
#include "Creeper.h"

#include "Collider_Manager.h"

#include "QuickSlot.h"
#include "QuickSlot_Reserve.h"
#include "BulletCnt.h"
#include "CrossFilter.h"
#include "Sound_Manager.h"


CScene_Stage::CScene_Stage(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pColliderMgr(CCollider_Manager::GetInstance())
	, m_pWave_Manager(CWave_Manager::GetInstance())	/*not referenced*/

{
	m_pInput_Device->AddRef();
}

HRESULT CScene_Stage::Ready_Scene()
{
	if (FAILED(Add_Component_ForStage()))
		return E_FAIL;

	if (FAILED(Add_GameObject_ForStage()))
		return E_FAIL;

 	if (FAILED(Add_GameObject_Monster_ForStage()))
 		return E_FAIL;

	if (FAILED(Add_Camera_ForStage()))
		return E_FAIL;

	if (FAILED(Add_BGM_ForStage()))
		return E_FAIL;


	return NOERROR;
}

_int CScene_Stage::Update_Scene(const _float & fTimeDelta)
{
	Update_InputState(fTimeDelta);
	
	_int iExitCode = CScene::Update_Scene(fTimeDelta);
	
	//모든 업데이트가 끝나면 캐릭터 충돌박스 리스트 제거
	m_pColliderMgr->Update_Manager(fTimeDelta);
	m_pWave_Manager->Update_Wave_Manager(fTimeDelta);

	return iExitCode;
}

_int CScene_Stage::LastUpdate_Scene(const _float & fTimeDelta)
{
	_int iExitCode = CScene::LastUpdate_Scene(fTimeDelta);
	return iExitCode;
}

void CScene_Stage::Render_Scene()
{
	CScene::Render_Scene();
}

HRESULT CScene_Stage::Add_GameObject_ForStage()
{

	CGameObject* pGameObject = nullptr;

	/*Add Terrain*/
	pGameObject = CTerrain::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		goto exception;

	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Terrain", pGameObject))) /*index 1*/
		goto exception;

	/*Add Player*/
	pGameObject = CPlayer::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		goto exception;

	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Player", pGameObject)))
		goto exception;

	/*Add House*/
	pGameObject = CHouse::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		goto exception;

	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Object", pGameObject)))
		goto exception;

	/*Add HouseRoof*/
	pGameObject = CHouseRoof::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		goto exception;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Object", pGameObject)))
		goto exception;

	/*Add QuickSlot*/
	pGameObject = CQuickSlot_Reserve::Create(m_pGraphicDev, 80.f, 70.f, 100.f, 40.f);
	if (nullptr == pGameObject)
		goto exception;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Slot", pGameObject)))
		goto exception;

	/*Add QuickSlotReserve*/
	pGameObject = CQuickSlot::Create(m_pGraphicDev, 160.f, 130.f, 140.f, 60.f);
	if (nullptr == pGameObject)
		goto exception;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Slot", pGameObject)))
		goto exception;

	/*Add BulletCnt*/
	pGameObject = CBulletCnt::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		goto exception;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_UI", pGameObject)))
		goto exception;

	/*Add CrossFilter*/
	pGameObject = CCrossFilter::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		goto exception;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_UI", pGameObject)))
		goto exception;

	////Add LightObj

	//4-5번방 오른쪽벽
	D3DXCOLOR Organge(248.f / 255.f, 188.f / 255.f, 80.f / 255.f, 1.f);
	D3DXCOLOR WhiteColor(1.f, 1.f, 1.f, 1.f);
	pGameObject = CLightEffectObj::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
		return E_FAIL;
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 8.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(76.5f, 2.5f, 56.5f));

	//4-5번방 왼쪽벽
	pGameObject = CLightEffectObj::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
		return E_FAIL;
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 8.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(49.5f, 2.5f, 56.5f));

	//4-5번방 가운데
	pGameObject = CLightEffectObj::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
		return E_FAIL;
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
	//dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 4.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 8.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(62.f, 1.5f, 56.5f));

	//6번방 입구
	pGameObject = CLightEffectObj::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
		return E_FAIL;
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 7.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(72.f, 2.5f, 79.5f));

	//7번방 오른쪽아래 벽
	pGameObject = CLightEffectObj::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
		return E_FAIL;
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 8.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(47.5, 2.5f, 56.5f));

	//2-1번방 문
	pGameObject = CLightEffectObj::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
		return E_FAIL;
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 5.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(48.5, 2.5f, 74.0f));

	pGameObject = CLightEffectObj::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
		return E_FAIL;
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 10.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(98.f, 5.f, 70.f));


	pGameObject = CLightEffectObj::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
		return E_FAIL;
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 10.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(58.5f, 2.5f, 89.5f));

	// bossR
	pGameObject = CLightEffectObj::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
		return E_FAIL;
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 10.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(75.f, 15.5f, 7.5f));

	//// bossM
	//pGameObject = CLightEffectObj::Create(m_pGraphicDev);
	//if (nullptr == pGameObject)
	//	return E_FAIL;
	//if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Effect", pGameObject)))
	//	return E_FAIL;
	//dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
	//dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 10.f);
	//dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(65.f, 20.0f, 7.5f));


	// bossL
	pGameObject = CLightEffectObj::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
		return E_FAIL;
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 10.f);
	dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(53.f, 15.5f, 7.5f));




	

	//for (int i = 0;i < 10; ++i)
	//{
	//	//4-5번방 가운데
	//	pGameObject = CLightEffectObj::Create(m_pGraphicDev);
	//	if (nullptr == pGameObject)
	//		return E_FAIL;
	//	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Effect", pGameObject)))
	//		return E_FAIL;
	//	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
	//	//dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 4.f);
	//	dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 8.f);
	//	dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(62.f, 1.5f, 56.5f+i*2));
	//}


 	//D3DXCOLOR Rime(0xbf / 255.f, 0xff / 255.f, 0x00 / 255.f, 1.f);
 	//dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Rime, 10.f);
 	//dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_Invincible");
 
 	//pGameObject = CLightEffectObj::Create(m_pGraphicDev);
 	//if (nullptr == pGameObject)
 	//	return E_FAIL;
 
 	//if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Effect", pGameObject)))
 	//	return E_FAIL;
 
 	//D3DXCOLOR Organge(248.f / 255.f, 188.f / 255.f, 80.f / 255.f, 1.f);
 	//dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 10.f);
 	//dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_ReloadLoop");

	//for (int i = 0; i < MAX_LIGHT_SIZE; ++i)
	//{
	//	/*light1*/
	//	pGameObject = CLightObj::Create(m_pGraphicDev);
	//	if (nullptr == pGameObject)
	//		goto exception;
	//	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
	//		goto exception;
	//	dynamic_cast<CLightObj*>(pGameObject)->SetColor(Engine::CLight::LIGHTCOLOR(int(Engine::CLight::LIGHTCOLOR::WHITE) + (i)));
	//	dynamic_cast<CLightObj*>(pGameObject)->GetTransformCom()->Move_ToDirection(_vec3(100.0f, 50.0f / (i + 1), 100.0f), 20.f*(i + 1), 1);
	//}

	/* Add SkyBox */
	pGameObject = CSkyBox::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		goto exception;

	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_SkyBox", pGameObject)))
		goto exception;

	return NOERROR;

exception:
	MSG_BOX("AddGameObject failed in Scene_Stage err: 30094");
	return E_FAIL;
}

HRESULT CScene_Stage::Add_GameObject_Monster_ForStage()
{
// 	CGameObject* pGameObject = nullptr;
// 
// 	_vec3 vMakePos;
// 	if (rand() % 2 == 0)
// 		vMakePos = _vec3(98.5f, 0.5f, 49.5f);
// 	else
// 		vMakePos = _vec3(40.5f, 0.5f, 99.5f);
// 
// 	/*Add Monster*/
// 	pGameObject = CBaseMonster::Create(m_pGraphicDev, vMakePos);
// 
// 	if (nullptr == pGameObject)
// 		goto exception;
// 
// 	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Monster", pGameObject)))
// 		goto exception;
// 	
// 	
//  	pGameObject = CGhost::Create(m_pGraphicDev, &vMakePos);
//  
//  	if (nullptr == pGameObject)
//  		goto exception;
//  
//  	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Monster", pGameObject)))
//  		goto exception;
// 
// 
// 	if (m_pWave_Manager->Add_Monster(CWave_Manager::MONSTER_CREEPER, CWave_Manager::SPAWN_1))
// 		goto exception;
// 
// 
// 	if (m_pWave_Manager->Add_Monster(CWave_Manager::MONSTER_BOSS, CWave_Manager::SPAWN_BOSS))
// 		goto exception;

	//pGameObject = CCreeper::Create(m_pGraphicDev);

	//if (nullptr == pGameObject)
	//	goto exception;

	//if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Monster", pGameObject)))
	//	goto exception;


	///*Add Monster*/
	//pGameObject = CBaseMonster::Create(m_pGraphicDev, vMakePos);

	//if (nullptr == pGameObject)
	//	goto exception;

	//if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Monster", pGameObject)))
	//	goto exception;


 	return NOERROR;
// 
// exception:
// 	MSG_BOX("AddGameObject failed in Scene_Stage err: 30094");
// 	return E_FAIL;
}

HRESULT CScene_Stage::Add_Component_ForStage()
{
	CComponent* pComponent = nullptr;
	
	/*Create&Add Texture_Terrain*/
	pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Terrain/Grass_%d.tga", 3);

	if (nullptr == pComponent)
		goto exception;
	
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Terrain", pComponent)))
		goto exception;

	/*Create&Add Texture_SkyBox*/
	pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_CUBE, L"../Bin/Resources/Textures/SkyBox/SkyBox.dds");

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_SkyBox", pComponent)))
		goto exception;

	/*Create&Add QuickSlot*/
	pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/UI/QuickSlot.png");

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_StageUI", pComponent)))
		goto exception;

	/*Create&Add QuickSlot*/
	pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/UI/QuickSlot_Reserve.png");

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_StageReserveUI", pComponent)))
		goto exception;

	/*Create&Add LowHp0*/
	pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/UI/LowHp0.png");

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_RowHP", pComponent)))
		goto exception;

	// For.Texture_Cross
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/UI/Cross.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Texture_Cross", pComponent)))
		return E_FAIL;

	/*Create&Add Shader_Terrain */
	pComponent = CShader::Create(m_pGraphicDev, CShader::SDTYPE_NORMAL, L"../Bin/Resources/Shader/Terrain_%d.fx", 3);
	//pComponent = CShader::Create(m_pGraphicDev, CShader::SDTYPE_NORMAL, L"../Bin/Resources/Shader/Player_%d.fx", 1);
	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Terrain", pComponent)))
		goto exception;

	/*Create&Add Terrain_Buffer */
	pComponent = CTerrain_Buffer::Create(m_pGraphicDev, L"../Bin/Resources/Textures/Terrain/Height.bmp", 1);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Buffer_Terrain", pComponent)))
		goto exception;

 	/*Create&Add Player_Texture*/
	pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_CUBE, L"../Bin/Resources/Textures/Player/Player/Player_%d.dds", 13);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Player", pComponent)))
		goto exception;

	/*Create&Add Monster_Texture*/
	pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_CUBE, L"../Bin/Resources/Textures/Monster/Monster_%d.dds", 34);

	if (nullptr == pComponent)
		goto exception;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Zombie", pComponent)))
		goto exception;


	/*Create&Add Rifle_Texture*/
	pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_CUBE, L"../Bin/Resources/Textures/Weapon/Rifle/Rifle_%d.dds", 6);
	if (nullptr == pComponent)
		goto exception;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Rifle", pComponent)))
		goto exception;

	/*Create&Add Shotgun_Texture*/
	pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_CUBE, L"../Bin/Resources/Textures/Weapon/Shotgun/Shotgun_%d.dds", 6);
	if (nullptr == pComponent)
		goto exception;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Shotgun", pComponent)))
		goto exception;

	//// For.Texture_Effect_Muzzle
	//pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Effect/MuzzleFlash/MuzzleFlash%d.png", 7);
	//if (nullptr == pComponent)
	//	return E_FAIL;
	//if (FAILED(Engine::CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_Muzzle", pComponent)))
	//	return E_FAIL;

	// For.Texture_Effect_Muzzle
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Effect/MuzzleFlash/MuzzleFlash%d.png", 6);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_Muzzle", pComponent)))
		return E_FAIL;

	//if (FAILED(CEffect_Manager::GetInstance()->Set_Effect_Type(
	//	CEffect_Manager::EFFECT_TYPE::EFFECT_FIXED,
	//	L"Component_Texture_Effect_Muzzle",
	//	dynamic_cast<CTexture*>(pComponent)
	//)))
	//{
	//	MSG_BOX("Effect_Manager_Texture_SizeOver : Effect_Muzzle");
	//	return E_FAIL;
	//}


	// For.Texture_Effect_FireBall
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Effect/FireBall/FireBall_%d.png", 5);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_FireBall", pComponent)))
		return E_FAIL;

	// For.Texture_Effect_FlameA
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Effect/FlameA/FlameA%d.tga", 50);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_FlameA", pComponent)))
		return E_FAIL;

	// For.Texture_Effect_FlameB
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Effect/FlameB/FlameB%d.tga", 30);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_FlameB", pComponent)))
		return E_FAIL;

	// For.Texture_Effect_FlameC
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Effect/FlameC/FlameC%d.tga", 30);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_FlameC", pComponent)))
		return E_FAIL;

	// For.Texture_Effect_FlameD
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Effect/FlameD/FlameD%d.tga", 30);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_FlameD", pComponent)))
		return E_FAIL;

	// For.Texture_Effect_ReloadLoop
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Effect/ReloadLoop/ReloadLoop00%d.png", 30);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_ReloadLoop", pComponent)))
		return E_FAIL;

	// For.Texture_Effect_Invincible
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TEXTURETYPE::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Effect/Invincible/Invincible%d.png", 30);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Texture_Effect_Invincible", pComponent)))
		return E_FAIL;





	/* Add CubeTexture */
	pComponent = Engine::CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_CUBE, L"../Bin/Resources/Cube/CubeTile_%d.dds", 6);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Texture_Cube", pComponent)))
		return E_FAIL;

	/* Add CubeTexture */
	pComponent = Engine::CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_CUBE, L"../Bin/Resources/CubeTile/CubeDDS/CubeTile_%d.dds", 103);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Texture_Monster", pComponent)))
		return E_FAIL;

	/*Create&Add Shader_Player */
	pComponent = CShader::Create(m_pGraphicDev, CShader::SDTYPE_NORMAL, L"../Bin/Resources/Shader/Player_%d.fx", 1);
	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Player", pComponent)))
		goto exception;

	/*Create&Add Shader_Light */
	pComponent = CShader::Create(m_pGraphicDev, CShader::SDTYPE_NORMAL, L"../Bin/Resources/Shader/ambient_%d.fx", 1);
	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Light", pComponent)))
		goto exception;

	/*Create&Add Shader_Weapon */
	pComponent = CShader::Create(m_pGraphicDev, CShader::SDTYPE_NORMAL, L"../Bin/Resources/Shader/Weapon_%d.fx", 1);
	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Weapon", pComponent)))
		goto exception;

	/*Create&Add Shader_Monster */
	pComponent = CShader::Create(m_pGraphicDev, CShader::SDTYPE_NORMAL, L"../Bin/Resources/Shader/Monster_%d.fx", 1);
	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Monster", pComponent)))
		goto exception;


	return NOERROR;

exception:
	MSG_BOX("AddComponent failed in Scene_Stage err: 30095");
	return E_FAIL;
}

HRESULT CScene_Stage::Add_Camera_ForStage()
{
 	CGameObject* pGameObject = nullptr;
	
	
	dynamic_cast<CCamera_Dynamic*>(m_pCamera_Manager->Get_Camera(L"Camera_Dynamic"))->Set_CameraMove(true);

	/*player camera*/
	/*setting target*/
	CGameObject* pCameraTarget = nullptr;
	CLayer*	pPlayerLayer = nullptr;

	pPlayerLayer = m_pObject_Manager->Get_Layer(SCENE_STAGE, L"Layer_Player");
	
	if(nullptr == pPlayerLayer)
		goto exception;

	pCameraTarget = pPlayerLayer->Get_ObjList().front();

	/*create*/
	pGameObject = CCamera_Player::Create(m_pGraphicDev, _vec3(0.f, 5.f, -5.f), _vec3(0.f, 0.f, 0.f), _vec3(0.f, 1.f, 0.f), dynamic_cast<CPlayer*>(pCameraTarget));

	if (nullptr == pGameObject)
		goto exception;
	
	if (FAILED(m_pCamera_Manager->Add_Camera(L"Camera_Player", pGameObject)))
		goto exception;



// 	pGameObject = CCamera_Dynamic::Create(m_pGraphicDev, _vec3(0.f, 5.f, -5.f), _vec3(0.f, 0.f, 0.f), _vec3(0.f, 1.f, 0.f));
// 
// 	if (nullptr == pGameObject)
// 		goto exception;
// 
// 	if (FAILED(m_pCamera_Manager->Add_Camera(L"Camera_Dynamic", pGameObject)))
// 		goto exception;
// 
 	m_pCamera_Manager->Change_Camera(L"Camera_Player");

	return NOERROR;

exception:
	MSG_BOX("AddCamera failed in Scene_Stage err: 30096");
	return E_FAIL;

}

HRESULT CScene_Stage::Add_BGM_ForStage()
{
	CSound_Manager::GetInstance()->StopSound(CSound_Manager::Channel_BGM);
	CSound_Manager::GetInstance()->PlayBGM(L"Stage.mp3", CSound_Manager::Channel_BGM, 0.8f);

	return NOERROR;
}

void CScene_Stage::Update_InputState(const _float & fTimeDelta)
{
	if (m_pInput_Device->Get_DIKeyState(DIK_F3) & 0x80)
	{
		m_pCamera_Manager->Change_Camera(L"Camera_Dynamic");
		m_pCamera_Manager->Set_CameraPos(_vec3(90.5f, 15.f, 72.f));
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_F4) & 0x80)
	{
		m_pCamera_Manager->Change_Camera(L"Camera_Player");
	}
	
	if (m_pInput_Device->Get_DIKeyState(DIK_F5) & 0x80)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if (m_pInput_Device->Get_DIKeyState(DIK_F6) & 0x80)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

CScene_Stage * CScene_Stage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Stage* pInstance = new CScene_Stage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Scene_Stage creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CScene_Stage::Free()
{
	/*decrease refcount*/
	Safe_Release(m_pInput_Device);


	_ulong dwRefCount = CScene::Free();
	return dwRefCount;
}

