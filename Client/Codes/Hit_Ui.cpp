#include "stdafx.h"
#include "..\Headers\Hit_Ui.h"


CHit_Ui::CHit_Ui(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


HRESULT CHit_Ui::Ready_GameObject()
{
	if (FAILED(Ready_component()))
		return E_FAIL;

	m_pBufferCom->Set_Parameter(BACKCX / 2, BACKCY / 2, BACKCX, BACKCY);

	return NOERROR;
}

_int CHit_Ui::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return 1;


	return 0;
}

void CHit_Ui::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTextureCom)
		return;

	m_pTextureCom->SetUp_TextureToGraphicDev(0, 0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CHit_Ui::Ready_component()
{
	Engine::CComponent*			pComponent = nullptr;

	/*add Cube buffer*/
	pComponent = m_pBufferCom = dynamic_cast<CRectUI_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectHITUI"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Rect_Buffer", pComponent));
	if (nullptr == m_pBufferCom)
		MSG_BOX("Buffer_Cube Clone Failed err:50624");
	m_pBufferCom->AddRef();

	/*Add Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_RowHP"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
	m_pTextureCom->AddRef();

	return NOERROR;
exception:
	MSG_BOX("LogoUI Component Ready Failed err: 13115");
	return E_FAIL;
}

CHit_Ui * CHit_Ui::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHit_Ui*	pInstance = new CHit_Ui(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CHit_Ui Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CHit_Ui::Free()
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);

	dwRefCnt = CGameObject::Free();

	return dwRefCnt;
}
