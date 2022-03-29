#pragma once

#include "Defines.h"
#include "gameObject.h"

class CLogo_UIQuit final : public CGameObject
{
private:
	explicit CLogo_UIQuit(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iNum);
	virtual ~CLogo_UIQuit() = default;

	/*FUNCTION*/
public:
	virtual HRESULT		Ready_GameObject() final;
	virtual _int		Update_GameObject(const _float& fTimeDelta) final;
	virtual void		Render_GameObject() final;

private:
	HRESULT				Ready_Component();
	/*MEMBER*/
	_uint				m_iTextureNum = 0;

private:
	/*component*/
	CRectUI_Texture*	m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;

	/*FUNCTION related ctor dtor*/
public:
	static CLogo_UIQuit*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, const _uint& iNum);

private:
	virtual _ulong		Free() final;
};

