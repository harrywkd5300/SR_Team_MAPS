#include "stdafx.h"
#include "..\Headers\jangPlayer.h"
#include "input_device.h"
#include "Object_Manager.h"
#include "Graphic_Device.h"
#include "LightObj.h"
#include "Camera_Manager.h"
//#include "Particle.h"
//#include "Particle_Bullet.h"
#include "Particle_Renderer.h"
#include "Well512a.h"
#include "IncludeWeapon.h"


CJangPlayer::CJangPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pTextureCom(nullptr)
	,m_pTerrainBufferCom(nullptr)
	,m_pBoneCom(nullptr)
	,m_fSpeed(0.f),m_fSprintSpeed(0.f)
	,m_iRecoilY(0)
	,m_fFiringRateTimer(0.f)
	,m_fRecoverTimer(0.f)
	,m_fMouseSensitive(0.f)
	/*test*/
	,m_eWalkState(WALK_RIGHT)
	, m_pRandomMgr(CRandomWell512::GetInstance())

{
	ZeroMemory(&m_fRecoverAngle, sizeof(_vec3));
	m_pInput_Device->AddRef();

	for (int i = 0; i < WEAPON_TYPE::WEAPON_END; ++i)
	{
		m_pWeapon[i] = nullptr;
	}
}

HRESULT CJangPlayer::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_Parameters()))
		return E_FAIL;

	if (FAILED(Ready_D3dFont()))
		return E_FAIL;
	
	if (FAILED(Ready_Weapon()))
		return E_FAIL;

	//m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);

	return NOERROR;
}

_int CJangPlayer::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	//m_pTransformCom->Fill_WorldMatrix();
	//m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);
	//return 0;
	//FixMouse();

	for (int i = 0; i < WEAPON_TYPE::WEAPON_END; ++i)
	{
		m_pWeapon[i]->Update_GameObject(fTimeDelta);
	}

	Update_Input(fTimeDelta);
	Recover_Recoil(fTimeDelta);


	m_pTransformCom->Fill_WorldMatrix();
	m_pTransformCom->On_Terrain(m_pTerrainBufferCom);

	/*BONE*/
	Transform_Test(fTimeDelta);
	Update_Bone(fTimeDelta);

	//if (RenderRun == false)
	//{
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);
	//	RenderRun = true;
	//}
	
	return 0;
}

_int CJangPlayer::LastUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CJangPlayer::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTransformCom)
		return;

	//m_pTransformCom->SetUp_MatrixToGraphicDev();

	SetShader(1);

	_uint numPasses = 0;

	StartShader(1);
	Render_Body();

	//m_pBufferCom->Render_Buffer();

	EndShader(1);

	//RenderRun = false;
	//Render_Bone();
	


	//m_pTransformCom->SetUp_MatrixToGraphicDev();
	//m_pTextureCom->SetUp_TextureToGraphicDev(0);
	//m_pBufferCom->Render_Buffer();

	//Render_Body();
	//Render_Bone();

	return;
}

void CJangPlayer::Render_Shadow(_matrix CameraWorldMatrix)
{
	if (nullptr == m_pBufferCom || nullptr == m_pTransformCom)
		return;

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &ShadowWorld);
	//m_pBufferCom->Render_Buffer();
	//

	D3DXMATRIX	matShadow;
	//D3DXVECTOR3 vLightPos = (_vec3)m_pLightMgr->GetLight(LIGHT_TYPE::DIRECTIONAL_LIGHT)->Get_LightInfo().Position;
	D3DXVECTOR3 vLightPos = (_vec3)m_pLightMgr->Get_NearLight(m_pTransformCom->Get_MatrixRow(CTransform::ROW_INFO::INFO_POSITION))->Get_LightInfo().Position;
	D3DXPLANE PlaneFloor(0.f, 1.f, 0.f, m_pTransformCom->Get_PositionY() * -1.f);  // 평면의 방정식	
	D3DXMatrixShadow(&matShadow, &D3DXVECTOR4(vLightPos.x, vLightPos.y, vLightPos.z, 1.f), &PlaneFloor);

	_matrix matParent, matWorld, matTrans;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matTrans, 0.f, -1.f, 0.f);

	///*Left Leg*/
	matParent = m_vecBoneCom[1]->Get_WorldMatrix();
	
	matWorld = matTrans *  matParent * matShadow;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_vecBodyCom[0]->Render_Buffer();

	///*Right Leg*/
	matParent = m_vecBoneCom[2]->Get_WorldMatrix();
	
	matWorld = matTrans * matParent * matShadow;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_vecBodyCom[0]->Render_Buffer();


	/* Body*/
	D3DXMatrixTranslation(&matTrans, 0.f, 1.25f, 0.f);
	matParent = m_vecBoneCom[0]->Get_WorldMatrix();

	matWorld = matTrans * matParent * matShadow;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_vecBodyCom[1]->Render_Buffer();

	/*Head*/
	D3DXMatrixTranslation(&matTrans, 0.f, 0.5f, 0.f);
	matParent = m_vecBoneCom[3]->Get_WorldMatrix();

	matWorld = matTrans * matParent * matShadow;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_vecBodyCom[2]->Render_Buffer();

	/*Left Arm*/
	D3DXMatrixTranslation(&matTrans, -0.4f, -0.75f, 0.f);
	matParent = m_vecBoneCom[4]->Get_WorldMatrix();

	matWorld = matTrans * matParent * matShadow;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_vecBodyCom[3]->Render_Buffer();


	/*Right Arm*/
	D3DXMatrixTranslation(&matTrans, +0.4f, -0.75f, 0.f);
	matParent = m_vecBoneCom[5]->Get_WorldMatrix();
	
	matWorld = matTrans * matParent * matShadow;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_vecBodyCom[3]->Render_Buffer();

	/*Left Arm*/
	D3DXMatrixTranslation(&matTrans, 0.f, -0.75f, 0.f);
	matParent = m_vecBoneCom[6]->Get_WorldMatrix();
	
	matWorld = matTrans * matParent * matShadow;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_vecBodyCom[3]->Render_Buffer();

	D3DXMatrixTranslation(&matTrans, 0.f, -0.75f, 0.f);
	matParent = m_vecBoneCom[7]->Get_WorldMatrix();
	matWorld = matTrans * matParent * matShadow;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_vecBodyCom[3]->Render_Buffer();
}

HRESULT CJangPlayer::Ready_Component()
{
	CComponent* pComponent = nullptr;

	/*------------Clone Component-----------*/

	/*Add TempCube*/
	pComponent = m_pBufferCom = dynamic_cast<CJang_Cube*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_Jang_Cube"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Buffer_Cube", pComponent));
	m_pBufferCom->AddRef();

	/*Add Transform*/
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
	m_pTransformCom->AddRef();

	/*Add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();

	/*Add Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Player"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Player", pComponent));
	m_pTextureCom->AddRef();

	/*Add Shader*/
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Shader_Player"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Shader_Player", pComponent));
	m_pShaderCom->AddRef();

	/*Add Material*/
	pComponent = m_pMatCom = dynamic_cast<CMaterial*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Material"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Material_Player", pComponent));
	m_pMatCom->SetUp_MaterialInfo(CMaterial::MATTYPE::MATTYPE_SHINY);
	m_pMatCom->AddRef();

	// For.Pickikng
	pComponent = m_pPickingCom = (Engine::CPicking*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Picking");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Com_Picking", pComponent));
	m_pPickingCom->AddRef();

	// --------------------- Test ----------------------------
	///*Add Particle_Renderer*/
	//m_pParticleRenderer = CParticle_Renderer::GetInstance();
	//m_pParticleRenderer->AddRef();

	///*Add Particle_Bullet*/
	//pComponent = m_pBulletParticle = dynamic_cast<CParticle_Bullet*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Particle_Bullet"));
	//if (nullptr == pComponent)
	//	goto exception;
	//m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Particle_Bullet", pComponent));
	//m_pBulletParticle->AddRef();
	//
	///*Add Bullet_Texture*/
	//pComponent = m_pBulletTexture = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Bullet_Flare"));
	//if (nullptr == pComponent)
	//	goto exception;
	//m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Bullet", pComponent));
	//m_pBulletTexture->AddRef();

	//if (FAILED(m_pParticleRenderer->Set_Particle_Type(
	//	CParticle_Renderer::PARTICLETYPE::PARTICLE_DIRECT_BULLET,
	//	L"Component_Texture_Bullet_Flare",
	//	m_pBulletTexture
	//)))
	//{
	//	MSG_BOX("Particle_Texture_SizeOver : PlayerWeapon");
	//}
	// --------------------- Test ----------------------------
	//////////////////////
	/*----------------Add Bone---------------------*/

	/*0 heap*/
 	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Bone");
 	if (nullptr == pComponent)
 		goto exception;
 	
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Bone0", pComponent));
	m_vecBoneCom.push_back(dynamic_cast<CBone*>(pComponent));
	pComponent->AddRef();

	/*1 left leg*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Bone");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Bone1", pComponent));
	m_vecBoneCom.push_back(dynamic_cast<CBone*>(pComponent));
	pComponent->AddRef();

	/*2 right leg*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Bone");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Bone2", pComponent));
	m_vecBoneCom.push_back(dynamic_cast<CBone*>(pComponent));
	pComponent->AddRef();

	/*3 neck*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Bone");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Bone3", pComponent));
	m_vecBoneCom.push_back(dynamic_cast<CBone*>(pComponent));
	pComponent->AddRef();

	/*4 left arm*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Bone");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Bone4", pComponent));
	m_vecBoneCom.push_back(dynamic_cast<CBone*>(pComponent));
	pComponent->AddRef();

	/*5 right arm*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Bone");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Bone5", pComponent));
	m_vecBoneCom.push_back(dynamic_cast<CBone*>(pComponent));
	pComponent->AddRef();

	/*6 left arm2*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Bone");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Bone6", pComponent));
	m_vecBoneCom.push_back(dynamic_cast<CBone*>(pComponent));
	pComponent->AddRef();
	
	/*7 right arm2*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Bone");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Bone7", pComponent));
	m_vecBoneCom.push_back(dynamic_cast<CBone*>(pComponent));
	pComponent->AddRef();


	/*========ADD BODY=========*/
	/*add leg*/
	/*0 Leg*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Cube_Leg");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Leg", pComponent));
	m_vecBodyCom.push_back(dynamic_cast<CJang_Cube*>(pComponent));
	pComponent->AddRef();

	/*add body*/
	/*1 body*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Cube_Body");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Body", pComponent));
	m_vecBodyCom.push_back(dynamic_cast<CJang_Cube*>(pComponent));
	pComponent->AddRef();

	/*2 Head*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Cube_Head");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Head", pComponent));
	m_vecBodyCom.push_back(dynamic_cast<CJang_Cube*>(pComponent));
	pComponent->AddRef();
	
	/*3 Arm*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Cube_Arm");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Arm", pComponent));
	m_vecBodyCom.push_back(dynamic_cast<CJang_Cube*>(pComponent));
	pComponent->AddRef();


	/*Add Cube_Bone*/
	pComponent = m_pBoneCubeCom = dynamic_cast<CCube_Color*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeCol"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"CubeCol", pComponent));
	m_pBoneCubeCom->AddRef();


///////////////////////////////////////////////////

	/*------------- Ref Component --------------*/
	/*Ref Terrain Buffer*/
	m_pTerrainBufferCom = dynamic_cast<Jang_CTerrain_Buffer*>(CObject_Manager::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain", 0, L"Buffer_Terrain"));
	m_pTerrainBufferCom->AddRef();

	return NOERROR;

exception:
	MSG_BOX("Player Component Ready Failed err: 13115");
	return E_FAIL;
}

HRESULT CJangPlayer::Ready_Parameters()
{
	ZeroMemory(&m_vRecoil, sizeof(_vec3));

	m_fRecoverTimer = 1.f;

	/*option parameters*/
	m_fMouseSensitive = 10.f;

	/*Player Parameters*/
	m_fSpeed = 2.f;
	m_fSprintSpeed = 15.f;

	/*Gun Parameters*/
	m_iRecoilY = 2;

	return NOERROR;
}

HRESULT CJangPlayer::Ready_D3dFont()
{
	m_pD3dFont = CGraphic_Device::GetInstance()->Get_Font();
	
	if (nullptr == m_pD3dFont)
	{
		MSG_BOX("Ready D3dFont Failed");
		return E_FAIL;
	}
	m_pD3dFont->AddRef();

	return NOERROR;
}

HRESULT CJangPlayer::Ready_Weapon()
{
	
	m_pWeapon[WEAPON_TYPE::WEAPON_SUB] = CPistol::Create(m_pGraphicDev);
	m_pWeapon[WEAPON_TYPE::WEAPON_SUB]->Set_User(this);
	m_pWeapon[WEAPON_TYPE::WEAPON_SUB]->AddRef();

	//m_pWeapon[WEAPON_TYPE::WEAPON_MAIN] = CRifle::Create(m_pGraphicDev);
	m_pWeapon[WEAPON_TYPE::WEAPON_MAIN] = CRifle::Create(m_pGraphicDev);
	m_pWeapon[WEAPON_TYPE::WEAPON_MAIN]->Set_User(this);
	m_pWeapon[WEAPON_TYPE::WEAPON_MAIN]->AddRef();

	m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY] = CShotgun::Create(m_pGraphicDev);
	m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY]->Set_User(this);
	m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY]->AddRef();

	m_pWeapon[WEAPON_TYPE::WEAPON_SUPER] = CChaingun::Create(m_pGraphicDev);
	m_pWeapon[WEAPON_TYPE::WEAPON_SUPER]->Set_User(this);
	m_pWeapon[WEAPON_TYPE::WEAPON_SUPER]->AddRef();

	eCurWeaponType = WEAPON_TYPE::WEAPON_SUPER;

	return NOERROR;
}


void CJangPlayer::Update_Input(const _float fTimeDelta)
{
	if (nullptr == m_pInput_Device)
		return;

#ifndef __J
	if (m_pInput_Device->Get_DIKeyState(DIK_LSHIFT) & 0x80)
	{
		if (m_pInput_Device->Get_DIKeyState(DIK_W) & 0x80)
			m_pTransformCom->Move_ToLook(m_fSprintSpeed, fTimeDelta);

		if (m_pInput_Device->Get_DIKeyState(DIK_A) & 0x80)
			m_pTransformCom->Move_ToLeft(m_fSprintSpeed, fTimeDelta);

		if (m_pInput_Device->Get_DIKeyState(DIK_D) & 0x80)
			m_pTransformCom->Move_ToRight(m_fSprintSpeed, fTimeDelta);

		if (m_pInput_Device->Get_DIKeyState(DIK_S) & 0x80)
			m_pTransformCom->Move_ToLook(-m_fSprintSpeed, fTimeDelta);

	}
	else /*not pressed shift*/
	{
		if (m_pInput_Device->Get_DIKeyState(DIK_W) & 0x80)
			m_pTransformCom->Move_ToLook(m_fSpeed, fTimeDelta);

		if (m_pInput_Device->Get_DIKeyState(DIK_A) & 0x80)
			m_pTransformCom->Move_ToLeft(m_fSpeed, fTimeDelta);

		if (m_pInput_Device->Get_DIKeyState(DIK_D) & 0x80)
			m_pTransformCom->Move_ToRight(m_fSpeed, fTimeDelta);

		if (m_pInput_Device->Get_DIKeyState(DIK_S) & 0x80)
			m_pTransformCom->Move_ToLook(-m_fSpeed, fTimeDelta);
	}
	/*MouseMove*/
	const _long& lMoveX = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_X);
	const _long& lMoveY = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_Y);

	if (lMoveY)
	{
		_float f = lMoveY * m_fMouseSensitive * fTimeDelta;
		m_pTransformCom->Rotation_X(f);
	}

	if (lMoveX)
	{
		_float f = lMoveX * m_fMouseSensitive * fTimeDelta;
		m_pTransformCom->Rotation_Y(f);
	}


	m_fFiringRateTimer += fTimeDelta;
	if (m_pInput_Device->Get_DIMouseState(CInput_Device::DIM_LBUTTON) & 0x80
		&& m_fFiringRateTimer > 0.1f)
	{
		m_fFiringRateTimer = 0;
		Recoil(fTimeDelta, 20, m_iRecoilY);
	}
#else
	m_eState = IDLE;

	if (m_pInput_Device->Get_DIKeyState(DIK_W) & 0x80)
	{
		m_pTransformCom->Move_ToLook(20.f, fTimeDelta);
		m_eState = WALK;
		Walk_Animation(fTimeDelta);
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_A) & 0x80)
		m_pTransformCom->Move_ToLeft(m_fSpeed, fTimeDelta);

	if (m_pInput_Device->Get_DIKeyState(DIK_D) & 0x80)
		m_pTransformCom->Move_ToRight(m_fSpeed, fTimeDelta);

	if (m_pInput_Device->Get_DIKeyState(DIK_S) & 0x80)
		m_pTransformCom->Move_ToLook(-m_fSpeed, fTimeDelta);

	if (m_pInput_Device->Get_DIKeyState(DIK_R) & 0x80)
		m_pWeapon[eCurWeaponType]->Reload_Weapon();

	if (m_pInput_Device->Get_DIKeyState(DIK_1) & 0x80)
		eCurWeaponType = WEAPON_TYPE::WEAPON_MAIN;

	if (m_pInput_Device->Get_DIKeyState(DIK_2) & 0x80)
		eCurWeaponType = WEAPON_TYPE::WEAPON_SUB;

	if (m_pInput_Device->Get_DIKeyState(DIK_3) & 0x80)
		eCurWeaponType = WEAPON_TYPE::WEAPON_HEAVY;

	if (m_pInput_Device->Get_DIKeyState(DIK_4) & 0x80)
		eCurWeaponType = WEAPON_TYPE::WEAPON_SUPER;

	const _long& lMoveX = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_X);
	const _long& lMoveY = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_Y);

// 	if (lMoveY)
// 	{
// 		_float f = lMoveY * m_fMouseSensitive * fTimeDelta;
// 		//m_pTransformCom->Rotation_X(f);
// 		m_vecBoneCom[3]->Rotation_X(f);
// 	}

	if (lMoveX)
	{
		_float f = lMoveX * m_fMouseSensitive * fTimeDelta;
		m_pTransformCom->Rotation_Y(f);
	}


	m_fFiringRateTimer += fTimeDelta;
	if (m_pInput_Device->Get_DIMouseState(CInput_Device::DIM_LBUTTON) & 0x80)
	{
		m_pWeapon[eCurWeaponType]->Use_Weapon(fTimeDelta,&m_fRecoverAngle);
		//m_fFiringRateTimer = 0;
		//Recoil(fTimeDelta, 20, m_iRecoilY);

		//// ---------------------- Test -------------------------
		////ResetBullet(const _vec3& vSrc, const _vec3& vDest, const _float &fSpeed);
		//D3DXVECTOR3 vPos(m_pTransformCom->Get_PositionX(), m_pTransformCom->Get_PositionY(), m_pTransformCom->Get_PositionZ());
		//vPos.y += 0.1f;
		//_vec3 *pPickPos = m_pPickingCom->Picking_ToBuffer(g_hWnd,0,0, m_pTerrainBufferCom);

		//_float fBulletSpeed = 20.f;
		//
		//if (pPickPos != nullptr)
		//{
		//	//cout << pPickPos->x << '/' << pPickPos->y << '/' << pPickPos->z << '/' << endl;
		//	m_pBulletParticle->ResetBullet(
		//		vPos,//vPos,	//*vCameraPos,
		//		D3DXVECTOR3(pPickPos->x, pPickPos->y, pPickPos->z ), //vPos,
		//		fBulletSpeed);

		//	if (m_pParticleRenderer->Set_Particle(
		//		CParticle_Renderer::PARTICLETYPE::PARTICLE_DIRECT_BULLET,
		//		L"Component_Texture_Bullet_Flare"
		//		, m_pBulletParticle
		//	) == false)
		//	{
		//		cout << "Failed" << endl;
		//	}
		//}
		
		

		// ---------------------- Test -------------------------
	}

#endif


}

void CJangPlayer::Recoil(const _float & fTimeDelta, const _int & iForceX, const _int & iForceY)
{
	//for recoil recovery timer
	m_fRecoverTimer = 0.f;

	//set recoilX
	//_float fMoveX = m_pRandomMgr->GetValueF(-iForceX * 0.5f, iForceX) * fTimeDelta;
	//m_pTransformCom->Rotation_Y(fMoveX);

	//m_fRecoverAngle.y += fMoveX;

	//set recoilY
	m_pTransformCom->Rotation_X(_float(-iForceY));

	m_fRecoverAngle.x += iForceY;

}

void CJangPlayer::Recover_Recoil(const _float & fTimeDelta)
{
	//m_fRecoverTimer += fTimeDelta;

	//if (m_fRecoverTimer < 0.5f)
	//{
	//	m_pTransformCom->Rotation_X(_float(m_iRecoilY) * 1.6f * fTimeDelta);
	//}
	_float m_fRecoverAngleX = m_fRecoverAngle.x * fTimeDelta;

	if (m_fRecoverAngle.x <= m_fRecoverAngleX)
	{
		m_fRecoverAngleX = m_fRecoverAngle.x;
		m_fRecoverAngle.x = 0.f;
		m_pTransformCom->Rotation_X(_float(m_fRecoverAngleX));
		return;
	}

	m_pTransformCom->Rotation_X(_float(m_fRecoverAngleX));
	m_fRecoverAngle.x -= m_fRecoverAngleX;

	_float m_fRecoverAngleY = m_fRecoverAngle.y * fTimeDelta;

	if (m_fRecoverAngle.y <= m_fRecoverAngleY)
	{
		m_fRecoverAngleY = m_fRecoverAngle.y;
		m_fRecoverAngle.y = 0.f;
		m_pTransformCom->Rotation_Y(_float(m_fRecoverAngleY));
		return;
	}

	m_pTransformCom->Rotation_Y(_float(m_fRecoverAngleY));
	m_fRecoverAngle.y -= m_fRecoverAngleY;
}

void CJangPlayer::Gravity_Test(const _float & fTimeDelta)
{

}

void CJangPlayer::FixMouse()
{
	SetCursorPos(BACKCX / 2, BACKCY / 2);
}

void CJangPlayer::Transform_Test(const _float & fTimeDelta)
{
	m_pTransformCom->Scaling(0.1f, 0.1f, 0.1f);
}

void CJangPlayer::Update_Bone(const _float & fTimeDelta)
{
	_float f = 60.f * fTimeDelta;


	if (IDLE == m_eState)
	{
		if (-5.f < m_vecBoneCom[1]->Get_DegreeX() && m_vecBoneCom[1]->Get_DegreeX() < 5.f)
		{
			m_vecBoneCom[1]->Set_DegreeX(0.f);
			m_vecBoneCom[2]->Set_DegreeX(0.f);
			m_eWalkState = WALK_RIGHT;
		}
		else if (m_vecBoneCom[1]->Get_DegreeX() > 0)
		{
			m_vecBoneCom[1]->Rotation_X(-f);
			m_vecBoneCom[2]->Rotation_X(f);

		}
		else if (m_vecBoneCom[1]->Get_DegreeX() < 0)
		{
			m_vecBoneCom[1]->Rotation_X(f);
			m_vecBoneCom[2]->Rotation_X(-f);
		}
	}


	// 	const _long& lMoveX = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_X);
	// 	const _long& lMoveY = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_Y);
	// 
	// 	if (lMoveY)
	// 	{
	// 		_float f = lMoveY * m_fMouseSensitive * fTimeDelta;
	// 		m_vecBoneCom[0]->Rotation_X(f);
	// 	}
	// 
	// 	if (lMoveX)
	// 	{
	// 		_float f = lMoveX * m_fMouseSensitive * fTimeDelta;
	// 		m_vecBoneCom[0]->Rotation_Y(f);
	// 	}

	//if (m_pInput_Device->Get_DIKeyState(DIK_Q) & 0x80)
	//	m_pTransformCom->Rotation_Y(-f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_E) & 0x80)
	//	m_pTransformCom->Rotation_Y(f);



	///*body*/
	//if (m_pInput_Device->Get_DIKeyState(DIK_R) & 0x80)
	//	m_vecBoneCom[0]->Rotation_X(-f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_F) & 0x80)
	//	m_vecBoneCom[0]->Rotation_X(f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_T) & 0x80)
	//	m_vecBoneCom[0]->Rotation_Y(-f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_G) & 0x80)
	//	m_vecBoneCom[0]->Rotation_Y(f);

	///*right arm*/
	//if (m_pInput_Device->Get_DIKeyState(DIK_Y) & 0x80)
	//	m_vecBoneCom[5]->Rotation_X(-f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_H) & 0x80)
	//	m_vecBoneCom[5]->Rotation_X(f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_U) & 0x80)
	//	m_vecBoneCom[5]->Rotation_Y(-f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_J) & 0x80)
	//	m_vecBoneCom[5]->Rotation_Y(f);

	///*left arm*/
	//if (m_pInput_Device->Get_DIKeyState(DIK_I) & 0x80)
	//	m_vecBoneCom[4]->Rotation_X(-f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_K) & 0x80)
	//	m_vecBoneCom[4]->Rotation_X(f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_O) & 0x80)
	//	m_vecBoneCom[4]->Rotation_Y(-f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_L) & 0x80)
	//	m_vecBoneCom[4]->Rotation_Y(f);

	///*right arm*/
	//if (m_pInput_Device->Get_DIKeyState(DIK_1) & 0x80)
	//	m_vecBoneCom[6]->Rotation_X(-f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_2) & 0x80)
	//	m_vecBoneCom[6]->Rotation_X(f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_3) & 0x80)
	//	m_vecBoneCom[6]->Rotation_Y(-f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_4) & 0x80)
	//	m_vecBoneCom[6]->Rotation_Y(f);

	///*left arm*/
	//if (m_pInput_Device->Get_DIKeyState(DIK_5) & 0x80)
	//	m_vecBoneCom[7]->Rotation_X(-f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_6) & 0x80)
	//	m_vecBoneCom[7]->Rotation_X(f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_7) & 0x80)
	//	m_vecBoneCom[7]->Rotation_Y(-f);

	//if (m_pInput_Device->Get_DIKeyState(DIK_8) & 0x80)
	//	m_vecBoneCom[7]->Rotation_Y(f);


	/*link& Position*/

	/*body*/
	m_vecBoneCom[0]->Parent(m_pTransformCom->Get_WorldMatrix());
	m_vecBoneCom[0]->Translation(0.f, 2.f, 0.f);
	m_vecBoneCom[0]->Fill_WorldMatrix();


	/*left leg*/
	m_vecBoneCom[1]->Parent(m_pTransformCom->Get_WorldMatrix());
	m_vecBoneCom[1]->Translation(-0.8f, 2.f, 0);
	m_vecBoneCom[1]->Fill_WorldMatrix();

	/*right leg*/
	m_vecBoneCom[2]->Parent(m_pTransformCom->Get_WorldMatrix());
	m_vecBoneCom[2]->Translation(+0.8f, 2.f, 0);
	m_vecBoneCom[2]->Fill_WorldMatrix();

	/*neck*/
	m_vecBoneCom[3]->Parent(m_vecBoneCom[0]->Get_WorldMatrix());
	m_vecBoneCom[3]->Translation(0.0f, 2.5f, 0.f);
	m_vecBoneCom[3]->Fill_WorldMatrix();


	/*left arm*/
	m_vecBoneCom[4]->Parent(m_vecBoneCom[0]->Get_WorldMatrix());
	m_vecBoneCom[4]->Translation(-1.f, 2.5f, 0.f);
	m_vecBoneCom[4]->Fill_WorldMatrix();

	/*Right arm*/
	m_vecBoneCom[5]->Parent(m_vecBoneCom[0]->Get_WorldMatrix());
	m_vecBoneCom[5]->Translation(1.f, 2.5f, 0.f);
	m_vecBoneCom[5]->Fill_WorldMatrix();

	/*left arm2*/
	m_vecBoneCom[6]->Parent(m_vecBoneCom[4]->Get_WorldMatrix());
	m_vecBoneCom[6]->Translation(-0.4f,-1.5f, 0.f);
	m_vecBoneCom[6]->Fill_WorldMatrix();

	/*Right arm2*/
	m_vecBoneCom[7]->Parent(m_vecBoneCom[5]->Get_WorldMatrix());
	m_vecBoneCom[7]->Translation(+0.4f, -1.5f, 0.f);
	m_vecBoneCom[7]->Fill_WorldMatrix();

}

void CJangPlayer::Update_Body(const _float & fTimeDelta)
{

}

void CJangPlayer::Walk_Animation(const _float & fTimeDelta)
{
	_float f = 180.f * fTimeDelta;

	/*1 Left 2 Right*/
	if (m_eState == WALK)
	{
		if (m_eWalkState == WALK_LEFT)
		{
			if (m_vecBoneCom[1]->Get_DegreeX() < -40.f)
				m_eWalkState = WALK_RIGHT;
			else
			{
				m_vecBoneCom[1]->Rotation_X(-f);
				m_vecBoneCom[2]->Rotation_X(f);
			}

		}
		else if (m_eWalkState == WALK_RIGHT)
		{
			if (m_vecBoneCom[1]->Get_DegreeX() > 40.f)
				m_eWalkState = WALK_LEFT;
			else
			{
				m_vecBoneCom[1]->Rotation_X(f);
				m_vecBoneCom[2]->Rotation_X(-f);
			}
		}
	}
}

void CJangPlayer::Render_Bone()
{
	//m_pTransformCom

	for (size_t i = 0; i < m_vecBoneCom.size(); ++i)
	{
		m_vecBoneCom[i]->SetUp_MatrixToGraphicDev();
		m_pBoneCubeCom->Render_Buffer();

		/*text*/
		_matrix matScale, matParent, matWorld, matTrans;

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 0.1f, 0.1f, 0.1f);
		D3DXMatrixTranslation(&matTrans, 0.1f, 0.1f, 0.f);
		matParent = m_vecBoneCom[i]->Get_WorldMatrix();

		matWorld = matScale *matTrans* matParent;
		//m_pShaderCom->GetShader(1)->SetMatrix("g_matWorld", &matWorld);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		_int x = _int(m_vecBoneCom[i]->Get_MatrixRow(CTransform::INFO_POSITION).x);
		_int y = _int(m_vecBoneCom[i]->Get_MatrixRow(CTransform::INFO_POSITION).y);
		_int z = _int(m_vecBoneCom[i]->Get_MatrixRow(CTransform::INFO_POSITION).z);

		_int rx = _int(m_vecBoneCom[i]->Get_DegreeX());
		_int ry = _int(m_vecBoneCom[i]->Get_DegreeY());
		_int rz = _int(m_vecBoneCom[i]->Get_DegreeZ());

		_tchar szBuf[128];
		wsprintf(szBuf, TEXT(" x: %d y: %d z: %d \n x: %d y: %d z: %d"), x, y, z, rx, ry, rz);

		m_pD3dFont->Render3DText(szBuf);
	}
}

void CJangPlayer::Render_Body()
{
// 	_matrix matParent, matWorld, matTrans;
// 	D3DXMatrixIdentity(&matWorld);
// 	D3DXMatrixTranslation(&matTrans, 0.f, -1.f, 0	_matrix matProj, matView;
// 	_matrix matWorldView;
// 	_matrix matWorldViewProjection;
// 	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
// 	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);.f);
// 	
// 
// 
// 	/*Left Leg*/
// 	matParent = m_vecBoneCom[1]->Get_WorldMatrix();
// 
// 	matWorld = matTrans * matParent;
// 	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
// 	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
// 	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorld", &matWorld);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
// 	m_pShaderCom->GetShader(1)->CommitChanges();
// 	//SetShader(1);
// 	//StartShader(1);
// 	m_vecBodyCom[0]->Render_Buffer();
// 	//EndShader(1);
// 
// 	/*Right Leg*/
// 	matParent = m_vecBoneCom[2]->Get_WorldMatrix();
// 
// 	matWorld = matTrans * matParent;
// 	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
// 	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
// 	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorld", &matWorld);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
// 	m_pShaderCom->GetShader(1)->CommitChanges();
// 	//SetShader(1);
// 	//StartShader(1);
// 	m_vecBodyCom[0]->Render_Buffer();
// 	//EndShader(1);
// 
// 	/* Body*/
// 	D3DXMatrixTranslation(&matTrans, 0.f, 1.25f, 0.f);
// 	matParent = m_vecBoneCom[0]->Get_WorldMatrix();
// 
// 	matWorld = matTrans * matParent;
// 	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
// 	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
// 	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorld", &matWorld);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
// 	m_pShaderCom->GetShader(1)->CommitChanges();
// 	//SetShader(1);
// 	//StartShader(1);
// 	m_vecBodyCom[1]->Render_Buffer();
// 	//EndShader(1);
// 
// 	/*Head*/
// 	D3DXMatrixTranslation(&matTrans, 0.f, 0.5f, 0.f);
// 	matParent = m_vecBoneCom[3]->Get_WorldMatrix();
// 
// 	matWorld = matTrans * matParent;
// 	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
// 	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
// 	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorld", &matWorld);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
// 	m_pShaderCom->GetShader(1)->CommitChanges();
// 	//SetShader(1);
// 	//StartShader(1);
// 	m_vecBodyCom[2]->Render_Buffer();
// 	//EndShader(1);
// 
// 	/*Left Arm*/
// 	D3DXMatrixTranslation(&matTrans, -0.4f, -0.75f, 0.f);
// 	matParent = m_vecBoneCom[4]->Get_WorldMatrix();
// 
// 	matWorld = matTrans * matParent;
// 	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
// 	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
// 	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorld", &matWorld);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
// 	m_pShaderCom->GetShader(1)->CommitChanges();
// 	//SetShader(1);
// 	//StartShader(1);
// 	m_vecBodyCom[3]->Render_Buffer();
// 	//EndShader(1);
// 
// 	/*Right Arm*/
// 	D3DXMatrixTranslation(&matTrans, +0.4f, -0.75f, 0.f);
// 	matParent = m_vecBoneCom[5]->Get_WorldMatrix();
// 
// 	matWorld = matTrans * matParent;
// 	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
// 	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
// 	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorld", &matWorld);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
// 	m_pShaderCom->GetShader(1)->CommitChanges();
// 	//SetShader(1);
// 	//StartShader(1);
// 	m_vecBodyCom[3]->Render_Buffer();
// 	//EndShader(1);
// 
// 
// 	/*Left Arm*/
// 	D3DXMatrixTranslation(&matTrans, 0.f, -0.75f, 0.f);
// 	matParent = m_vecBoneCom[6]->Get_WorldMatrix();
// 
// 	matWorld = matTrans * matParent;
// 	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
// 	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
// 	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorld", &matWorld);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
// 	m_pShaderCom->GetShader(1)->CommitChanges();
// 	//SetShader(1);
// 	//StartShader(1);
// 	m_vecBodyCom[3]->Render_Buffer();
// 	//EndShader(1);
// 
// 	/*Right Arm*/
// 	D3DXMatrixTranslation(&matTrans, 0.f, -0.75f, 0.f);
// 	matParent = m_vecBoneCom[7]->Get_WorldMatrix();
// 
// 	matWorld = matTrans * matParent;
// 	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
// 	
// 	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
// 	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorld", &matWorld);
// 	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
// 	m_pShaderCom->GetShader(1)->CommitChanges();
// 	//SetShader(1);
// 	//StartShader(1);
// 	m_vecBodyCom[3]->Render_Buffer();
// 	//EndShader(1);


}

CJangPlayer * CJangPlayer::Create(LPDIRECT3DDEVICE9 pGrahpicDev)
{
	CJangPlayer* pInstance = new CJangPlayer(pGrahpicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CPlayer Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CJangPlayer::Free()
{
	for (int i = 0; i < WEAPON_TYPE::WEAPON_END; ++i)
	{
		Safe_Release(m_pWeapon[i]);
		Safe_Release(m_pWeapon[i]);
	}

	//decrease refcount
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMatCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTerrainBufferCom);
	Safe_Release(m_pD3dFont);


	Safe_Release(m_pPickingCom);
	Safe_Release(m_pRandomMgr);

	//Safe_Release(m_pBoneCom);


	for (size_t i = 0; i < m_vecBoneCom.size(); ++i)
		Safe_Release(m_vecBoneCom[i]);

	for (size_t i = 0; i < m_vecBodyCom.size(); ++i)
		Safe_Release(m_vecBodyCom[i]);

	Safe_Release(m_pBoneCubeCom);


	//--------------- test ------------------
	//Safe_Release(m_pBulletParticle);
	//Safe_Release(m_pParticleRenderer);
	//Safe_Release(m_pBulletTexture);
	
	//--------------- test ------------------

	return CGameObject::Free();
}

void CJangPlayer::SetShader(_uint iShaderNum)
{
	_matrix matProj, matView, matWorld;

	D3DXVECTOR4 gWorldCameraPosition;

	//D3DLIGHT9 DirectionalLight;
	//DirectionalLight.Position.x = 0.f;
	//DirectionalLight.Position.y = 100.f;
	//DirectionalLight.Position.z = 100.f;
	////DirectionalLight.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
	////DirectionalLight.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	////DirectionalLight.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	//DirectionalLight.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	//DirectionalLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//DirectionalLight.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	
	//matWorld = testWorld;
	//m_pGraphicDev->GetTransform(D3DTS_WORLD, &matWorld);
	matWorld = m_pTransformCom->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_matrix matWorldView;
	_matrix matWorldViewProjection;
	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);

	
	_vec3 CameraPosition = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_CameraPos();
	//D3DXMatrixInverse(&matView, nullptr, &matView);
	//CameraPosition.x = matView._41;
	//CameraPosition.y = matView._42;
	//CameraPosition.z = matView._43;



	m_pShaderCom->GetShader(iShaderNum)->SetMatrix("g_matWorld", &matWorld);
	m_pShaderCom->GetShader(iShaderNum)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);

	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_vWorldCameraPos", &CameraPosition, sizeof(CameraPosition));

	CLight_Manager::GetInstance()->Set_ShaderLightList(m_pShaderCom->GetShader(iShaderNum));

	//m_pShaderCom->GetShader(iShaderNum)->SetValue("g_DirectionalLight.vPos", &DirectionalLight.Position, sizeof(DirectionalLight.Position));
	//m_pShaderCom->GetShader(iShaderNum)->SetValue("g_DirectionalLight.Ambient", &DirectionalLight.Ambient, sizeof(DirectionalLight.Ambient));
	//m_pShaderCom->GetShader(iShaderNum)->SetValue("g_DirectionalLight.Diffuse", &DirectionalLight.Diffuse, sizeof(DirectionalLight.Diffuse));
	//m_pShaderCom->GetShader(iShaderNum)->SetValue("g_DirectionalLight.Specular", &DirectionalLight.Specular, sizeof(DirectionalLight.Specular));

	//int iLightArrSize = MAX_LIGHT_SIZE;
	//m_pShaderCom->GetShader(iShaderNum)->SetValue("g_iLightArrSize", &iLightArrSize, sizeof(iLightArrSize));


	//D3DXHANDLE hLight;
	//D3DXHANDLE hLightPos;
	//D3DXHANDLE hLightAmbient;
	//D3DXHANDLE hLightDiffuse;
	//D3DXHANDLE hLightSpecular;
	//D3DXHANDLE hLightRadius;


	//for (_uint i = 0; i < iLightArrSize; ++i)
	//{
	//	CLight* pLightCom = 0;
	//	const  D3DLIGHT9* pLight = 0;
	//	pLightCom = m_pLight[i]->GetLightCom();
	//	pLight = &pLightCom->Get_LightInfo();

	//	hLight = m_pShaderCom->GetShader(iShaderNum)->GetParameterElement("g_PointLightArr", i);

	//	hLightPos = m_pShaderCom->GetShader(iShaderNum)->GetParameterByName(hLight, "vPos");
	//	hLightAmbient = m_pShaderCom->GetShader(iShaderNum)->GetParameterByName(hLight, "Ambient");
	//	hLightDiffuse = m_pShaderCom->GetShader(iShaderNum)->GetParameterByName(hLight, "Diffuse");
	//	hLightSpecular = m_pShaderCom->GetShader(iShaderNum)->GetParameterByName(hLight, "Specular");
	//	hLightRadius = m_pShaderCom->GetShader(iShaderNum)->GetParameterByName(hLight, "fRadius");

	//	m_pShaderCom->GetShader(iShaderNum)->SetValue(hLightPos, m_pLight[i]->GetTransformCom()->Get_MatrixRow(CTransform::ROW_INFO::INFO_POSITION), sizeof(_vec3));
	//	m_pShaderCom->GetShader(iShaderNum)->SetValue(hLightAmbient, &pLight->Ambient, sizeof(pLight->Ambient));
	//	m_pShaderCom->GetShader(iShaderNum)->SetValue(hLightDiffuse, &pLight->Diffuse, sizeof(pLight->Diffuse));
	//	m_pShaderCom->GetShader(iShaderNum)->SetValue(hLightSpecular, &pLight->Specular, sizeof(pLight->Specular));
	//	m_pShaderCom->GetShader(iShaderNum)->SetFloat(hLightRadius, pLight->Range);
	//	//m_pShaderCom->GetShader(iShaderNum)->SetValue("g_PointLightArr.vPos", m_pLight[i]->GetTransformCom()->Get_MatrixRow(CTransform::ROW_INFO::INFO_POSITION), sizeof(_vec3));
	//	//m_pShaderCom->GetShader(iShaderNum)->SetValue("g_PointLightArr.Ambient", &pLight->Ambient, sizeof(pLight->Ambient));
	//	//m_pShaderCom->GetShader(iShaderNum)->SetValue("g_PointLightArr.Diffuse", &pLight->Diffuse, sizeof(pLight->Diffuse));
	//	//m_pShaderCom->GetShader(iShaderNum)->SetValue("g_PointLightArr.Specular", &pLight->Specular, sizeof(pLight->Specular));
	//	//m_pShaderCom->GetShader(iShaderNum)->SetFloat("g_PointLightArr.fRadius", pLight->Range);
	//}
	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Ambient", &m_pMatCom->Get_MaterialInfo().Ambient, sizeof(m_pMatCom->Get_MaterialInfo().Ambient));
	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Diffuse", &m_pMatCom->Get_MaterialInfo().Diffuse, sizeof(m_pMatCom->Get_MaterialInfo().Diffuse));
	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Emissive", &m_pMatCom->Get_MaterialInfo().Emissive, sizeof(m_pMatCom->Get_MaterialInfo().Emissive));
	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Specular", &m_pMatCom->Get_MaterialInfo().Specular, sizeof(m_pMatCom->Get_MaterialInfo().Specular));
	m_pShaderCom->GetShader(iShaderNum)->SetFloat("g_Material.fPower", m_pMatCom->Get_MaterialInfo().Power);


	m_pShaderCom->GetShader(iShaderNum)->SetTexture("DiffuseMap_Tex", m_pTextureCom->GetTexture(1));
	m_pShaderCom->GetShader(iShaderNum)->SetTexture("SpecularMap_Tex", m_pTextureCom->GetTexture(2));
	m_pShaderCom->GetShader(iShaderNum)->SetTexture("NormalMap_Tex", m_pTextureCom->GetTexture(3));

}

void CJangPlayer::StartShader(_uint iShaderNum)
{
	_uint numPasses = 0;
	m_pShaderCom->GetShader(iShaderNum)->Begin(&numPasses, NULL);
	m_pShaderCom->GetShader(iShaderNum)->BeginPass(0);
			
}

void CJangPlayer::EndShader(_uint iShaderNum)
{
	m_pShaderCom->GetShader(iShaderNum)->EndPass();
	m_pShaderCom->GetShader(iShaderNum)->End();
}
