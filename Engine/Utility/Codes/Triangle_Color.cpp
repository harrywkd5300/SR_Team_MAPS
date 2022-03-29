#include "..\Headers\Triangle_Color.h"



CTriangle_Color::CTriangle_Color(LPDIRECT3DDEVICE9 pGrahpicDev)
	:CVIBuffer(pGrahpicDev)
{
}

CTriangle_Color::CTriangle_Color(const CTriangle_Color & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CTriangle_Color::Ready_Buffer()
{
	//
	/*input parameters to buffer*/
	//

	m_iVertexSize = sizeof(VTXCOL);
	m_iVertexCount = 3;
	m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	m_iIndexSize = sizeof(INDEX16);
	m_iTriCount = 1;
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	if (nullptr == m_pVB || nullptr == m_pIB)
		return E_FAIL;
	
	/*memory successfully allocated*/

	//
	/*input info to vertexbuffer*/
	//

	VTXCOL* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	
	pVertex[0].vPosition	= _vec3(0.f, 1.f, 0.f);
	pVertex[0].dwColor		= D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVertex[1].vPosition	= _vec3(1.f, -1.f, 0.f);
	pVertex[1].dwColor		= D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVertex[2].vPosition	= _vec3(-1.f, -1.f, 0.f);
	pVertex[2].dwColor		= D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();

	//
	/*input info to indexbuffer*/
	//
	
	INDEX16* pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;
	
	m_pIB->Unlock();

	return NOERROR;
}

void CTriangle_Color::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}


CTriangle_Color* CTriangle_Color::Create(LPDIRECT3DDEVICE9 pGrahpicDev)
{
	CTriangle_Color* pInstance = new CTriangle_Color(pGrahpicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		MSG_BOX("Triangle_Color Creation Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTriangle_Color::Clone()
{
	return new CTriangle_Color(*this);
}

_ulong CTriangle_Color::Free()
{
	return CVIBuffer::Free();
}
