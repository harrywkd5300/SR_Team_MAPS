#include "..\Headers\Billboard_Buffer.h"
#include "GameObject.h"


CBillboard_Buffer::CBillboard_Buffer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CBillboard_Buffer::CBillboard_Buffer(const CBillboard_Buffer & rhs)
	: CVIBuffer(rhs)//, m_vScale(rhs.m_vScale)
{
}



HRESULT CBillboard_Buffer::Ready_Buffer(const _float & fSizeX, const _float & fSizeY, const _float & fSizeZ)
{
	m_iVertexCount = 4;
	m_iVertexSize = sizeof(VTXCOLLIDE);
	m_dwVertexFVF = VTXCOLLIDE::FVF;

	m_iIndexSize = sizeof(INDEX16);
	m_iTriCount = 2;
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		goto exception;

	if (nullptr == m_pVB || nullptr == m_pIB)
		goto exception;

	VTXCOLLIDE* pVertex = nullptr;

	const _float x = 0.5f* fSizeX;
	const _float y = 0.5f* fSizeY;
	const _float z = 0.5f* fSizeZ;
	m_pVertexPos = new _vec3[m_iVertexCount];

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-x, y, 0.f);
	m_pVertexPos[0] = pVertex[0].vPosition;

	pVertex[1].vPosition = _vec3(x, y, 0.f);
	m_pVertexPos[1] = pVertex[1].vPosition;

	pVertex[2].vPosition = _vec3(x, -y, 0.f);
	m_pVertexPos[2] = pVertex[2].vPosition;

	pVertex[3].vPosition = _vec3(-x, -y, 0.f);
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
	MSG_BOX("CBillboard_Buffer Ready Failed");
	return E_FAIL;
}

void CBillboard_Buffer::SetTarget(CGameObject * pTarget)
{
	if (pTarget == nullptr)
	{
		m_pTarget = pTarget;
	}
}

void CBillboard_Buffer::Set_Size(const _float & fSizeX, const _float & fSizeY, const _float & fSizeZ)
{
	VTXCOLLIDE* pVertex = nullptr;

	const _float x = 0.5f* fSizeX;
	const _float y = 0.5f* fSizeY;
	const _float z = 0.5f* fSizeZ;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-x, y, 0.f);
	m_pVertexPos[0] = pVertex[0].vPosition;

	pVertex[1].vPosition = _vec3(x, y, 0.f);
	m_pVertexPos[1] = pVertex[1].vPosition;

	pVertex[2].vPosition = _vec3(x, -y, 0.f);
	m_pVertexPos[2] = pVertex[2].vPosition;

	pVertex[3].vPosition = _vec3(-x, -y, 0.f);
	m_pVertexPos[3] = pVertex[3].vPosition;

	m_pVB->Unlock();
}

void CBillboard_Buffer::Set_Position(const _matrix & matWorld)
{
	for (int i = 0; i < m_iVertexCount; ++i)
	{
		D3DXVec3TransformCoord(&m_pVertexPos[i], &m_pVertexPos[i], &matWorld);
	}
}


CBillboard_Buffer * CBillboard_Buffer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float & fSizeX, const _float & fSizeY, const _float & fSizeZ)
{
	CBillboard_Buffer* pInstance = new CBillboard_Buffer(pGraphicDev);
	if (FAILED(pInstance->Ready_Buffer(fSizeX, fSizeY, fSizeZ)))
	{
		MSG_BOX("CBillboard_Buffer creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CBillboard_Buffer::Clone()
{
	return new CBillboard_Buffer(*this);
}

_ulong CBillboard_Buffer::Free()
{
	return CVIBuffer::Free();
}
