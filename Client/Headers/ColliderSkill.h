#pragma once

#include "LightEffectObj.h"
namespace Engine
{
	class CRect_Texture;
	class CRenderer;
	class CTexture;
	class CTransform;
	class CLight_Manager;
	class CEffect;
	class CEffect_Bullet;
	class CEffect_Manager;
	class CEffect_Fixed;
	class CAABB_Buffer;
}

class CCollider_Manager;

class CColliderSkill : public CLightEffectObj
{
public:
	enum RUN_TYPE{AABB,AREA,RUN_TYPE_END}; //발동 타입 (충돌형,지역형)
protected:
	explicit CColliderSkill(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CColliderSkill() = default;
public:
	virtual HRESULT Ready_GameObject(void) ;
	virtual _int Update_GameObject(const _float& fTimeDelta) ;
	virtual void Render_GameObject(void) ;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	
	bool isRun() { return bIsRun; }
	void Run_SKill() { bIsRun = true; }
	_float GetDamage() { return m_iDamage; }
	_float GetSkillArea() { return m_fSkillAttackArea; }
	_float GetCurRange() { return m_fCurRange; }
	_float GetMaxRange() { return m_fSkillRange; }
	CAABB_Buffer* Get_ColliderBox_AABB() { return m_pColliderBox_AABB; }
	CHAR_TYPE Get_CharType() { return eUserCharType; }
	RUN_TYPE Get_RunType() { return m_eRunType; }
	virtual void	Set_Info(_float fRange,_float fSkillAttackArea, _float fSpeed, 
		_int iDamage, _vec3 vDir, RUN_TYPE eRunType)
	{
		m_fSkillRange = fRange;
		m_fSpeed = fSpeed;
		m_iDamage = iDamage;
		m_vDir = vDir;
		m_eRunType = eRunType;
		m_fSkillAttackArea = fSkillAttackArea;
	}
protected:
	HRESULT Ready_Component(void);	
protected:
	CHAR_TYPE eUserCharType = CHAR_TYPE::CHAR_TYPE_MONSTER;
	RUN_TYPE m_eRunType = RUN_TYPE::AABB;
	_float m_fSkillRange = 0.f;
	_float m_fCurRange = 0.f;
	_float m_fSkillAttackArea = 0.f;
	_float m_fSpeed =0.f;
	_int m_iDamage =0;
	_vec3 m_vDir;
	bool bIsRun = false;
	CCollider_Manager*		m_pColliderMgr = nullptr;
	CAABB_Buffer*			m_pColliderBox_AABB = nullptr;
	
public:
	static CColliderSkill* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void);

};

