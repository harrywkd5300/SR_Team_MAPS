#include "stdafx.h"
#include "..\Headers\Pistol.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Particle_Renderer.h"
#include "WELL512a.h"

CPistol::CPistol(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
	
}

HRESULT CPistol::Ready_GameObject(void)
{
	m_tInfo.m_fFireRate = 0.3f;
	m_tInfo.m_fBulletSpeed = 20.f;
	m_tInfo.m_fReloadRate = 1.f;

	m_tInfo.m_fXRecoilPower = 1.f;
	m_tInfo.m_fYRecoilPower = 1.f;

	m_tInfo.m_bBulletEffect = true;
	
	m_tInfo.m_iCurBullet = 7;
	m_tInfo.m_iMaxBullet = 7;
	m_tInfo.m_iDamage = 10.f;
	
	if (FAILED(Ready_Component()))
		return E_FAIL;		


	return NOERROR;
}


HRESULT CPistol::Ready_Component(void)
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

	/*Add Particle_Bullet*/
	pComponent = m_pBulletParticle = dynamic_cast<CParticle_Bullet*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Particle_Bullet"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Particle_Bullet", pComponent));
	m_pBulletParticle->AddRef();

	/*Add Bullet_Texture*/
	pComponent = m_pBulletTexture = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Bullet_Pistol"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Bullet", pComponent));
	m_pBulletTexture->AddRef();
	m_pTextureTag = L"Component_Texture_Bullet_Pistol";

	if (FAILED(m_pParticleRenderer->Set_Particle_Type(
		CParticle_Renderer::PARTICLETYPE::PARTICLE_DIRECT_BULLET,
		L"Component_Texture_Bullet_Pistol",
		m_pBulletTexture
	)))
	{
		MSG_BOX("Particle_Texture_SizeOver : Pistol");
	}

	m_pTerrainBufferCom = dynamic_cast<CTerrain_Buffer*>(CObject_Manager::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain", 0, L"Buffer_Terrain"));
	m_pTerrainBufferCom->AddRef();

	return NOERROR;

exception:
	MSG_BOX("CPistol Component Ready Failed err: 13115");
	return E_FAIL;
}

CPistol * CPistol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPistol *	pInstance = new CPistol(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CPistol Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CPistol::Free(void)
{
	_ulong		dwRefCnt = 0;
		
	dwRefCnt = CWeapon::Free();

	return dwRefCnt;
}
