#include "stdafx.h"
#include "..\Headers\boss.h"
#include "object_Manager.h"
#include "Layer.h"
#include "Player.h"
#include "camera_manager.h"
#include "camera_player.h"
#include "Collider_Manager.h"
#include "WELL512a.h"
#include "Projectile.h"
#include "FireWeapon.h"
#include "fireball.h"
#include "Sound_Manager.h"

CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
	,m_pRendererCom(nullptr)
	,m_pBufferCom(nullptr)
	,m_pTextureCom(nullptr)
	,m_pTargetTransform(nullptr)
	,m_pTerrainBufferCom(nullptr)
	,m_pCamera_Manager(CCamera_Manager::GetInstance())

{
	//m_pCamera_Manager->AddRef();
	ZeroMemory(&m_tSkillInfo, sizeof(BOSSSKILL_INFO));
}

HRESULT CBoss::Ready_Monster(const _vec3 * vPos)
{

	if (FAILED(CCharacter::Ready_Character_Component()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Load_Character_Model(L"../Data/m16.dat")))
		return E_FAIL;

	if (FAILED(Load_Character_Animate(L"../Data/Monster_Boss_Animate.dat")))
		return E_FAIL;

	if (FAILED(Ready_Path()))
		return E_FAIL;

	if (FAILED(Ready_Parameters(vPos)))
		return E_FAIL;

	if (FAILED(Ready_Skill()))
		return E_FAIL;

	m_pColliderBox_AABB->Set_Scale(_vec3(5.0f, 30.0f, 5.0f));


	m_vecBone[BONE_TRANSFORM]->Set_ReceivedKeyFrame(false);
	Refresh_Target();

	return NOERROR;
}

_int CBoss::Update_GameObject(const _float & fTimeDelta)
{
	if (true == m_bIsDead)
	{
		if (!m_bSpecialState)
			return 1;
	}

	if (nullptr == m_pRendererCom || nullptr == m_pTransformCom)
		return -1;


	m_pWeapon->Update_GameObject(fTimeDelta);
	m_pWeaponFire->Update_GameObject(fTimeDelta);

	Update_Timer(fTimeDelta);

	const _vec3& vPlayerPos = _vec3(m_pTargetTransform->Get_PositionX(), m_pTargetTransform->Get_PositionY(), m_pTargetTransform->Get_PositionZ());

	if(!m_bSpecialState)
		LookAt(vPlayerPos);

	if (0 != m_fGetHitDelay)
	{
		m_fGetHitDelay = 0;
		m_iHp -= m_iGetHitDamage;

		if (m_iHp <= 0)
		{
			m_iHp = 0;
			m_bIsDead = true;

			m_iCurStateIndex = STATE_DEAD;
			m_bSpecialState = true;
			m_bDirectly_ChangeState = true;
		}
	}

	Set_MonsterLocal();

	_float fDistance_FromPlayer = 0.f;
	


	/*get Distance from player*/
	if (nullptr != m_pTargetTransform)
	{
		_vec3 vTargetPos = m_pTargetTransform->Get_Position();
		_vec3 vMyPos = m_vecBone[BONE_TRANSFORM]->Get_Position();

		_vec3 v = vTargetPos - vMyPos;
		fDistance_FromPlayer = D3DXVec3Length(&v);

	}
	
	m_fSoundLack += fTimeDelta * 1.f;

	if (m_fSoundLack > 18.f)
	{
		CSound_Manager::GetInstance()->PlaySound(L"Black Desert Online - Nouver Spawn Alerts.mp3", CSound_Manager::Channel_Mon, 0.6f);
		m_fSoundLack = 0.f;
	}



	if (STATE_NORMALATTACK == m_iCurStateIndex && 3 == m_dwFrame)
	{
		CCamera_Player* pCamera = dynamic_cast<CCamera_Player*>(m_pCamera_Manager->Get_CurCamera());
		
		if (nullptr != pCamera)
			(pCamera)->Set_OutEvent(CCamera_Player::OUTEVENT_WEAKSHAKE);
	}

	if (!m_bSpecialState)
		m_iCurStateIndex = STATE_IDLE;


	if (fDistance_FromPlayer < m_tSkillInfo.fNormalAttack_Range 
		&& m_tSkillInfo.fNormalAttack_Timer < 0.f)
	{ 
		m_tSkillInfo.fNormalAttack_Timer = m_tSkillInfo.fNormalAttack_CoolTime;

		m_iCurStateIndex = STATE_NORMALATTACK;
		m_bSpecialState = true;
		m_bLookAt = false;
	}
	//Black Desert Online - Nouver Spawn Alerts.mp3

	/*fireball*/
	if (fDistance_FromPlayer < m_tSkillInfo.fFireBall_Range
		&& m_tSkillInfo.fFireBall_Timer < 0.f)
	{
		m_tSkillInfo.fFireBall_Timer = m_tSkillInfo.fFireBall_Cooldown;
		m_bSpecialState = true;

		m_iCurStateIndex = STATE_FIREBALL;
		_vec3 Position(m_vecBone[4]->Get_WorldMatrix()._41, m_vecBone[4]->Get_WorldMatrix()._42, m_vecBone[4]->Get_WorldMatrix()._43);
		
		m_pWeapon->Use_Weapon_Monster(fTimeDelta, Position, m_pTargetTransform->Get_Position());
	}



	if (fDistance_FromPlayer < m_tSkillInfo.fSweepAttack_Range
		&& m_tSkillInfo.fSweepAttack_Timer < 0.f)
	{
		m_tSkillInfo.fSweepAttack_Timer = m_tSkillInfo.fSweepAttack_Cooldown;
		
		
		m_iCurStateIndex = STATE_SWEEP;
		m_bSpecialState = true;

		//m_iCurStateIndex = STATE_FIREBALL;
		//_vec3 Position(m_vecBone[4]->Get_WorldMatrix()._41, m_vecBone[4]->Get_WorldMatrix()._42, m_vecBone[4]->Get_WorldMatrix()._43);
		//m_pWeapon->Use_Weapon_Monster(fTimeDelta, _vec3(64.f,10.f,0.f), m_pTargetTransform->Get_Position());

		//m_pWeapon->Use_Weapon_Monster(fTimeDelta, Position, m_pTargetTransform->Get_Position());
		//m_pWeaponFire->Use_Weapon_Monster(fTimeDelta, Position, m_pTargetTransform->Get_Position());
	}
	

	//cout << "distance: " << fDistance_FromPlayer << endl;

	m_pTransformCom->Fill_WorldMatrix();

	Update_Frame(fTimeDelta);
	Update_Animate(fTimeDelta);

	Update_Bone(fTimeDelta);

	m_pTransformCom->On_Terrain(m_pTerrainBufferCom);
	
	Update_ExtraAnimate(fTimeDelta);

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);

	return 0;
}

_int CBoss::LastUpdate_GameObject(const _float & fTimeDelta)
{
	//if (m_bLastDead)
	if (m_bIsDead && !m_bSpecialState)
		return 1;


// 	m_pColliderBox_AABB->Set_Position(m_pTransformCom->Get_Position());
// 	m_pColliderBox_AABB->Update_ColliderBox_VertexBuffer();

	if(m_vecBossRightHand.size() >= 2)
	{
		_vec3 v;

		v = _vec3(m_vecBone[BONE_RARM]->Get_WorldMatrix()._41, m_vecBone[BONE_RARM]->Get_WorldMatrix()._42, m_vecBone[BONE_RARM]->Get_WorldMatrix()._43);
		v.y -= 2.f;
		m_vecBossRightHand[0]->Set_Position(v);
		m_vecBossRightHand[0]->Update_ColliderBox_VertexBuffer();

		
		v = _vec3(m_vecBone[9]->Get_WorldMatrix()._41, m_vecBone[9]->Get_WorldMatrix()._42, m_vecBone[9]->Get_WorldMatrix()._43);
		v.y -= 2.f;
		m_vecBossRightHand[1]->Set_Position(v);
		m_vecBossRightHand[1]->Update_ColliderBox_VertexBuffer();

		if (CCollider_Manager::GetInstance()->CheckCollAABB(m_vecBossRightHand[0], m_pPlayer->Get_ColliderBox_AABB()))
		{
			m_pPlayer->Hit(10.f, 3.f);
			//cout << "피격" << endl;
		}
		else if (CCollider_Manager::GetInstance()->CheckCollAABB(m_vecBossRightHand[1], m_pPlayer->Get_ColliderBox_AABB()))
		{
			m_pPlayer->Hit(10.f, 3.f);
			//cout << "피격" << endl;
		}
		
		m_vecBossRightHand[0]->Render_Debug_Buffer();
		m_vecBossRightHand[1]->Render_Debug_Buffer();

	}

	CMonster::LastUpdate_GameObject(fTimeDelta);

	return 0;
}

void CBoss::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTextureCom || nullptr == m_pTransformCom)
		return;

	if (m_bIsDead && !m_bSpecialState)
		return;

	SetShader(1);
	Render_Model();
	//m_pColliderBox_AABB->Render_Debug_Buffer();
	StartShader(1);
	EndShader(1);
}

HRESULT CBoss::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;
	/*Add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();

	/*Add CubeTex*/
	pComponent = m_pBufferCom = dynamic_cast<CCube_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeTex"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Cube_Buffer", pComponent));
	m_pBufferCom->AddRef();

	//pComponent = m_pTransformCom = dynamic_cast<CTransform*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	//if (nullptr == pComponent)
	//	goto exception;
	//m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
	//m_pTransformCom->AddRef();

	/*Add Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Zombie"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Cube_Texture", pComponent));
	m_pTextureCom->AddRef();

	/*Add TerrainBuffer*/
	m_pTerrainBufferCom = dynamic_cast<CTerrain_Buffer*>(CObject_Manager::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain", 0, L"Buffer_Terrain"));
	m_pTerrainBufferCom->AddRef();

	/*Add CubeTex*/
	pComponent = m_pBodyCubeBuffer = dynamic_cast<CCube_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeTex"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Buffer_CubeTex", pComponent));
	m_pBodyCubeBuffer->AddRef();

	/*Add ColliderBox_AABB*/

	for (size_t i = 0; i < 2; ++i)
	{
		CAABB_Buffer*	pAABBComponent= dynamic_cast<CAABB_Buffer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_ColliderBox_AABB"));

		if (nullptr == pAABBComponent)
			goto exception;

		m_vecBossRightHand.push_back(pAABBComponent);
	}
	m_vecBossRightHand[0]->Set_Scale(_vec3(4.f,4.f,4.f));
	m_vecBossRightHand[1]->Set_Scale(_vec3(3.f, 3.f, 3.f));


	// --------------------- Shader ------------------------
	/*Add Shader*/
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Shader_Monster"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Shader_Monster", pComponent));
	m_pShaderCom->AddRef();

	/*Add Material*/
	pComponent = m_pMatCom = dynamic_cast<CMaterial*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Material"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Material_Player", pComponent));
	m_pMatCom->SetUp_MaterialInfo(CMaterial::MATTYPE::MATTYPE_SHINY);
	m_pMatCom->AddRef();
	// --------------------- Shader ------------------------


	return NOERROR;

exception:
	MSG_BOX("Creeper Component Ready Failed err: 13115");
	return E_FAIL;
}

HRESULT CBoss::Ready_Parameters(const _vec3* vPos)
{

	/*set target_transform*/
	auto iter = CObject_Manager::GetInstance()->Get_Layer(SCENETYPE::SCENE_STAGE, L"Layer_Player")->Get_ObjList().begin();
	m_pTargetTransform = dynamic_cast<CPlayer*>(*iter)->Get_Transform();
	if (nullptr == m_pTargetTransform)
		return E_FAIL;
	m_pTargetTransform->AddRef();

	/*Ready Default Setting*/
	m_eCharType = CHAR_TYPE::CHAR_TYPE_MONSTER;

	_float fScaleSize = 1.f;

	/*set scale*/
	m_pTransformCom->Scaling(fScaleSize,fScaleSize,fScaleSize);

	if (nullptr != vPos)
		m_pTransformCom->Set_Position(*vPos);

	m_pTransformCom->Fill_WorldMatrix();

	m_iHp = 1200;
	m_fMonsterSpeed = 1.f;
	m_fMonsterSprintSpeed = 3.f;
	m_fAttackRange = 1.0f;
	m_fRecognizeRange = 5.f;


	m_tSkillInfo.fNormalAttack_Range = 20.f;
	m_tSkillInfo.fNormalAttack_CoolTime = 10.f;

	m_tSkillInfo.fFireBall_Range = 80.f;
	m_tSkillInfo.fFireBall_Cooldown = 15.f;

	m_tSkillInfo.fSweepAttack_Timer = 8.f;
	m_tSkillInfo.fSweepAttack_Range = 20.f;

	return NOERROR;
}

HRESULT CBoss::Ready_Path(void)
{
	_vec3 vDir1 = _vec3(89.5f, 0.f, 72.0f) - m_vMakePos;
	_vec3 vDir2 = _vec3(59.0f, 0.f, 88.5f) - m_vMakePos;

	_float fDist1 = D3DXVec3Length(&vDir1);
	_float fDist2 = D3DXVec3Length(&vDir2);

	if (fDist1 < fDist2)
		m_vDoorPos = _vec3(89.5f, 0.f, 72.0f);
	else
		m_vDoorPos = _vec3(59.0f, 0.f, 88.5f);

	this->Set_MoveLoad();

	return NOERROR;
}

HRESULT CBoss::Ready_Skill(void)
{
	m_pWeapon = CProjectile::Create(m_pGraphicDev);

	m_pWeapon->Set_Monster(this);

	m_pWeaponFire = CFireball::Create(m_pGraphicDev);
	m_pWeaponFire->Set_Monster(this);

	return NOERROR;
}

void CBoss::Update_Timer(const _float & fTimeDelta)
{
	m_tSkillInfo.fNormalAttack_Timer -= fTimeDelta;
	m_tSkillInfo.fFireBall_Timer -= fTimeDelta;
	m_tSkillInfo.fProjectile_Timer -= 0.f;
	m_tSkillInfo.fSweepAttack_Timer -= 0.f;

}

void CBoss::Update_Bone(const _float & fTimeDelta)
{
	/*Link Parent Vector*/
	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		_int iIndex = m_vecBone[i]->Get_ParentIndex();

		if (-1 == iIndex)
			continue;

		m_vecBone[i]->Parent(m_vecBone[iIndex]->Get_WorldMatrix());
		m_vecBone[i]->Fill_WorldMatrix();
	}
}

void CBoss::Update_ExtraAnimate(const _float & fTimeDelta)
{
}

void CBoss::Render_Model()
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
		D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
		D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);

		//cout << m_vecBodyInfo[i]->iTextureNum << endl;
		m_pShaderCom->GetShader(1)->SetTexture("DiffuseMap_Tex", m_pTextureCom->GetTexture(m_vecBodyInfo[i]->iTextureNum+1 ));
		m_pShaderCom->GetShader(1)->SetTexture("SpecularMap_Tex", m_pTextureCom->GetTexture(m_vecBodyInfo[i]->iTextureNum + 4));
		m_pShaderCom->GetShader(1)->SetTexture("NormalMap_Tex", m_pTextureCom->GetTexture(m_vecBodyInfo[i]->iTextureNum + 7));

		m_pShaderCom->GetShader(1)->SetMatrix("g_matWorld", &matWorld);
		m_pShaderCom->GetShader(1)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
		m_pShaderCom->GetShader(1)->CommitChanges();

		m_pBodyCubeBuffer->Render_Buffer();
		//m_pBodyCubeBuffer->Render_Buffer();

	}
}

void CBoss::Set_MoveLoad(void)
{
	// 몬스터 시작이 1인경우
	iMoveIdx[0][0][0] = LIVING1;  	iMoveIdx[0][0][1] = LIVING2;
	iMoveIdx[0][1][0] = LIVING1;  	iMoveIdx[0][1][1] = LIVING2; iMoveIdx[0][1][2] = LIVING3;
	iMoveIdx[0][2][0] = LIVING1;  	iMoveIdx[0][2][1] = LIVING2; iMoveIdx[0][2][2] = ROOM7;
	iMoveIdx[0][3][0] = LIVING1;  	iMoveIdx[0][3][1] = LIVING2; iMoveIdx[0][3][2] = LIVING4;
	iMoveIdx[0][4][0] = LIVING1;  	iMoveIdx[0][4][1] = LIVING2; iMoveIdx[0][4][2] = LIVING4; iMoveIdx[0][4][3] = LIVING5;
	iMoveIdx[0][5][0] = LIVING1;  	iMoveIdx[0][5][1] = LIVING2; iMoveIdx[0][5][2] = LIVING3; iMoveIdx[0][5][3] = ROOM6;
	// 몬스터 시작이 2인경우
	iMoveIdx[1][0][0] = LIVING2;  	iMoveIdx[1][0][1] = LIVING1;
	iMoveIdx[1][1][0] = LIVING2;  	iMoveIdx[1][1][1] = LIVING3;
	iMoveIdx[1][2][0] = LIVING2;  	iMoveIdx[1][2][1] = ROOM7;
	iMoveIdx[1][3][0] = LIVING2;  	iMoveIdx[1][3][1] = LIVING4;
	iMoveIdx[1][4][0] = LIVING2;  	iMoveIdx[1][4][1] = LIVING4; iMoveIdx[1][4][2] = LIVING5;
	iMoveIdx[1][5][0] = LIVING2;  	iMoveIdx[1][5][1] = LIVING3; iMoveIdx[1][5][2] = ROOM6;
	// 몬스터 시작이 3인경우
	iMoveIdx[2][0][0] = LIVING3;  	iMoveIdx[2][0][1] = LIVING2;
	iMoveIdx[2][1][0] = LIVING3;  	iMoveIdx[2][1][1] = ROOM6;
	iMoveIdx[2][2][0] = LIVING3;  	iMoveIdx[2][2][1] = LIVING2; iMoveIdx[2][2][2] = LIVING1;
	iMoveIdx[2][3][0] = LIVING3;  	iMoveIdx[2][3][1] = LIVING2; iMoveIdx[2][3][2] = LIVING4;
	iMoveIdx[2][4][0] = LIVING3;  	iMoveIdx[2][4][1] = LIVING2; iMoveIdx[2][4][2] = ROOM7;
	iMoveIdx[2][5][0] = LIVING3;  	iMoveIdx[2][5][1] = LIVING2; iMoveIdx[2][5][2] = LIVING4; iMoveIdx[2][5][3] = LIVING5;
	// 몬스터 시작이 4인경우
	iMoveIdx[3][0][0] = LIVING4;  	iMoveIdx[3][0][1] = LIVING2;
	iMoveIdx[3][1][0] = LIVING4;  	iMoveIdx[3][1][1] = LIVING5;
	iMoveIdx[3][2][0] = LIVING4;  	iMoveIdx[3][2][1] = LIVING2; iMoveIdx[3][2][2] = LIVING1;
	iMoveIdx[3][3][0] = LIVING4;  	iMoveIdx[3][3][1] = LIVING2; iMoveIdx[3][3][2] = ROOM7;
	iMoveIdx[3][4][0] = LIVING4;  	iMoveIdx[3][4][1] = LIVING2; iMoveIdx[3][4][2] = LIVING3;
	iMoveIdx[3][5][0] = LIVING4;  	iMoveIdx[3][5][1] = LIVING2; iMoveIdx[3][5][2] = LIVING3; iMoveIdx[3][5][3] = ROOM6;
	// 몬스터 시작이 5인경우
	iMoveIdx[4][0][0] = LIVING5;  	iMoveIdx[4][0][1] = LIVING4;
	iMoveIdx[4][1][0] = LIVING5;  	iMoveIdx[4][1][1] = LIVING4; iMoveIdx[4][1][2] = LIVING2;
	iMoveIdx[4][2][0] = LIVING5;  	iMoveIdx[4][2][1] = LIVING4; iMoveIdx[4][2][2] = LIVING2; iMoveIdx[4][2][3] = LIVING1;
	iMoveIdx[4][3][0] = LIVING5;  	iMoveIdx[4][3][1] = LIVING4; iMoveIdx[4][3][2] = LIVING2; iMoveIdx[4][3][3] = ROOM7;
	iMoveIdx[4][4][0] = LIVING5;  	iMoveIdx[4][4][1] = LIVING4; iMoveIdx[4][4][2] = LIVING2; iMoveIdx[4][4][3] = LIVING3;
	iMoveIdx[4][5][0] = LIVING5;  	iMoveIdx[4][5][1] = LIVING4; iMoveIdx[4][5][2] = LIVING2; iMoveIdx[4][5][3] = LIVING3; iMoveIdx[4][5][4] = ROOM6;
	// 몬스터 시작이 6인경우
	iMoveIdx[5][0][0] = ROOM6;  	iMoveIdx[5][0][1] = LIVING3;
	iMoveIdx[5][1][0] = ROOM6;  	iMoveIdx[5][1][1] = LIVING3; iMoveIdx[5][1][2] = LIVING2;
	iMoveIdx[5][2][0] = ROOM6;  	iMoveIdx[5][2][1] = LIVING3; iMoveIdx[5][2][2] = LIVING2; iMoveIdx[5][2][3] = LIVING1;
	iMoveIdx[5][3][0] = ROOM6;  	iMoveIdx[5][3][1] = LIVING3; iMoveIdx[5][3][2] = LIVING2; iMoveIdx[5][3][3] = LIVING4;
	iMoveIdx[5][4][0] = ROOM6;  	iMoveIdx[5][4][1] = LIVING3; iMoveIdx[5][4][2] = LIVING2; iMoveIdx[5][4][3] = ROOM7;
	iMoveIdx[5][5][0] = ROOM6;  	iMoveIdx[5][5][1] = LIVING3; iMoveIdx[5][5][2] = LIVING2; iMoveIdx[5][5][3] = LIVING4; iMoveIdx[5][5][4] = LIVING5;
	// 몬스터 시작이 7인경우
	iMoveIdx[6][0][0] = ROOM7;  	iMoveIdx[6][0][1] = LIVING2;
	iMoveIdx[6][1][0] = ROOM7;  	iMoveIdx[6][1][1] = LIVING2; iMoveIdx[6][1][2] = LIVING1;
	iMoveIdx[6][2][0] = ROOM7;  	iMoveIdx[6][2][1] = LIVING2; iMoveIdx[6][2][2] = LIVING3;
	iMoveIdx[6][3][0] = ROOM7;  	iMoveIdx[6][3][1] = LIVING2; iMoveIdx[6][3][2] = LIVING4;
	iMoveIdx[6][4][0] = ROOM7;  	iMoveIdx[6][4][1] = LIVING2; iMoveIdx[6][4][2] = LIVING4; iMoveIdx[6][4][3] = LIVING5;
	iMoveIdx[6][5][0] = ROOM7;  	iMoveIdx[6][5][1] = LIVING2; iMoveIdx[6][5][2] = LIVING3; iMoveIdx[6][5][3] = ROOM6;
}

void CBoss::Move_Monster(const _float & fTimeDelta)
{
	const _vec3& vPlayerPos = _vec3(m_pTargetTransform->Get_PositionX(), m_pTargetTransform->Get_PositionY(), m_pTargetTransform->Get_PositionZ());
	const _vec3& vMyPos = _vec3(m_pTransformCom->Get_PositionX(), m_pTransformCom->Get_PositionY(), m_pTransformCom->Get_PositionZ());

	auto iter = CObject_Manager::GetInstance()->Get_Layer(SCENETYPE::SCENE_STAGE, L"Layer_Player")->Get_ObjList().begin();

	CHARWAY playerWay = dynamic_cast<CPlayer*>((*iter))->Get_CharWay();

	if (OUTSIDE == playerWay)
	{
		m_vArrivePos = vPlayerPos;

		/*if Inside*/
		_vec3 vDir_ToDest = m_vArrivePos - vMyPos;
		vDir_ToDest.y = 0.f;
		D3DXVec3Normalize(&vDir_ToDest, &vDir_ToDest);


		const _float f = D3DXVec3Dot(&vDir_ToDest, &_vec3(0.f, 0.f, 1.f));
		_float fRadian = acosf(f);

		if (vMyPos.x > m_vArrivePos.x)
			fRadian = 2 * D3DX_PI - fRadian;

		m_pTransformCom->Set_DegreeY(D3DXToDegree(fRadian));

		_vec3 vPosTmp = m_pTransformCom->Get_IFMoveToLookVec(m_fMonsterSpeed, fTimeDelta);
		_vec3 vDir = (_vec3&)m_pTransformCom->Get_WorldMatrix().m[CTransform::INFO_LOOK];

		if (m_pColliderMgr->Process_AABB(&vDir, this, vPosTmp))
		{
			m_pTransformCom->Move_ToDirection(vDir, m_fMonsterSpeed, fTimeDelta);
		}
		else
		{
			m_pTransformCom->Move_ToLook(m_fMonsterSpeed, fTimeDelta);
		}

		return;
	}

	/* if OUTSIDE*/
	if (OUTSIDE == eCharWay && !m_bInHouse)
	{
		_vec3 vDir_ToDoor = m_vDoorPos - vMyPos;
		vDir_ToDoor.y = 0.f;
		D3DXVec3Normalize(&vDir_ToDoor, &vDir_ToDoor);

		const _float f = D3DXVec3Dot(&vDir_ToDoor, &_vec3(0.f, 0.f, 1.f));
		_float fRadian = acosf(f);

		if (vMyPos.x > m_vDoorPos.x)
			fRadian = 2 * D3DX_PI - fRadian;

		m_pTransformCom->Set_DegreeY(D3DXToDegree(fRadian));

		_vec3 vPosTmp = m_pTransformCom->Get_IFMoveToLookVec(m_fMonsterSpeed, fTimeDelta);
		_vec3 vDir = (_vec3&)m_pTransformCom->Get_WorldMatrix().m[CTransform::INFO_LOOK];

		if (m_pColliderMgr->Process_AABB(&vDir, this, vPosTmp))
		{
			m_pTransformCom->Move_ToDirection(vDir, m_fMonsterSpeed, fTimeDelta);
		}
		else
		{
			m_pTransformCom->Move_ToLook(m_fMonsterSpeed, fTimeDelta);
		}

		m_pTransformCom->Move_ToLook(m_fMonsterSpeed, fTimeDelta);



		if (OUTSIDE != eCharWay)
			m_bInHouse = true;

		return;
	}


	for (int i = 0; i < 7; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			if (NONE == iMoveIdx[eCharWay][i][j])
			{
				if (playerWay == iMoveIdx[eCharWay - 1][i][j - 1])
				{
					m_iStartPos = i;
					m_iEndPos = j;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 6; ++i)
		m_MoveArray[i] = NONE;

	for (int i = 0; i < m_iEndPos; ++i)
		m_MoveArray[i] = iMoveIdx[eCharWay - 1][m_iStartPos][i];

	if (m_PlayerArrIdx != playerWay)
	{
		m_PlayerArrIdx = playerWay;
		m_iMoveIdx = 0;
	}
	else
	{
		if (m_MoveArray[m_iMoveIdx] == eCharWay)
		{
			if (2 != m_iEndPos)
				++m_iMoveIdx;
		}
	}

	switch (m_MoveArray[m_iMoveIdx])
	{
	case LIVING1:
		if (LIVING2 == playerWay)
			m_vArrivePos = vPlayerPos;
		if (LIVING3 == playerWay)
			m_vArrivePos = vPlayerPos;
		if (LIVING4 == playerWay)
			m_vArrivePos = vPlayerPos;
		if (LIVING5 == playerWay)
			m_vArrivePos = vPlayerPos;
		if (ROOM6 == playerWay)
			m_vArrivePos = _vec3(73.f, 0.5f, 77.5f);
		if (ROOM7 == playerWay)
			m_vArrivePos = _vec3(46.f, 0.5f, 73.f);
		break;

	case LIVING2:

		if (LIVING1 == playerWay)
			m_vArrivePos = vPlayerPos;

		if (LIVING3 == playerWay)
			m_vArrivePos = vPlayerPos;

		if (LIVING4 == playerWay)
			m_vArrivePos = vPlayerPos;

		if (LIVING5 == playerWay)
			m_vArrivePos = vPlayerPos;

		if (ROOM6 == playerWay)
			m_vArrivePos = _vec3(73.f, 0.5f, 77.5f);

		if (ROOM7 == playerWay)
			m_vArrivePos = _vec3(46.f, 0.5f, 73.f);

		break;

	case LIVING3:

		if (LIVING1 == playerWay)
			m_vArrivePos = vPlayerPos;

		if (LIVING2 == playerWay)
			m_vArrivePos = vPlayerPos;

		if (LIVING4 == playerWay)
			m_vArrivePos = vPlayerPos;

		if (LIVING5 == playerWay)
			m_vArrivePos = vPlayerPos;

		if (ROOM6 == playerWay)
			m_vArrivePos = _vec3(73.f, 0.5f, 81.5f);

		if (ROOM7 == playerWay)
			m_vArrivePos = _vec3(46.f, 0.5f, 73.f);

		break;

	case LIVING4:

		if (LIVING1 == playerWay)
			m_vArrivePos = vPlayerPos;

		if (LIVING2 == playerWay)
			m_vArrivePos = vPlayerPos;

		if (LIVING3 == playerWay)
			m_vArrivePos = vPlayerPos;
		if (LIVING5 == playerWay)
			m_vArrivePos = _vec3(62.5f, 0.5f, 56.f);
		if (ROOM6 == playerWay)
			m_vArrivePos = _vec3(73.f, 0.5f, 77.5f);
		if (ROOM7 == playerWay)
			m_vArrivePos = _vec3(47.5f, 0.5f, 73.f);

		break;
	case LIVING5:
		if (LIVING1 == playerWay)
			m_vArrivePos = m_vArrivePos;
		if (LIVING2 == playerWay)
			m_vArrivePos = m_vArrivePos;
		if (LIVING4 == playerWay)
			m_vArrivePos = m_vArrivePos;
		if (ROOM6 == playerWay)
			m_vArrivePos = _vec3(73.f, 0.5f, 77.5f);
		if (ROOM7 == playerWay)
			m_vArrivePos = m_vArrivePos;
		break;
	case ROOM6:
		if (LIVING1 == playerWay)
			m_vArrivePos = _vec3(73.f, 0.5f, 78.5f);
		if (LIVING2 == playerWay)
			m_vArrivePos = _vec3(73.f, 0.5f, 78.5f);
		if (LIVING3 == playerWay)
			m_vArrivePos = _vec3(73.f, 0.5f, 78.5f);
		if (LIVING4 == playerWay)
			m_vArrivePos = _vec3(73.f, 0.5f, 78.5f);
		if (LIVING5 == playerWay)
			m_vArrivePos = _vec3(73.f, 0.5f, 78.5f);
		if (ROOM7 == playerWay)
			m_vArrivePos = _vec3(73.f, 0.5f, 78.5f);

		break;

	case ROOM7:
		if (LIVING1 == playerWay)
			m_vArrivePos = _vec3(50.5f, 0.5f, 73.f);
		if (LIVING2 == playerWay)
			m_vArrivePos = _vec3(50.5f, 0.5f, 73.f);
		if (LIVING3 == playerWay)
			m_vArrivePos = _vec3(50.5f, 0.5f, 73.f);
		if (LIVING4 == playerWay)
			m_vArrivePos = _vec3(50.5f, 0.5f, 73.f);
		if (LIVING5 == playerWay)
			m_vArrivePos = _vec3(50.5f, 0.5f, 73.f);
		if (ROOM6 == playerWay)
			m_vArrivePos = _vec3(50.5f, 0.5f, 73.f);

		break;
	}

	if (playerWay == eCharWay)
		m_vArrivePos = vPlayerPos;

	/*if Inside*/
	_vec3 vDir_ToDest = m_vArrivePos - vMyPos;

	_float fDist = D3DXVec3Length(&vDir_ToDest);
	vDir_ToDest.y = 0.f;


	D3DXVec3Normalize(&vDir_ToDest, &vDir_ToDest);

	//if (fDist < 3.f)
	//{
	//	m_fSkillTime += fTimeDelta * m_fCount;
	//	if (m_fSkillTime > 5.f)
	//	{
	//		//m_bLastDead = true;
	//		m_fSkillTime = 0.f;
	//		return;
	//	}
	//	return;
	//}

	const _float f = D3DXVec3Dot(&vDir_ToDest, &_vec3(0.f, 0.f, 1.f));
	_float fRadian = acosf(f);

	if (vMyPos.x > m_vArrivePos.x)
		fRadian = 2 * D3DX_PI - fRadian;

	m_pTransformCom->Set_DegreeY(D3DXToDegree(fRadian));
	m_pTransformCom->Move_ToLook(m_fMonsterSpeed, fTimeDelta);

	_vec3 vPosTmp = m_pTransformCom->Get_IFMoveToLookVec(m_fMonsterSpeed, fTimeDelta);
	_vec3 vDir = (_vec3&)m_pTransformCom->Get_WorldMatrix().m[CTransform::INFO_LOOK];

	if (m_pColliderMgr->Process_AABB(&vDir, this, vPosTmp))
	{
		m_pTransformCom->Move_ToDirection(vDir, m_fMonsterSpeed, fTimeDelta);
	}
	else
	{
		m_pTransformCom->Move_ToLook(m_fMonsterSpeed, fTimeDelta);
	}
}

_bool CBoss::Hit(_uint iDamage, const _float & fLifeTime)
{
	m_fGetHitDelay = fLifeTime;
	m_iGetHitDamage = iDamage;

	return true;
}

_bool CBoss::Compare_WalkState()
{
// 	if (STATE_DEAD == m_iCurStateIndex || STATE_SPRINT == m_iCurStateIndex || true == m_bSpecialState)
// 		return false;

	return true;
}

void CBoss::LookAt(const _vec3 & vPoint)
{
	_vec3 vLookAt;
	const _vec3& vMyPos = _vec3(m_pTransformCom->Get_PositionX(), m_pTransformCom->Get_PositionY(), m_pTransformCom->Get_PositionZ());

	vLookAt = vPoint - vMyPos;
	D3DXVec3Normalize(&vLookAt, &vLookAt);


	const _float f = D3DXVec3Dot(&vLookAt, &_vec3(0.f, 0.f, 1.f));
	_float fRadian = acosf(f);

	if (vMyPos.x > vPoint.x)
		fRadian = 2 * D3DX_PI - fRadian;

	m_pTransformCom->Set_DegreeY(D3DXToDegree(fRadian));
}

void CBoss::Refresh_Target()
{

	if (nullptr == m_pPlayer)
	{
		auto iter = CObject_Manager::GetInstance()->Get_Layer(SCENETYPE::SCENE_STAGE, L"Layer_Player")->Get_ObjList().begin();
		if (m_pPlayer == nullptr)
		{
			m_pPlayer = dynamic_cast<CPlayer*>(*iter);
		}
	}

}


void CBoss::Set_MonsterLocal()
{
	_vec3 vPlayerPos = _vec3(m_pTransformCom->Get_PositionX(), m_pTransformCom->Get_PositionY(), m_pTransformCom->Get_PositionZ());

	if (36.5f < vPlayerPos.x && vPlayerPos.x < 48.5f && 44.f < vPlayerPos.z && vPlayerPos.z < 80.5f)
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


void CBoss::SetShader(_uint iShaderNum)
{
	_uint numPasses = 0;
	m_pShaderCom->GetShader(iShaderNum)->Begin(&numPasses, NULL);
	m_pShaderCom->GetShader(iShaderNum)->BeginPass(0);
}

void CBoss::StartShader(_uint iShaderNum)
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

void CBoss::EndShader(_uint iShaderNum)
{
	m_pShaderCom->GetShader(iShaderNum)->EndPass();
	m_pShaderCom->GetShader(iShaderNum)->End();
}

CBoss * CBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vPos)
{
	CBoss*	pInstance = new CBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Monster(vPos)))
	{
		MSG_BOX("CCreeper Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CBoss::Free(void)
{
	_ulong		dwRefCnt = 0;

	
	//Safe_Release(m_pCamera_Manager);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pTargetTransform);
	Engine::Safe_Release(m_pTerrainBufferCom);
	Safe_Release(m_pBodyCubeBuffer);

	Engine::Safe_Release(m_pWeapon);
	Engine::Safe_Release(m_pWeaponFire);

	Safe_Release(m_pMatCom);
	Safe_Release(m_pShaderCom);


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

	dwRefCnt = CMonster::Free();

	return dwRefCnt;
}

