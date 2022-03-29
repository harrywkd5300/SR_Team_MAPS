#include "..\Headers\Temp_Cube.h"



CTemp_Cube::CTemp_Cube(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CTemp_Cube::CTemp_Cube(CTemp_Cube & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CTemp_Cube::Ready_Buffer()
{
	//
	/*Input parameter to buffer*/
	//
	m_iVertexCount = 8;
	m_iVertexSize = sizeof(VTXCOL);
	m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	m_iTriCount = 12;
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
	
	
	pVertex[0].vPosition = _vec3(-1.0f, -1.0f, -1.0f);
	pVertex[0].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVertex[1].vPosition = _vec3(-1.0f, 1.0f, -1.0f);
	pVertex[1].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVertex[2].vPosition = _vec3(1.0f, 1.0f, -1.0f);
	pVertex[2].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVertex[3].vPosition = _vec3(1.0f, -1.0f, -1.0f);
	pVertex[3].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVertex[4].vPosition = _vec3(-1.0f, -1.0f, 1.0f);
	pVertex[4].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVertex[5].vPosition = _vec3(-1.0f, 1.0f, 1.0f);
	pVertex[5].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVertex[6].vPosition = _vec3(1.0f, 1.0f, 1.0f);
	pVertex[6].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVertex[7].vPosition = _vec3(1.0f, -1.0f, 1.0f);
	pVertex[7].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();

	//
	/*Input info to Indexbuffer*/
	//
	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// front side
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	// back side
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 6;
	pIndex[2]._2 = 5;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 7;
	pIndex[3]._2 = 6;

	// left side
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// right side
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// top
	pIndex[8]._0 = 1;
	pIndex[8]._1 = 5;
	pIndex[8]._2 = 6;

	pIndex[9]._0 = 1;
	pIndex[9]._1 = 6;
	pIndex[9]._2 = 2;

	// bottom
	pIndex[10]._0 = 4;
	pIndex[10]._1 = 0;
	pIndex[10]._2 = 3;

	pIndex[11]._0 = 4;
	pIndex[11]._1 = 3;
	pIndex[11]._2 = 7;

	m_pIB->Unlock();


	return NOERROR;
}

void CTemp_Cube::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CTemp_Cube * CTemp_Cube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTemp_Cube* pInstance = new CTemp_Cube(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		MSG_BOX("Temp_Cube creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTemp_Cube::Clone()
{
	return new CTemp_Cube(*this);
}

_ulong CTemp_Cube::Free()
{
	return CVIBuffer::Free();
}
