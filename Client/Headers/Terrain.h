#pragma once

#include "gameObject.h"
#include "defines.h"


class CLightObj;

class CTerrain
	:public CGameObject
{

/*ctor dtor*/
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain() = default;

/*FUNCTION*/
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
	CMaterial*			m_pMatCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;


/*related ctor dtor*/
public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual _ulong Free() final;

};

