#include "stdafx.h"
#include "..\Headers\Rifle.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Particle_Renderer.h"
#include "Effect_Manager.h"
#include "Collider_Manager.h"
#include "WELL512a.h"
#include "Player.h"


CRifle::CRifle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
	
}

HRESULT CRifle::Ready_GameObject(void)
{
	m_tInfo.m_fFireRate = 0.12f;
	m_tInfo.m_fBulletSpeed = 25.f;
	m_tInfo.m_fReloadRate = 2.2f;

	m_tInfo.m_fXRecoilPower = 1.5f;
	m_tInfo.m_fYRecoilPower = 1.5f;

	//m_tInfo.m_bBulletEffect = true;
	m_tInfo.m_bBulletEffect = true;
	m_tInfo.m_bETCEffect = true;
	
	m_tInfo.m_iCurBullet = 25;
	m_tInfo.m_iMaxBullet = 25;
	m_tInfo.m_iDamage = 8.f;
	
	if (FAILED(Ready_Component()))
		return E_FAIL;		

	//L"../Data/TestAnimate.dat"
	if (FAILED(Load_WeaponFile(L"../Data/Weapon_Rifle.dat")))
		return E_FAIL;

	m_vecPartsTransform[2]->Set_ReceivedKeyFrame(false);


	return NOERROR;
}

_int CRifle::Update_GameObject(const _float & fTimeDelta)
{

	if (nullptr == m_pVecPlayerBone)
		return -1;

	Update_PartsTransform(fTimeDelta);

	if (m_tInfo.bIsEquipped)
		m_vecPartsTransform[PARTS_HANDLE]->Parent(((*m_pVecPlayerBone)[CPlayer::BONE_HAND])->Get_WorldMatrix());
	else
		m_vecPartsTransform[PARTS_HANDLE]->Parent(((*m_pVecPlayerBone)[CPlayer::BONE_BACK])->Get_WorldMatrix());

	m_vecPartsTransform[PARTS_HANDLE]->Fill_WorldMatrix();

	/*update timedelta & Update Bone*/
	CWeapon::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);
	return 0;
}

_int CRifle::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return 0;
}

void CRifle::Render_GameObject(void)
{

	SetShader(1);
	StartShader(1);
	Render_Model();
	EndShader(1);

}


void CRifle::Use_Weapon(const _float fTimeDelta, _vec3 * vRecoverAngle)
{
	if (nullptr == m_pUser || nullptr == m_pTextureTag)
		return;

	// 재장전 중이라면
	if (m_tInfo.m_bReloading == true)
	{
		// 재장전 시간 체크
		if (m_tInfo.m_fReloadRateTimer > m_tInfo.m_fReloadRate)
		{
			m_tInfo.m_iCurBullet = m_tInfo.m_iMaxBullet; // 탄창 교체
			m_tInfo.m_bReloading = false;				// 재장전 끝
			//cout << "Reload!" << m_tInfo.m_iCurBullet << endl;
		}
		else
		{
			return;
		}
	}

	// 잔탄 확인
	if (m_tInfo.m_iCurBullet <= 0)
	{
		// 플레이어가 재장전 키 누르기 전까지 발사 불가능
		if (m_tInfo.m_bReloading == false)
		{
			//cout << "재장전 해 주세요." << m_tInfo.m_iCurBullet << endl;
			return;
		}
	}

	// 발사 처리
	if (m_tInfo.m_fFireRateTimer > m_tInfo.m_fFireRate)
	{
		CSound_Manager::GetInstance()->PlaySound(L"Tank_Attack_Launch04.wav", CSound_Manager::Channel_Weapon, 1.0f);
		const _matrix& mat = m_vecPartsTransform[PARTS_MUZZLE]->Get_WorldMatrix();
		//_float f = D3DXVec3Length(&(_vec3)mat.m[0]);
		D3DXVECTOR3 vPos(mat._41, mat._42, mat._43);
		//vPos *= f;
		//D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		vPos.y += 0.01f;
		_vec3 DestPos;
		_vec3 *pPickPos = &DestPos;
		m_pPickingCom->Picking_ForWeapon(g_hWnd, pPickPos, 0, 0, m_pTerrainBufferCom);

		if (pPickPos != nullptr)
		{

			Recoil(fTimeDelta, vRecoverAngle);//반동
			CCharacter* pDestChar = nullptr;
			if (m_tInfo.m_bBulletEffect == true)
			{
				if (nullptr == m_pBulletParticle || nullptr == m_pParticleRenderer)
					return;

				pDestChar = m_pColliderMgr->CheckCollBullet(g_hWnd, &DestPos, m_pUser->Get_CharType(), mat.m[3], *pPickPos);

				m_pBulletParticle->ResetBullet(
					vPos,
					D3DXVECTOR3(DestPos.x, DestPos.y, DestPos.z), //vPos,
																  //D3DXVECTOR3(20.0f, 10.0f, 20.0f),
					m_tInfo.m_fBulletSpeed);

				if (m_pParticleRenderer->Set_Particle(
					CParticle_Renderer::PARTICLETYPE::PARTICLE_DIRECT_BULLET,
					m_pTextureTag,
					m_pBulletParticle) == false)
				{
					return;
					//cout << "Failed" << endl;
				}

				if (pDestChar != nullptr)
				{
					pDestChar->Hit(m_tInfo.m_iDamage, m_pBulletParticle->Get_Info()._lifeTime);
// 					{
// 						//cout << "몬스터 사망" << endl;
// 					}
// 					cout << "몬스터 피격" << endl;
				}
			}
			else
			{
				if (nullptr == m_pBulletEffect || nullptr == m_pEffectManager)
					return;

				pDestChar = m_pColliderMgr->CheckCollBullet(g_hWnd, &DestPos, m_pUser->Get_CharType(), mat.m[3], *pPickPos);

				m_pBulletEffect->ResetBullet(
					vPos,
					D3DXVECTOR3(DestPos.x, DestPos.y, DestPos.z), //vPos,
																  //D3DXVECTOR3(0.1f, 0.1f, 0.1f),
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					m_tInfo.m_fBulletSpeed);

				if (m_pEffectManager->Set_Effect(
					CEffect_Manager::EFFECT_TYPE::EFFECT_DIRECT_BULLET,
					m_pBulletEffectTextureTag,
					m_pBulletEffect) == false)
				{
					return;
					//cout << "Failed" << endl;
				}

			}
			//발사후 처리
			--m_tInfo.m_iCurBullet;
			m_tInfo.m_fFireRateTimer = 0.f;

			//cout << "탄창:" << m_tInfo.m_iCurBullet << endl;

			if (m_tInfo.m_bETCEffect == true)
			{
				if (nullptr == m_pMuzzleEffect || nullptr == m_pEffectManager)
					return;

				m_pMuzzleEffect->ResetEffect(
					vPos,
					_vec3(0.2f, 0.2f, 0.2f),
					CEffect_Fixed::TYPE_TIMER,
					0.1f,
					m_vecPartsTransform[PARTS_MUZZLE]
				);

				if (m_pEffectManager->Set_Effect(
					CEffect_Manager::EFFECT_TYPE::EFFECT_FIXED,
					m_pMuzzleEffectTextureTag,
					m_pMuzzleEffect) == false)
				{
					return;
				}

				//if (nullptr == m_pBulletParticle || nullptr == m_pParticleRenderer)
				//	return;

				//m_pMuzzleParticle->ResetEffect(
				//	vPos,
				//	D3DXCOLOR(0.94f, 0.64f, 0.4f, 1.0f),
				//	0.01f
				//);
				//if (m_pParticleRenderer->Set_Particle(
				//	CParticle_Renderer::PARTICLETYPE::PARTICLE_EFFECT,
				//	m_pMuzzleTextureTag,
				//	m_pMuzzleParticle) == false)
				//{
				//	return;
				//	//cout << "Failed" << endl;
				//}
			}

		}
		else
		{
			//cout << "빗나감" << endl;
		}

	}
}


void CRifle::Update_PartsTransform(const _float & fTimeDelta)
{
	m_vecPartsTransform[PARTS_HANDLE]->Scaling(_vec3(0.3f, 0.3f, 0.3f));
	m_vecPartsTransform[PARTS_HANDLE]->Set_DegreeX(90.f);
	m_vecPartsTransform[PARTS_HANDLE]->Set_DegreeZ(90.f);

	m_vecPartsTransform[2]->Set_DegreeX(45.f);	//수직손잡이
	
	if (!m_tInfo.bIsEquipped)
		m_vecPartsTransform[PARTS_HANDLE]->Translation(_vec3(0.f, 0.f, -0.8f));
}

HRESULT CRifle::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;
		
	/*Add Transform*/
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
	m_pTransformCom->AddRef();
	
	// For.Buffers
	pComponent = m_pBufferCom = dynamic_cast<CRect_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectTex"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	m_pBufferCom->AddRef();

	/*Add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();

	// For.Pickikng
	pComponent = m_pPickingCom = (Engine::CPicking*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Picking");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Com_Picking", pComponent));
	m_pPickingCom->AddRef();

	/*Add Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Rifle"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Rifle", pComponent));
	m_pTextureCom->AddRef();


	// ------------------------ Particle ------------------------
	/*Add Particle_Bullet*/
	pComponent = m_pBulletParticle = dynamic_cast<CParticle_Bullet*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Particle_Bullet"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Particle_Bullet", pComponent));
	m_pBulletParticle->AddRef();

	///*Add Particle_Effect*/
	//pComponent = m_pMuzzleParticle = dynamic_cast<CParticle_Effect*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Particle_Effect"));
	//if (nullptr == pComponent)
	//	goto exception;
	//m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Particle_Effect", pComponent));
	//m_pMuzzleParticle->AddRef();

	/*Add Bullet_Texture*/
	pComponent = m_pBulletTexture = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Bullet_Rifle"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Bullet", pComponent));
	m_pBulletTexture->AddRef();
	m_pTextureTag = L"Component_Texture_Bullet_Rifle";

	if (FAILED(m_pParticleRenderer->Set_Particle_Type(
		CParticle_Renderer::PARTICLETYPE::PARTICLE_DIRECT_BULLET,
		L"Component_Texture_Bullet_Rifle",
		m_pBulletTexture
	)))
	{
		MSG_BOX("Particle_Texture_SizeOver : Rifle");
	}
	
	///*Add Muzzle_Texture*/
	//pComponent = m_pMuzzleTexture = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Effect_Muzzle"));
	//if (nullptr == pComponent)
	//	goto exception;
	//m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Muzzle", pComponent));
	//m_pMuzzleTexture->AddRef();
	//m_pMuzzleTextureTag = L"Component_Texture_Effect_Muzzle";

	//if (FAILED(m_pEffectManager->Set_Effect_Type(
	//	CEffect_Manager::EFFECT_TYPE::EFFECT_FIXED,
	//	L"Component_Texture_Effect_Muzzle",
	//	m_pMuzzleTexture
	//)))
	//{
	//	MSG_BOX("Particle_Texture_SizeOver : Rifle");
	//}

	

	// ------------------------ Particle ------------------------


	// ------------------------ Effect ------------------------

	/*Add Effect_Muzzle*/
	pComponent = m_pMuzzleEffect = dynamic_cast<CEffect_Fixed*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Effect_Fixed"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Effect_Muzzle", pComponent));
	m_pMuzzleEffect->AddRef();

	EFTINFO pInfo = m_pMuzzleEffect->Get_Info();
	pInfo.fFrameSpeed = 60.f;
	pInfo.fMaxFrame = 6.f;
	pInfo._isAlive = true;
	pInfo._isLight = true;
	//pInfo._fLightRange = 2.0f;
	m_pMuzzleEffect->Set_Info(pInfo);
	m_pMuzzleEffect->Set_Light(D3DXCOLOR(248.f / 255.f, 188.f / 255.f, 80.f / 255.f, 1.f), 2.0f);
	m_pMuzzleEffect->SetType(CEffect_Fixed::FIXED_TYPE::TYPE_TIMER);

	/*Add Muzzle_Texture*/
	pComponent = m_pMuzzleEffectTexture = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_Muzzle"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Effect_Muzzle", pComponent));
	m_pMuzzleEffectTexture->AddRef();
	m_pMuzzleEffectTextureTag = L"Component_Texture_Effect_Muzzle";
	
	if (FAILED(m_pEffectManager->Set_Effect_Type(
		CEffect_Manager::EFFECT_TYPE::EFFECT_FIXED,
		L"Component_Texture_Effect_Muzzle",
		m_pMuzzleEffectTexture
	)))
	{
		MSG_BOX("Effect_Manager_Texture_SizeOver : Rifle");
	}

	// ------------------------ Effect ------------------------

	// ------------------------ Shader ------------------------
	/*Add Shader*/
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Shader_Weapon"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));
	m_pShaderCom->AddRef();

	/*Add Material*/
	pComponent = m_pMatCom = dynamic_cast<CMaterial*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Material"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Material", pComponent));
	m_pMatCom->SetUp_MaterialInfo(CMaterial::MATTYPE::MATTYPE_SHINY);
	m_pMatCom->AddRef();
	// ------------------------ Shader ------------------------


	m_pTerrainBufferCom = dynamic_cast<CTerrain_Buffer*>(CObject_Manager::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain", 0, L"Buffer_Terrain"));
	m_pTerrainBufferCom->AddRef();



	/*============== form =============*/
	pComponent = m_pBodyTexture = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Cube"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Cube", pComponent));
	m_pBodyTexture->AddRef();

	pComponent = m_pCubeTexBuffer = dynamic_cast<CCube_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeTex"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"CubeTex", pComponent));
	m_pCubeTexBuffer->AddRef();

	//pComponent = m_pShaderCom = dynamic_cast<CShader*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Shader_Player"));
	//if (nullptr == pComponent)
	//	goto exception;
	//m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));
	//m_pShaderCom->AddRef();


	return NOERROR;

exception:
	MSG_BOX("CRifle Component Ready Failed err: 13115");
	return E_FAIL;
}

CRifle * CRifle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRifle *	pInstance = new CRifle(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CRifle Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CRifle::Free(void)
{
	_ulong		dwRefCnt = 0;
		

	for (size_t i = 0; i < m_vecPartsTransform.size(); ++i)
		Safe_Release(m_vecPartsTransform[i]);
	m_vecPartsTransform.clear();

	for (size_t i = 0; i < m_vecBodyInfo.size(); ++i)
		Safe_Delete(m_vecBodyInfo[i]);
	m_vecBodyInfo.clear();

	Safe_Release(m_pCubeTexBuffer);
	//Safe_Release(m_pShaderCom);
	Safe_Release(m_pBodyTexture);


	dwRefCnt = CWeapon::Free();

	return dwRefCnt;
}
