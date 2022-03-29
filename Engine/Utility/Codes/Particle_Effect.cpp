#include "..\Headers\Particle_Effect.h"
#include "WELL512a.h"

CParticle_Effect::CParticle_Effect(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CParticle(pGraphicDevice)
{
}

CParticle_Effect::CParticle_Effect(const CParticle_Effect & rhs)
	: CParticle(rhs)
{

}

HRESULT CParticle_Effect::Ready_Particle_Effect()
{

	return NOERROR;
}

void CParticle_Effect::Reset_Particle(const Attribute & attribute)
{
	m_tInfo = attribute;
}

void CParticle_Effect::Update_Particle(const _float & fTimeDelta)
{
	//m_tInfo._position = m_tInfo._position + (m_tInfo._velocity) * fTimeDelta;	// p = p0 + 시간변화량*v'(속도변화량)		
	m_tInfo._age += fTimeDelta;

	if (m_tInfo._age > m_tInfo._lifeTime) // kill 
		m_tInfo._isAlive = false;
}

void CParticle_Effect::ResetEffect(const _vec3 & vSrc, D3DXCOLOR effectColor,_float fLifeTime)
{
	ZeroMemory(&m_tInfo, sizeof(Attribute));


	m_tInfo._position = vSrc;
	m_tInfo._lifeTime = fLifeTime;
	m_tInfo._age = 0.f;
	m_tInfo._isAlive = true;
	m_tInfo._color = effectColor;

	//cout << "vPos: " << m_tInfo._position.x << '/' << m_tInfo._position.y << '/' << m_tInfo._position.z << '/' << endl;
	//cout << "vDest: " << m_tInfo._velocity.x << '/' << m_tInfo._velocity.y << '/' << m_tInfo._velocity.z << '/' << endl;
	//cout << "Time: " << m_tInfo._lifeTime << endl;

}

CParticle_Effect * CParticle_Effect::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CParticle_Effect* pInstance = new CParticle_Effect(pGraphicDevice);

	if (FAILED(pInstance->Ready_Particle_Effect()))
	{
		MSG_BOX("CParticle_Effect creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CParticle_Effect::Clone()
{
	AddRef();
	return this;
	//return CParticle_Effect::Create(m_pGraphicDev);
}

_ulong CParticle_Effect::Free()
{
	_ulong dwRefCount = 0;

	dwRefCount = CParticle::Free();

	return dwRefCount;
}
