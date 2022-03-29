#include "stdafx.h"
#include "..\Headers\FireWeaponSkill.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "WELL512a.h"
#include "Collider_Manager.h"

CFireWeaponSkill::CFireWeaponSkill(LPDIRECT3DDEVICE9 pGraphicDev)
	: CColliderSkill(pGraphicDev)
{

}

HRESULT CFireWeaponSkill::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;		

	return NOERROR;
}

_int CFireWeaponSkill::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pEffect || nullptr == m_pEffectManager 
		|| nullptr == m_pTransformCom || nullptr == m_pTextureTag 
		|| nullptr == m_pTextureCom)
		return 0;

	m_pEffect->ResetEffect(
		m_pTransformCom->Get_Position(),
		D3DXVECTOR3(m_vScale.x, m_vScale.y, m_vScale.z), //vScale
		CEffect_Fixed::TYPE_INFINITY,
		0.f,
		m_pTransformCom
	);

	
	//m_pTransformCom->Move_ToDirection(m_vDir, m_fSpeed, fTimeDelta);
	m_pTransformCom->Move_ToDirection(m_vDir, m_fSpeed, fTimeDelta);

	m_fCurTime += fTimeDelta;
	//_vec3 vDir = m_vDest - m_pTransformCom->Get_Position();
	
	//float vDist = D3DXVec3Length(&vDir);

	//m_vDir += vDir * fTimeDelta;

	//m_fCurRange += D3DXVec3Length(&(m_vDir*1.f*fTimeDelta));
	//m_fCurRange -= m_vDir.y *1.f*fTimeDelta;

	//cout << "vDir:" << m_vDir.x << " " << m_vDir.y << " " << m_vDir.z << endl;
	//cout << "vPos:" << m_pTransformCom->Get_Position().x <<" "<< m_pTransformCom->Get_Position().y << " " << m_pTransformCom->Get_Position().z << endl;
	//cout << "m_fSkillRange:" << m_fSkillRange << endl;
	//cout << "m_fCurRange:" << m_fCurRange << endl;
	//m_vDir.y -= GRAVITY * fTimeDelta;
	//D3DXVec3Normalize(&m_vDir,&m_vDir);
	//m_vDir *= m_fSpeed;

	//cout << vDist << endl;
	//cout << m_fSkillRange << endl;

	if (m_fCurTime >= m_fTime)
	{
		m_bIsDead = true;
	}
	//if (m_fSkillRange <= m_fCurRange)
	//	m_bIsDead = true;


	return 0;
}

void CFireWeaponSkill::Render_GameObject(void)
{
	

	return;
}

_int CFireWeaponSkill::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return CColliderSkill::LastUpdate_GameObject(fTimeDelta);
}

HRESULT CFireWeaponSkill::Ready_Component(void)
{
	CComponent* pComponent = nullptr;

	if(FAILED(CColliderSkill::Ready_Component()))
		goto exception;

	return NOERROR;

exception:
	MSG_BOX("ColliderSkill Component Ready Failed err: 13115");
	return E_FAIL;

}

CFireWeaponSkill * CFireWeaponSkill::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFireWeaponSkill *	pInstance = new CFireWeaponSkill(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CFireWeaponSkill Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CFireWeaponSkill::Free(void)
{
	_ulong		dwRefCnt = 0;

	dwRefCnt = CColliderSkill::Free();

	return dwRefCnt;
}
