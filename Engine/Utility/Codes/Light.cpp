#include "..\Headers\Light.h"


CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
: CComponent(pGraphicDev)
{

}

CLight::CLight(const CLight& rhs)
: CComponent(rhs)
, m_Light(rhs.m_Light)
{

}

HRESULT CLight::SetUp_LightPosition(const _vec3& vPos)
{
	m_Light.Position.x = vPos.x;
	m_Light.Position.y = vPos.y;
	m_Light.Position.z = vPos.z;

	return NOERROR;
}

HRESULT CLight::SetUp_LightInfo(D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, const _float& fRadius)
{
	m_Light.Diffuse = Diffuse;
	m_Light.Ambient = Ambient;
	m_Light.Specular = Specular;
	m_Light.Range = fRadius;

	return NOERROR;
}

HRESULT CLight::SetUp_LightInfo(LIGHTCOLOR eType)
{
	//float pos[3];
	//float ambient[4];
	//float diffuse[4];
	//float specular[4];
	//float radius;
	//D3DXVECTOR3 velocity;
	switch (eType)
	{
	case LIGHTCOLOR::WHITE:
		m_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light.Range = LIGHT_RANGE_DEFAULT;
		break;
	case LIGHTCOLOR::RED:
		m_Light.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		m_Light.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		m_Light.Specular = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		m_Light.Range = LIGHT_RANGE_DEFAULT;
		break;
	case LIGHTCOLOR::GREEN:
		m_Light.Ambient = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		m_Light.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		m_Light.Specular = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		m_Light.Range = LIGHT_RANGE_DEFAULT;
		break;
	case LIGHTCOLOR::BLUE:
		m_Light.Ambient = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		m_Light.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		m_Light.Specular = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		m_Light.Range = LIGHT_RANGE_DEFAULT;
		break;
	case LIGHTCOLOR::YELLOW:
		m_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		m_Light.Specular = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		m_Light.Range = LIGHT_RANGE_DEFAULT;
		break;
	case LIGHTCOLOR::CYAN:
		m_Light.Ambient = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		m_Light.Diffuse = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		m_Light.Specular = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		m_Light.Range = LIGHT_RANGE_DEFAULT;
		break;
	case LIGHTCOLOR::MAGENTA:
		m_Light.Ambient = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		m_Light.Diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		m_Light.Specular = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		m_Light.Range = LIGHT_RANGE_DEFAULT;
		break;
	case LIGHTCOLOR::CORNFLOWER_BLUE:
		m_Light.Ambient = D3DXCOLOR((100.0f / 255.0f), (149.0f / 255.0f), (237.0f / 255.0f), 1.0f);
		m_Light.Diffuse = D3DXCOLOR((100.0f / 255.0f), (149.0f / 255.0f), (237.0f / 255.0f), 1.0f);
		m_Light.Specular = D3DXCOLOR((100.0f / 255.0f), (149.0f / 255.0f), (237.0f / 255.0f), 1.0f);
		m_Light.Range = LIGHT_RANGE_DEFAULT;
		break;
	case LIGHTCOLOR::BLACK:
		m_Light.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		m_Light.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		m_Light.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		m_Light.Range = LIGHT_RANGE_DEFAULT;
		break;
	case LIGHTCOLOR::TEST:
		m_Light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
		m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
		m_Light.Range = LIGHT_RANGE_DEFAULT;
		break;
	case LIGHTCOLOR::DIRECTIONAL_LIGHT:
		m_Light.Position.x = 500.f;
		m_Light.Position.y = 500.f;
		m_Light.Position.z = -500.f;
		m_Light.Ambient = D3DXCOLOR(0.20f, 0.20f, 0.20f, 0.20f);
		m_Light.Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);
		m_Light.Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
		//m_Light.Ambient = D3DXCOLOR(0.05f, 0.05f, 0.05f, 0.05f);
		//m_Light.Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);
		//m_Light.Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
		m_Light.Range = 512;
		break;
	default:
		return E_FAIL;
	}

	return NOERROR;
}

//HRESULT CLight::SetUp_LightToGraphicDev()
//{
//	if (NULL == m_pGraphicDev)
//		return E_FAIL;
//
//	
//
//	return S_OK;
//}


HRESULT CLight::Ready_Light(LIGHTCOLOR eType)
{
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));

	if (FAILED(SetUp_LightInfo(eType)))
	{
		return E_FAIL;
	}
	return NOERROR;
}

HRESULT CLight::Ready_Light(const D3DLIGHT9 * pLightInfo)
{
	m_Light = *pLightInfo;

	return NOERROR;
}

void CLight::Set_ShaderLightList(LPD3DXEFFECT pEffect, LIGHT_TYPE eType, _uint iIndex)
{
	switch (eType)
	{
	case LIGHT_TYPE::POINT_LIGHT:
	{
		D3DXHANDLE hLight;
		D3DXHANDLE hLightPos;
		D3DXHANDLE hLightAmbient;
		D3DXHANDLE hLightDiffuse;
		D3DXHANDLE hLightSpecular;
		D3DXHANDLE hLightRadius;
		
		hLight = pEffect->GetParameterElement("g_PointLightArr", iIndex);
		hLightPos = pEffect->GetParameterByName(hLight, "vPos");
		hLightAmbient = pEffect->GetParameterByName(hLight, "Ambient");
		hLightDiffuse = pEffect->GetParameterByName(hLight, "Diffuse");
		hLightSpecular = pEffect->GetParameterByName(hLight, "Specular");
		hLightRadius = pEffect->GetParameterByName(hLight, "fRadius");

		pEffect->SetValue(hLightPos, &m_Light.Position, sizeof(m_Light.Position));
		pEffect->SetValue(hLightAmbient, &m_Light.Ambient, sizeof(m_Light.Ambient));
		pEffect->SetValue(hLightDiffuse, &m_Light.Diffuse, sizeof(m_Light.Diffuse));
		pEffect->SetValue(hLightSpecular, &m_Light.Specular, sizeof(m_Light.Specular));
		pEffect->SetFloat(hLightRadius, m_Light.Range);
	}
	break;
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
	{
		pEffect->SetValue("g_DirectionalLight.vPos", &m_Light.Position, sizeof(m_Light.Position));
		pEffect->SetValue("g_DirectionalLight.Ambient", &m_Light.Ambient, sizeof(m_Light.Ambient));
		pEffect->SetValue("g_DirectionalLight.Diffuse", &m_Light.Diffuse, sizeof(m_Light.Diffuse));
		pEffect->SetValue("g_DirectionalLight.Specular", &m_Light.Specular, sizeof(m_Light.Specular));
	}
	break;
	default:
		return;
	}
}
//
//void CLight::Set_ShaderShadow(LPD3DXEFFECT pEffect, LIGHT_TYPE eType, _uint iIndex)
//{
//	_uint		numPasses = 0;
//
//	//else if (D3DLIGHT_POINT == m_LightInfo.Type)
//	//{
//	//	numPasses = 1;
//	//	pEffect->SetVector("g_vLightPos", &_vec4(m_LightInfo.Position, 1.f));
//	//	pEffect->SetFloat("g_fRange", m_LightInfo.Range);
//
//	//	pEffect->SetVector("g_vLightDiffuse", (_vec4*)(&m_LightInfo.Diffuse));
//	//	pEffect->SetVector("g_vLightAmbient", (_vec4*)(&m_LightInfo.Ambient));
//	//	pEffect->SetVector("g_vLightSpecular", (_vec4*)(&m_LightInfo.Specular));
//
//	//	_matrix			matWorld, matView, matProj;
//
//	//	D3DXMatrixIdentity(&matWorld);
//	//	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
//	//	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
//
//	//	_matrix matWorldView;
//	//	_matrix matWorldViewProjection;
//
//	//	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
//	//	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);
//
//
//	//	pEffect->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
//
//	//	D3DXMatrixInverse(&matView, nullptr, &matView);
//	//	pEffect->SetVector("g_vWorldCameraPos", (_vec4*)&matView.m[3][0]);
//
//	//	pEffect->BeginPass(numPasses);
//
//	//	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXVIEWPORT));
//	//	m_pGraphicDev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
//	//	m_pGraphicDev->SetIndices(m_pIB);
//	//	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
//
//	//	pEffect->EndPass();
//	//}
//
//	switch (eType)
//	{
//	case LIGHT_TYPE::POINT_LIGHT:
//	{
//		D3DXHANDLE hLight;
//		D3DXHANDLE hLightPos;
//		D3DXHANDLE hLightAmbient;
//		D3DXHANDLE hLightDiffuse;
//		D3DXHANDLE hLightSpecular;
//		D3DXHANDLE hLightRadius;
//
//		hLight = pEffect->GetParameterElement("g_PointLightArr", iIndex);
//		hLightPos = pEffect->GetParameterByName(hLight, "vPos");
//		hLightAmbient = pEffect->GetParameterByName(hLight, "Ambient");
//		hLightDiffuse = pEffect->GetParameterByName(hLight, "Diffuse");
//		hLightSpecular = pEffect->GetParameterByName(hLight, "Specular");
//		hLightRadius = pEffect->GetParameterByName(hLight, "fRadius");
//
//		pEffect->SetValue(hLightPos, &m_Light.Position, sizeof(m_Light.Position));
//		pEffect->SetValue(hLightAmbient, &m_Light.Ambient, sizeof(m_Light.Ambient));
//		pEffect->SetValue(hLightDiffuse, &m_Light.Diffuse, sizeof(m_Light.Diffuse));
//		pEffect->SetValue(hLightSpecular, &m_Light.Specular, sizeof(m_Light.Specular));
//		pEffect->SetFloat(hLightRadius, m_Light.Range);
//	}
//	break;
//	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
//	{
//		numPasses = 0;
//		pEffect->SetVector("g_vLightDir", &_vec4(m_LightInfo.Direction, 0.f));
//
//		pEffect->SetVector("g_vLightDiffuse", (_vec4*)(&m_LightInfo.Diffuse));
//		pEffect->SetVector("g_vLightAmbient", (_vec4*)(&m_LightInfo.Ambient));
//		pEffect->SetVector("g_vLightSpecular", (_vec4*)(&m_LightInfo.Specular));
//
//		_matrix			matWorld, matView, matProj;
//
//		D3DXMatrixIdentity(&matWorld);
//		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
//		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
//
//		_matrix matWorldView;
//		_matrix matWorldViewProjection;
//
//		D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
//		D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);
//
//
//		pEffect->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
//
//		D3DXMatrixInverse(&matView, nullptr, &matView);
//		pEffect->SetVector("g_vWorldCameraPos", (_vec4*)&matView.m[3][0]);
//
//		pEffect->BeginPass(numPasses);
//
//		m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXVIEWPORT));
//		m_pGraphicDev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
//		m_pGraphicDev->SetIndices(m_pIB);
//		m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
//
//		pEffect->EndPass(); 
//	}
//	break;
//	default:
//		return;
//	}
//}

CComponent* CLight::Clone(void)
{
	return new CLight(*this);
}

CLight* CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev, LIGHTCOLOR eType)
{
	CLight*	pInstance = new CLight(pGraphicDev);

	if(FAILED(pInstance->Ready_Light(eType)))
	{
		MessageBox(NULL, L"CLight Created Failed", NULL, MB_OK);
		Safe_Release(pInstance);
	}	
	return pInstance;
}

CLight * CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9 * pLightInfo)
{
	CLight*	pInstance = new CLight(pGraphicDev);

	if (FAILED(pInstance->Ready_Light(pLightInfo)))
	{
		MessageBox(NULL, L"CLight Created Failed", NULL, MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}


_ulong CLight::Free(void)
{
	_ulong dwRefCount = 0;

	CComponent::Free();

	return dwRefCount;

}

