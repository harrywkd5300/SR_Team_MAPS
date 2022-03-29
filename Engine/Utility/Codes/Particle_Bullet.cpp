#include "..\Headers\Particle_Bullet.h"
#include "WELL512a.h"

CParticle_Bullet::CParticle_Bullet(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CParticle(pGraphicDevice)
{
}

CParticle_Bullet::CParticle_Bullet(const CParticle_Bullet & rhs)
	: CParticle(rhs)
{

}

HRESULT CParticle_Bullet::Ready_Particle_Bullet()
{

	return NOERROR;
}

void CParticle_Bullet::Reset_Particle(const Attribute & attribute)
{
	m_tInfo = attribute;
}

void CParticle_Bullet::Update_Particle(const _float & fTimeDelta)
{
	m_tInfo._position = m_tInfo._position + (m_tInfo._velocity) * fTimeDelta;	// p = p0 + 시간변화량*v'(속도변화량)		
	m_tInfo._age += fTimeDelta;

	if (m_tInfo._age > m_tInfo._lifeTime) // kill 
		m_tInfo._isAlive = false;
}

void CParticle_Bullet::ResetBullet(const _vec3 & vSrc, const _vec3 & vDest, const _float & fSpeed)
{
	ZeroMemory(&m_tInfo, sizeof(Attribute));

	_vec3 vDir = vDest - vSrc;
	_float fDist = D3DXVec3Length(&(vDir));

	D3DXVec3Normalize(&vDir, &vDir);
	
	float fRandSpeed = fSpeed * CRandomWell512::GetInstance()->GetValueF(0.5f, 1.5f);
	//fRandAcc += 0.05;
	//vDir *= fSpeed * fRandAcc;
	vDir *= fRandSpeed;
	//vDir *= fSpeed * cosf(D3DXToRadian((rand())));
	


	m_tInfo._position = vSrc;
	m_tInfo._velocity = vDir;
	m_tInfo._lifeTime = fDist / (fRandSpeed);
	m_tInfo._age = 0.f;
	m_tInfo._isAlive = true;
	m_tInfo._color = D3DXCOLOR(0.94f, 0.64f, 0.4f, 1.0f);

	//cout << "vPos: " << m_tInfo._position.x << '/' << m_tInfo._position.y << '/' << m_tInfo._position.z << '/' << endl;
	//cout << "vDest: " << m_tInfo._velocity.x << '/' << m_tInfo._velocity.y << '/' << m_tInfo._velocity.z << '/' << endl;
	//cout << "Time: " << m_tInfo._lifeTime << endl;

}

CParticle_Bullet * CParticle_Bullet::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CParticle_Bullet* pInstance = new CParticle_Bullet(pGraphicDevice);

	if (FAILED(pInstance->Ready_Particle_Bullet()))
	{
		MSG_BOX("CParticle_Bullet creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CParticle_Bullet::Clone()
{
	AddRef();
	return this;
	//return CParticle_Bullet::Create(m_pGraphicDev);
}

_ulong CParticle_Bullet::Free()
{
	_ulong dwRefCount = 0;

	dwRefCount = CParticle::Free();

	return dwRefCount;
}
