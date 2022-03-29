#include "..\Headers\Rect_Color.h"



CRect_Color::CRect_Color(LPDIRECT3DDEVICE9 pGrahpicDev)
	:CVIBuffer(pGrahpicDev)
{
}

CRect_Color::CRect_Color(const CRect_Color & rhs)
	:CVIBuffer(rhs)
{

}

HRESULT CRect_Color::Ready_Buffer()
{
	//
	/*Input parameter to buffer*/
	//
	m_iVertexCount = 4;
	m_iVertexSize = sizeof(VTXCOL);
	m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	
	m_iTriCount = 2;
	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	if (nullptr == m_pVB || nullptr == m_pIB)
		return E_FAIL;

	/*Memory allocated Buffer*/
	
	//
	/*Input info to vertexbuffer*/
	//
	VTXCOL* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, m_dwVertexFVF);

// 	pVertex[0].vPosition	= _vec3(-1.f, 1.f, 0.f);
// 	pVertex[0].dwColor		= D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
// 
// 	pVertex[1].vPosition	= _vec3(1.f, 1.f, 0.f);
// 	pVertex[1].dwColor		= D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
// 
// 	pVertex[2].vPosition	= _vec3(1.f, -1.f, 0.f);
// 	pVertex[2].dwColor		= D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
// 
// 	pVertex[3].vPosition	= _vec3(-1.f, -1.f, 0.f);
// 	pVertex[3].dwColor		= D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);


	m_pVB->Unlock();

	//
	/*Input info to Indexbuffer*/
	//
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
}

void CRect_Color::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent * CRect_Color::Clone()
{
	return new CRect_Color(*this);
}


CRect_Color * CRect_Color::Create(LPDIRECT3DDEVICE9 pGrahpicDev)
{
	CRect_Color* pInstance = new CRect_Color(pGrahpicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		MSG_BOX("Rect_Color Buffer creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CRect_Color::Free()
{
	return CVIBuffer::Free();;
}

