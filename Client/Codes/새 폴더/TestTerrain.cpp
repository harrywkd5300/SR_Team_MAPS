#include "stdafx.h"
#include "..\Headers\TestTerrain.h"
#include "..\Headers\Camera_Manager.h"
#include "LightObj.h"
#include "Light_Manager.h"



CTestTerrain::CTestTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	,m_pBufferCom(nullptr)
	,m_pTextureCom(nullptr)
	,m_pTransformCom(nullptr)
	,m_pRendererCom(nullptr)
	,m_pMatCom(nullptr)
{
}

HRESULT CTestTerrain::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	//gpCube = LoadModel(L"../Bin/Resources/Mesh/Cube.x");

	return NOERROR;
}

_int CTestTerrain::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	//m_pTransformCom->Move_ToDirection(_vec3(-100.0f, -100.f, -100.0f), 2.f, fTimeDelta);
	//m_pTransformCom->Fill_WorldMatrix();
	//_matrix matProj, matView, matWorld;

	//D3DXVECTOR4 gWorldLightPosition(0.f, -100.f, -100.f, 1.f);
	//D3DXVECTOR4 gLightColor(1.f, 1.f, 1.f, 1.0f);
	//D3DXVECTOR4 gWorldCameraPosition;w
	//D3DXVec4Normalize(&gWorldLightPosition, &gWorldLightPosition);

	//_vec3 CameraPosition = CCamera_Manager::GetInstance()->Get_Camera(L"Camera_Dynamic")->Get_CameraPos();
	//gWorldCameraPosition.x = CameraPosition.x;
	//gWorldCameraPosition.y = CameraPosition.y;
	//gWorldCameraPosition.z = CameraPosition.z;
	//gWorldCameraPosition.w = 1.f;

	////D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), BACKCX / float(BACKCY), 0.2f, 1000.f);
	////matWorld = m_pTransformCom->Get_Matrix();
	//D3DXMatrixIdentity(&matWorld);
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	//D3DXMATRIXA16 matWorldView;
	//D3DXMATRIXA16 matWorldViewProjection;
	//D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
	//D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);

	//D3DXMatrixIdentity(&matWorld);
	//m_pShaderCom->GetShader(1)->SetMatrix("worldMatrix", &matWorld);
	//m_pShaderCom->GetShader(1)->SetMatrix("worldInverseTransposeMatrix", &matWorld);
	//m_pShaderCom->GetShader(1)->SetMatrix("worldViewProjectionMatrix", &matWorldViewProjection);
	////m_pShaderCom->GetShader(1)->SetValue("globalAmbient", &D3DXVECTOR4(0.2f,0.2f,0.2f,1.0f), sizeof(D3DXVECTOR4));

	////m_pShaderCom->GetShader(1)->SetValue("cameraPos", &CameraPosition, sizeof(CameraPosition));
	////D3DXMatrixInverse(&matView, nullptr, &matView);
	//m_pShaderCom->GetShader(1)->SetValue("cameraPos", (_vec3*)&matView.m[3][0], sizeof(_vec3));
	////m_pShaderCom->GetShader(1)->SetVector("cameraPos", &gWorldCameraPosition);
	//float  g_sceneAmbient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	//m_pShaderCom->GetShader(1)->SetValue("globalAmbient", &g_sceneAmbient, sizeof(g_sceneAmbient));
	//int g_numLights = 1;
	//m_pShaderCom->GetShader(1)->SetValue("numLights", &g_numLights, sizeof(g_numLights));

	//CLight* pLightCom = 0;
	//const  D3DLIGHT9* pLight = 0;
	//D3DXHANDLE hLight;
	//D3DXHANDLE hLightPos;
	//D3DXHANDLE hLightAmbient;
	//D3DXHANDLE hLightDiffuse;
	//D3DXHANDLE hLightSpecular;
	//D3DXHANDLE hLightRadius;

	//pLightCom = m_pLight[0]->GetLightCom();
	//pLight = &pLightCom->Get_LightInfo();

	//m_pShaderCom->GetShader(1)->SetValue("lights.pos", m_pLight[0]->GetTransformCom()->Get_MatrixRow(CTransform::ROW_INFO::INFO_POSITION), sizeof(_vec3));
	//m_pShaderCom->GetShader(1)->SetValue("lights.ambient", &pLight->Ambient, sizeof(pLight->Ambient));
	//m_pShaderCom->GetShader(1)->SetValue("lights.diffuse", &pLight->Diffuse, sizeof(pLight->Diffuse));
	//m_pShaderCom->GetShader(1)->SetValue("lights.specular", &pLight->Specular, sizeof(pLight->Specular));
	//m_pShaderCom->GetShader(1)->SetFloat("lights.radius", pLight->Range);


	m_pRendererCom->Add_RenderList(CRenderer::RENDER_BACKGROUND, this);
	return 0;
}


void CTestTerrain::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTextureCom || nullptr == m_pTransformCom)
		return;

	_matrix matProj, matView, matWorld;

	D3DXVECTOR4 gWorldCameraPosition;

	//D3DLIGHT9 DirectionalLight;
	//DirectionalLight.Position.x = 0.f;
	//DirectionalLight.Position.y = 100.f;
	//DirectionalLight.Position.z = 100.f;
	////DirectionalLight.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
	////DirectionalLight.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	////DirectionalLight.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	//DirectionalLight.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	//DirectionalLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//DirectionalLight.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	matWorld = m_pTransformCom->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_matrix matWorldView;
	_matrix matWorldViewProjection;
	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);

	_vec3 CameraPosition;
	D3DXMatrixInverse(&matView, nullptr, &matView);
	CameraPosition.x = matView._41;
	CameraPosition.y = matView._42;
	CameraPosition.z = matView._43;

	

	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorld", &matWorld);
	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);

	m_pShaderCom->GetShader(1)->SetValue("g_vWorldCameraPos", &CameraPosition, sizeof(CameraPosition));

	CLight_Manager::GetInstance()->Set_ShaderLightList(m_pShaderCom->GetShader(1));

	//m_pShaderCom->GetShader(1)->SetValue("g_DirectionalLight.vPos", &DirectionalLight.Position, sizeof(DirectionalLight.Position));
	//m_pShaderCom->GetShader(1)->SetValue("g_DirectionalLight.Ambient", &DirectionalLight.Ambient, sizeof(DirectionalLight.Ambient));
	//m_pShaderCom->GetShader(1)->SetValue("g_DirectionalLight.Diffuse", &DirectionalLight.Diffuse, sizeof(DirectionalLight.Diffuse));
	//m_pShaderCom->GetShader(1)->SetValue("g_DirectionalLight.Specular", &DirectionalLight.Specular, sizeof(DirectionalLight.Specular));

	//int iLightArrSize = MAX_LIGHT_SIZE;
	//m_pShaderCom->GetShader(1)->SetValue("g_iLightArrSize", &iLightArrSize, sizeof(iLightArrSize));


	//D3DXHANDLE hLight;
	//D3DXHANDLE hLightPos;
	//D3DXHANDLE hLightAmbient;
	//D3DXHANDLE hLightDiffuse;
	//D3DXHANDLE hLightSpecular;
	//D3DXHANDLE hLightRadius;

	//
	//for (_uint i = 0; i < iLightArrSize; ++i)
	//{
	//	CLight* pLightCom = 0;
	//	const  D3DLIGHT9* pLight = 0;
	//	pLightCom = m_pLight[i]->GetLightCom();
	//	pLight = &pLightCom->Get_LightInfo();

	//	hLight = m_pShaderCom->GetShader(1)->GetParameterElement("g_PointLightArr", i);

	//	hLightPos = m_pShaderCom->GetShader(1)->GetParameterByName(hLight, "vPos");
	//	hLightAmbient = m_pShaderCom->GetShader(1)->GetParameterByName(hLight, "Ambient");
	//	hLightDiffuse = m_pShaderCom->GetShader(1)->GetParameterByName(hLight, "Diffuse");
	//	hLightSpecular = m_pShaderCom->GetShader(1)->GetParameterByName(hLight, "Specular");
	//	hLightRadius = m_pShaderCom->GetShader(1)->GetParameterByName(hLight, "fRadius");

	//	m_pShaderCom->GetShader(1)->SetValue(hLightPos, m_pLight[i]->GetTransformCom()->Get_MatrixRow(CTransform::ROW_INFO::INFO_POSITION), sizeof(_vec3));
	//	m_pShaderCom->GetShader(1)->SetValue(hLightAmbient, &pLight->Ambient, sizeof(pLight->Ambient));
	//	m_pShaderCom->GetShader(1)->SetValue(hLightDiffuse, &pLight->Diffuse, sizeof(pLight->Diffuse));
	//	m_pShaderCom->GetShader(1)->SetValue(hLightSpecular, &pLight->Specular, sizeof(pLight->Specular));
	//	m_pShaderCom->GetShader(1)->SetFloat(hLightRadius, pLight->Range);
	//	//m_pShaderCom->GetShader(1)->SetValue("g_PointLightArr.vPos", m_pLight[i]->GetTransformCom()->Get_MatrixRow(CTransform::ROW_INFO::INFO_POSITION), sizeof(_vec3));
	//	//m_pShaderCom->GetShader(1)->SetValue("g_PointLightArr.Ambient", &pLight->Ambient, sizeof(pLight->Ambient));
	//	//m_pShaderCom->GetShader(1)->SetValue("g_PointLightArr.Diffuse", &pLight->Diffuse, sizeof(pLight->Diffuse));
	//	//m_pShaderCom->GetShader(1)->SetValue("g_PointLightArr.Specular", &pLight->Specular, sizeof(pLight->Specular));
	//	//m_pShaderCom->GetShader(1)->SetFloat("g_PointLightArr.fRadius", pLight->Range);
	//}
	m_pShaderCom->GetShader(1)->SetValue("g_Material.Ambient", &m_pMatCom->Get_MaterialInfo().Ambient, sizeof(m_pMatCom->Get_MaterialInfo().Ambient));
	m_pShaderCom->GetShader(1)->SetValue("g_Material.Diffuse", &m_pMatCom->Get_MaterialInfo().Diffuse, sizeof(m_pMatCom->Get_MaterialInfo().Diffuse));
	m_pShaderCom->GetShader(1)->SetValue("g_Material.Emissive", &m_pMatCom->Get_MaterialInfo().Emissive, sizeof(m_pMatCom->Get_MaterialInfo().Emissive));
	m_pShaderCom->GetShader(1)->SetValue("g_Material.Specular", &m_pMatCom->Get_MaterialInfo().Specular, sizeof(m_pMatCom->Get_MaterialInfo().Specular));
	m_pShaderCom->GetShader(1)->SetFloat("g_Material.fPower", m_pMatCom->Get_MaterialInfo().Power);
	

	m_pShaderCom->GetShader(1)->SetTexture("DiffuseMap_Tex", m_pTextureCom->GetTexture(1));
	m_pShaderCom->GetShader(1)->SetTexture("SpecularMap_Tex", m_pTextureCom->GetTexture(2));
	m_pShaderCom->GetShader(1)->SetTexture("NormalMap_Tex", m_pTextureCom->GetTexture(3));

	_uint numPasses = 0;

	m_pShaderCom->GetShader(1)->Begin(&numPasses, NULL);
	{
		for (_uint i = 0; i < numPasses; ++i)
		{
			m_pShaderCom->GetShader(1)->BeginPass(i);
			{

				m_pTransformCom->SetUp_MatrixToGraphicDev();
				m_pBufferCom->Render_Buffer();
			}
			m_pShaderCom->GetShader(1)->EndPass();
		}
	}
	m_pShaderCom->GetShader(1)->End();

	return;
}

HRESULT CTestTerrain::Ready_Component()
{
	CComponent* pComponent = nullptr;

	//
	/*======Add Static Component=========*/
	//

	/*add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();

	/*Add Transform*/	
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
	m_pTransformCom->AddRef();

	/*add terrain buffer*/
	pComponent = m_pBufferCom = dynamic_cast<Jang_CTerrain_Buffer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Buffer_Terrain"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Buffer_Terrain", pComponent));
	m_pBufferCom->AddRef();

	/*Add Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Terrain"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Terrain", pComponent));
	m_pTextureCom->AddRef();

	/*Add Shader*/
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Shader_Terrain"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Shader_Terrain", pComponent));
	m_pShaderCom->AddRef();

	/*Add Material*/
	pComponent = m_pMatCom = dynamic_cast<CMaterial*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Material"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Material_Terrain", pComponent));
	m_pMatCom->SetUp_MaterialInfo(CMaterial::MATTYPE::MATTYPE_SHINY);
	m_pMatCom->AddRef();

			

	return NOERROR;

exception:
	MSG_BOX("CTerrain Component Ready Failed err: 13113");
	return E_FAIL;
}

CTestTerrain* CTestTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestTerrain* pInstance = new CTestTerrain(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("Create Terrain failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CTestTerrain::Free()
{
	_ulong dwRefCount = 0;

	/*decrease refcount*/
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMatCom);

	//Safe_Release(gpCube);

	dwRefCount = CGameObject::Free();
	return dwRefCount;
}

//LPD3DXMESH CTestTerrain::LoadModel(const _tchar * filename)
//{
//	LPD3DXMESH ret = NULL;
//	D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, m_pGraphicDev, NULL, NULL, NULL, NULL, &ret);
//
//	return ret;
//}
