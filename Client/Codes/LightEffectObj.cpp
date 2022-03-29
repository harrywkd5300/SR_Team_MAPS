#include "stdafx.h"
#include "..\Headers\LightEffectObj.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "WELL512a.h"

CLightEffectObj::CLightEffectObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pEffectManager(CEffect_Manager::GetInstance())
	, m_pEffect(nullptr)
	, m_pTextureTag(nullptr)
	, m_fFrame(0.f)
{
	m_pEffectManager->AddRef();
	m_vScale = _vec3(1.f, 1.f, 1.f);
}

HRESULT CLightEffectObj::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;		

	//m_pTransformCom->Set_Position(_vec3(59.5f, 2.5f, 65.f));

	return NOERROR;
}

_int CLightEffectObj::Update_GameObject(const _float & fTimeDelta)
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

void CLightEffectObj::Render_GameObject(void)
{
	

	return;
}

_int CLightEffectObj::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

void CLightEffectObj::Set_LightColor(D3DXCOLOR color,float fRange)
{
	if (m_pEffect == nullptr)
		return;
	
	m_pEffect->Set_Light(color, fRange);
}

void CLightEffectObj::Set_Texture(_tchar * pComponentTag, _float fMaxFrame)
{
	Engine::CComponent*			pComponent = nullptr;


	if (nullptr != m_pTextureCom)
	{
		m_mapComponent[CComponent::UPDATETYPE_NOTIFY].erase(L"Texture_Effect");
		Safe_Release(m_pTextureCom);
		Safe_Release(m_pTextureCom);
	}

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, pComponentTag));
	if (nullptr == pComponent)
		return;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Effect", pComponent));
	m_pTextureCom->AddRef();
	m_pTextureTag = pComponentTag;

	if (FAILED(m_pEffectManager->Set_Effect_Type(
		CEffect_Manager::EFFECT_TYPE::EFFECT_FIXED,
		pComponentTag,
		m_pTextureCom
	)))
	{
		MSG_BOX("Effect_Manager_Texture_SizeOver : LightObj");
		return;
	}
	//if(m_pEffect)
	m_pEffect = dynamic_cast<CEffect_Fixed*>(m_pEffectManager->Get_Effect(CEffect_Manager::EFFECT_TYPE::EFFECT_FIXED, m_pTextureTag));
	
	EFTINFO pInfo = m_pEffect->Get_Info();
	pInfo.fFrameSpeed = 60.f;
	pInfo.fMaxFrame = fMaxFrame;
	pInfo._isLight = true;
	pInfo._isAlive = true;
	m_pEffect->Set_Info(pInfo);
	//m_pEffect = static_cast<CEffect*>(m_pEffectManager->Get_Effect(CEffect_Manager::EFFECT_TYPE::EFFECT_FIXED, m_pTextureTag));


	return;
}

HRESULT CLightEffectObj::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;
		
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
	MSG_BOX("Effect Component Ready Failed err: 13115");
	return E_FAIL;

}

CLightEffectObj * CLightEffectObj::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLightEffectObj *	pInstance = new CLightEffectObj(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CLightEffectObj Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CLightEffectObj::Free(void)
{
	_ulong		dwRefCnt = 0;
		
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Safe_Release(m_pEffectManager);
	//Safe_Release(m_pEffect);
	m_pEffect->Reset_Info();
	m_pEffect->SetIsDead();

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
