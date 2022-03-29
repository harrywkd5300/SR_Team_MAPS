#include "stdafx.h"
#include "..\Headers\CrossFilter.h"


CCrossFilter::CCrossFilter(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


HRESULT CCrossFilter::Ready_GameObject()
{
	if (FAILED(Ready_component()))
		return E_FAIL;

	m_pBufferCom->Set_Parameter(BACKCX / 2, BACKCY / 2, 10.f, 10.f);

	return NOERROR;
}

_int CCrossFilter::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return 1;
	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_ALPHA, this);

	return 0;
}

void CCrossFilter::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTextureCom)
		return;

	m_pTextureCom->SetUp_TextureToGraphicDev(0, 0);
	m_pBufferCom->Render_Buffer();

}

HRESULT CCrossFilter::Ready_component()
{
	Engine::CComponent*			pComponent = nullptr;

	/*Add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();

	/*add Cube buffer*/
	pComponent = m_pBufferCom = dynamic_cast<CRectUI_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectCross"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Rect_Buffer", pComponent));
	if (nullptr == m_pBufferCom)
		MSG_BOX("Buffer_Cube Clone Failed err:50624");
	m_pBufferCom->AddRef();

	/*Add Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Cross"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
	m_pTextureCom->AddRef();

	return NOERROR;
exception:
	MSG_BOX("LogoUI Component Ready Failed err: 13115");
	return E_FAIL;
}

CCrossFilter * CCrossFilter::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrossFilter*	pInstance = new CCrossFilter(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CCrossFilter Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CCrossFilter::Free()
{
	_ulong		dwRefCnt = 0;
	//Safe_Release(m_pPlayer);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);

	dwRefCnt = CGameObject::Free();

	return dwRefCnt;
}
