#include "stdafx.h"
#include "..\Headers\Effect.h"
#include "Component_Manager.h"
#include "Light_Manager.h"



CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTextureCom(nullptr)	
	, m_fFrame(0.f)
{

}

HRESULT CEffect::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;		

	m_pLightMgr = CLight_Manager::GetInstance();
	m_pLightMgr->AddRef();

	m_pTransformCom->Scaling(2.f, 2.f, 2.f);
	m_pTransformCom->Set_MatrixRow(_vec3(12.f * (rand()%2+1), 6.5f, 27.f * (rand() % 2 + 1)), Engine::CTransform::ROW_INFO::INFO_POSITION);
	
	//m_pTransformCom->Move_ToDirection(_vec3(20.0f, 8.0f , 50.0f), 20.f, 1);

	return NOERROR;
}

_int CEffect::Update_GameObject(const _float & fTimeDelta)
{
	//Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_fFrame += 60.f * fTimeDelta;

	if (m_fFrame >= 30.f)
		m_fFrame = 0.f;

	_matrix		matView, matWorld;
	_vec3		vRight, vUp, vLook;
	float fScale=2.f;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matWorld, nullptr, &matView);

	memcpy(&vRight, &matWorld.m[0][0], sizeof(_vec3));
	memcpy(&vUp, &matWorld.m[1][0], sizeof(_vec3));
	memcpy(&vLook, &matWorld.m[2][0], sizeof(_vec3));

	m_pTransformCom->Set_MatrixRow(vRight*fScale, Engine::CTransform::ROW_INFO::INFO_RIGHT);
	m_pTransformCom->Set_MatrixRow(vUp*fScale, Engine::CTransform::ROW_INFO::INFO_UP);
	m_pTransformCom->Set_MatrixRow(vLook*fScale, Engine::CTransform::ROW_INFO::INFO_LOOK);


	//m_pTransformCom->Fill_WorldMatrix();
	m_pLightCom->SetUp_LightPosition(m_pTransformCom->Get_MatrixRow(CTransform::ROW_INFO::INFO_POSITION));

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDERTYPE::RENDER_ALPHA, this);
	if(nullptr != m_pLightMgr)
		m_pLightMgr->Add_RenderLight(m_pLightCom, LIGHT_TYPE::POINT_LIGHT);

	return 0;
}

void CEffect::Render_GameObject(void)
{
	if (nullptr == m_pBufferCom)
		return;		

	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);	

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 1);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	
	m_pTransformCom->SetUp_MatrixToGraphicDev();
	m_pTextureCom->SetUp_TextureToGraphicDev(0, _uint(m_fFrame));
	m_pBufferCom->Render_Buffer();	

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CEffect::Set_LightColor(D3DXCOLOR color,float fRange)
{
	if (nullptr != m_pLightCom)
	{
		m_pLightCom->SetUp_LightInfo(color, color, color, fRange);
	}
}

void CEffect::Set_Texture(const _tchar * pComponentTag)
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

	return;
}

HRESULT CEffect::Ready_Component(void)
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

	/*Add Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_Invincible"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Effect", pComponent));
	m_pTextureCom->AddRef();

	/*Add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();

	/*Add Light*/
	pComponent = m_pLightCom = dynamic_cast<CLight*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Light"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Light", pComponent));
	m_pLightCom->AddRef();
	//D3DXCOLOR Organge(248.f / 255.f, 188.f / 255.f, 80.f / 255.f, 1.f);
	//D3DXCOLOR Rime(0xbf / 255.f, 0xff / 255.f, 0x00 / 255.f, 1.f);
	//m_pLightCom->SetUp_LightInfo(Rime, Rime, Rime, 10.f);

	return NOERROR;

exception:
	MSG_BOX("Effect Component Ready Failed err: 13115");
	return E_FAIL;

}

CEffect * CEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect *	pInstance = new CEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CEffect Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CEffect::Free(void)
{
	_ulong		dwRefCnt = 0;
		
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pLightCom);
	Engine::Safe_Release(m_pLightMgr);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
