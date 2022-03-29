#pragma once

#include "Defines.h"
#include "gameObject.h"


class CLightObj;

class CTestObject
	:public CGameObject
{
/*ctor dtor*/
private:
	explicit CTestObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestObject() =default;

/*FUNCTION*/
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();



private:
	HRESULT Ready_Component();
	void	Update_Input(const _float fTimeDelta);



/*MEMBER*/
private:
	CCube_Texture*	m_pBufferCom = nullptr;
	//CJang_Cube*		m_pBufferCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CMaterial*		m_pMatCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CInput_Device*	m_pInput_Device = nullptr;

/*related ctor dtor*/
public:
	static CTestObject* Create(LPDIRECT3DDEVICE9 pGrahpicDev);
private:
	virtual _ulong Free() final;

	//----------        Test           ----------
	LPD3DXMESH				gpCube = NULL;
	LPD3DXMESH LoadModel(const _tchar * filename);
	//----------        Test           ----------
public:
	CLightObj* m_pLight[8];
};

