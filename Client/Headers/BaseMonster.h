#pragma once

#include "defines.h"
#include "Monster.h"

namespace Engine
{
	class CRandomWell512;
}

class CBaseMonster : public CMonster
{
protected:
	virtual _bool Compare_AnimateType() { return false; };
public:
	enum BONENUMBER { BONE_TRANSFORM, BONE_TWOARMS, BONE_LLEG, BONE_LKNEE, BONE_RLEG, BONE_RKNEE, BONE_UPPER, BONE_LARM, BONE_LELBOW, BONE_RARM, BONE_RELBOW, BONE_HEAD = 11, BONE_HAND, BONE_EYE };
	enum STATENUMBER{STATE_IDLE,STATE_WALK,STATE_ATTACK,STATE_GETHIT,STATE_DEAD};

private:
	explicit CBaseMonster(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 vMakePos);
	virtual ~CBaseMonster() = default;

/*FUNCITON*/
public:
	void			Set_MonsterLocal();

public:
	virtual HRESULT Ready_GameObject() final;
	virtual _int	Update_GameObject(const _float& fTimeDelta) final;
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject() final;

private:
	HRESULT			Ready_Path();
	HRESULT			Ready_Component();
	HRESULT			Ready_Parameters();

	void			Update_Bone(const _float& fTimeDelta);
	void			Update_ExtraAnimate(const _float& fTimeDelta);
	void			Render_Model();
	
	void			Refresh_Target();
	
	virtual _bool	Hit(_uint iDamage, const _float& fLifeTime);

	//virtual _bool	Compare_AnimateType() { return true; };

/*MEMBER*/
private:
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CCube_Texture*			m_pBufferCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
	Engine::CVIBuffer*				m_pTerrainBufferCom = nullptr;
	CGameObject*					m_pPlayer = nullptr;
	//CShader*						m_pShaderCom = nullptr;
	CTransform*						m_pTargetTransform = nullptr; /*Target Transform*/
	CRandomWell512*					m_pWELL512 = nullptr;
	// ---------------- Shader ----------------------
private:
	CShader*			m_pShaderCom = nullptr;
	CMaterial*			m_pMatCom = nullptr;

public:
	void SetShader(_uint iShaderNum);
	void StartShader(_uint iShaderNum);
	void EndShader(_uint iShaderNum);
	// ---------------- Shader ----------------------
	/*Ref Component*/

private:
	_bool			m_bHaveDoor = false;
	_bool			m_bInHouse = false;
	_vec3			m_vArrivePos ;
	_vec3			m_vMakePos;
	_vec3			m_vDoorPos;
	CHARWAY			m_MoveArray[6];
	CHARWAY			m_PlayerArrIdx = NONE;
	_uint			m_iMoveIdx = 0;
	_uint			m_iStartPos = 0;
	_uint			m_iEndPos = 0;

/*parameter*/
	_float			m_fMonsterSpeed = 0.f;
	_float			m_fAttackRange = 0.f;

/*life time*/
	_float			m_fGetHitDelay = 0.f;
	_uint			m_iGetHitDamage = 0;

	/*speach time*/
	_float			m_fSpeachTime = 0.f;
	_bool			m_bAttack = true;

private:
	void			Move_Monster(const _float & fTimeDelta);
	void			Set_MoveLoad();

public:
	static CBaseMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 vMakePos);

protected:
	virtual _ulong Free(void) final;
};

