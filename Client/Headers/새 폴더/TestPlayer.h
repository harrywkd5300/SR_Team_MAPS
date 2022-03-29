#pragma once

#include "defines.h"
#include "gameObject.h"


namespace Engine
{
	class CD3DFont;
}

class CTestPlayer
	:public CGameObject
{

public:
	
/*ctor dtor*/
private:
	explicit CTestPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestPlayer() = default;


/*FUNCTION*/
public:
	const _vec3& Get_MatrixRow(CTransform::ROW_INFO eInfo) {return m_pTransformCom->Get_MatrixRow(eInfo);}
	const _matrix& Get_ScaleMatrix() { return m_pTransformCom->Get_ScaleMatrix(); }

public:
	virtual HRESULT Ready_GameObject();
	
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	
	virtual void Render_GameObject();

private:
	HRESULT Ready_Component();
	HRESULT	Ready_Parameters();
	HRESULT Ready_D3dFont();

	void	Update_Input(const _float fTimeDelta);

	void	Recoil(const _float& fTimeDelta, const _int& iForceX, const _int& iForceY); // 수평반동, 수직반동
	void	Recover_Recoil(const _float& fTimeDelta);
	void	Gravity_Test(const _float& fTimeDelta);

	void	FixMouse();


/*MEMBER*/
private:
	CInput_Device*	m_pInput_Device = nullptr;
	CD3DFont*		m_pD3dFont = nullptr;

	/*Clone Component*/	
	//CTemp_Cube*		m_pBufferCom = nullptr;
	CCube_Texture*		m_pBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	
	vector<CCube_Color*>	m_vecBodyCom;
	vector<CBone*>			m_vecBoneCom;
	CBone*					m_pBoneCom = nullptr;
	CCube_Color*			m_pBoneCubeCom = nullptr;

	/*Ref Component*/
	CVIBuffer*		m_pTerrainBufferCom = nullptr;

	/*Option Parameters*/
	_float	m_fMouseSensitive = 0.f;


	/*Player Parameters*/
	_float	m_fSpeed = 0.f;
	_float	m_fSprintSpeed = 0.f;

	/*Gun Parameters*/
	_vec3	m_vRecoil;
	_float	m_fRecoverTimer = 0;

	_int	m_iRecoilY = 0;			/*RecoilForceY*/
	_float	m_fFiringRateTimer = 0;	/*Fire Rating*/


/*related ctor dtor*/
public:
	static CTestPlayer* Create(LPDIRECT3DDEVICE9 pGrahpicDev);
private:
	virtual _ulong Free() final;
};

