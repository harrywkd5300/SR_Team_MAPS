#include "..\Headers\Texture.h"



CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	:CResources(pGraphicDev)
{
}

CTexture::CTexture(const CTexture & rhs)
	:CResources(rhs), m_vecTexture(rhs.m_vecTexture)
{
	for (auto& pTexture : m_vecTexture)
		pTexture->AddRef();

}

HRESULT CTexture::Ready_Texture(TEXTURETYPE eType, const _tchar * pFilePath, _uint iTexCount)
{
	m_vecTexture.reserve(iTexCount);


	for (size_t i=0; i<iTexCount; ++i)
	{
		_tchar szFilePath[128] = L"";
		wsprintf(szFilePath, pFilePath, i);

		if (TEXTYPE_NORMAL == eType)
		{
			LPDIRECT3DTEXTURE9 pTexture = nullptr;
			D3DXCreateTextureFromFile(m_pGraphicDev, szFilePath, &pTexture);
			m_vecTexture.push_back(pTexture);
		}
		else if (TEXTYPE_CUBE == eType)
		{
			LPDIRECT3DCUBETEXTURE9 pTexture = nullptr;
			D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFilePath, &pTexture);
			m_vecTexture.push_back(pTexture);
		}
	}
	return NOERROR;
}

HRESULT CTexture::SetUp_TextureToGraphicDev(_uint dwStage, _uint iTextureIndex)
{
	if (iTextureIndex >= m_vecTexture.size())
	{
		MSG_BOX("Received TextureIndex over vector size err:89518");
		return E_FAIL;
	}

	m_pGraphicDev->SetTexture(dwStage, m_vecTexture[iTextureIndex]);
	return NOERROR;
}

LPDIRECT3DBASETEXTURE9 CTexture::GetTexture(const _uint & iTextureIndex)
{
	if (m_vecTexture.empty() || m_vecTexture.size() < iTextureIndex || iTextureIndex <= 0)
		return nullptr;

	return m_vecTexture[iTextureIndex - 1];
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTURETYPE eType, const _tchar * pFilePath, const _uint& iTexCount)
{
	CTexture* pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Texture(eType, pFilePath, iTexCount)))
	{
		MSG_BOX("CTexture creation failed err: 994865");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTexture::Clone()
{
	return new CTexture(*this);
}

_ulong CTexture::Free()
{
	_ulong dwRefCount = 0;

	for (auto& pTexture : m_vecTexture)
		Safe_Release(pTexture);
	
	m_vecTexture.clear();

	CResources::Free();
	return dwRefCount;
}

