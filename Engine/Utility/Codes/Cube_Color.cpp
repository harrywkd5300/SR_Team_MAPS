#include "..\Headers\Cube_Color.h"



CCube_Color::CCube_Color(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CCube_Color::CCube_Color(const CCube_Color & rhs)
	: CVIBuffer(rhs)
	,m_vSize(rhs.m_vSize)
{
}

HRESULT CCube_Color::Ready_Buffer(const _float& fSizeX, const _float& fSizeY, const _float& fSizeZ)
{
	ZeroMemory(&m_vSize, sizeof(_vec3));
	m_vSize= _vec3(fSizeX, fSizeY, fSizeZ);

	m_iVertexCount = 8;
	m_iVertexSize = sizeof(VTXCOL);
	m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	m_iIndexSize = sizeof(INDEX16);
	m_iTriCount = 16;
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		goto exception;

	if (nullptr == m_pVB || nullptr == m_pIB)
		goto exception;

	VTXCOL* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	const _float x = 0.5f* fSizeX;
	const _float y = 0.5f* fSizeY;
	const _float z = 0.5f* fSizeZ;


	pVertex[0].vPosition = _vec3(-x, y, -z);
	pVertex[0].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;

	pVertex[1].vPosition = _vec3(x, y, -z);
	pVertex[1].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;

	pVertex[2].vPosition = _vec3(x, -y, -z);
	pVertex[2].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;

	pVertex[3].vPosition = _vec3(-x, -y, -z);
	pVertex[3].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;

	pVertex[4].vPosition = _vec3(-x, y, z);
	pVertex[4].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;

	pVertex[5].vPosition = _vec3(x, y, z);
	pVertex[5].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;

	pVertex[6].vPosition = _vec3(x, -y, z);
	pVertex[6].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;

	pVertex[7].vPosition = _vec3(-x, -y, z);
	pVertex[7].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;


	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	//+x
	pIndex[0]._0 = 1; pIndex[0]._1 = 5; pIndex[0]._2 = 6;
	pIndex[1]._0 = 1; pIndex[1]._1 = 6; pIndex[1]._2 = 2;

	//-x
	pIndex[2]._0 = 4; pIndex[2]._1 = 0; pIndex[2]._2 = 3;
	pIndex[3]._0 = 4; pIndex[3]._1 = 3; pIndex[3]._2 = 7;

	//+y
	pIndex[4]._0 = 4; pIndex[4]._1 = 5; pIndex[4]._2 = 1;
	pIndex[5]._0 = 4; pIndex[5]._1 = 1; pIndex[5]._2 = 0;

	//-y
	pIndex[6]._0 = 3; pIndex[6]._1 = 2; pIndex[6]._2 = 6;
	pIndex[7]._0 = 3; pIndex[7]._1 = 6; pIndex[7]._2 = 7;

	//+z
	pIndex[8]._0 = 5; pIndex[8]._1 = 4; pIndex[8]._2 = 7;
	pIndex[9]._0 = 5; pIndex[9]._1 = 7; pIndex[9]._2 = 6;

	//-z
	pIndex[10]._0 = 0; pIndex[10]._1 = 1; pIndex[10]._2 = 2;
	pIndex[11]._0 = 0; pIndex[11]._1 = 2; pIndex[11]._2 = 3;

	m_pIB->Unlock();

	return NO_ERROR;

exception:
	MSG_BOX("Cube_Texture Ready Failed");
	return E_FAIL;
}

void CCube_Color::Set_Size(const _float & fSizeX, const _float & fSizeY, const _float & fSizeZ)
{
	VTXCOL* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	const _float x = 0.5f* fSizeX;
	const _float y = 0.5f* fSizeY;
	const _float z = 0.5f* fSizeZ;


	pVertex[0].vPosition = _vec3(-x, y, -z);
	pVertex[0].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;

	pVertex[1].vPosition = _vec3(x, y, -z);
	pVertex[1].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;

	pVertex[2].vPosition = _vec3(x, -y, -z);
	pVertex[2].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;

	pVertex[3].vPosition = _vec3(-x, -y, -z);
	pVertex[3].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;

	pVertex[4].vPosition = _vec3(-x, y, z);
	pVertex[4].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;

	pVertex[5].vPosition = _vec3(x, y, z);
	pVertex[5].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;

	pVertex[6].vPosition = _vec3(x, -y, z);
	pVertex[6].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;

	pVertex[7].vPosition = _vec3(-x, -y, z);
	pVertex[7].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);;


	m_pVB->Unlock();
}

CCube_Color * CCube_Color::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fSizeX, const _float& fSizeY, const _float& fSizeZ)
{
	CCube_Color* pInstance = new CCube_Color(pGraphicDev);



	if (FAILED(pInstance->Ready_Buffer(fSizeX, fSizeY, fSizeZ)))
	{
		MSG_BOX("Cube_Color creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCube_Color::Clone()
{
	return new CCube_Color(*this);
}

_ulong CCube_Color::Free()
{
	return CVIBuffer::Free();
}
