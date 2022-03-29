#pragma once
#include "Particle.h"

BEGIN(Engine)


class DLL_EXPORT CParticle_Bullet final : public CParticle
{
public:
	explicit CParticle_Bullet(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CParticle_Bullet(const CParticle_Bullet& rhs);
	virtual ~CParticle_Bullet() = default;

	/*FUNCTION*/
public:
	HRESULT Ready_Particle_Bullet();
	virtual void Reset_Particle(const Attribute& attribute);
	virtual void Update_Particle(const _float& fTimeDelta);
	void	ResetBullet(const _vec3& vSrc,const _vec3& vDest, const _float &fSpeed);
	/*MEMBER*/
private:

public:
	static	CParticle_Bullet*	Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CComponent*	Clone();

private:
	virtual _ulong Free() final;
};

END