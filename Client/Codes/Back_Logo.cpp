#include "stdafx.h"
#include "..\Headers\Back_Logo.h"
#include "Input_Device.h"
#include "management.h"
#include "scene_stage.h"

CBack_Logo::CBack_Logo(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTextureCom(nullptr)
{
}


HRESULT CBack_Logo::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pBufferCom->Set_Parameter(BACKCX / 2, BACKCY / 2 - 100.f, 1200.f, 500.f);

	return NO_ERROR;
}


_int CBack_Logo::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return 0;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_ALPHA, this);

	return 0;
}

void CBack_Logo::Render_GameObject()
{
	if (nullptr == m_pBufferCom)
	{
		MSG_BOX("Buffer isn't allocated");
		return;
	}

	m_pTextureCom->SetUp_TextureToGraphicDev(0);

	m_pBufferCom->Render_Buffer();
}


HRESULT CBack_Logo::Ready_Component()
{

	//
	/*=======Add Static Component========*/
	//
	/*Add Rect_Texture*/
	CComponent* pComponent = nullptr;
	pComponent = m_pBufferCom = dynamic_cast<CRectUI_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectUITex"));

	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	m_pBufferCom->AddRef();

	/*Add Texture*/
	pComponent = nullptr;
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_LOGO, L"Component_Texture_BackLogo"));

	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	m_pTextureCom->AddRef();

	/*Add Renderer*/
	pComponent = nullptr;
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));

	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));
	m_pRendererCom->AddRef();

	return NOERROR;

	/*Component Add Failed*/
exception:
	MSG_BOX("Buffer Clone Failed err:96403");
	return E_FAIL;

}

void CBack_Logo::Update_Input()
{

}


CBack_Logo * CBack_Logo::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CBack_Logo* pInstance = new CBack_Logo(pGraphicDevice);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("Logo_Back Creation Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CBack_Logo::Free()
{
	_ulong dwRefCount = 0;

	/*Decrease RefCount*/
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	dwRefCount = Engine::CGameObject::Free();
	
	return dwRefCount;
}
