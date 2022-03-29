#pragma once

#include "gameObject.h"
#include "defines.h"

namespace Engine
{
	class CTexture;
	class CRenderer;
	class CTransform;
	class CCube_Texture;
	class CCamera_Manager;
}

class CSkyBox final
	:public CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkyBox() = default;

/*FUNCTION*/
public:
	virtual HRESULT Ready_GameObject() final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject() final;

private:
	HRESULT Ready_Component();


/*MEMBER*/
private:
	/*clone Component*/
	CCube_Texture*		m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;

	CCamera_Manager*	m_pCamera_Manager = nullptr;

	/*related ctor dtor*/
public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual _ulong Free() final;
};

