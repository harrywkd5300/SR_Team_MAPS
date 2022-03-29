#pragma once
#include "Effect.h"

BEGIN(Engine)

class CTransform;


class DLL_EXPORT CEffect_Fixed final : public CEffect
{
public:
	enum FIXED_TYPE{TYPE_INFINITY,TYPE_TIMER,FIXED_TYPE_END};
public:
	explicit CEffect_Fixed(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CEffect_Fixed(const CEffect_Fixed& rhs);
	virtual ~CEffect_Fixed() = default;

	/*FUNCTION*/
public:
	HRESULT Ready_Effect_Fixed();
	virtual void Reset_Effect(const EFTINFO& attribute);
	virtual void Update_Effect(const _float& fTimeDelta);
	//virtual void Clone_Effect(CEffect* pCopyEffect);
	void	ResetEffect(const _vec3& vSrc, const _vec3& vScale, FIXED_TYPE eType, _float fLifeTime = 0.f, CTransform* pTarget = nullptr);
	void	SetType(FIXED_TYPE eType) { m_eType = eType; }
	/*MEMBER*/
private:
	FIXED_TYPE m_eType = TYPE_INFINITY;
	CTransform* m_pTarget;
public:
	static	CEffect_Fixed*	Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CComponent*	Clone();

private:
	virtual _ulong Free() final;
};

END