#pragma once

#include "Monster.h"


class CWeapon;
class CGhost final : public CMonster
{
	enum BONENUMBER { BONE_TRANSFORM, BONE_TWOARMS, BONE_LLEG, BONE_LKNEE, BONE_RLEG, BONE_RKNEE, BONE_UPPER, BONE_LARM, BONE_LELBOW, BONE_RARM, BONE_RELBOW, BONE_HEAD = 11, BONE_HAND, BONE_EYE };
	enum STATENUMBER { STATE_IDLE, STATE_WALK, STATE_ATTACK, STATE_GETHIT, STATE_DEAD };
private:
	explicit CGhost(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGhost() = default;

public:
	virtual	HRESULT	Ready_GameObject() { return NOERROR; };/*empty*/

	HRESULT	Ready_Monster(const _vec3* vPos);

	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject() final;

	virtual _bool	Hit(_uint iDamage, const _float& fLifeTime);

private:
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CCube_Texture*			m_pBufferCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
	Engine::CTransform*				m_pTargetBuffer = nullptr;
	Engine::CVIBuffer*				m_pTerrainBufferCom = nullptr;
	// ---------------- Shader ----------------------
private:
	CShader*			m_pShaderCom = nullptr;
	CMaterial*			m_pMatCom = nullptr;

public:
	void SetShader(_uint iShaderNum);
	void StartShader(_uint iShaderNum);
	void EndShader(_uint iShaderNum);
	// ---------------- Shader ----------------------

private:
	_float				m_fMonsterSpeed = 0.f;
	_float				m_fSkillTime = 0.f;
	_float				m_fCount = 1.f;
	/*life time*/
	_float			m_fGetHitDelay = 0.f;
	_uint			m_iGetHitDamage = 0;


	CWeapon*			m_pWeapon;

private:
	HRESULT			Ready_Component();
	HRESULT			Ready_Parameters(const _vec3* vPos = nullptr);
	HRESULT			Ready_Skill(void);
private:
	void			Move_Monster(const _float & fTimeDelta);

public:
	static CGhost* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vPos);
protected:
	virtual _ulong Free(void) final;

};

