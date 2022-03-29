#pragma once
#include "Effect.h"

BEGIN(Engine)


class DLL_EXPORT CEffect_Bullet final : public CEffect
{
public:
	explicit CEffect_Bullet(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CEffect_Bullet(const CEffect_Bullet& rhs);
	virtual ~CEffect_Bullet() = default;

	/*FUNCTION*/
public:
	HRESULT Ready_Effect_Bullet();
	virtual void Reset_Effect(const EFTINFO& attribute);
	virtual void Update_Effect(const _float& fTimeDelta);
	void	ResetBullet(const _vec3& vSrc,const _vec3& vDest, const _vec3& vScale, const _float &fSpeed);
	/*MEMBER*/
private:

public:
	static	CEffect_Bullet*	Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CComponent*	Clone();

private:
	virtual _ulong Free() final;
};

END