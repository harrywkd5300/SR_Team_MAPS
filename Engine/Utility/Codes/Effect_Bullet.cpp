#include "..\Headers\Effect_Bullet.h"
#include "WELL512a.h"
#include "Light.h"
#include "Light_Manager.h"

CEffect_Bullet::CEffect_Bullet(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CEffect(pGraphicDevice)
{
}

CEffect_Bullet::CEffect_Bullet(const CEffect_Bullet & rhs)
	: CEffect(rhs)
{

}

HRESULT CEffect_Bullet::Ready_Effect_Bullet()
{
	m_iVertexCount = 4;
	m_iVertexSize = sizeof(VTXTEX);
	m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	m_iIndexSize = sizeof(INDEX16);
	m_iTriCount = 2;
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		goto exception;

	if (nullptr == m_pVB || nullptr == m_pIB)
		goto exception;

	VTXTEX* pVertex = nullptr;
	m_pColliderBoxVertex = new _vec3[4];
	m_pVertexPos = new _vec3[4];


	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-1.f, 1.f, 0.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);
	m_pVertexPos[0] = pVertex[0].vPosition;

	pVertex[1].vPosition = _vec3(1.f, 1.f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);
	m_pVertexPos[1] = pVertex[1].vPosition;

	pVertex[2].vPosition = _vec3(1.f, -1.f, 0.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);
	m_pVertexPos[2] = pVertex[2].vPosition;

	pVertex[3].vPosition = _vec3(-1.f, -1.f, 0.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);
	m_pVertexPos[3] = pVertex[3].vPosition;

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return NO_ERROR;

exception:
	MSG_BOX("Effect_Bullet Ready Failed err:10154");
	return E_FAIL;
}

void CEffect_Bullet::Reset_Effect(const EFTINFO & attribute)
{
	m_tInfo = attribute;
}

void CEffect_Bullet::Update_Effect(const _float & fTimeDelta)
{
	//_matrix matWorld, matTrans, matScale;
	//
	//D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);
	//D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, m_tInfo.vScale.z);

	//matWorld = matScale * matTrans;

	m_tInfo.fFrame += m_tInfo.fFrameSpeed* fTimeDelta;

	if (m_tInfo.fFrame >= m_tInfo.fMaxFrame)
		m_tInfo.fFrame = 0.f;
	//m_tInfo.vScale *= 1+ fTimeDelta;
	//m_tInfo.vPos = m_tInfo.vPos + (m_tInfo.vLook) * fTimeDelta;	// p = p0 + 시간변화량*v'(속도변화량)		
	m_tInfo._age += fTimeDelta;

	//cout << "Scale: " << m_tInfo.vScale.x << m_tInfo.vScale.y << m_tInfo.vScale.z <<endl;
	if (m_tInfo._isLight = true)
	{
		if (m_pLight == nullptr || m_pLightMgr == nullptr)
		{
			Set_Light(m_tInfo._Lightcolor, m_tInfo._fLightRange);
		}
		m_pLight->SetUp_LightPosition(m_tInfo.vPos);
	}

	if (m_tInfo._age > m_tInfo._lifeTime) // kill 
		m_tInfo._isAlive = false;
}

void CEffect_Bullet::ResetBullet(const _vec3 & vSrc, const _vec3 & vDest,const _vec3& vScale, const _float & fSpeed)
{
	//ZeroMemory(&m_tInfo, sizeof(Attribute));

	_vec3 vDir = vDest - vSrc;
	_float fDist = D3DXVec3Length(&(vDir));

	D3DXVec3Normalize(&vDir, &vDir);
	
	//float fRandSpeed = fSpeed * CRandomWell512::GetInstance()->GetValueF(0.5f, 1.5f);

	vDir *= fSpeed;

	m_tInfo.vPos = vSrc;
	m_tInfo.vLook = vDir;
	m_tInfo.vScale = vScale;
	m_tInfo._lifeTime = fDist / (fSpeed);
	m_tInfo._age = 0.f;
	m_tInfo._isLight = false;
	m_tInfo._isAlive = true;
	//m_tInfo._color = D3DXCOLOR(0.94f, 0.64f, 0.4f, 1.0f);

}

CEffect_Bullet * CEffect_Bullet::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CEffect_Bullet* pInstance = new CEffect_Bullet(pGraphicDevice);

	if (FAILED(pInstance->Ready_Effect_Bullet()))
	{
		MSG_BOX("CEffect_Bullet creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CEffect_Bullet::Clone()
{
	return new CEffect_Bullet(*this);
}

_ulong CEffect_Bullet::Free()
{
	//_ulong dwRefCount = 0;

	//dwRefCount = CEffect::Free();

	//return dwRefCount;
	return CEffect::Free();
}
