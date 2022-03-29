#include "stdafx.h"
#include "..\Headers\Player.h"
#include "input_device.h"
#include "Object_Manager.h"
#include "Graphic_Device.h"
#include "camera_manager.h"
#include "Camera_Player.h"
#include "Particle_Renderer.h"
#include "Well512a.h"
#include "IncludeWeapon.h"
#include "camera_dynamic.h"
#include "Collider_Manager.h"
#include "Sound_Manager.h"
#include "Hit_Ui.h"

#include "object_Manager.h"
#include "Layer.h"
#include "QuickSlot.h"
#include "QuickSlot_Reserve.h"

#include "Wave_Manager.h"

#include "House.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCharacter(pGraphicDev)
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pCamera_Manager(CCamera_Manager::GetInstance())
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pShaderCom(nullptr)
	,m_pTerrainBufferCom(nullptr)
	,m_pBoneCom(nullptr)
	,m_fSpeed(0.f),m_fSprintSpeed(0.f)
	,m_fMouseSensitive(0.f)
	, m_pCurWeapon(nullptr)
	/*animate*/
	, m_pRandomMgr(CRandomWell512::GetInstance())

{
	ZeroMemory(&m_fRecoverAngle, sizeof(_vec3));
	m_pInput_Device->AddRef();

	for (int i = 0; i < WEAPON_TYPE::WEAPON_END; ++i)
	{
		m_pWeapon[i] = nullptr;
	}
	m_eCharType = CHAR_TYPE::CHAR_TYPE_PLAYER;
}

HRESULT CPlayer::Ready_GameObject()
{
	if (FAILED(CCharacter::Ready_Character_Component()))
		return E_FAIL;
	
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_D3dFont()))
		return E_FAIL;

	if (FAILED(Load_Character_Model(L"../Data/TestPlayer.dat")))
		return E_FAIL;

	if (FAILED(Load_Character_Animate(L"../Data/TestAnimate.dat")))
		return E_FAIL;

	m_vecBone[BONE_TRANSFORM]->Set_ReceivedKeyFrame(false);
	//m_vecBone[BONE_TWOARMS]->Set_ReceivedKeyFrame(false);
	//m_vecBone[BONE_HEAD]->Set_ReceivedKeyFrame(false);
	m_vecBone[BONE_EYE]->Set_ReceivedKeyFrame(false);

	if (FAILED(Ready_Weapon()))
		return E_FAIL;

	if (FAILED(Ready_HitUI()))
		return E_FAIL;

	m_pColliderBox_AABB->Set_Scale(_vec3(0.2f,1.0f,0.2f));
	


	// Add_ColliderBox
	CCollider_Manager::GetInstance()->Add_Character(this);

	if (FAILED(Ready_Parameters()))
		return E_FAIL;

	return NOERROR;
}

_int CPlayer::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	FixMouse(BACKCX/2, BACKCY/2);
	ShowCursor(false);

	m_pTransformCom->Scaling(_vec3(0.1f, 0.1f, 0.1f));

	/*for test*/
	_bool IsCamera_Dynamic = false;
	
	CCamera* pCamera = m_pCamera_Manager->Get_CurCamera();
	
	if (dynamic_cast<CCamera_Dynamic*>(pCamera))
		IsCamera_Dynamic = true;

	if (m_bHit)
	{
		if (nullptr != m_pHitObject)
		{
			dynamic_cast<CHit_Ui*>(m_pHitObject)->Set_Alpha(150.f);
			m_pRendererCom->Add_RenderList(CRenderer::RENDER_ALPHA, m_pHitObject);
		}
		m_iHitTime += fTimeDelta * 1.f;
		if (m_iHitTime > 1.f)
		{
			m_iHitTime = 0.f;
			m_bHit = false;
		}
	}

	if(!IsCamera_Dynamic)/*if current camera is not dynamic camera*/
		Update_Input(fTimeDelta);

	m_pTransformCom->Fill_WorldMatrix();

	Update_Frame(fTimeDelta);
	Update_Animate(fTimeDelta);
	
	Set_PlayerLocal();

	Update_Bone2(fTimeDelta);
	Recover_Recoil(fTimeDelta);

	if (m_iCurHp != m_iHp)
	{
		m_iCurHp = m_iHp;
	}

	m_pTransformCom->On_Terrain(m_pTerrainBufferCom);

	/*ON_TERRAIN: notify height to camera*/
	const _float fTerrainHeight = m_pTransformCom->On_Terrain(m_pTerrainBufferCom);
	

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);

	for (size_t i = 0; i < EQUIP_END; ++i)
	{
		if (nullptr != m_pEquip[i])
			m_pEquip[i]->Update_GameObject(fTimeDelta);
	}

	//cout << "Pos :" << m_pTransformCom->Get_Position().x << " " << m_pTransformCom->Get_Position().y <<" "<< m_pTransformCom->Get_Position().z << endl;

	return 0;
}

_int CPlayer::LastUpdate_GameObject(const _float& fTimeDelta)
{
	CCharacter::LastUpdate_GameObject(fTimeDelta);
	Update_Bone2(fTimeDelta);

	return 0;
}

void CPlayer::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTransformCom)
		return;

	SetShader(1);
	StartShader(1);
	Render_Model();
	EndShader(1);

	//Render_Bone2();


	if (m_iCurStateIndex == STATE_FSPRINT)
	{
		m_pEffectParticle->ResetEffect(
			m_pTransformCom->Get_Position(),
			D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f),
			0.1f
		);
		if (m_pParticleRenderer->Set_Particle(
			CParticle_Renderer::PARTICLETYPE::PARTICLE_EFFECT,
			m_pSprintEffectTexTag,
			m_pEffectParticle) == false)
		{
			return;
			//cout << "Failed" << endl;
		}
	}
	//m_pColliderBox_AABB->Render_Debug_Buffer();

	

	//_matrix matWorld, matScale;
	//D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixScaling(&matScale, 0.3f, 0.3f, 0.3f);
	//D3DXMatrixTranslation(&matWorld, m_pTransformCom->Get_PositionX(), m_pTransformCom->Get_PositionY(), m_pTransformCom->Get_PositionZ());
	//matWorld = matScale * matWorld;

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	////m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_WorldMatrix());
	//m_pColliderBox_AABB->Render_Buffer();

	//m_pColliderBox_AABB->Render_Debug_Buffer();
}

_bool CPlayer::Hit(_uint iDamage, const _float & fLifeTime)
{
	m_iHp -= iDamage;

	m_bHit = true;
	return true;
}

HRESULT CPlayer::Ready_Component()
{
	CComponent* pComponent = nullptr;

	/*------------Clone Component-----------*/

	/*Add TempCube*/
	pComponent = m_pBufferCom = dynamic_cast<CCube_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeTex"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Buffer_Cube", pComponent));
	m_pBufferCom->AddRef();

	/*Add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();

	/*Add CubeTex*/
	pComponent = m_pBodyCubeBuffer = dynamic_cast<CCube_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeTex"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Buffer_CubeTex", pComponent));
	m_pBodyCubeBuffer->AddRef();

	/*Add Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Player"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Player", pComponent));
	m_pTextureCom->AddRef();

	/*Add CubeCol*/
	pComponent = m_pBoneCubeBuffer = dynamic_cast<CCube_Color*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeCol"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Buffer_CubeCol", pComponent));
	m_pBoneCubeBuffer->AddRef();

	/*-------------------------------------------------------------------*/
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

	// ----------------------------- Effect -----------------------------

	// ADD.SprintEffectTex
	pComponent = m_pSprintEffectTex = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Effect_Dust"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Sprint_Effect", pComponent));
	m_pSprintEffectTex->AddRef();
	m_pSprintEffectTexTag = L"Component_Texture_Effect_Dust";
	if (FAILED(m_pParticleRenderer->Set_Particle_Type(
		CParticle_Renderer::PARTICLETYPE::PARTICLE_EFFECT,
		L"Component_Texture_Effect_Dust",
		m_pSprintEffectTex
	)))
	{
		MSG_BOX("Particle_Texture_SizeOver : Player");
	}

	// ----------------------------- Effect -----------------------------

	/*------------- Ref Component --------------*/
	/*Ref Terrain Buffer*/
	m_pTerrainBufferCom = dynamic_cast<CTerrain_Buffer*>(CObject_Manager::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain", 0, L"Buffer_Terrain"));
	m_pTerrainBufferCom->AddRef();
	
	return NOERROR;

exception:
	MSG_BOX("Player Component Ready Failed err: 13115");
	return E_FAIL;
}

HRESULT CPlayer::Ready_Parameters()
{
	/*option parameters*/
	m_fMouseSensitive = 10.f;

	/*Player Parameters*/
	m_fSpeed = 2.f;
	m_fSprintSpeed = 5.f;
	m_iHp = 1000;
	m_eCharType = CHAR_TYPE::CHAR_TYPE_PLAYER;

	m_pTransformCom->Set_Position(_vec3(59.5f, 0.5f, 65.f));
	//m_pTransformCom->Set_Position(_vec3(59.5f, 0.5f, 0));

	return NOERROR;
}

HRESULT CPlayer::Ready_D3dFont()
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

HRESULT CPlayer::Ready_Weapon()
{
	for (int i = 0 ;i < WEAPON_TYPE::WEAPON_END; ++i)
	{
		m_pWeapon[i] = nullptr;
	}

	//m_pWeapon[WEAPON_TYPE::WEAPON_MAIN] = CShotgun::Create(m_pGraphicDev);
	//m_pWeapon[WEAPON_TYPE::WEAPON_MAIN]->Set_User(this);
	//m_pWeapon[WEAPON_TYPE::WEAPON_MAIN]->AddRef();
	//eCurWeaponType = WEAPON_TYPE::WEAPON_MAIN;

	// for (size_t i = 0; i < EQUIP_END; ++i)
	//		m_pEquip[i] = nullptr;

	// m_pEquip[EQUIP_MAIN] = m_pWeapon[WEAPON_TYPE::WEAPON_MAIN];
	// m_pCurWeapon = m_pEquip[EQUIP_MAIN];
	// m_pCurWeapon->Set_Equip(true);

	// m_pEquip[EQUIP_SUB] = m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY];
	 //m_pEquip[EQUIP_SUB]->Set_Equip(false);

	 // -------------------------------  
	//m_pWeapon[WEAPON_TYPE::WEAPON_SUB] = CPistol::Create(m_pGraphicDev);
	//m_pWeapon[WEAPON_TYPE::WEAPON_SUB]->Set_User(this);
	//m_pWeapon[WEAPON_TYPE::WEAPON_SUB]->AddRef();
	m_pWeapon[WEAPON_TYPE::WEAPON_SUB] = nullptr;

	m_pWeapon[WEAPON_TYPE::WEAPON_MAIN] = CRifle::Create(m_pGraphicDev);
	m_pWeapon[WEAPON_TYPE::WEAPON_MAIN]->Set_User(this);
	m_pWeapon[WEAPON_TYPE::WEAPON_MAIN]->AddRef();

	m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY] = CShotgun::Create(m_pGraphicDev);
	m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY]->Set_User(this);
	m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY]->AddRef();
	
	m_pWeapon[WEAPON_TYPE::WEAPON_SUPER] = nullptr;

 	//m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY] = CShotgun::Create(m_pGraphicDev);
 	//m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY]->Set_User(this);
 	//m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY]->AddRef();

	//m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY] = CRifle::Create(m_pGraphicDev);
	//m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY]->Set_User(this);
	//m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY]->AddRef();

	//m_pWeapon[WEAPON_TYPE::WEAPON_SUPER] = CChaingun::Create(m_pGraphicDev);
	//m_pWeapon[WEAPON_TYPE::WEAPON_SUPER]->Set_User(this);
	//m_pWeapon[WEAPON_TYPE::WEAPON_SUPER]->AddRef();

	eCurWeaponType = WEAPON_TYPE::WEAPON_MAIN;

	for (size_t i = 0; i < EQUIP_END; ++i)
		m_pEquip[i] = nullptr;

	m_pEquip[EQUIP_MAIN] = m_pWeapon[WEAPON_TYPE::WEAPON_MAIN];
	
	m_pCurWeapon = m_pEquip[EQUIP_MAIN];
	m_pCurWeapon->Set_Equip(true);

	m_pEquip[EQUIP_SUB] = m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY];
	m_pEquip[EQUIP_SUB]->Set_Equip(false);



	// --------------------------------------------
	//m_pEquip[EQUIP_SUB] = m_pWeapon[WEAPON_TYPE::WEAPON_HEAVY];
	//m_pEquip[EQUIP_SUB]->Set_Equip(false);


	return NOERROR;
}

HRESULT CPlayer::Ready_HitUI()
{
	CGameObject* pObject = m_pHitObject = CHit_Ui::Create(m_pGraphicDev);
	if (FAILED(CObject_Manager::GetInstance()->Add_GameObject(SCENE_STAGE, L"Layer_UI", pObject)))
		return E_FAIL;

	return NOERROR;
}

void CPlayer::Update_Input(const _float& fTimeDelta)
{
	if (nullptr == m_pInput_Device)
		return;

	CCamera* pCamera = m_pCamera_Manager->Get_CurCamera();

	if (!m_bSpecialState)
	{
		m_iCurStateIndex = STATE_IDLE;
		if (dynamic_cast<CCamera_Player*>(pCamera))
			dynamic_cast<CCamera_Player*>(pCamera)->Set_Event(CCamera_Player::EVENT_NONE);
	}


	if (m_pInput_Device->Get_DIKeyState(DIK_F11) & 0x80 && m_b1)
	{
		CObject_Manager::GetInstance()->Release_TargetLayer(SCENE_STAGE, L"Layer_Light");
		m_b1 = false;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_F12) & 0x80 && m_b2)
	{
		CWave_Manager::GetInstance()->Set_BossClearLight();
		m_b2 = false;
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_F10) & 0x80 )
	{
		m_b1 = true;
		m_b2 = true;
	}


	if (m_pInput_Device->Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3 vPosTmp = m_pTransformCom->Get_IFMoveToLeftVec(m_fSpeed, fTimeDelta);
		_vec3 vDir = (_vec3&)m_pTransformCom->Get_WorldMatrix().m[CTransform::INFO_LOOK];

		m_iSpeedCnt += fTimeDelta * 1.f;
		if (m_iSpeedCnt > 0.6f)
		{
			m_iSpeedCnt = 0.f;
			CSound_Manager::GetInstance()->PlaySound(L"FS_Gen_Dirt01.wav", CSound_Manager::Channel_Player, 0.f);
		}

		if (m_pColliderMgr->Process_AABB(&vDir, this, vPosTmp))
		{
			m_pTransformCom->Move_ToDirection(vDir, m_fSpeed*0.7f, fTimeDelta);
		}
		else
		{
			m_pTransformCom->Move_ToLeft(m_fSpeed*0.7f, fTimeDelta);
		}
			
		if (!m_bSpecialState)
			m_iCurStateIndex = STATE_RWALK;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_D) & 0x80)
	{
		//m_pTransformCom->Move_ToRight(m_fSpeed*0.5f, fTimeDelta);
		_vec3 vPosTmp = m_pTransformCom->Get_IFMoveToRightVec(m_fSpeed, fTimeDelta);
		_vec3 vDir = (_vec3&)m_pTransformCom->Get_WorldMatrix().m[CTransform::INFO_LOOK];

		m_iSpeedCnt += fTimeDelta * 1.f;
		if (m_iSpeedCnt > 0.6f)
		{
			m_iSpeedCnt = 0.f;
			CSound_Manager::GetInstance()->PlaySound(L"FS_Gen_Dirt01.wav", CSound_Manager::Channel_Player, 0.2f);
		}

		if (m_pColliderMgr->Process_AABB(&vDir, this, vPosTmp))
		{
			m_pTransformCom->Move_ToDirection(vDir, m_fSpeed*0.7f, fTimeDelta);
		}
		else
		{
			m_pTransformCom->Move_ToRight(m_fSpeed*0.7f, fTimeDelta);
		}
			
		if (!m_bSpecialState)
			m_iCurStateIndex = STATE_RWALK;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_LSHIFT) & 0x80 && m_pInput_Device->Get_DIKeyState(DIK_W) & 0x80 && !m_bSpecialState)
	{
			_vec3 vPosTmp = m_pTransformCom->Get_IFMoveToLookVec(m_fSprintSpeed, fTimeDelta);
			_vec3 vDir = (_vec3&)m_pTransformCom->Get_WorldMatrix().m[CTransform::INFO_LOOK];

			if (m_pColliderMgr->Process_AABB(&vDir, this, vPosTmp))
			{
				m_pTransformCom->Move_ToDirection(vDir, m_fSprintSpeed, fTimeDelta);
			}
			else
			{
				m_pTransformCom->Move_ToLook(m_fSprintSpeed, fTimeDelta);
			}

			if (!m_bSpecialState)
				m_iCurStateIndex = STATE_FSPRINT;
	}
	else if (m_pInput_Device->Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3 vPosTmp = m_pTransformCom->Get_IFMoveToLookVec(m_fSpeed, fTimeDelta);
		_vec3 vDir = (_vec3&)m_pTransformCom->Get_WorldMatrix().m[CTransform::INFO_LOOK];
			
		m_iSpeedCnt += fTimeDelta * 1.f;
		if (m_iSpeedCnt > 0.6f)
		{
			m_iSpeedCnt = 0.f;
			CSound_Manager::GetInstance()->PlaySound(L"FS_Gen_Dirt01.wav", CSound_Manager::Channel_Player, 0.2f);
		}

		if (m_pColliderMgr->Process_AABB(&vDir, this, vPosTmp))
		{
			m_pTransformCom->Move_ToDirection(vDir, m_fSpeed, fTimeDelta);
		}
		else
		{
			m_pTransformCom->Move_ToLook(m_fSpeed, fTimeDelta);
		}

		if (!m_bSpecialState)
			m_iCurStateIndex = STATE_FWALK;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3 vPosTmp = m_pTransformCom->Get_IFMoveToLookVec(-m_fSpeed, fTimeDelta);
		_vec3 vDir = (_vec3&)m_pTransformCom->Get_WorldMatrix().m[CTransform::INFO_LOOK];

		m_iSpeedCnt += fTimeDelta * 1.f;
		if (m_iSpeedCnt > 0.6f)
		{
			m_iSpeedCnt = 0.f;
			CSound_Manager::GetInstance()->PlaySound(L"FS_Gen_Dirt01.wav", CSound_Manager::Channel_Player, 0.2f);
		}

		if (m_pColliderMgr->Process_AABB(&vDir, this, vPosTmp))
		{
			m_pTransformCom->Move_ToDirection(vDir, -m_fSpeed*0.5f, fTimeDelta);
		}
		else
		{
			m_pTransformCom->Move_ToLook(-m_fSpeed*0.5f, fTimeDelta);
		}

		if (!m_bSpecialState)
			m_iCurStateIndex = STATE_FWALK;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_R) & 0x80 && Compare_FireState())
	{
		if (nullptr == m_pCurWeapon)
			return;

		if (m_pCurWeapon->Get_WeaponInfo().m_iCurBullet == m_pCurWeapon->Get_WeaponInfo().m_iMaxBullet)
			return;

		m_iCurStateIndex = STATE_RELOAD;
		m_bSpecialState = true;

		if (dynamic_cast<CCamera_Player*>(pCamera))
			dynamic_cast<CCamera_Player*>(pCamera)->Set_Event(CCamera_Player::EVENT_ZOOMIN);

		//m_pWeapon[eCurWeaponType]->Reload_Weapon();
		if (nullptr != m_pCurWeapon)
			m_pCurWeapon->Reload_Weapon();
	}

	if (m_pInput_Device->Get_DIMouseState(CInput_Device::DIM_LBUTTON) & 0x80 && Compare_FireState())
	{
		//m_pWeapon[eCurWeaponType]->Use_Weapon(fTimeDelta, &m_fRecoverAngle);
		if(nullptr != m_pCurWeapon)
			m_pCurWeapon->Use_Weapon(fTimeDelta, &m_fRecoverAngle);

	}
	
	if (m_pInput_Device->Get_DIMouseState(CInput_Device::DIM_RBUTTON) & 0x80 && Compare_FireState())
	{
		if (dynamic_cast<CCamera_Player*>(pCamera))
			dynamic_cast<CCamera_Player*>(pCamera)->Set_Event(CCamera_Player::EVENT_AIM);
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_1) & 0x80)
	{
		if (nullptr != m_pEquip[EQUIP_MAIN] && m_pCurWeapon != m_pEquip[EQUIP_MAIN])
		{
			m_iCurStateIndex = STATE_CHANGE;
			m_bSpecialState = true;

			if (dynamic_cast<CCamera_Player*>(pCamera))
				dynamic_cast<CCamera_Player*>(pCamera)->Set_Event(CCamera_Player::EVENT_ZOOMIN);

			m_pCurWeapon->Set_Equip(false);
			
			m_pCurWeapon = m_pEquip[EQUIP_MAIN];
			
			m_pCurWeapon->Set_Equip(true);

			auto& iter = CObject_Manager::GetInstance()->Get_Layer(SCENETYPE::SCENE_STAGE, L"Layer_Slot")->Get_ObjList().begin();
			auto& iter_end = CObject_Manager::GetInstance()->Get_Layer(SCENETYPE::SCENE_STAGE, L"Layer_Slot")->Get_ObjList().end();

			dynamic_cast<CQuickSlot_Reserve*>((*iter))->Set_ChangeItem(false);

			++iter;

			dynamic_cast<CQuickSlot*>((*iter))->Set_ChangeItem(true);
		}
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_2) & 0x80)
	{
		if (nullptr != m_pEquip[EQUIP_SUB] && m_pCurWeapon != m_pEquip[EQUIP_SUB])
		{
			m_iCurStateIndex = STATE_CHANGE;
			m_bSpecialState = true;

			if (dynamic_cast<CCamera_Player*>(pCamera))
				dynamic_cast<CCamera_Player*>(pCamera)->Set_Event(CCamera_Player::EVENT_ZOOMIN);

			m_pCurWeapon->Set_Equip(false);

			m_pCurWeapon = m_pEquip[EQUIP_SUB];

			m_pCurWeapon->Set_Equip(true);

			auto& iter = CObject_Manager::GetInstance()->Get_Layer(SCENETYPE::SCENE_STAGE, L"Layer_Slot")->Get_ObjList().begin();
			auto& iter_end = CObject_Manager::GetInstance()->Get_Layer(SCENETYPE::SCENE_STAGE, L"Layer_Slot")->Get_ObjList().end();

			dynamic_cast<CQuickSlot_Reserve*>((*iter))->Set_ChangeItem(true);

			++iter;

			dynamic_cast<CQuickSlot*>((*iter))->Set_ChangeItem(false);
		}
	}
	//if (m_pInput_Device->Get_DIKeyState(DIK_3) & 0x80)
		//eCurWeaponType = WEAPON_TYPE::WEAPON_HEAVY;

	//if (m_pInput_Device->Get_DIKeyState(DIK_4) & 0x80)
		//eCurWeaponType = WEAPON_TYPE::WEAPON_SUPER;

	const _long& lMoveX = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_X);
	const _long& lMoveY = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_Y);


	if (lMoveX)
	{
		_float f = lMoveX * m_fMouseSensitive * fTimeDelta;
		m_pTransformCom->Rotation_Y(f);
	}


	if (lMoveY)
	{
		_float f = lMoveY * m_fMouseSensitive * fTimeDelta;
		m_vecBone[BONE_EYE]->Rotation_HeadX(f);
	}

}

void CPlayer::LastUpdate_Input(const _float & fTimeDelta)
{

}
void CPlayer::Recover_Recoil(const _float & fTimeDelta)
{
	_float m_fRecoverAngleX = m_fRecoverAngle.x * fTimeDelta;

	if (m_fRecoverAngle.x <= m_fRecoverAngleX)
	{
		m_fRecoverAngleX = m_fRecoverAngle.x;
		m_fRecoverAngle.x = 0.f;

		m_vecBone[BONE_EYE]->Rotation_HeadX(_float(m_fRecoverAngleX));
		return;
	}

	m_vecBone[BONE_EYE]->Rotation_HeadX(_float(m_fRecoverAngleX));

	m_fRecoverAngle.x -= m_fRecoverAngleX;

	_float m_fRecoverAngleY = m_fRecoverAngle.y * fTimeDelta;

	if (m_fRecoverAngle.y <= m_fRecoverAngleY)
	{
		m_fRecoverAngleY = m_fRecoverAngle.y;
		m_fRecoverAngle.y = 0.f;
		m_vecBone[BONE_EYE]->Rotation_HeadY(_float(m_fRecoverAngleY));
		return;
	}

	m_vecBone[BONE_EYE]->Rotation_HeadY(_float(m_fRecoverAngleY));
	m_fRecoverAngle.y -= m_fRecoverAngleY;

}

void CPlayer::Gravity_Test(const _float & fTimeDelta)
{

}
void CPlayer::Set_PlayerLocal()
{
	_vec3 vPlayerPos = _vec3(m_pTransformCom->Get_PositionX(), m_pTransformCom->Get_PositionY(), m_pTransformCom->Get_PositionZ());

	if (36.5f < vPlayerPos.x && vPlayerPos.x < 48.5f)
	{
		eCharWay = ROOM7;
		return;
	}
	else if (48.5f < vPlayerPos.x  && vPlayerPos.x < 77.5f && 44.f < vPlayerPos.z && vPlayerPos.z < 56.5f)
	{
		eCharWay = LIVING5;
		return;
	}
	else if (48.5f < vPlayerPos.x  && vPlayerPos.x < 77.5f && 56.5f <= vPlayerPos.z && vPlayerPos.z < 63.5f)
	{
		eCharWay = LIVING4;
		return;
	}
	else if (48.5f < vPlayerPos.x  && vPlayerPos.x < 71.5f && 63.5f <= vPlayerPos.z && vPlayerPos.z < 80.5f)
	{
		eCharWay = LIVING2;
		return;
	}
	else if (71.5f <= vPlayerPos.x  && vPlayerPos.x < 90.f && 63.5f <= vPlayerPos.z && vPlayerPos.z < 79.5f)
	{
		eCharWay = LIVING3;
		return;
	}
	else if (48.5f < vPlayerPos.x  && vPlayerPos.x < 71.5f && 79.5f <= vPlayerPos.z && vPlayerPos.z < 89.f)
	{
		eCharWay = LIVING1;
		return;
	}
	else if (71.5f <= vPlayerPos.x  && vPlayerPos.x < 90.5f && 79.5f <= vPlayerPos.z && vPlayerPos.z < 89.f)
	{
		eCharWay = ROOM6;
		return;
	}
	else
	{
		eCharWay = OUTSIDE;
		return;
	}
}
void CPlayer::FixMouse(const _uint & iX, const _uint & iY)
{
	POINT			pt = { LONG(iX), LONG(iY) };

	ClientToScreen(g_hWnd, &pt);

	SetCursorPos(pt.x, pt.y);
}


void CPlayer::SetShader(_uint iShaderNum)
{
	_matrix matProj, matView, matWorld;

	D3DXVECTOR4 gWorldCameraPosition;

	matWorld = m_pTransformCom->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_matrix matWorldView;
	_matrix matWorldViewProjection;
	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);


	_vec3 CameraPosition = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_CameraPos();


	m_pShaderCom->GetShader(iShaderNum)->SetMatrix("g_matWorld", &matWorld);
	m_pShaderCom->GetShader(iShaderNum)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);

	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_vWorldCameraPos", &CameraPosition, sizeof(CameraPosition));

	CLight_Manager::GetInstance()->Set_ShaderLightList(m_pShaderCom->GetShader(iShaderNum));

	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Ambient", &m_pMatCom->Get_MaterialInfo().Ambient, sizeof(m_pMatCom->Get_MaterialInfo().Ambient));
	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Diffuse", &m_pMatCom->Get_MaterialInfo().Diffuse, sizeof(m_pMatCom->Get_MaterialInfo().Diffuse));
	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Emissive", &m_pMatCom->Get_MaterialInfo().Emissive, sizeof(m_pMatCom->Get_MaterialInfo().Emissive));
	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Specular", &m_pMatCom->Get_MaterialInfo().Specular, sizeof(m_pMatCom->Get_MaterialInfo().Specular));
	m_pShaderCom->GetShader(iShaderNum)->SetFloat("g_Material.fPower", m_pMatCom->Get_MaterialInfo().Power);


	m_pShaderCom->GetShader(iShaderNum)->SetTexture("DiffuseMap_Tex", m_pTextureCom->GetTexture(1));
	m_pShaderCom->GetShader(iShaderNum)->SetTexture("SpecularMap_Tex", m_pTextureCom->GetTexture(2));
	m_pShaderCom->GetShader(iShaderNum)->SetTexture("NormalMap_Tex", m_pTextureCom->GetTexture(3));
}

void CPlayer::StartShader(_uint iShaderNum)
{
	_uint numPasses = 0;
	m_pShaderCom->GetShader(iShaderNum)->Begin(&numPasses, NULL);
	m_pShaderCom->GetShader(iShaderNum)->BeginPass(0);
}

void CPlayer::EndShader(_uint iShaderNum)
{
	m_pShaderCom->GetShader(iShaderNum)->EndPass();
	m_pShaderCom->GetShader(iShaderNum)->End();
}

void CPlayer::Render_Shadow(_matrix CameraWorldMatrix)
{
	D3DXMATRIX	matShadow;
	//D3DXVECTOR3 vLightPos = (_vec3)m_pLightMgr->GetLight(LIGHT_TYPE::DIRECTIONAL_LIGHT)->Get_LightInfo().Position;
	D3DXVECTOR3 vLightPos = (_vec3)m_pLightMgr->Get_NearLight(m_pTransformCom->Get_MatrixRow(CTransform::ROW_INFO::INFO_POSITION))->Get_LightInfo().Position;
	D3DXPLANE PlaneFloor(0.f, 1.f, 0.f, m_pTransformCom->Get_PositionY()* -1.f);  // 평면의 방정식	
	D3DXMatrixShadow(&matShadow, &D3DXVECTOR4(vLightPos.x, vLightPos.y, vLightPos.z, 1.f), &PlaneFloor);

	_matrix matParent, matWorld, matTrans;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matTrans, 0.f, -1.f, 0.f);

	for (size_t i = 0; i < m_vecBodyInfo.size(); ++i)
	{
		_matrix matWorld, matScale, matTrans, matParent;
		D3DXMatrixTranslation(&matTrans, m_vecBodyInfo[i]->vPosition.x, m_vecBodyInfo[i]->vPosition.y, m_vecBodyInfo[i]->vPosition.z);
		D3DXMatrixScaling(&matScale, m_vecBodyInfo[i]->vScale.x, m_vecBodyInfo[i]->vScale.y, m_vecBodyInfo[i]->vScale.z);

		if (m_vecBodyInfo[i]->iParentIndex != -1)
		{
			matParent = m_vecBone[m_vecBodyInfo[i]->iParentIndex]->Get_WorldMatrix();

			matWorld = matScale*matTrans*matParent*matShadow;
		}
		else /*has not parent*/
		{
			matWorld = matScale* matTrans*matShadow;
		}

		m_pTextureCom->SetUp_TextureToGraphicDev(0, m_vecBodyInfo[i]->iTextureNum);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		m_pBodyCubeBuffer->Render_Buffer();

	}
}

void CPlayer::Update_Bone2(const _float & fTimeDelta)
{
 	/*Link Parent Vector*/
	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		_int iIndex = m_vecBone[i]->Get_ParentIndex();

		if (-1 == iIndex)
			continue;

		m_vecBone[i]->Parent(m_vecBone[iIndex]->Get_WorldMatrix());
		m_vecBone[i]->Fill_WorldMatrix();

		if (i == 6)
		{
			/*예외처리 Upper*/
			iIndex = m_vecBone[BONE_TWOARMS]->Get_ParentIndex();
			m_vecBone[BONE_TWOARMS]->Parent(m_vecBone[iIndex]->Get_WorldMatrix());
			m_vecBone[BONE_TWOARMS]->Fill_WorldMatrix();
		}
	}
}

void CPlayer::Render_Bone2()
{
	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		m_vecBone[i]->SetUp_MatrixToGraphicDev();
		m_pBoneCubeBuffer->Render_Buffer();
	}
}

void CPlayer::Render_Model()
{
	_matrix matProj, matView;
	_matrix matWorldView;
	_matrix matWorldViewProjection;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (size_t i = 0; i < m_vecBodyInfo.size(); ++i)
	{
		_matrix matWorld, matScale, matTrans, matParent;
		D3DXMatrixTranslation(&matTrans, m_vecBodyInfo[i]->vPosition.x, m_vecBodyInfo[i]->vPosition.y, m_vecBodyInfo[i]->vPosition.z);
		D3DXMatrixScaling(&matScale, m_vecBodyInfo[i]->vScale.x, m_vecBodyInfo[i]->vScale.y, m_vecBodyInfo[i]->vScale.z);

		if (m_vecBodyInfo[i]->iParentIndex != -1)
		{
			matParent = m_vecBone[m_vecBodyInfo[i]->iParentIndex]->Get_WorldMatrix();

			matWorld = matScale*matTrans*matParent;
		}
		else /*has not parent*/
		{
			matWorld = matScale* matTrans;
		}



		//m_pTextureCom->SetUp_TextureToGraphicDev(0, m_vecBodyInfo[i]->iTextureNum);
		//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		
		D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
		D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);

		m_pShaderCom->GetShader(1)->SetTexture("DiffuseMap_Tex", m_pTextureCom->GetTexture(m_vecBodyInfo[i]->iTextureNum+1) );
		m_pShaderCom->GetShader(1)->SetTexture("SpecularMap_Tex", m_pTextureCom->GetTexture(m_vecBodyInfo[i]->iTextureNum+2));
		m_pShaderCom->GetShader(1)->SetTexture("NormalMap_Tex", m_pTextureCom->GetTexture(m_vecBodyInfo[i]->iTextureNum+3));

		m_pShaderCom->GetShader(1)->SetMatrix("g_matWorld", &matWorld);
		m_pShaderCom->GetShader(1)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
		m_pShaderCom->GetShader(1)->CommitChanges();

		m_pBodyCubeBuffer->Render_Buffer();

	}
}

_bool CPlayer::Compare_AnimateType()
{
	if (m_iCurStateIndex == STATE_FWALK || m_iCurStateIndex == STATE_BWALK || m_iCurStateIndex == STATE_FSPRINT)
		return true;

	return false;
}

_bool CPlayer::Compare_FireState()
{
	/*Special State: STATE_CHANGE, STATE_RELOAD*/
	if(m_bSpecialState || STATE_FSPRINT == m_iCurStateIndex)
		return false;

	return true;
}


CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGrahpicDev)
{
	CPlayer* pInstance = new CPlayer(pGrahpicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CPlayer Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CPlayer::Free()
{
	for (int i = 0; i < WEAPON_TYPE::WEAPON_END; ++i)
	{
		Safe_Release(m_pWeapon[i]);
		_uint n = Safe_Release(m_pWeapon[i]);
		int m = 0;
	}

	//decrease refcount
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pTerrainBufferCom);
	Safe_Release(m_pD3dFont);


	for (size_t i = 0; i < m_vecBone.size(); ++i)
		Safe_Release(m_vecBone[i]);
	m_vecBone.clear();

	for (size_t i = 0; i < m_vecBodyInfo.size(); ++i)
		Safe_Delete(m_vecBodyInfo[i]);
	m_vecBodyInfo.clear();

	for (size_t i = 0; i < m_vvKeyFrameTime.size(); ++i)
	{
		for (size_t j = 0; j < m_vvKeyFrameTime[i].size(); ++j)
			Safe_Delete(m_vvKeyFrameTime[i][j]);
		m_vvKeyFrameTime[i].clear();
	}
	m_vvKeyFrameTime.clear();

	Safe_Release(m_pBoneCubeBuffer);
	Safe_Release(m_pBodyCubeBuffer);
	Safe_Release(m_pTextureCom);

	Safe_Release(m_pPickingCom);
	Safe_Release(m_pMatCom);
	Safe_Release(m_pShaderCom);
	//Safe_Release(m_pRandomMgr);

	Safe_Release(m_pSprintEffectTex);


	return CCharacter::Free();
}
