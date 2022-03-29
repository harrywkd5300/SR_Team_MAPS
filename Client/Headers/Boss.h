#pragma once

#include "Defines.h"
#include "Monster.h"


struct BOSSSKILL_INFO
{
	_float			fNormalAttack_Timer = 0.f;
	_float			fNormalAttack_CoolTime = 0.f;
	_float			fNormalAttack_Range = 0.f;
	_float			fNormalAttack_Scale = 0.f;

	_float			fSweepAttack_Timer = 0.f;
	_float			fSweepAttack_Cooldown = 0.f;
	_float			fSweepAttack_Range = 0.f;


	_float			fFireBall_Timer = 0.f;
	_float			fFireBall_Cooldown = 0.f;
	_float			fFireBall_Range = 0.f;
	_float			fFireBall_MinRange = 0.f;

	_float			fProjectile_Timer = 0.f;
	_float			fProjectile_Cooldown = 0.f;
	_float			fProjectile_Range = 0.f;


};
class CPlayer;

namespace Engine
{
	class CCamera_Manager;
}
class CWeapon;

class CBoss : public CMonster
{
	enum BONENUMBER {BONE_TRANSFORM,BONE_ARCFIREBALL = 3,BONE_RARM=8, BONE_NECK = 10, BONE_HEAD = 11, BONE_FIREBALL1=14, BONE_FIREBALL2=16};
	enum STATENUMBER { STATE_IDLE, STATE_SWEEP, STATE_NORMALATTACK, STATE_FIREBALL, STATE_INFERNO,STATE_DEAD};

private:
	explicit CBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss() = default;



/*FUNCTION*/
public:
	virtual HRESULT Ready_GameObject() { return NOERROR; } /*empty*/
	
	
	HRESULT Ready_Monster(const _vec3* vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject() final;

private:
	HRESULT			Ready_Component(void);
	HRESULT			Ready_Parameters(const _vec3* vPos);
	HRESULT			Ready_Path(void);
	HRESULT			Ready_Skill(void);

	void			Update_Timer(const _float& fTimeDelta);
	void			Update_Bone(const _float& fTimeDelta);
	void			Update_ExtraAnimate(const _float& fTimeDelta);
	void			Render_Model();

private:
	void			Set_MoveLoad(void);
	void			Set_MonsterLocal(void);
	void			Move_Monster(const _float & fTimeDelta);

	virtual _bool	Hit(_uint iDamage, const _float& fLifeTime);
	_bool			Compare_WalkState();
	void			LookAt(const _vec3& vPoint);
	void			Refresh_Target();


/*MEMBER*/
private:
	CCamera_Manager*				m_pCamera_Manager = nullptr;

	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CCube_Texture*			m_pBufferCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
	Engine::CTransform*				m_pTargetTransform = nullptr;
	Engine::CVIBuffer*				m_pTerrainBufferCom = nullptr;
	
	CWeapon*						m_pWeapon;
	CWeapon*						m_pWeaponFire;
private:
	_float			m_fSkillTime = 0.f;
	_float			m_fCount = 1.f;
	_bool			m_bHaveDoor = false;
	_bool			m_bInHouse = false;
	_bool			m_bLastDead = false;
	_vec3			m_vArrivePos;
	_vec3			m_vMakePos;
	_vec3			m_vDoorPos;
	CHARWAY			m_MoveArray[6];
	CHARWAY			m_PlayerArrIdx = NONE;
	_uint			m_iMoveIdx = 0;
	_uint			m_iStartPos = 0;
	_uint			m_iEndPos = 0;

	/*parameters*/
	_float			m_fMonsterSpeed = 0.f;
	_float			m_fMonsterSprintSpeed = 0.f;
	_float			m_fAttackRange = 0.f;
	_float			m_fRecognizeRange = 0.f;
	_float			m_fIncomingDeadTimer= 0.f;

	/*life time*/
	_float			m_fGetHitDelay = 0.f;
	_uint			m_iGetHitDamage = 0;

	_bool			m_bLookAt = true;
	CPlayer*		m_pPlayer = nullptr;


	/*Skill CoolTimer*/
	BOSSSKILL_INFO	m_tSkillInfo;

	vector<CAABB_Buffer*>	m_vecBossRightHand;
	// ---------------- Shader ----------------------
private:
	CShader*			m_pShaderCom = nullptr;
	CMaterial*			m_pMatCom = nullptr;

public:
	void SetShader(_uint iShaderNum);
	void StartShader(_uint iShaderNum);
	void EndShader(_uint iShaderNum);
	// ---------------- Shader ----------------------

public:
	static CBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vPos = nullptr);

protected:
	virtual _ulong Free(void) final;

};

