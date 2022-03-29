#include "stdafx.h"
#include "..\Headers\Logo_UI.h"

#include "object_Manager.h"


CLogo_UI::CLogo_UI(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iNum)
	:CGameObject(pGraphicDev)
	, m_iTextureNum(iNum)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTextureCom(nullptr)
{
}

HRESULT CLogo_UI::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pBufferCom->Set_Parameter(BACKCX / 2, BACKCY / 2 , 500.f, 70.f);

	return NOERROR;
}

_int CLogo_UI::Update_GameObject(const _float & fTimeDelta)
{
	if (true == m_bIsDead)
		return 1;
	if (nullptr == m_pRendererCom)
		return -1;

	RECT rc;
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT rcMouse = { LONG(ptMouse.x - 5.f), LONG(ptMouse.y - 5.f), LONG(ptMouse.x + 5.f), LONG(ptMouse.y + 5.f) };
	RECT rcUI	 = { LONG(BACKCX / 2 - 250.f), LONG(BACKCY / 2 - 55.f), LONG(BACKCX / 2 + 250.f), LONG(BACKCY / 2 + 15.f) };

	//cout << " X : " << ptMouse.x << " Y : " << ptMouse.y << endl;
	if (IntersectRect(&rc, &rcMouse, &rcUI))
	{
		m_iTextureNum = 1;
		m_bClick = true;
	}
	else
	{
		m_iTextureNum = 0;
		m_bClick = false;
	}


	m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);

	return 0;
}

void CLogo_UI::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTextureCom)
		return;

	m_pTextureCom->SetUp_TextureToGraphicDev(0, m_iTextureNum);
	m_pBufferCom->Render_Buffer();
}

HRESULT CLogo_UI::Ready_Component()
{
	Engine::CComponent*			pComponent = nullptr;

	/*Add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();

	/*add Cube buffer*/
	pComponent = m_pBufferCom = dynamic_cast<CRectUI_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectUITexButton1"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Rect_Buffer", pComponent));
	if (nullptr == m_pBufferCom)
		MSG_BOX("Buffer_Cube Clone Failed err:50624");
	m_pBufferCom->AddRef();

	/*Add Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_LOGO, L"Component_Texture_LogoUI"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
	m_pTextureCom->AddRef();

	return NOERROR;
exception:
	MSG_BOX("LogoUI Component Ready Failed err: 13115");
	return E_FAIL;
}


CLogo_UI * CLogo_UI::Create(LPDIRECT3DDEVICE9 pGraphicDevice, const _uint& iNum)
{
	CLogo_UI*	pInstance = new CLogo_UI(pGraphicDevice, iNum);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CLogo_UI Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CLogo_UI::Free()
{
	_ulong		dwRefCnt = 0;

	/*Decrease RefCount*/
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
