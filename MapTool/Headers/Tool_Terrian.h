#pragma once

#include "Tool_Define.h"
#include "gameObject.h"

class CTerrian : public CGameObject
{
private:
	explicit CTerrian(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrian() = default;

public:
	virtual HRESULT Ready_GameObject() final;

	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject() final;

private:
	HRESULT Ready_Component();

	/*MEMBER*/
private:
	CTerrain_Buffer*	m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;


	/*related ctor dtor*/
public:
	static CTerrian* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual _ulong Free() final;
};

