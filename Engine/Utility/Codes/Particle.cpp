#include "..\Headers\Particle.h"
#include "Texture.h"

CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice)
{
	ZeroMemory(&m_tInfo, sizeof(Attribute));
	m_tInfo._isAlive = false;
}

CParticle::CParticle(const CParticle& rhs)
	: CComponent(rhs), m_tInfo(rhs.m_tInfo)
{

}


void CParticle::Clone_Particle(const CParticle * pCopyParticle)
{
	if (pCopyParticle == nullptr)
	{
		return;
	}
	m_tInfo._position = pCopyParticle->m_tInfo._position;
	m_tInfo._velocity = pCopyParticle->m_tInfo._velocity;
	m_tInfo._acceleration = pCopyParticle->m_tInfo._acceleration;
	m_tInfo._lifeTime = pCopyParticle->m_tInfo._lifeTime;
	m_tInfo._age = pCopyParticle->m_tInfo._age;
	m_tInfo._color = pCopyParticle->m_tInfo._color;
	m_tInfo._colorFade = pCopyParticle->m_tInfo._colorFade;
	m_tInfo._isAlive = pCopyParticle->m_tInfo._isAlive;

	//cout << "vPos: " << m_tInfo._position.x << '/' << m_tInfo._position.y << '/' << m_tInfo._position.z << '/' << endl;
	//cout << "vDest: " << m_tInfo._velocity.x << '/' << m_tInfo._velocity.y << '/' << m_tInfo._velocity.z << '/' << endl;
	//cout << "Time: " << m_tInfo._lifeTime << endl;

}


//CParticle * CParticle::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CTexture* ParticleTex)
//{
//
//	CParticle* pInstance = new CParticle(pGraphicDevice);
//
//	if (FAILED(pInstance->Ready_Particle(ParticleTex)))
//	{
//		MSG_BOX("CParticle creation failed");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

//CComponent * CParticle::Clone()
//{
//	AddRef();
//	return this;
//}

_ulong CParticle::Free()
{
	_ulong dwRefCount = 0;

	dwRefCount = CComponent::Free();

	return dwRefCount;
}

