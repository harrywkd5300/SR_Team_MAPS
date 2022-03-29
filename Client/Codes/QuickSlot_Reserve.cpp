#include "stdafx.h"
#include "..\Headers\QuickSlot_Reserve.h"


CQuickSlot_Reserve::CQuickSlot_Reserve(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


HRESULT CQuickSlot_Reserve::Ready_GameObject(const _float & fDgreeX, const _float & fDgreeY, const _float & fSizeX, const _float & fSizeY)
{
	if (FAILED(Ready_component()))
		return E_FAIL;
	m_pBufferCom->Set_Parameter(BACKCX - fDgreeX, BACKCY - fDgreeY, fSizeX, fSizeY);
	return NOERROR;
}

_int CQuickSlot_Reserve::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return 1;
	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_ALPHA, this);

	return 0;
}

void CQuickSlot_Reserve::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTextureCom)
		return;

	m_pTextureCom->SetUp_TextureToGraphicDev(0, 0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CQuickSlot_Reserve::Ready_component()
{
	Engine::CComponent*			pComponent = nullptr;

	/*Add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();

	/*add Cube buffer*/
	pComponent = m_pBufferCom = dynamic_cast<CRectUI_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectStageUI2"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Rect_Buffer", pComponent));
	if (nullptr == m_pBufferCom)
		MSG_BOX("Buffer_Cube Clone Failed err:50624");
	m_pBufferCom->AddRef();

	/*Add Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_StageUI"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
	m_pTextureCom->AddRef();

	return NOERROR;
exception:
	MSG_BOX("LogoUI Component Ready Failed err: 13115");
	return E_FAIL;
}

void CQuickSlot_Reserve::Set_ChangeItem(_bool Equip)
{
	m_bEquip = Equip;
	if (m_bEquip)
	{
		m_pBufferCom->Set_Parameter(BACKCX - 160.f, BACKCY - 130.f, 140.f, 60.f);
	}
	else
	{
		m_pBufferCom->Set_Parameter(BACKCX - 80.f, BACKCY - 70.f, 100.f, 40.f);
	}
}

CQuickSlot_Reserve * CQuickSlot_Reserve::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float & fDgreeX, const _float & fDgreeY, const _float & fSizeX, const _float & fSizeY)
{
	CQuickSlot_Reserve*	pInstance = new CQuickSlot_Reserve(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(fDgreeX, fDgreeY, fSizeX, fSizeY)))
	{
		MSG_BOX("CQuickSlot_Reserve Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;

}

_ulong CQuickSlot_Reserve::Free()
{
	_ulong		dwRefCnt = 0;
	//Safe_Release(m_pPlayer);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);

	dwRefCnt = CGameObject::Free();

	return dwRefCnt;
}
