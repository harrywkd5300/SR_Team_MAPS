#pragma once

#include "defines.h"
#include "gameObject.h"


namespace Engine
{
	class CD3DFont;
	class CParticle_Renderer;
	class CPicking;
	class CRandomWell512;
	class CParticle_Bullet;
	
}
//class CParticle_Bullet;
class CLightObj;
class CWeapon;



class CJangPlayer
	:public CGameObject
{

public:
	enum ANIMATION_STATE {IDLE,WALK};
	enum WALK_STATE {WALK_IDLE,WALK_LEFT,WALK_RIGHT};
	WALK_STATE				m_eWalkState = WALK_RIGHT;
	ANIMATION_STATE			m_eState = IDLE;
	vector<CJang_Cube*>	m_vecBodyCom;
	
	
	/*BONETEST*/
	void	Transform_Test(const _float& fTimeDelta);
	void	Update_Bone(const _float& fTimeDelta);
	void	Update_Body(const _float& fTimeDelta);
	void	Walk_Animation(const _float& fTimeDelta);
	void	Render_Bone();
	void	Render_Body();
	

/*ctor dtor*/
private:
	explicit CJangPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CJangPlayer() = default;


/*FUNCTION*/
public:
	const _vec3& Get_MatrixRow(CTransform::ROW_INFO eInfo) {return m_pTransformCom->Get_MatrixRow(eInfo);}
	const _matrix& Get_ScaleMatrix() { return m_pTransformCom->Get_ScaleMatrix(); }
	CTransform* GetTransformCom() { return m_pTransformCom; };

public:
	virtual HRESULT Ready_GameObject();
	
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	
	virtual void Render_GameObject();
	virtual void Render_Shadow(_matrix CameraWorldMatrix);

private:
	HRESULT Ready_Component();
	HRESULT	Ready_Parameters();
	HRESULT Ready_D3dFont();
	HRESULT Ready_Weapon();

	void	Update_Input(const _float fTimeDelta);

	void	Recoil(const _float& fTimeDelta, const _int& iForceX, const _int& iForceY); // 수평반동, 수직반동
	void	Recover_Recoil(const _float& fTimeDelta);
	void	Gravity_Test(const _float& fTimeDelta);

	void	FixMouse();


/*MEMBER*/
private:
	CInput_Device*	m_pInput_Device = nullptr;
	CD3DFont*		m_pD3dFont = nullptr;
	CRandomWell512*	m_pRandomMgr = nullptr;

	/*Clone Component*/	
	CJang_Cube*		m_pBufferCom = nullptr;
	//CCube_Texture*		m_pBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CMaterial*			m_pMatCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CPicking*			m_pPickingCom = nullptr;
	
	vector<CBone*>		m_vecBoneCom;
	CBone*				m_pBoneCom = nullptr;
	CCube_Color*		m_pBoneCubeCom = nullptr;

	// ------------- Weapon ----------------
	CWeapon*			m_pWeapon[WEAPON_TYPE::WEAPON_END];
	WEAPON_TYPE			eCurWeaponType;
	// ------------- Weapon ----------------

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
	_vec3	m_fRecoverAngle;

	_int	m_iRecoilY = 0;			/*RecoilForceY*/
	_float	m_fFiringRateTimer = 0;	/*Fire Rating*/


/*related ctor dtor*/
public:
	static CJangPlayer* Create(LPDIRECT3DDEVICE9 pGrahpicDev);
private:
	virtual _ulong Free() final;

	// ------------------ Test --------------------
public:
	CLightObj* m_pLight[MAX_LIGHT_SIZE];
	void SetShader(_uint iShaderNum);
	void StartShader(_uint iShaderNum);
	void EndShader(_uint iShaderNum);
	_matrix testWorld; 

	//// -------------- Weapon Class Info --------------
	//CParticle_Bullet* m_pBulletParticle = nullptr;
	//CParticle_Renderer* m_pParticleRenderer = nullptr;
	//CTexture* m_pBulletTexture = nullptr;
	//// -------------- Weapon Class Info --------------

	//bool RenderRun = false;
};

