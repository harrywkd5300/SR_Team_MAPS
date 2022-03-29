#pragma once

#include "Weapon.h"

namespace Engine
{
	class CRect_Texture;
	class CRenderer;
	class CTexture;
	class CTransform;
	class CRandomWell512;
	class CParticle_Renderer;
	class CParticle_Effect;
	class CParticle_Bullet;
	class CEffect;
	class CEffect_Bullet;
	class CEffect_Fixed;
	class CEffect_Manager;
	class CPicking;
	class CTransform;
}


class CFireball  : public CWeapon
{
protected:
	explicit CFireball(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireball() = default;
public:
	virtual HRESULT Ready_GameObject(void) ;
	//virtual _int Update_GameObject(const _float& fTimeDelta, _vec3* vRecoverAngle) ;
	//virtual void Render_GameObject(void) ;

public:
	//void	Reload_Weapon();
	//virtual void	Use_Weapon(const _float fTimeDelta);
	virtual void	Use_Weapon_Monster(const _float fTimeDelta, const _vec3& vSourPos, const _vec3& vDestPos);

protected:
	HRESULT Ready_Component(void);	
public:
	static CFireball* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;

};

