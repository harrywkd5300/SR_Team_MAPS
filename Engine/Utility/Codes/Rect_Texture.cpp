#include "..\Headers\Rect_Texture.h"



CRect_Texture::CRect_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CRect_Texture::CRect_Texture(const CRect_Texture & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CRect_Texture::Ready_Buffer()
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

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	
	pVertex[0].vPosition = _vec3(-1.f, 1.f, 0.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(1.f, 1.f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec3(1.f, -1.f, 0.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec3(-1.f, -1.f, 0.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

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
	MSG_BOX("Rect_Texture Ready Failed err:10154");
	return E_FAIL;
}

CRect_Texture * CRect_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRect_Texture* pInstance = new CRect_Texture(pGraphicDev);
	if (FAILED(pInstance->Ready_Buffer()))
	{
		MSG_BOX("Rect_Texture creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRect_Texture::Clone()
{
	return new CRect_Texture(*this);
}

_ulong CRect_Texture::Free()
{
	return CVIBuffer::Free();
}
