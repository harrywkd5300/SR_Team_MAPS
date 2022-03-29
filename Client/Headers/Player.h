#pragma once

#include "defines.h"
#include "engine_defines.h"
#include "Character.h"

namespace Engine
{
	class CD3DFont;
	class CRandomWell512;
	class CCamera_Manager;

	class CParticle_Renderer;
	class CParticle_Bullet;
}

class CSound_Manager;
class CWeapon;

class CPlayer
	:public CCharacter
{
public:
	enum BONENUMBER{BONE_TRANSFORM,BONE_TWOARMS,BONE_LLEG,BONE_LKNEE,BONE_RLEG,BONE_RKNEE, BONE_UPPER, BONE_LARM, BONE_LELBOW, BONE_RARM, BONE_RELBOW, BONE_HEAD = 11, BONE_HAND, BONE_EYE, BONE_BACK};
	enum STATENUMBER {STATE_IDLE,STATE_FWALK,STATE_BWALK,STATE_LWALK,STATE_RWALK,STATE_FSPRINT,STATE_CHANGE,STATE_RELOAD};
	enum EQUIPMENT{EQUIP_MAIN,EQUIP_SUB,EQUIP_END};

/*ctor dtor*/
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer() = default;


/*FUNCTION*/
public:
	const _vec3&			Get_MatrixRow(CTransform::ROW_INFO eInfo) {return m_pTransformCom->Get_MatrixRow(eInfo);}
	const _matrix&			Get_ScaleMatrix() { return m_pTransformCom->Get_ScaleMatrix(); }
	const vector<CBone*>*	Get_BoneVector() { return &m_vecBone; }

public:
	virtual HRESULT Ready_GameObject();
	
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	
	virtual void Render_GameObject();

	virtual	_bool	Hit(_uint iDamage, const _float& fLifeTime);

private:
	HRESULT Ready_Component();
	HRESULT	Ready_Parameters();
	HRESULT Ready_D3dFont();
	HRESULT Ready_Weapon();
	HRESULT Ready_HitUI();

	void	Update_Input(const _float& fTimeDelta);
	void	LastUpdate_Input(const _float& fTimeDelta);
	void	Recover_Recoil(const _float& fTimeDelta);
	void	Gravity_Test(const _float& fTimeDelta);
	void	Set_PlayerLocal();
	void	FixMouse(const _uint & iX, const _uint & iY);


public:
	void SetShader(_uint iShaderNum);
	void StartShader(_uint iShaderNum);
	void EndShader(_uint iShaderNum);
	virtual void Render_Shadow(_matrix CameraWorldMatrix);
	const _matrix& GetBoneWorldMatrix(BONENUMBER eType) { return m_vecBone[eType]->Get_WorldMatrix(); }
	CWeapon*			Get_EquipWeapon() { return m_pCurWeapon; }

/*MEMBER*/
private:
	CInput_Device*		m_pInput_Device = nullptr;
	CD3DFont*			m_pD3dFont = nullptr;
	CRandomWell512*		m_pRandomMgr = nullptr;
	CCamera_Manager*	m_pCamera_Manager = nullptr;

	/*Clone Component*/	
	CCube_Texture*		m_pBufferCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	
	CBone*				m_pBoneCom = nullptr;
	CCube_Color*		m_pBoneCubeCom = nullptr;
	CPicking*			m_pPickingCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CMaterial*			m_pMatCom = nullptr;

	_float				m_iCurHp = 0.f;
	_bool				m_bHit = false;
	_bool				m_bMoveSound = true;
	_float				m_iHitTime = 0.f;
	_float				m_fSoundRack = 0.f;

	/*Ref Component*/
	CVIBuffer*			m_pTerrainBufferCom = nullptr;
	CGameObject*		m_pHitObject = nullptr;

	/*Option Parameters*/
	_float	m_fMouseSensitive = 0.f;

	/*Player Parameters*/
	_float	m_fSpeed = 0.f;
	_float	m_fSprintSpeed = 0.f;
	_bool	m_b1 = true;
	_bool	m_b2 = true;


	CWeapon*			m_pEquip[EQUIP_END];
	CWeapon*			m_pCurWeapon = nullptr;

	CWeapon*			m_pWeapon[WEAPON_TYPE::WEAPON_END];
	WEAPON_TYPE			eCurWeaponType;

	_vec3				m_fRecoverAngle;

	_float				m_iSpeedCnt = 0.f;

	/**************************/
	/*trans to character.h*/
//  	vector<CBone*>			m_vecBone;
// 	//CTransform*				m_pTransformCom = nullptr; /*Ref Bone[0] : do not release*/
// 	CCube_Color*			m_pBoneCubeBuffer = nullptr;
// 
// 	vector<BODYINFO*>		m_vecBodyInfo;
// 	CCube_Texture*			m_pBodyCubeBuffer = nullptr;
// 
// 	vector<vector<_float*>>	m_vvKeyFrameTime;
// 	
// 	/*frame*/
//  	_float	m_fFrameTimer = 0.f;
//  	_ulong	m_dwFrame = 0;
// 	
// 	_uint	m_iPreStateIndex = 0;
//  	_uint	m_iCurStateIndex = 0;
	CTexture*				m_pTextureCom = nullptr; /*temp*/


	// ----------------------------- Effect -----------------------------
	CTexture*				m_pSprintEffectTex = nullptr;
	_tchar*					m_pSprintEffectTexTag = nullptr;
	// ----------------------------- Effect -----------------------------

/*function*/
public:
	void	Update_Bone2(const _float& fTimeDelta);
	void	Render_Bone2();
	void	Render_Model();

private:
	virtual _bool Compare_AnimateType();
	_bool	Compare_FireState();

/*related ctor dtor*/
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGrahpicDev);
private:
	virtual _ulong Free() final;



};

