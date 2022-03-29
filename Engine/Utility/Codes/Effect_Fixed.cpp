#include "..\Headers\Effect_Fixed.h"
#include "WELL512a.h"
#include "Light.h"
#include "Light_Manager.h"
#include "Transform.h"
CEffect_Fixed::CEffect_Fixed(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CEffect(pGraphicDevice), m_pTarget(nullptr)
{
}

CEffect_Fixed::CEffect_Fixed(const CEffect_Fixed & rhs)
	: CEffect(rhs),m_eType(rhs.m_eType), m_pTarget(rhs.m_pTarget)
{

}

HRESULT CEffect_Fixed::Ready_Effect_Fixed()
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
	MSG_BOX("Effect_Fixed Ready Failed err:10154");
	return E_FAIL;
}

void CEffect_Fixed::Reset_Effect(const EFTINFO & attribute)
{
	m_tInfo = attribute;
}

void CEffect_Fixed::Update_Effect(const _float & fTimeDelta)
{

	m_tInfo.fFrame += m_tInfo.fFrameSpeed* fTimeDelta;

	if (m_tInfo.fFrame >= m_tInfo.fMaxFrame)
		m_tInfo.fFrame = 0.f;
	

	if (m_pTarget != nullptr)
	{
		m_tInfo.vPos = m_pTarget->Get_Position();
	}

	m_tInfo._age += fTimeDelta;
	

	if (m_tInfo._isLight == true)
	{
		if (m_pLight == nullptr || m_pLightMgr == nullptr)
		{
			Set_Light(m_tInfo._Lightcolor, m_tInfo._fLightRange);
		}
		m_pLight->SetUp_LightPosition(m_tInfo.vPos);
		m_pLightMgr->Add_RenderLight(m_pLight, LIGHT_TYPE::POINT_LIGHT);
	}

	if (m_tInfo._age > m_tInfo._lifeTime) // kill 
		m_tInfo._isAlive = false;
}
//void CEffect_Fixed::Clone_Effect(const CEffect * pCopyEffect)
//{
//
//}
//
//void CEffect_Fixed::Clone_Effect(CEffect * pCopyEffect)
//{
//	CEffect::Clone_Effect(pCopyEffect);
//	m_pLight = pCopyEffect->Get_Light();
//	if (m_pLight != nullptr)
//	{
//		m_pLight->AddRef();
//	}
//	m_pLightMgr = pCopyEffect->Get_LightManager();
//	if (m_pLightMgr != nullptr)
//	{
//		m_pLightMgr->AddRef();
//	}
//	m_eType = dynamic_cast<CEffect_Fixed*>(pCopyEffect)->m_eType;
//	m_pTarget = dynamic_cast<CEffect_Fixed*>(pCopyEffect)->m_pTarget;
//}

void CEffect_Fixed::ResetEffect(const _vec3 & vSrc, const _vec3 & vScale, FIXED_TYPE eType, _float fLifeTime, CTransform* pTarget)
{
	//ZeroMemory(&m_tInfo, sizeof(Attribute));


	if (pTarget != nullptr)
	{
		m_pTarget = pTarget;
	}
	m_tInfo.vPos = vSrc;
	m_tInfo.vScale = vScale;
	m_tInfo._lifeTime = fLifeTime;
	m_tInfo._age = 0.f;
	//m_tInfo._isLight = false;
	m_tInfo._isAlive = true;

	m_eType = eType;
	//m_tInfo._color = D3DXCOLOR(0.94f, 0.64f, 0.4f, 1.0f);
}


CEffect_Fixed * CEffect_Fixed::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CEffect_Fixed* pInstance = new CEffect_Fixed(pGraphicDevice);

	if (FAILED(pInstance->Ready_Effect_Fixed()))
	{
		MSG_BOX("CEffect_Fixed creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CEffect_Fixed::Clone()
{
	return new CEffect_Fixed(*this);
}

_ulong CEffect_Fixed::Free()
{
	//_ulong dwRefCount = 0;

	//dwRefCount = CEffect::Free();

	//return dwRefCount;
	return CEffect::Free();
}
