#include "stdafx.h"
#include "..\Headers\BulletCnt.h"

#include "Graphic_Device.h"
#include "object_Manager.h"
#include "Layer.h"
#include "Player.h"
#include "Weapon.h"

CBulletCnt::CBulletCnt(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pRendererCom(nullptr)
	, m_iMaxBulletCnt(0)
	, m_iRemainBulletCnt(0)
{
}


HRESULT CBulletCnt::Ready_GameObject()
{
	if (FAILED(Ready_D3dFont()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;
	return NOERROR;
}

_int CBulletCnt::Update_GameObject(const _float & fTimeDelta)
{
	CLayer* pLayer = CObject_Manager::GetInstance()->Get_Layer(SCENETYPE::SCENE_STAGE, L"Layer_Player");

	if (nullptr == pLayer)
		return 0;

	auto iter = pLayer->Get_ObjList().begin();
	if (nullptr == (*iter))
		return 0;

	m_iRemainBulletCnt = dynamic_cast<CPlayer*>((*iter))->Get_EquipWeapon()->Get_WeaponInfo().m_iCurBullet;
	m_iMaxBulletCnt = dynamic_cast<CPlayer*>((*iter))->Get_EquipWeapon()->Get_WeaponInfo().m_iMaxBullet;
	m_bReloard = dynamic_cast<CPlayer*>((*iter))->Get_EquipWeapon()->Get_WeaponInfo().m_bReloading;
	m_bChangeUpdate = dynamic_cast<CPlayer*>((*iter))->Get_EquipWeapon()->Get_UpdateChange();
	m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);

	return 0;
}

void CBulletCnt::Render_GameObject()
{
	_char					m_szBullet[32] = "";

	if (m_iRemainBulletCnt == m_iMaxBulletCnt)
	{
		sprintf_s(m_szBullet, " %d / %d", m_iMaxBulletCnt, m_iMaxBulletCnt);
		m_pD3dFont->DrawTextW(BACKCX - 150.f, BACKCY - 200.f, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), m_szBullet);
	}
	else
	{
		sprintf_s(m_szBullet, " %d / %d", m_iRemainBulletCnt, m_iMaxBulletCnt);
		m_pD3dFont->DrawTextW(BACKCX - 150.f, BACKCY - 200.f, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), m_szBullet);
	}

	if (m_bReloard && !m_bChangeUpdate)
	{
		m_pD3dFont->DrawTextW(BACKCX - 160.f, BACKCY - 230.f, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), "Reloading...");
	}
	else
	{
		if (0 == m_iRemainBulletCnt)
		{
			m_pD3dFont->DrawTextW(BACKCX - 160.f, BACKCY - 230.f, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), "Reload!!!");
		}
	}
}


_ulong CBulletCnt::Free()
{
	Safe_Release(m_pD3dFont);
	Safe_Release(m_pRendererCom);

	return CGameObject::Free();
}

HRESULT CBulletCnt::Ready_D3dFont()
{
	m_pD3dFont = CGraphic_Device::GetInstance()->Get_Font();

	if (nullptr == m_pD3dFont)
	{
		MSG_BOX("Ready D3dFont Failed");
		return E_FAIL;
	}
	m_pD3dFont->AddRef();

	return NOERROR;
}

HRESULT CBulletCnt::Ready_Component()
{
	CComponent*	pComponent = nullptr;

	/*Add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();


	return NOERROR;
exception:
	MSG_BOX("BullCnt Ready Failed err: 13115");
	return E_FAIL;
}

CBulletCnt * CBulletCnt::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBulletCnt*	pInstance = new CBulletCnt(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CBulletCnt Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}
