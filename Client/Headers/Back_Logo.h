#pragma once

#include "Defines.h"
#include "GameObject.h"



class CBack_Logo final
	:public CGameObject
{

//*ctor dtor*/
private:
	explicit CBack_Logo(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual ~CBack_Logo() = default;


/*FUNCTION*/
public:
	virtual HRESULT		Ready_GameObject() final;
	virtual _int		Update_GameObject(const _float& fTimeDelta) final;
	virtual void		Render_GameObject() final;

private:
	HRESULT				Ready_Component();
	void				Update_Input();


/*MEMBER*/
private:
	/*component*/
	CRectUI_Texture*		m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;


/*FUNCTION related ctor dtor*/
public:
	static CBack_Logo*	Create(LPDIRECT3DDEVICE9 pGraphicDevice);

private:
	virtual _ulong		Free() final;


};

