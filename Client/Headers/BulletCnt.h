#pragma once

#include "Defines.h"
#include "gameObject.h"

namespace Engine
{
	class CD3DFont;
}

class CBulletCnt final : public CGameObject
{
private:
	explicit CBulletCnt(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBulletCnt() = default;

public:
	virtual HRESULT		Ready_GameObject() final;
	virtual _int		Update_GameObject(const _float& fTimeDelta) final;
	virtual void		Render_GameObject() final;

	/*FUNCTION related ctor dtor*/
private:
	HRESULT				Ready_D3dFont();
	HRESULT				Ready_Component();

private:
	CD3DFont*			m_pD3dFont = nullptr;
	CRenderer*			m_pRendererCom = nullptr;

private:
	_uint				m_iMaxBulletCnt = 0;
	_uint				m_iRemainBulletCnt = 0;
	_bool				m_bReloard = false;
	_bool				m_bChangeUpdate = false;

public:
	static CBulletCnt*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual _ulong		Free() final;

};

