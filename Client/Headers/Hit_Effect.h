#pragma once

#include "Defines.h"
#include "gameObject.h"

class CHit_Effect final : public CGameObject
{
private:
	explicit CHit_Effect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHit_Effect() = default;

	/*FUNCTION*/
public:
	virtual HRESULT		Ready_GameObject() final;
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
public:
	static CHit_Effect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual _ulong		Free() final;
};

