#include "..\Headers\Material.h"


CMaterial::CMaterial(LPDIRECT3DDEVICE9 pGraphicDev)
: CComponent(pGraphicDev)
{

}

CMaterial::CMaterial(const CMaterial& rhs)
: CComponent(rhs)
, m_Material(rhs.m_Material)
{

}

HRESULT CMaterial::SetUp_MaterialInfo(D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, D3DXCOLOR Emissive, const _float& fPower)
{
	m_Material.Diffuse = Diffuse;
	m_Material.Ambient = Ambient;
	m_Material.Specular = Specular;
	m_Material.Emissive = Emissive;

	m_Material.Power = fPower;

	return S_OK;
}

HRESULT CMaterial::SetUp_MaterialInfo(MATTYPE eType)
{
	//float ambient[4];
	//float diffuse[4];
	//float emissive[4];
	//float specular[4];
	//float shininess;

	//D3DCOLORVALUE   Diffuse;        /* Diffuse color RGBA */
	//D3DCOLORVALUE   Ambient;        /* Ambient color RGB */
	//D3DCOLORVALUE   Specular;       /* Specular 'shininess' */
	//D3DCOLORVALUE   Emissive;       /* Emissive color RGB */
	//float           Power;
	switch (eType)
	{
	case MATTYPE::MATTYPE_NORMAL:
		m_Material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_Material.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		m_Material.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		m_Material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		m_Material.Power = 0.0f;
		break;
	case MATTYPE::MATTYPE_DULL:
		m_Material.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		m_Material.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		m_Material.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		m_Material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		m_Material.Power = 0.0f;
		break;
	case MATTYPE::MATTYPE_SHINY:
		m_Material.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		m_Material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_Material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_Material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		m_Material.Power = 32.0f;
		break;
	case MATTYPE::MATTYPE_TEST:
		m_Material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_Material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_Material.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
		m_Material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		m_Material.Power = 16.0f;
		break;
	default:
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMaterial::SetUp_MaterialToGraphicDev()
{
	if (NULL == m_pGraphicDev)
		return E_FAIL;

	m_pGraphicDev->SetMaterial(&m_Material);

	return S_OK;
}


HRESULT CMaterial::Ready_Material(MATTYPE eType)
{
	ZeroMemory(&m_Material, sizeof(D3DMATERIAL9));

	if (FAILED(SetUp_MaterialInfo(eType)))
	{
		return E_FAIL;
	}
	return S_OK;
}

CComponent* CMaterial::Clone(void)
{
	return new CMaterial(*this);
}

CMaterial* CMaterial::Create(LPDIRECT3DDEVICE9 pGraphicDev, MATTYPE eType)
{
	CMaterial*	pInstance = new CMaterial(pGraphicDev);

	if(FAILED(pInstance->Ready_Material(eType)))
	{
		MessageBox(NULL, L"CMaterial Created Failed", NULL, MB_OK);
		Safe_Release(pInstance);
	}	
	return pInstance;
}


_ulong CMaterial::Free(void)
{
	_ulong dwRefCount = 0;

	CComponent::Free();

	return dwRefCount;

}

