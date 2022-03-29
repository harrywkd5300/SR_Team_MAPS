#pragma once

#include "gameObject.h"
#include "defines.h"


class CLightObj;

class CTestTerrain
	:public CGameObject
{

/*ctor dtor*/
private:
	explicit CTestTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestTerrain() = default;

/*FUNCTION*/
public:
	virtual HRESULT Ready_GameObject() final;

	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject() final;


private:
	HRESULT Ready_Component();

/*MEMBER*/
private:
	Jang_CTerrain_Buffer*	m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CMaterial*				m_pMatCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;


/*related ctor dtor*/
public:
	static CTestTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual _ulong Free() final;

	// ------------------ Test --------------------
public:
	//LPD3DXMESH				gpCube = NULL;
	//LPD3DXMESH LoadModel(const _tchar * filename);
	CLightObj* m_pLight[MAX_LIGHT_SIZE];
};

