#include "stdafx.h"
#include "..\Headers\Ghost.h"
#include "object_Manager.h"
#include "Layer.h"
#include "Player.h"
#include "Fireball.h"
#include "camera_manager.h"


CGhost::CGhost(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
	, m_pRendererCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTargetBuffer(nullptr)
	, m_pTerrainBufferCom(nullptr)
{

}

HRESULT CGhost::Ready_Monster(const _vec3 * vPos)
{
	if (FAILED(CCharacter::Ready_Character_Component()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_Skill()))
		return E_FAIL;

	if (FAILED(Ready_Parameters(vPos)))
		return E_FAIL;
	
	return NOERROR;
}

_int CGhost::Update_GameObject(const _float & fTimeDelta)
{
	if (true == m_bIsDead)
	{
		if (m_bSpecialState)
			return 1;
	}

	if (nullptr == m_pRendererCom || nullptr == m_pTransformCom)
		return -1;

	if (0 != m_fGetHitDelay)
	{
		m_fGetHitDelay -= fTimeDelta;

		if (m_fGetHitDelay <= 0)
		{
			m_fGetHitDelay = 0;
			m_iHp -= m_iGetHitDamage;

			m_iCurStateIndex = STATE_GETHIT;
			m_bSpecialState = true;
			m_bDirectly_ChangeState = true;

			if (m_iHp <= 0)
			{
				m_iHp = 0;
				m_bIsDead = true;
				cout << " dead " << m_iHp << endl;
				m_iCurStateIndex = STATE_DEAD;
				m_bSpecialState = true;
				if (m_vecBone.empty())
					return 0;
				m_vecBone[BONE_TRANSFORM]->Set_ReceivedKeyFrame(true);
			}

		}
	}



	m_pWeapon->Update_GameObject(fTimeDelta);
	Move_Monster(fTimeDelta);

	m_pTransformCom->Fill_WorldMatrix();
	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);
	
	//cout << " hp : " << m_iHp << endl;

	return 0;
}

_int CGhost::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (true == m_bIsDead)
	{
		if (m_bSpecialState)
			return 1;

		return 0;
	}

	CMonster::LastUpdate_GameObject(fTimeDelta);

	return 0;
}

void CGhost::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTextureCom || nullptr == m_pTransformCom)
		return;

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pTextureCom->SetUp_TextureToGraphicDev(0, 16);
	//m_pTransformCom->SetUp_MatrixToGraphicDev();
	SetShader(1);
	StartShader(1);
	//m_pBufferCom->Render_Buffer();
	EndShader(1);
	//m_pColliderBox_AABB->Render_Debug_Buffer();
}

_bool CGhost::Hit(_uint iDamage, const _float & fLifeTime)
{
	m_iGetHitDamage = iDamage;
	m_fGetHitDelay = fLifeTime;

	return false;
}

void CGhost::SetShader(_uint iShaderNum)
{
	_uint numPasses = 0;
	m_pShaderCom->GetShader(iShaderNum)->Begin(&numPasses, NULL);
	m_pShaderCom->GetShader(iShaderNum)->BeginPass(0);
}

void CGhost::StartShader(_uint iShaderNum)
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


	m_pShaderCom->GetShader(iShaderNum)->SetTexture("DiffuseMap_Tex", m_pTextureCom->GetTexture(17));
	m_pShaderCom->GetShader(iShaderNum)->SetTexture("SpecularMap_Tex", m_pTextureCom->GetTexture(18));
	m_pShaderCom->GetShader(iShaderNum)->SetTexture("NormalMap_Tex", m_pTextureCom->GetTexture(19));
	m_pShaderCom->GetShader(1)->CommitChanges();
	m_pBufferCom->Render_Buffer();
}

void CGhost::EndShader(_uint iShaderNum)
{
	m_pShaderCom->GetShader(iShaderNum)->EndPass();
	m_pShaderCom->GetShader(iShaderNum)->End();
}

HRESULT CGhost::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;
	/*Add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();

	pComponent = m_pBufferCom = dynamic_cast<CCube_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeTex"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Cube_Buffer", pComponent));
	m_pBufferCom->AddRef();

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
	m_pTransformCom->AddRef();

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Zombie"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Cube_Texture", pComponent));
	m_pTextureCom->AddRef();


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



	/*Ready TerrainBuffer*/
	m_pTerrainBufferCom = dynamic_cast<CTerrain_Buffer*>(CObject_Manager::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain", 0, L"Buffer_Terrain"));
	m_pTerrainBufferCom->AddRef();


	return NOERROR;

exception:
	MSG_BOX("Ghost Component Ready Failed err: 13115");
	return E_FAIL;
}

HRESULT CGhost::Ready_Parameters(const _vec3* vPos)
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	/*set default parameters*/
	m_iHp = 100;
	m_eCharType = CHAR_TYPE::CHAR_TYPE_MONSTER;
	m_fMonsterSpeed = 1.f;

	/*set scale*/
	m_pTransformCom->Scaling(_vec3(0.6f, 0.6f, 0.4f));

	/*set position*/
	if (nullptr != vPos)
		m_pTransformCom->Translation(vPos->x, 2.f, vPos->z);

	m_pTransformCom->Fill_WorldMatrix();

	return NOERROR;
}

HRESULT CGhost::Ready_Skill(void)
{
	m_pWeapon = CFireball::Create(m_pGraphicDev);

	m_pWeapon->Set_Monster(this);

	//m_pWeapon->AddRef();

	return NOERROR;
}

void CGhost::Move_Monster(const _float & fTimeDelta)
{
	if (nullptr == m_pTargetBuffer)
	{
		auto iter = CObject_Manager::GetInstance()->Get_Layer(SCENETYPE::SCENE_STAGE, L"Layer_Player")->Get_ObjList().begin();
		m_pTargetBuffer = dynamic_cast<CPlayer*>(*iter)->Get_Transform();
		m_pTargetBuffer->AddRef();
	}
	_vec3 vPlayerPos = _vec3(m_pTargetBuffer->Get_PositionX() , m_pTargetBuffer->Get_PositionY() , m_pTargetBuffer->Get_PositionZ());
	_vec3 vMyPos = _vec3(m_pTransformCom->Get_PositionX(), m_pTransformCom->Get_PositionY(), m_pTransformCom->Get_PositionZ());

	_vec3 vTargetPos = vPlayerPos  + _vec3(1.f, 1.f, 1.f);
	_vec3 vDir_ToDest = vTargetPos - vMyPos;
	vDir_ToDest.y = 0.f;
	const _float fDist = D3DXVec3Length(&vDir_ToDest);
	D3DXVec3Normalize(&vDir_ToDest, &vDir_ToDest);

	const _float f = D3DXVec3Dot(&vDir_ToDest, &_vec3(0.f, 0.f, 1.f));
	_float fRadian = acosf(f);

	if (vMyPos.x > vTargetPos.x)
		fRadian = 2 * D3DX_PI - fRadian;

	m_pTransformCom->Set_DegreeY(D3DXToDegree(fRadian));

	if (fDist > 3.f)
	{
		_vec3 vDir = vTargetPos - vMyPos;
		m_pTransformCom->Move_ToLook(m_fMonsterSpeed, fTimeDelta);
	}
	else
	{
		m_fSkillTime += fTimeDelta * m_fCount;
		if (m_fSkillTime > 1.f)
		{
			m_pWeapon->Use_Weapon_Monster(fTimeDelta, vMyPos, vPlayerPos);
			m_fSkillTime = 0.f;
			return;
		}
		_vec3 vRight;
		_vec3 vLook = vTargetPos - vMyPos;
		D3DXVec3Cross(&vRight, &vLook, &_vec3(0.f, 1.f, 0.f));
		m_pTransformCom->Move_ToDirection(vRight, m_fMonsterSpeed, fTimeDelta);
	}
}

CGhost * CGhost::Create(LPDIRECT3DDEVICE9 pGraphicDev,const _vec3* vPos)
{
	CGhost*	pInstance = new CGhost(pGraphicDev);
	if (FAILED(pInstance->Ready_Monster(vPos)))
	{
		MSG_BOX("CBaseMonster Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CGhost::Free(void)
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pTargetBuffer);
	Engine::Safe_Release(m_pTerrainBufferCom);
	Engine::Safe_Release(m_pWeapon);
	Safe_Release(m_pMatCom);
	Engine::Safe_Release(m_pShaderCom);

	dwRefCnt = CMonster::Free();

	return dwRefCnt;
}
