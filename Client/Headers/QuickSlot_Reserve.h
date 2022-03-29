#pragma once

#include "Defines.h"
#include "gameObject.h"

class CQuickSlot_Reserve final : public CGameObject
{
private:
	explicit CQuickSlot_Reserve(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuickSlot_Reserve() = default;

	/*FUNCTION*/
public:
	virtual HRESULT		Ready_GameObject(const _float& fDgreeX, const _float& fDgreeY, const _float& fSizeX, const _float& fSizeY) final;
	virtual _int		Update_GameObject(const _float& fTimeDelta) final;
	virtual void		Render_GameObject() final;

	/*FUNCTION related ctor dtor*/
private:
	HRESULT				Ready_component();

private:
	/*component*/
	CRectUI_Texture*	m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;

private:
	_bool				m_bEquip = false;

public:
	void				Set_ChangeItem(_bool Equip);
public:
	static CQuickSlot_Reserve*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fDgreeX, const _float& fDgreeY, const _float& fSizeX, const _float& fSizeY);

private:
	virtual _ulong		Free() final;

};

