#include "stdafx.h"
#include "..\Headers\ColliderSkill.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "WELL512a.h"
#include "Collider_Manager.h"

CColliderSkill::CColliderSkill(LPDIRECT3DDEVICE9 pGraphicDev)
	: CLightEffectObj(pGraphicDev), m_pColliderMgr(CCollider_Manager::GetInstance())
{
	m_pColliderMgr->AddRef();
}

HRESULT CColliderSkill::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;		

	return NOERROR;
}

_int CColliderSkill::Update_GameObject(const _float & fTimeDelta)
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

	m_pTransformCom->Move_ToDirection(m_vDir, m_fSpeed, fTimeDelta);

	m_fCurRange += D3DXVec3Length(&(m_vDir*m_fSpeed*fTimeDelta));

	if (m_fSkillRange <= m_fCurRange)
		m_bIsDead = true;
	//m_pTransformCom->Set_Position(m_pTransformCom->Get_Position() *CRandomWell512::GetInstance()->GetValueF(0.99f, 1.0f) * fTimeDelta);
	//m_pTransformCom->Move_ToDirection(_vec3(0.f, 1.f, 0.f), 0.1f, fTimeDelta);
	
	
	//if (m_pEffectManager->Set_Effect_Output(
	//	CEffect_Manager::EFFECT_TYPE::EFFECT_FIXED,
	//	m_pTextureTag,
	//	m_pEffect,
	//	m_pEffect
	//	) == false)
	//{
	//	return 0;
	//}

	return 0;
}

void CColliderSkill::Render_GameObject(void)
{
	

	return;
}

_int CColliderSkill::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
	{
		return 1;
	}

	m_pColliderBox_AABB->Set_Position(m_pTransformCom->Get_Position());
	//m_pColliderBox_AABB->Set_Scale();
	m_pColliderBox_AABB->Update_ColliderBox_VertexBuffer();
	//m_pColliderMgr->Add_ColliderSkill(this);

	return CLightEffectObj::LastUpdate_GameObject(fTimeDelta);
}

HRESULT CColliderSkill::Ready_Component(void)
{
	CComponent* pComponent = nullptr;

	if(FAILED(CLightEffectObj::Ready_Component()))
		goto exception;

	/*Add ColliderBox_AABB*/
	pComponent = m_pColliderBox_AABB = dynamic_cast<CAABB_Buffer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_ColliderBox_AABB"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"ColliderBox_AABB", pComponent));
	m_pColliderBox_AABB->AddRef();

	///*Add Effect_Bullet*/
	//pComponent = m_pEffect = dynamic_cast<CEffect_Fixed*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Effect_Fixed"));
	//if (nullptr == pComponent)
	//	goto exception;
	//m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Effect_Fixed", pComponent));
	//m_pEffect->AddRef();

	///*Add Texture*/
	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_FlameB"));
	//if (nullptr == pComponent)
	//	goto exception;
	//m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Effect", pComponent));
	//m_pTextureCom->AddRef();
	//m_pTextureTag = L"Component_Texture_Effect_FlameB";

	//EFTINFO pInfo = m_pEffect->Get_Info();
	//pInfo.fFrameSpeed = 60.f;
	//pInfo.fMaxFrame = 35.f;
	//m_pEffect->Set_Info(pInfo);

	return NOERROR;

exception:
	MSG_BOX("ColliderSkill Component Ready Failed err: 13115");
	return E_FAIL;

}

CColliderSkill * CColliderSkill::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CColliderSkill *	pInstance = new CColliderSkill(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CColliderSkill Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CColliderSkill::Free(void)
{
	_ulong		dwRefCnt = 0;
		
	Engine::Safe_Release(m_pColliderMgr);
	Engine::Safe_Release(m_pColliderBox_AABB);


	dwRefCnt = CLightEffectObj::Free();

	return dwRefCnt;
}
