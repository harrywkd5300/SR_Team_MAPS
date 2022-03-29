#include "..\Headers\VIBuffer.h"



CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CResources(pGraphicDev), m_pVB(nullptr), m_iVertexCount(0), m_iVertexSize(0), m_dwVertexFVF(0)
	, m_iIndexSize(0), m_IndexFmt(D3DFMT_UNKNOWN), m_iTriCount(0)
	, m_pVertexPos(nullptr), m_iVertexCountX(0), m_iVertexCountZ(0), m_fVertexItv(0.f), m_pColliderBoxVertex(nullptr)
{
	
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CResources(rhs), m_iVertexCount(rhs.m_iVertexCount), m_iVertexSize(rhs.m_iVertexSize), m_dwVertexFVF(rhs.m_dwVertexFVF)
	, m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB), m_iIndexSize(rhs.m_iIndexSize), m_IndexFmt(rhs.m_IndexFmt), m_iTriCount(rhs.m_iTriCount)
	, m_iVertexCountX(rhs.m_iVertexCountX), m_iVertexCountZ(rhs.m_iVertexCountZ), m_fVertexItv(rhs.m_fVertexItv), m_pVertexPos(rhs.m_pVertexPos)
	//, m_vScale(rhs.m_vScale)//,m_pColliderBoxVertex(rhs.m_pColliderBoxVertex)
{
	//if (m_pVertexPos != nullptr)
	//{
	//	m_pVertexPos = new _vec3[rhs.m_iVertexCount];
	//	for (int i = 0; i < rhs.m_iVertexCount; ++i)
	//	{
	//		m_pVertexPos[i] = rhs.m_pVertexPos[i];
	//	}	
	//}

	//if (m_pColliderBoxVertex != nullptr)
	//{
		//m_pColliderBoxVertex = new _vec3[rhs.m_iVertexCount];
		//for (int i = 0; i < rhs.m_iVertexCount; ++i)
		//{
		//	m_pColliderBoxVertex[i] = rhs.m_pColliderBoxVertex[i];
		//}
	//}

	m_pVB->AddRef();
	m_pIB->AddRef();
}


void CVIBuffer::Set_Size(const _float & fSizeX, const _float & fSizeY, const _float & fSizeZ)
{
}

HRESULT CVIBuffer::Ready_Buffer()
{
	/* FVF(Flexible Vertex Format) */
	/*Length,usage,fvf,pool,&VB,Shared*/
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_iVertexSize*m_iVertexCount,0,m_dwVertexFVF,D3DPOOL_MANAGED,&m_pVB,nullptr)))
	{
		MSG_BOX("VertexBuffer Not Created");
		return E_FAIL;
	}
	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_iIndexSize*m_iTriCount, 0, m_IndexFmt, D3DPOOL_MANAGED, &m_pIB, nullptr)))
	{
		MSG_BOX("IndexBuffer creation failed");
		return E_FAIL;
	}

	return NO_ERROR;
}

void CVIBuffer::Render_Buffer()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_iVertexSize);
	m_pGraphicDev->SetFVF(m_dwVertexFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

_ulong CVIBuffer::Free()
{
	_ulong dwRefCount = 0;
	
	if (m_bIsClone)
	{
		/*decrease refcount*/
		Safe_Release(m_pVB);
		Safe_Release(m_pIB);
	}
	else /*is not Clone*/
	{
		if (dwRefCount = Safe_Release(m_pVB))
			return dwRefCount;
		
		if (dwRefCount = Safe_Release(m_pIB))
			return dwRefCount;

		Safe_Delete_Array(m_pVertexPos);
		Safe_Delete_Array(m_pColliderBoxVertex);
	}

	

	return CResources::Free();
}