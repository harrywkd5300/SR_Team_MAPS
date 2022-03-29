#pragma once

#include "ColliderSkill.h"

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

class CFireWeaponSkill : public CColliderSkill
{

private:
	explicit CFireWeaponSkill(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireWeaponSkill() = default;
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta) ;
	virtual void Render_GameObject(void) ;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	void Set_Info(_vec3 vDest,_vec3 vSour, _float fTime , _float fSpeed, _float AttackArea, int Damage) 
	{ 
		m_vDest = vDest;
		//m_vDest.x += CRandomWell512::GetInstance()->GetValueF(-5.f,5.f);
		//m_vDest.z += CRandomWell512::GetInstance()->GetValueF(-5.f,5.f);
		m_vSour = vSour;
		m_fTime = fTime;
		//m_vDir = _vec3(5.f, 5.f, 0.f);
		m_vDir = vDest - vSour;
		m_fSpeed = fSpeed;
		m_fSkillAttackArea = AttackArea;
		m_iDamage = Damage;
		//m_fSpeed = D3DXVec3Length(&vDir);
		//m_fCurTime = 0.f;
	}
protected:
	HRESULT Ready_Component(void);	
protected:
	_vec3 m_vDest;
	_vec3 m_vSour;
	_float m_fTime =0.f;
	_float m_fSpeed = 0.f;
	_float m_fCurTime = 0.f;
public:
	static CFireWeaponSkill* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void);

};

