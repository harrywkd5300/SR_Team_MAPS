#include "stdafx.h"
#include "..\Headers\Tool_Terrian.h"
#include "Camera_Manager.h"


CTerrian::CTerrian(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pShaderCom(nullptr)
{
}
HRESULT CTerrian::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	return NOERROR;
}

_int CTerrian::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Fill_WorldMatrix();
	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA,this);

	return 0;
}

void CTerrian::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTextureCom || nullptr == m_pTransformCom)
		return;

	_matrix matProj, matView, matWorld;


	D3DXVECTOR4 gWorldLightPosition(0.f, -100.f, -100.f, 1.f);
	D3DXVECTOR4 gLightColor(1.f, 1.f, 1.f, 1.0f);
	D3DXVECTOR4 gWorldCameraPosition;
	D3DXVec4Normalize(&gWorldLightPosition, &gWorldLightPosition);

	_vec3 CameraPosition = CCamera_Manager::GetInstance()->Get_Camera(L"Layer_Camera")->Get_CameraPos();
	gWorldCameraPosition.x = CameraPosition.x;
	gWorldCameraPosition.y = CameraPosition.y;
	gWorldCameraPosition.z = CameraPosition.z;
	gWorldCameraPosition.w = 1.f;

	//D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), BACKCX / float(BACKCY), 0.2f, 1000.f);
	//matWorld = m_pTransformCom->Get_Matrix();
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMATRIXA16 matWorldView;
	D3DXMATRIXA16 matWorldViewProjection;
	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);

	m_pShaderCom->GetShader(1)->SetMatrix("gWorldMatrix", &matWorld);
	m_pShaderCom->GetShader(1)->SetMatrix("gWorldViewProjectionMatrix", &matWorldViewProjection);

	m_pShaderCom->GetShader(1)->SetVector("gWorldLightPosition", &gWorldLightPosition);
	m_pShaderCom->GetShader(1)->SetVector("gWorldCameraPosition", &gWorldCameraPosition);

	m_pShaderCom->GetShader(1)->SetVector("gLightColor", &gLightColor);
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
				//m_pTextureCom->SetUp_TextureToGraphicDev(0, 0);
				m_pTransformCom->SetUp_MatrixToGraphicDev();
				m_pBufferCom->Render_Buffer();
			}
			m_pShaderCom->GetShader(1)->EndPass();
		}
	}
	m_pShaderCom->GetShader(1)->End();
	return;
}

HRESULT CTerrian::Ready_Component()
{
	CComponent* pComponent = nullptr;
	//
	/*======Add Static Component=========*/
	//
	/*add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	if (nullptr == m_pRendererCom)
		MSG_BOX("Renderer Clone Failed err:50624");
	m_pRendererCom->AddRef();

	/*Add Transform*/
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
	if (nullptr == m_pTransformCom)
		MSG_BOX("Transform Clone Failed err:50624");
	m_pTransformCom->AddRef();

	/*add terrain buffer*/
	pComponent = m_pBufferCom = dynamic_cast<CTerrain_Buffer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_Terrain"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Buffer_Terrain", pComponent));
	if (nullptr == m_pBufferCom)
		MSG_BOX("Terrain_Buffer Clone Failed err:50624");
	m_pBufferCom->AddRef();

	/*Add Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Terrain"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Terrain", pComponent));
	if (nullptr == m_pTextureCom)
		MSG_BOX("Texture Clone Failed err:50624");
	m_pTextureCom->AddRef();
	
	/*Add Shader*/
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Shader_Terrain"));
	if (nullptr == pComponent)
		MSG_BOX("Shader Clone Failed err:50624");
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Shader_Terrain", pComponent));
	m_pShaderCom->AddRef();

	return NOERROR;
}


CTerrian * CTerrian::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrian* pInstance = new CTerrian(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("Create Terrain failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CTerrian::Free()
{
	_ulong dwRefCount = 0;

	/*decrease refcount*/
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);


	dwRefCount = CGameObject::Free();
	return dwRefCount;
}


