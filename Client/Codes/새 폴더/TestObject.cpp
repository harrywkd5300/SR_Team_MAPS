#include "stdafx.h"
#include "..\Headers\TestObject.h"
#include "..\Headers\Camera_Manager.h"
#include "Input_Device.h"
#include "LightObj.h"


CTestObject::CTestObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pTextureCom(nullptr)
{
}

HRESULT CTestObject::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	gpCube = LoadModel(L"../Bin/Resources/Mesh/Cube.x");

	//m_pTransformCom->Move_ToDirection(_vec3(0.f, 0.f, 1.f), 100, 1);
	//m_pTransformCom->Move_ToLook(10.f, 1);
	//m_pTransformCom->Move_ToLeft(10.f, 1);

	return NOERROR;
}

_int CTestObject::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom || nullptr == m_pTransformCom)
		return -1;

	Update_Input(fTimeDelta);


	//m_pTransformCom->Move_ToDirection(_vec3(-100.0f, +100.f, -100.0f), 2.f, fTimeDelta);

	/*Collide to Ground*/
// 	const _float& y = m_pTransformCom->Get_MatrixRow(CTransform::INFO_POSITION).y;
// 
// 	if (y < 0)
// 		y = 0;

	
	m_pTransformCom->Fill_WorldMatrix();
	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);

	return 0;
}


void CTestObject::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTransformCom || nullptr == m_pTextureCom)
		return;


	m_pTransformCom->SetUp_MatrixToGraphicDev();

	_matrix matProj, matView, matWorld;

	D3DXVECTOR4 gWorldLightPosition(0.f, -100.f, -100.f, 1.f);
	D3DXVECTOR4 gLightColor(1.f, 1.f, 1.f, 1.0f);
	D3DXVECTOR4 gWorldCameraPosition;
	D3DXVec4Normalize(&gWorldLightPosition, &gWorldLightPosition);

	_vec3 CameraPosition = CCamera_Manager::GetInstance()->Get_Camera(L"Camera_Dynamic")->Get_CameraPos();
	gWorldCameraPosition.x = CameraPosition.x;
	gWorldCameraPosition.y = CameraPosition.y;
	gWorldCameraPosition.z = CameraPosition.z;
	gWorldCameraPosition.w = 1.f;

	//D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), BACKCX / float(BACKCY), 0.2f, 1000.f);
	//matWorld = m_pTransformCom->Get_Matrix();
	D3DXMatrixIdentity(&matWorld);

	matWorld = m_pTransformCom->Get_WorldMatrix();
	
	//D3DXMatrixRotationY(&matWorld, gRotationY);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMATRIXA16 matWorldView;
	D3DXMATRIXA16 matWorldViewProjection;
	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);

	//D3DXMatrixIdentity(&matWorld);
	//matWorld = m_pTransformCom->Get_Matrix();
	m_pShaderCom->GetShader(1)->SetMatrix("worldMatrix", &matWorld);
	m_pShaderCom->GetShader(1)->SetMatrix("worldInverseTransposeMatrix", &matWorld);
	m_pShaderCom->GetShader(1)->SetMatrix("worldViewProjectionMatrix", &matWorldViewProjection);
	//m_pShaderCom->GetShader(1)->SetValue("globalAmbient", &D3DXVECTOR4(0.2f,0.2f,0.2f,1.0f), sizeof(D3DXVECTOR4));

	//m_pShaderCom->GetShader(1)->SetValue("cameraPos", &CameraPosition, sizeof(CameraPosition));
	D3DXMatrixInverse(&matView, nullptr, &matView);
	m_pShaderCom->GetShader(1)->SetValue("cameraPos", (_vec3*)&matView.m[3][0], sizeof(_vec3));
	//m_pShaderCom->GetShader(1)->SetVector("cameraPos", &gWorldCameraPosition);
	float  g_sceneAmbient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	m_pShaderCom->GetShader(1)->SetValue("globalAmbient", &g_sceneAmbient, sizeof(g_sceneAmbient));
	int g_numLights = 1;
	m_pShaderCom->GetShader(1)->SetValue("numLights", &g_numLights, sizeof(g_numLights));

	CLight* pLightCom = 0;
	const  D3DLIGHT9* pLight = 0;

	pLightCom = m_pLight[0]->GetLightCom();
	pLight = &pLightCom->Get_LightInfo();

	m_pShaderCom->GetShader(1)->SetValue("lights.pos", m_pLight[0]->GetTransformCom()->Get_MatrixRow(CTransform::ROW_INFO::INFO_POSITION), sizeof(_vec3));
	m_pShaderCom->GetShader(1)->SetValue("lights.ambient", &pLight->Ambient, sizeof(pLight->Ambient));
	m_pShaderCom->GetShader(1)->SetValue("lights.diffuse", &pLight->Diffuse, sizeof(pLight->Diffuse));
	m_pShaderCom->GetShader(1)->SetValue("lights.specular", &pLight->Specular, sizeof(pLight->Specular));
	m_pShaderCom->GetShader(1)->SetFloat("lights.radius", pLight->Range);

	m_pShaderCom->GetShader(1)->SetValue("material.Ambient", &m_pMatCom->Get_MaterialInfo().Ambient, sizeof(m_pMatCom->Get_MaterialInfo().Ambient));
	m_pShaderCom->GetShader(1)->SetValue("material.Diffuse", &m_pMatCom->Get_MaterialInfo().Diffuse, sizeof(m_pMatCom->Get_MaterialInfo().Diffuse));
	m_pShaderCom->GetShader(1)->SetValue("material.Emissive", &m_pMatCom->Get_MaterialInfo().Emissive, sizeof(m_pMatCom->Get_MaterialInfo().Emissive));
	m_pShaderCom->GetShader(1)->SetValue("material.Specular", &m_pMatCom->Get_MaterialInfo().Specular, sizeof(m_pMatCom->Get_MaterialInfo().Specular));
	m_pShaderCom->GetShader(1)->SetFloat("material.Power", m_pMatCom->Get_MaterialInfo().Power);


	m_pShaderCom->GetShader(1)->SetTexture("colorMapTexture", m_pTextureCom->GetTexture(1));
	m_pShaderCom->GetShader(1)->SetTexture("normalMapTexture", m_pTextureCom->GetTexture(3));

	static _uint numPasses = 0;

	m_pShaderCom->GetShader(1)->Begin(&numPasses, NULL);
	{
		for (_uint i = 0; i < numPasses; ++i)
		{
			m_pShaderCom->GetShader(1)->BeginPass(i);
			{
				//m_pTextureCom->SetUp_TextureToGraphicDev(0, 0);
				//m_pTransformCom->SetUp_MatrixToGraphicDev();
				m_pBufferCom->Render_Buffer();

			}
			m_pShaderCom->GetShader(1)->EndPass();
		}
	}
	m_pShaderCom->GetShader(1)->End();

	return;
}

//
//void CTestObject::Render_GameObject()
//{
//	if (nullptr == m_pBufferCom || nullptr == m_pTransformCom)
//		return;
//
//	_matrix matProj, matView, matWorld;
//
//	D3DXVECTOR4	gWorldLightPosition(100.0f, 100.0f, -300.0f, 1.0f);
//	//D3DXVECTOR4 gWorldLightPosition(0.f, 100.f, 0.f, 1.f);
//	D3DXVECTOR4 gLightColor(1.f, 1.f, 1.f, 1.0f);
//	//D3DXVECTOR4				gWorldCameraPosition(0.0f, 0.0f, -200.0f, 1.0f);
//	D3DXVECTOR4 gWorldCameraPosition;
//	//D3DXVec4Normalize(&gWorldLightPosition, &gWorldLightPosition);
//
//	_vec3 CameraPosition = CCamera_Manager::GetInstance()->Get_Camera(L"Camera_Dynamic")->Get_CameraPos();
//	gWorldCameraPosition.x = CameraPosition.x;
//	gWorldCameraPosition.y = CameraPosition.y;
//	gWorldCameraPosition.z = CameraPosition.z;
//	gWorldCameraPosition.w = 1.f;
//	
//
//	//D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), BACKCX / float(BACKCY), 0.2f, 1000.f);
//	//matWorld = m_pTransformCom->Get_Matrix();
//
//	//D3DXMATRIXA16 matView;
//	//D3DXVECTOR3 vEyePt(gWorldCameraPosition.x, gWorldCameraPosition.y, gWorldCameraPosition.z);
//	//D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
//	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
//	//D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
//
//	// 투영행렬을 만든다.
//	//D3DXMATRIXA16			matProj;
//	//D3DXMatrixPerspectiveFovLH(&matProj, (D3DX_PI / 4.0f), BACKCX/float(BACKCY), 0.2f, 1000);
//
//	//static _float gRotationY = 0.f;
//	//gRotationY += 0.4f * D3DX_PI / 180.0f;
//	//if (gRotationY > 2 * D3DX_PI)
//	//{
//	//	gRotationY -= 2 * D3DX_PI;
//	//}
//	_matrix matSize;
//	D3DXMatrixScaling(&matSize, 0.1f, 0.1f, 0.1f);
//	D3DXMatrixIdentity(&matWorld);
//	matWorld = matSize * m_pTransformCom->Get_Matrix();
//	
//	//D3DXMatrixRotationY(&matWorld, gRotationY);
//
//	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
//	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
//
//
//	D3DXMATRIXA16 matWorldView;
//	D3DXMATRIXA16 matWorldViewProjection;
//	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
//	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);
//
//	m_pShaderCom->GetShader(1)->SetMatrix("gWorldMatrix", &matWorld);
//	m_pShaderCom->GetShader(1)->SetMatrix("gWorldViewProjectionMatrix", &matWorldViewProjection);
//
//	m_pShaderCom->GetShader(1)->SetVector("gWorldLightPosition", &gWorldLightPosition);
//	m_pShaderCom->GetShader(1)->SetVector("gWorldCameraPosition", &gWorldCameraPosition);
//
//	m_pShaderCom->GetShader(1)->SetVector("gLightColor", &gLightColor);
//	m_pShaderCom->GetShader(1)->SetTexture("DiffuseMap_Tex", m_pTextureCom->GetTexture(1));
//	m_pShaderCom->GetShader(1)->SetTexture("SpecularMap_Tex", m_pTextureCom->GetTexture(2));
//	m_pShaderCom->GetShader(1)->SetTexture("NormalMap_Tex", m_pTextureCom->GetTexture(3));
//
//	_uint numPasses = 0;
//
//	m_pShaderCom->GetShader(1)->Begin(&numPasses, NULL);
//	{
//		for (_uint i = 0; i < numPasses; ++i)
//		{
//			m_pShaderCom->GetShader(1)->BeginPass(i);
//			{
//				//m_pTransformCom->SetUp_MatrixToGraphicDev();
//				m_pBufferCom->Render_Buffer();
//				//gpCube->DrawSubset(0);
//			}
//			m_pShaderCom->GetShader(1)->EndPass();
//		}
//	}
//	m_pShaderCom->GetShader(1)->End();
//}

LPD3DXMESH CTestObject::LoadModel(const _tchar * filename)
{
	LPD3DXMESH ret = NULL;
	D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM,m_pGraphicDev, NULL, NULL, NULL, NULL, &ret);

	return ret;
}

HRESULT CTestObject::Ready_Component()
{
	CComponent* pComponent = nullptr;

	/*Add TempCube*/
	pComponent = m_pBufferCom = dynamic_cast<CCube_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeTex"));
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Buffer_Cube", pComponent));
	m_pBufferCom->AddRef();

	/*Add Transform*/
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
	m_pTransformCom->AddRef();

	/*Add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();

	/*Add Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Player"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Player", pComponent));
	m_pTextureCom->AddRef();

	/*Add Shader*/
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Shader_Player"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Shader_Player", pComponent));
	m_pShaderCom->AddRef();

	/*Add Material*/
	pComponent = m_pMatCom = dynamic_cast<CMaterial*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Material"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Material_Player", pComponent));
	m_pMatCom->SetUp_MaterialInfo(D3DXCOLOR(1.f, 1.f, 1.f, 1.f), D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), D3DXCOLOR(0.f, 0.f, 0.f, 0.f), 10.f);
	m_pMatCom->AddRef();

	return NOERROR;

exception:
	MSG_BOX("TempObject Component Ready Failed err: 13116");
	return E_FAIL;
}

void CTestObject::Update_Input(const _float fTimeDelta)
{
	if (nullptr == m_pInput_Device)
		return;

	if (m_pInput_Device->Get_DIKeyState(DIK_I) & 0x80)
		m_pTransformCom->Move_ToLook(10.f, fTimeDelta);

	if (m_pInput_Device->Get_DIKeyState(DIK_J) & 0x80)
		m_pTransformCom->Move_ToLeft(10.f, fTimeDelta);

	if (m_pInput_Device->Get_DIKeyState(DIK_L) & 0x80)
		m_pTransformCom->Move_ToRight(10.f, fTimeDelta);

	if (m_pInput_Device->Get_DIKeyState(DIK_K) & 0x80)
		m_pTransformCom->Move_ToLook(-10.f, fTimeDelta);

	if (m_pInput_Device->Get_DIKeyState(DIK_Q) & 0x80)
	{
		_float f = 10.f * fTimeDelta;
		m_pTransformCom->Rotation_X(f);
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_E) & 0x80)
	{
		_float f = 10.f * fTimeDelta;
		m_pTransformCom->Rotation_Y(f);
	}


	///*MouseMove*/
	//const _long& lMoveX = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_X);
	//const _long& lMoveY = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_Y);

	//if (lMoveY)
	//{
	//	_float f = lMoveY * 10.f;
	//	m_pTransformCom->Rotation_X(f);
	//}

	//if (lMoveX)
	//{
	//	_float f = lMoveX * 10.f;
	//	m_pTransformCom->Rotation_Y(f);
	//}
}

CTestObject * CTestObject::Create(LPDIRECT3DDEVICE9 pGrahpicDev)
{
	CTestObject* pInstance = new CTestObject(pGrahpicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CTestObject Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CTestObject::Free()
{
	//decrease refcount
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pMatCom);

	Safe_Release(gpCube);

	return CGameObject::Free();
}
