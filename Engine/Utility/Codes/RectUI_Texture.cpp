#include "..\Headers\RectUI_Texture.h"



CRectUI_Texture::CRectUI_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CRectUI_Texture::CRectUI_Texture(const CRectUI_Texture & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CRectUI_Texture::Ready_Buffer()
{
	m_iVertexCount = 4;
	m_iVertexSize = sizeof(VTXVIEWPORT);
	m_dwVertexFVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
	
	m_iIndexSize = sizeof(INDEX16);
	m_iTriCount = 2;
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		goto exception;

	if (nullptr == m_pVB || nullptr == m_pIB)
		goto exception;
	
	VTXVIEWPORT* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	
	pVertex[0].vPos = _vec4(-1.f, -1.f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(1.f, -1.f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(1.f, 1.f, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(-1.f, 1.f, 0.f, 1.f);
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

void CRectUI_Texture::Set_Parameter(const _float& fPosX, const _float& fPosY, const _float& fSizeX, const _float& fSizeY)
{

	VTXVIEWPORT* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(fPosX - fSizeX * 0.5f, fPosY - fSizeY * 0.5f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(fPosX + fSizeX * 0.5f, fPosY - fSizeY * 0.5f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(fPosX + fSizeX * 0.5f, fPosY + fSizeY * 0.5f, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(fPosX - fSizeX * 0.5f, fPosY + fSizeY * 0.5f, 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

}

CRectUI_Texture * CRectUI_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRectUI_Texture* pInstance = new CRectUI_Texture(pGraphicDev);
	if (FAILED(pInstance->Ready_Buffer()))
	{
		MSG_BOX("RectUI_Texture creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRectUI_Texture::Clone()
{
	return new CRectUI_Texture(*this);
}

_ulong CRectUI_Texture::Free()
{
	return CVIBuffer::Free();
}
