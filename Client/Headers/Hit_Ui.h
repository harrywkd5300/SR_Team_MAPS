#pragma once

#include "Defines.h"
#include "gameObject.h"

class CHit_Ui final : public CGameObject
{
private:
	explicit CHit_Ui(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHit_Ui() = default;

	/*FUNCTION*/
public:
	virtual HRESULT		Ready_GameObject() final;
	virtual _int		Update_GameObject(const _float& fTimeDelta) final;
	virtual void		Render_GameObject() final;

public:
	void				Set_Alpha(_float iAlpha) { m_iAlphaTime = iAlpha; }

	/*FUNCTION related ctor dtor*/
private:
	HRESULT				Ready_component();
private:
	_float				m_iAlphaTime = 150.f;

private:
	/*component*/
	CRectUI_Texture*	m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

public:
	static CHit_Ui*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual _ulong		Free() final;
};

