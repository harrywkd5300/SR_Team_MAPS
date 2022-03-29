#include "..\Headers\AABB_Buffer.h"
#include "GameObject.h"


CAABB_Buffer::CAABB_Buffer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_vPos = _vec3(0.f, 0.f, 0.f);
	m_vScale = _vec3(1.f, 1.f, 1.f);
	
}

CAABB_Buffer::CAABB_Buffer(const CAABB_Buffer & rhs)
	: CVIBuffer(rhs),m_matWorld(rhs.m_matWorld), m_vPos(rhs.m_vPos),m_vScale(rhs.m_vScale) //,m_pIndex(rhs.m_pIndex)//, m_vScale(rhs.m_vScale)
{
	//if (m_pColliderBoxVertex != nullptr)
	//{
	m_pIndex = new INDEX16[rhs.m_iTriCount];
	for (int i = 0; i < rhs.m_iTriCount; ++i)
	{
		m_pIndex[i] = rhs.m_pIndex[i];
	}

	m_pColliderBoxVertex = new _vec3[rhs.m_iVertexCount];
	for (int i = 0; i < rhs.m_iVertexCount; ++i)
	{
		m_pColliderBoxVertex[i] = rhs.m_pVertexPos[i];
	}
	//}
}

HRESULT CAABB_Buffer::Ready_Buffer(const _float& fSizeX, const _float& fSizeY, const _float& fSizeZ)
{

	m_iVertexCount = 8;
	m_iVertexSize = sizeof(VTXCOLLIDE);
	m_dwVertexFVF = VTXCOLLIDE::FVF;

	m_iIndexSize = sizeof(INDEX16);
	m_iTriCount = 12;
	m_IndexFmt = D3DFMT_INDEX16;

	m_vScale.x = fSizeX;
	m_vScale.y = fSizeY;
	m_vScale.z = fSizeZ;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		goto exception;

	if (nullptr == m_pVB || nullptr == m_pIB)
		goto exception;

	_uint VertexBufferSize = 8;
	_uint IndexBufferSize = 12;
	m_iIndexSize = 12;
	_uint iIndex = 0;

	const _float x = 0.5f;//* fSizeX;
	const _float y = 0.5f;//* fSizeY;
	const _float z = 0.5f;//* fSizeZ;
	
	m_pVertexPos = new _vec3[VertexBufferSize];
	m_pColliderBoxVertex = new _vec3[VertexBufferSize];
	VTXCOLLIDE* pVertex = new VTXCOLLIDE[VertexBufferSize];

	pVertex[0].vPosition = _vec3(-x, y, -z);
	m_pVertexPos[0] = pVertex[0].vPosition;

	pVertex[1].vPosition = _vec3(x, y, -z);
	m_pVertexPos[1] = pVertex[1].vPosition;

	pVertex[2].vPosition = _vec3(x, -y, -z);
	m_pVertexPos[2] = pVertex[2].vPosition;

	pVertex[3].vPosition = _vec3(-x, -y, -z);
	m_pVertexPos[3] = pVertex[3].vPosition;

	pVertex[4].vPosition = _vec3(-x, y, z);
	m_pVertexPos[4] = pVertex[4].vPosition;

	pVertex[5].vPosition = _vec3(x, y, z);
	m_pVertexPos[5] = pVertex[5].vPosition;

	pVertex[6].vPosition = _vec3(x, -y, z);
	m_pVertexPos[6] = pVertex[6].vPosition;

	pVertex[7].vPosition = _vec3(-x, -y, z);
	m_pVertexPos[7] = pVertex[7].vPosition;

	m_pIndex = new INDEX16[IndexBufferSize];

	//+x
	m_pIndex[0]._0 = 1; m_pIndex[0]._1 = 5; m_pIndex[0]._2 = 6;
	m_pIndex[1]._0 = 1; m_pIndex[1]._1 = 6; m_pIndex[1]._2 = 2;
	//-x
	m_pIndex[2]._0 = 4; m_pIndex[2]._1 = 0; m_pIndex[2]._2 = 3;
	m_pIndex[3]._0 = 4; m_pIndex[3]._1 = 3; m_pIndex[3]._2 = 7;

	//+y
	m_pIndex[4]._0 = 4; m_pIndex[4]._1 = 5; m_pIndex[4]._2 = 1;
	m_pIndex[5]._0 = 4; m_pIndex[5]._1 = 1; m_pIndex[5]._2 = 0;

	//-y
	m_pIndex[6]._0 = 3; m_pIndex[6]._1 = 2; m_pIndex[6]._2 = 6;
	m_pIndex[7]._0 = 3; m_pIndex[7]._1 = 6; m_pIndex[7]._2 = 7;

	//+z
	m_pIndex[8]._0 = 5; m_pIndex[8]._1 = 4; m_pIndex[8]._2 = 7;
	m_pIndex[9]._0 = 5; m_pIndex[9]._1 = 7; m_pIndex[9]._2 = 6;

	//-z
	m_pIndex[10]._0 = 0; m_pIndex[10]._1 = 1; m_pIndex[10]._2 = 2;
	m_pIndex[11]._0 = 0; m_pIndex[11]._1 = 2; m_pIndex[11]._2 = 3;


	VTXCOLLIDE*			pVertexBuffer = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertexBuffer, 0);
	memcpy(pVertexBuffer, pVertex, sizeof(VTXCOLLIDE)*VertexBufferSize);
	m_pVB->Unlock();

	INDEX16*			m_pIndexBuffer = nullptr;
	m_pIB->Lock(0, 0, (void**)&m_pIndexBuffer, 0);
	memcpy(m_pIndexBuffer, m_pIndex, sizeof(INDEX16)*IndexBufferSize);
	m_pIB->Unlock();

	Engine::Safe_Delete_Array(pVertex);
	//Engine::Safe_Delete_Array(m_pIndex);

	return NO_ERROR;

exception:
	MSG_BOX("CAABB_Buffer Ready Failed");
	return E_FAIL;
}

void CAABB_Buffer::SetTarget(CGameObject * pTarget)
{
	if (pTarget == nullptr)
	{
		m_pTarget = pTarget;
	}
}

void CAABB_Buffer::Set_Size(const _float & fSizeX, const _float & fSizeY, const _float & fSizeZ)
{
	m_vScale.x = fSizeX;
	m_vScale.y = fSizeY;
	m_vScale.z = fSizeZ;

	//VTXCOLLIDE* pVertex = nullptr;

	//const _float x = 0.5f* fSizeX;
	//const _float y = 0.5f* fSizeY;
	//const _float z = 0.5f* fSizeZ;


	//m_pVB->Lock(0, 0, (void**)&pVertex, 0);


	//pVertex[0].vPosition = _vec3(-x, y, -z);
	//m_pVertexPos[0] = pVertex[0].vPosition;

	//pVertex[1].vPosition = _vec3(x, y, -z);
	//m_pVertexPos[1] = pVertex[1].vPosition;

	//pVertex[2].vPosition = _vec3(x, -y, -z);
	//m_pVertexPos[2] = pVertex[2].vPosition;

	//pVertex[3].vPosition = _vec3(-x, -y, -z);
	//m_pVertexPos[3] = pVertex[3].vPosition;

	//pVertex[4].vPosition = _vec3(-x, y, z);
	//m_pVertexPos[4] = pVertex[4].vPosition;

	//pVertex[5].vPosition = _vec3(x, y, z);
	//m_pVertexPos[5] = pVertex[5].vPosition;

	//pVertex[6].vPosition = _vec3(x, -y, z);
	//m_pVertexPos[6] = pVertex[6].vPosition;

	//pVertex[7].vPosition = _vec3(-x, -y, z);
	//m_pVertexPos[7] = pVertex[7].vPosition;


	//m_pVB->Unlock();
}

void CAABB_Buffer::Set_Position(_vec3 vPos)
{
	m_vPos = vPos;
}

void CAABB_Buffer::Set_Scale(_vec3 vScale)
{
	m_vScale = vScale;
}

void CAABB_Buffer::Update_ColliderBox_VertexBuffer()
{
	_matrix matTrans, matScale;
	//D3DXMatrixIdentity(&matWorld, &matWorld);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	m_matWorld = matScale * matTrans;

	for (int i = 0; i < m_iVertexCount; ++i)
	{
		D3DXVec3TransformCoord(&m_pColliderBoxVertex[i], &m_pVertexPos[i], &m_matWorld);
	}
}

//void CAABB_Buffer::Set_Position(const _matrix &matWorld)
//{
//	if (m_pColliderBoxVertex == nullptr)
//		return;
//
//	_matrix matScale;
//	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
//
//	m_matWorld = matScale * matWorld;
//
//	for (int i = 0; i < m_iVertexCount; ++i)
//	{
//		D3DXVec3TransformCoord(&m_pColliderBoxVertex[i], &m_pVertexPos[i], &matResult);
//	}
//}

void CAABB_Buffer::Render_Debug_Buffer()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_iVertexSize);
	m_pGraphicDev->SetFVF(m_dwVertexFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

INDEX16 * CAABB_Buffer::Get_IndexBuffer()
{
	return m_pIndex;
}

CAABB_Buffer * CAABB_Buffer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fSizeX, const _float& fSizeY, const _float& fSizeZ)
{
	CAABB_Buffer* pInstance = new CAABB_Buffer(pGraphicDev);
	if (FAILED(pInstance->Ready_Buffer(fSizeX,fSizeY,fSizeZ)))
	{
		MSG_BOX("CAABB_Buffer creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CAABB_Buffer::Clone()
{
	return new CAABB_Buffer(*this);
}

_ulong CAABB_Buffer::Free()
{
	//Safe_Delete_Array(m_pColliderBoxVertex);
	Engine::Safe_Delete_Array(m_pIndex);
	return CVIBuffer::Free();
}
