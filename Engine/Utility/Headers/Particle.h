#pragma once

#include "Component.h"

BEGIN(Engine)

class CTexture;

class DLL_EXPORT CParticle
	:public CComponent
{

protected:
	explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CParticle(const CParticle& rhs);
	virtual ~CParticle()=default;

/*FUNCTION*/
public:
	//HRESULT Ready_Particle();
	virtual void Reset_Particle(const Attribute& attribute) = 0;
	virtual void Update_Particle(const _float& fTimeDelta) = 0;
	virtual void Clone_Particle(const CParticle* pCopyParticle);
	const Attribute& Get_Info() { return m_tInfo; }
	inline bool IsAlive() { return m_tInfo._isAlive; }

/*MEMBER*/
protected:
	Attribute					m_tInfo;

/* related ctor&dtor */
public:
	virtual CComponent*	Clone() = 0;

protected:
	virtual _ulong Free();
};

END