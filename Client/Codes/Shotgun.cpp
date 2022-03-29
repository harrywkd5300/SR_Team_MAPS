#include "stdafx.h"
#include "..\Headers\Shotgun.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Particle_Renderer.h"
#include "Effect_Manager.h"
#include "Collider_Manager.h"
#include "WELL512a.h"
#include "Player.h"

CShotgun::CShotgun(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{

}

HRESULT CShotgun::Ready_GameObject(void)
{
	m_tInfo.m_fFireRate = 1.0f;
	m_tInfo.m_fBulletSpeed = 10.f;
	m_tInfo.m_fReloadRate = 1.8f;

	m_tInfo.m_fXRecoilPower = 4.0f;
	m_tInfo.m_fYRecoilPower = 8.0f;

	m_tInfo.m_fBulletRange = 5.f;

	m_tInfo.m_bBulletEffect = true;
	m_tInfo.m_bETCEffect = true;

	m_tInfo.m_iCurBullet = 2;
	m_tInfo.m_iMaxBullet = 2;
	m_tInfo.m_iDamage = 50.f;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Load_WeaponFile(L"../Data/Weapon_Shotgun.dat")))
		return E_FAIL;


	return NOERROR;
}


_int CShotgun::Update_GameObject(const _float & fTimeDelta)
{
	m_vecPartsTransform[PARTS_HANDLE]->Scaling(_vec3(0.5f, 0.5f, 0.5f));
	m_vecPartsTransform[PARTS_HANDLE]->Set_DegreeX(90.f);
	m_vecPartsTransform[PARTS_HANDLE]->Set_DegreeZ(90.f);

	m_vecPartsTransform[2]->Set_DegreeZ(60.f);	//개머리판

	if (m_tInfo.bIsEquipped)
	{
		m_vecPartsTransform[PARTS_HANDLE]->Translation(_vec3(0.f, 0.55f, 1.f));
		m_vecPartsTransform[PARTS_HANDLE]->Parent(((*m_pVecPlayerBone)[CPlayer::BONE_HAND])->Get_WorldMatrix());
	}
	else
	{
		m_vecPartsTransform[PARTS_HANDLE]->Translation(_vec3(0.f, 0.f, -0.8f));
		m_vecPartsTransform[PARTS_HANDLE]->Parent(((*m_pVecPlayerBone)[CPlayer::BONE_BACK])->Get_WorldMatrix());
	}

	m_vecPartsTransform[PARTS_HANDLE]->Fill_WorldMatrix();

	/*update timedelta & Update Bone*/
	CWeapon::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);
	return 0;
}


_int CShotgun::LastUpdate_GameObject(const _float & fTimeDelta)
{


	return 0;
}


void CShotgun::Render_GameObject(void)
{
	SetShader(1);
	StartShader(1);
	Render_Model();
	EndShader(1);
}


void CShotgun::Use_Weapon(const _float fTimeDelta, _vec3 * vRecoverAngle)
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
		CSound_Manager::GetInstance()->PlaySound(L"wpn_shotgundoublebarrel_fire_2d.wav", CSound_Manager::Channel_Weapon, 1.f);
		//CTransform* m_pTargetTransformCom = dynamic_cast<CTransform*>(m_pUser->Get_Component(L"Transform"));
		//m_fFiringRateTimer = 0;
		//Recoil(fTimeDelta, 20, m_iRecoilY);

		// ---------------------- Test -------------------------
		//ResetBullet(const _vec3& vSrc, const _vec3& vDest, const _float &fSpeed);
		const _matrix& mat = m_vecPartsTransform[PARTS_MUZZLE]->Get_WorldMatrix();
		D3DXVECTOR3 vPos(mat._41, mat._42, mat._43);
		//D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		vPos.y += 0.01f;
		_vec3 DestPos;
		_vec3 *pPickPos = &DestPos;
		m_pPickingCom->Picking_ForWeapon(g_hWnd, pPickPos, 0, 0, m_pTerrainBufferCom);

		if (pPickPos != nullptr)
		{
			Recoil(fTimeDelta, vRecoverAngle);//반동
			CCharacter* pDestChar = nullptr;					  //cout << pPickPos->x << '/' << pPickPos->y << '/' << pPickPos->z << '/' << endl;
			if (m_tInfo.m_bBulletEffect == true)
			{
				if (nullptr == m_pBulletParticle || nullptr == m_pParticleRenderer)
					return;
				_vec3 DestPos = *pPickPos;
				//pDestChar = m_pColliderMgr->CheckCollBullet(g_hWnd, &DestPos, m_pUser->Get_CharType(), mat.m[3], *pPickPos);

				_vec3 vDir = DestPos - vPos;
				float fDist = D3DXVec3Length(&(vDir));
				//_vec3 ArrivePos = DestPos;
				D3DXVec3Normalize(&vDir, &vDir);
				//DestPos = vPos + vDir * m_tInfo.m_fBulletRange;

				CCharacter* pDestChar = nullptr;

				for (int i = 0; i < 10; ++i)
				{
					_vec3 ArrivePos = DestPos;
					_vec3 calcDir = vDir;
					calcDir.x -= CRandomWell512::GetInstance()->GetValueF(-0.2f,0.2);
					calcDir.y -= CRandomWell512::GetInstance()->GetValueF(-0.2f,0.2f);
					calcDir.z -= CRandomWell512::GetInstance()->GetValueF(-0.2f,0.2f);
					D3DXVec3Normalize(&calcDir, &calcDir);
					calcDir *= m_tInfo.m_fBulletRange;
					calcDir += vPos;

					pDestChar = m_pColliderMgr->CheckCollBullet(g_hWnd, &calcDir, m_pUser->Get_CharType(), mat.m[3], calcDir);

					m_pBulletParticle->ResetBullet(
						vPos,
						D3DXVECTOR3(calcDir.x, calcDir.y, calcDir.z), //vPos,
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
// 						{
// 							cout << "몬스터 사망" << endl;
// 						}
// 						cout << "몬스터 피격" << endl;
					}
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
			}

		}


	}
}

HRESULT CShotgun::Ready_Component(void)
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
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Shotgun"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Shotgun", pComponent));
	m_pTextureCom->AddRef();

	/*Add Particle_Bullet*/
	pComponent = m_pBulletParticle = dynamic_cast<CParticle_Bullet*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Particle_Bullet"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Particle_Bullet", pComponent));
	m_pBulletParticle->AddRef();

	/*Add Bullet_Texture*/
	pComponent = m_pBulletTexture = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Bullet_Shotgun"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Bullet", pComponent));
	m_pBulletTexture->AddRef();
	m_pTextureTag = L"Component_Texture_Bullet_Shotgun";

	if (FAILED(m_pParticleRenderer->Set_Particle_Type(
		CParticle_Renderer::PARTICLETYPE::PARTICLE_DIRECT_BULLET,
		L"Component_Texture_Bullet_Shotgun",
		m_pBulletTexture
	)))
	{
		MSG_BOX("Particle_Texture_SizeOver : Shotgun");
	}
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
		MSG_BOX("Effect_Manager_Texture_SizeOver : Shotgun");
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
	MSG_BOX("CShotgun Component Ready Failed err: 13115");
	return E_FAIL;
}

CShotgun * CShotgun::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShotgun *	pInstance = new CShotgun(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CShotgun Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CShotgun::Free(void)
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

//#include "stdafx.h"
//#include "..\Headers\Shotgun.h"
//#include "Component_Manager.h"
//#include "Object_Manager.h"
//#include "Particle_Renderer.h"
//#include "WELL512a.h"
//
//CShotgun::CShotgun(LPDIRECT3DDEVICE9 pGraphicDev)
//	:CWeapon(pGraphicDev)
//{
//	
//}
//
//HRESULT CShotgun::Ready_GameObject(void)
//{
//	m_tInfo.m_fFireRate = 1.0f;
//	m_tInfo.m_fBulletSpeed = 10.f;
//	m_tInfo.m_fReloadRate = 1.8f;
//
//	m_tInfo.m_fXRecoilPower = 4.0f;
//	m_tInfo.m_fYRecoilPower = 8.0f;
//
//	m_tInfo.m_fBulletRange = 10.f;
//
//	m_tInfo.m_bBulletEffect = true;
//	
//	m_tInfo.m_iCurBullet = 2;
//	m_tInfo.m_iMaxBullet = 2;
//	m_tInfo.m_iDamage = 50.f;
//	
//	if (FAILED(Ready_Component()))
//		return E_FAIL;		
//
//
//	return NOERROR;
//}
//
//
//
//
//void CShotgun::Use_Weapon(const _float fTimeDelta, _vec3 * vRecoverAngle)
//{
//	if (nullptr == m_pBulletParticle || nullptr == m_pUser || nullptr == m_pTextureTag)
//		return;
//
//	// 재장전 중이라면
//	if (m_tInfo.m_bReloading == true)
//	{
//		// 재장전 시간 체크
//		if (m_tInfo.m_fReloadRateTimer > m_tInfo.m_fReloadRate)
//		{
//			m_tInfo.m_iCurBullet = m_tInfo.m_iMaxBullet; // 탄창 교체
//			m_tInfo.m_bReloading = false;				// 재장전 끝
//			//cout << "Reload!" << m_tInfo.m_iCurBullet << endl;
//		}
//		else
//		{
//			return;
//		}
//	}
//
//	// 잔탄 확인
//	if (m_tInfo.m_iCurBullet <= 0)
//	{
//		// 플레이어가 재장전 키 누르기 전까지 발사 불가능
//		if (m_tInfo.m_bReloading == false)
//		{
//			cout << "재장전 해 주세요." << m_tInfo.m_iCurBullet << endl;
//			return;
//		}
//	}
//
//	// 발사 처리
//	if (m_tInfo.m_fFireRateTimer > m_tInfo.m_fFireRate)
//	{
//		//CTransform* m_pTargetTransformCom = dynamic_cast<CTransform*>(m_pUser->Get_Component(L"Transform"));
//		//m_fFiringRateTimer = 0;
//		//Recoil(fTimeDelta, 20, m_iRecoilY);
//
//		// ---------------------- Test -------------------------
//		//ResetBullet(const _vec3& vSrc, const _vec3& vDest, const _float &fSpeed);
//		D3DXVECTOR3 vPos(m_pTargetTransformCom->Get_PositionX(), m_pTargetTransformCom->Get_PositionY(), m_pTargetTransformCom->Get_PositionZ());
//		//D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
//		vPos.y += 0.1f;
//		_vec3 PickPos;
//		_vec3 *pPickPos = &PickPos;
//		m_pPickingCom->Picking_ToBuffer(g_hWnd,pPickPos, 0, 0, m_pTerrainBufferCom);
//
//		if (pPickPos != nullptr)
//		{
//
//			Recoil(fTimeDelta, vRecoverAngle);//반동
//											  //cout << pPickPos->x << '/' << pPickPos->y << '/' << pPickPos->z << '/' << endl;
//			if (m_tInfo.m_bBulletEffect == true)
//			{
//				_vec3 vDir = PickPos - vPos;
//				float fDist = D3DXVec3Length(&(vDir));
//
//				D3DXVec3Normalize(&vDir, &vDir);
//				PickPos = vPos + vDir * m_tInfo.m_fBulletRange;
//
//				for (int i = 0; i < 10; ++i)
//				{
//				
//					m_pBulletParticle->ResetBullet(
//						vPos,//vPos,	//*vCameraPos,
//						D3DXVECTOR3(
//							PickPos.x - CRandomWell512::GetInstance()->GetValueF(0.1f * m_tInfo.m_fBulletRange),
//							PickPos.y - CRandomWell512::GetInstance()->GetValueF(0.1f * m_tInfo.m_fBulletRange),
//							PickPos.z - CRandomWell512::GetInstance()->GetValueF(0.1f * m_tInfo.m_fBulletRange)), //vPos,
//						m_tInfo.m_fBulletSpeed);
//
//					if (m_pParticleRenderer->Set_Particle(
//						CParticle_Renderer::PARTICLETYPE::PARTICLE_DIRECT_BULLET,
//						m_pTextureTag,
//						m_pBulletParticle) == false)
//					{
//						return;
//						cout << "Failed" << endl;
//					}
//				}
//			}
//			//발사후 처리
//			--m_tInfo.m_iCurBullet;
//			m_tInfo.m_fFireRateTimer = 0.f;
//
//			cout << "탄창:" << m_tInfo.m_iCurBullet << endl;
//		}
//
//
//	}
//}
//
//HRESULT CShotgun::Ready_Component(void)
//{
//	Engine::CComponent*			pComponent = nullptr;
//		
//	/*Add Transform*/
//	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
//	if (nullptr == pComponent)
//		goto exception;
//	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
//	m_pTransformCom->AddRef();
//	
//	// For.Buffers
//	pComponent = m_pBufferCom = dynamic_cast<CRect_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectTex"));
//	if (nullptr == pComponent)
//		goto exception;
//	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
//	m_pBufferCom->AddRef();
//
//	/*Add Renderer*/
//	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
//	if (nullptr == pComponent)
//		goto exception;
//	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
//	m_pRendererCom->AddRef();
//
//	// For.Pickikng
//	pComponent = m_pPickingCom = (Engine::CPicking*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Picking");
//	if (nullptr == pComponent)
//		return E_FAIL;
//	m_mapComponent[Engine::CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Com_Picking", pComponent));
//	m_pPickingCom->AddRef();
//
//	/*Add Texture*/
//	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_Invincible"));
//	if (nullptr == pComponent)
//		goto exception;
//	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Effect", pComponent));
//	m_pTextureCom->AddRef();
//
//	/*Add Particle_Bullet*/
//	pComponent = m_pBulletParticle = dynamic_cast<CParticle_Bullet*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Particle_Bullet"));
//	if (nullptr == pComponent)
//		goto exception;
//	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Particle_Bullet", pComponent));
//	m_pBulletParticle->AddRef();
//
//	/*Add Bullet_Texture*/
//	pComponent = m_pBulletTexture = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Bullet_Shotgun"));
//	if (nullptr == pComponent)
//		goto exception;
//	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Bullet", pComponent));
//	m_pBulletTexture->AddRef();
//	m_pTextureTag = L"Component_Texture_Bullet_Shotgun";
//
//	if (FAILED(m_pParticleRenderer->Set_Particle_Type(
//		CParticle_Renderer::PARTICLETYPE::PARTICLE_DIRECT_BULLET,
//		L"Component_Texture_Bullet_Shotgun",
//		m_pBulletTexture
//	)))
//	{
//		MSG_BOX("Particle_Texture_SizeOver : Shotgun");
//	}
//
//	m_pTerrainBufferCom = dynamic_cast<CTerrain_Buffer*>(CObject_Manager::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain", 0, L"Buffer_Terrain"));
//	m_pTerrainBufferCom->AddRef();
//
//	return NOERROR;
//
//exception:
//	MSG_BOX("CShotgun Component Ready Failed err: 13115");
//	return E_FAIL;
//}
//
//CShotgun * CShotgun::Create(LPDIRECT3DDEVICE9 pGraphicDev)
//{
//	CShotgun *	pInstance = new CShotgun(pGraphicDev);
//
//	if (FAILED(pInstance->Ready_GameObject()))
//	{
//		MSG_BOX("CShotgun Created Failed");
//		Engine::Safe_Release(pInstance);
//	}
//	return pInstance;
//}
//
//_ulong CShotgun::Free(void)
//{
//	_ulong		dwRefCnt = 0;
//		
//	dwRefCnt = CWeapon::Free();
//
//	return dwRefCnt;
//}
