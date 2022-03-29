#include "stdafx.h"
#include "..\Headers\FireWeapon.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Particle_Renderer.h"
#include "WELL512a.h"
#include "Effect_Manager.h"
#include "Collider_Manager.h"
#include "Character.h"
#include "ColliderSkill.h"
#include "FireWeaponSkill.h"

CFireWeapon::CFireWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
	
}

HRESULT CFireWeapon::Ready_GameObject(void)
{
	m_tInfo.m_fFireRate = 0.5f;
	m_tInfo.m_fBulletSpeed = 20.f;
	m_tInfo.m_fBulletRange = 20.f;
	m_tInfo.m_fReloadRate = 1.f;

	m_tInfo.m_fXRecoilPower = 0.f;
	m_tInfo.m_fYRecoilPower = 0.f;

	m_tInfo.m_bBulletEffect = false;
	m_tInfo.m_bETCEffect = true;
	
	m_tInfo.m_iCurBullet = 99999;
	m_tInfo.m_iMaxBullet = 99999;
	m_tInfo.m_iDamage = 20.f;
	
	if (FAILED(Ready_Component()))
		return E_FAIL;		


	return NOERROR;
}


void CFireWeapon::Use_Weapon_Monster(const _float fTimeDelta, const _vec3 & vSourPos, const _vec3 & vDestPos)
{
	if (nullptr == m_pUser || nullptr == m_pBulletEffectTextureTag)
		return;
	// 잔탄 확인
	if (m_tInfo.m_iCurBullet <= 0)
	{
		if (m_tInfo.m_bReloading == false)
		{
			m_tInfo.m_fReloadRateTimer = 0.f;
			m_tInfo.m_bReloading = true; // 리로드 시작
			return;
		}
		// 재장전 시간 체크
		if (m_tInfo.m_fReloadRateTimer > m_tInfo.m_fReloadRate)
		{
			m_tInfo.m_iCurBullet = m_tInfo.m_iMaxBullet;
			m_tInfo.m_bReloading = false; // 리로드 끝
			return;
		}
	}
	// 발사 처리
	if (m_tInfo.m_fFireRateTimer > m_tInfo.m_fFireRate)
	{
		_vec3 SourPos = vSourPos;
		SourPos.y += 0.01f;
		_vec3 DestPos = vDestPos;
		CCharacter* pDestChar = nullptr;

		if (nullptr == m_pBulletEffect || nullptr == m_pEffectManager)
			return;

	
			CFireWeaponSkill* pSkill = CFireWeaponSkill::Create(m_pGraphicDev);
			if (nullptr == pSkill)
				return;
			if (FAILED(CObject_Manager::GetInstance()->Add_GameObject(SCENE_STAGE, L"Layer_Skill", pSkill)))
				return;

			_vec3 vDir = vDestPos - SourPos;

		float fTime = sqrtf(-vDir.y / GRAVITY*0.5f);
			//float fXSpeed = vDir.x / fTime;
			//float fZSpeed = vDir.z / fTime;
			//float fYSpeed = 0.f;

			//float fYDist = -vDir.y;
			//vDir.x = fXSpeed;
			//vDir.y = fYSpeed;
			//vDir.z = fZSpeed;

			//_vec3 vDir = vDestPos - SourPos;
			//D3DXVec3Normalize(&vDir, &vDir);
			//vDir *= m_tInfo.m_fBulletSpeed;

			//cout << "fYDist:" << fYDist << endl;

			pSkill->Set_Texture(m_pBulletEffectTextureTag, 49.f);
			pSkill->Set_LightColor(D3DXCOLOR(248.f / 255.f, 188.f / 255.f, 80.f / 255.f, 1.f), 2.f);
			pSkill->Get_Transform()->Set_Position(SourPos);

			_vec3 vDestPosition = vDestPos;


			fTime = D3DXVec3Length(&vDir) / 10.f;
			pSkill->Set_Info(vDestPosition, vSourPos, fTime, 10.0f, 2.f, m_tInfo.m_iDamage);
			/*dynamic_cast<CColliderSkill>(pSkill)->Set_Info(vDestPosition, 2.f, 1.f, m_tInfo.m_iDamage, vDir, CColliderSkill::RUN_TYPE::AABB);*/
			//pSkill->Set_Info(fYDist, 2.f, 1.f, m_tInfo.m_iDamage, vDir, CColliderSkill::RUN_TYPE::AABB);

			m_pColliderMgr->Add_ColliderSkill(pSkill);
		

		//발사후 처리
		--m_tInfo.m_iCurBullet;
		m_tInfo.m_fFireRateTimer = 0.f;

	}

}

HRESULT CFireWeapon::Ready_Component(void)
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
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_Invincible"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Effect", pComponent));
	m_pTextureCom->AddRef();

	// ------------------------ Effect ------------------------

	/*Add Effect_Bullet*/
	pComponent = m_pBulletEffect = dynamic_cast<CEffect_Bullet*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Effect_Bullet"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Effect_Bullet", pComponent));
	m_pBulletEffect->AddRef();


	/*Add Bullet_Texture*/
	pComponent = m_pBulletEffectTexture = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_FlameA"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Bullet", pComponent));
	m_pBulletEffectTexture->AddRef();
	m_pBulletEffectTextureTag = L"Component_Texture_Effect_FlameA";

	EFTINFO pInfo = m_pBulletEffect->Get_Info();
	pInfo.fFrameSpeed = 60.f;
	pInfo.fMaxFrame = 50;
	pInfo._isAlive = true;
	pInfo._isLight = true;
	//pInfo._fLightRange = 2.0f;
	m_pBulletEffect->Set_Info(pInfo);
	m_pBulletEffect->Set_Light(D3DXCOLOR(248.f / 255.f, 188.f / 255.f, 80.f / 255.f, 1.f), 2.0f);

	if (FAILED(m_pEffectManager->Set_Effect_Type(
		CEffect_Manager::EFFECT_TYPE::EFFECT_DIRECT_BULLET,
		L"Component_Texture_Effect_FlameA",
		m_pBulletEffectTexture
	)))
	{
		MSG_BOX("Effect_Manager_Texture_SizeOver : Fireball");
	}

	// ------------------------ Effect ------------------------

	m_pTerrainBufferCom = dynamic_cast<CTerrain_Buffer*>(CObject_Manager::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain", 0, L"Buffer_Terrain"));
	m_pTerrainBufferCom->AddRef();

	return NOERROR;

exception:
	MSG_BOX("CFireWeapon Component Ready Failed err: 13115");
	return E_FAIL;
}

CFireWeapon * CFireWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFireWeapon *	pInstance = new CFireWeapon(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CFireWeapon Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CFireWeapon::Free(void)
{
	_ulong		dwRefCnt = 0;
		
	dwRefCnt = CWeapon::Free();

	return dwRefCnt;
}
