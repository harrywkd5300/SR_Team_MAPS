#pragma once
#include "Particle.h"

BEGIN(Engine)


class DLL_EXPORT CParticle_Effect final : public CParticle
{
public:
	explicit CParticle_Effect(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CParticle_Effect(const CParticle_Effect& rhs);
	virtual ~CParticle_Effect() = default;

	/*FUNCTION*/
public:
	HRESULT Ready_Particle_Effect();
	virtual void Reset_Particle(const Attribute& attribute);
	virtual void Update_Particle(const _float& fTimeDelta);
	void	ResetEffect(const _vec3 & vSrc, D3DXCOLOR effectColor, _float fLifeTime);
	/*MEMBER*/
private:

public:
	static	CParticle_Effect*	Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CComponent*	Clone();

private:
	virtual _ulong Free() final;
};

END