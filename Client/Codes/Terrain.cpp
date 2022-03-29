#include "stdafx.h"
#include "..\Headers\Terrain.h"
#include "..\Headers\Camera_Manager.h"
#include "LightObj.h"
#include "Light_Manager.h"


CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	,m_pBufferCom(nullptr)
	,m_pTextureCom(nullptr)
	,m_pTransformCom(nullptr)
	,m_pRendererCom(nullptr)
	,m_pMatCom(nullptr)
{
}

HRESULT CTerrain::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CTerrain::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Fill_WorldMatrix();

	m_pRendererCom->Add_RenderList(CRenderer::RENDERTYPE::RENDER_BACKGROUND, this);
	return 0;
}

void CTerrain::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTextureCom || nullptr == m_pTransformCom)
		return;

// 	m_pTextureCom->SetUp_TextureToGraphicDev(0, 0);
// 	m_pTransformCom->SetUp_MatrixToGraphicDev();

	_matrix matProj, matView, matWorld;

	D3DXVECTOR4 gWorldCameraPosition;

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

				//m_pTransformCom->SetUp_MatrixToGraphicDev();
				m_pBufferCom->Render_Buffer();
			}
			m_pShaderCom->GetShader(1)->EndPass();
		}
	}
	m_pShaderCom->GetShader(1)->End();
	//m_pBufferCom->Render_Buffer();

	return;
}

HRESULT CTerrain::Ready_Component()
{
	CComponent* pComponent = nullptr;

	/*======Add Static Component=========*/

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
	pComponent = m_pBufferCom = dynamic_cast<CTerrain_Buffer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Buffer_Terrain"));
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

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain* pInstance = new CTerrain(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("Create Terrain failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CTerrain::Free()
{
	_ulong dwRefCount = 0;

	/*decrease refcount*/
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMatCom);


	dwRefCount = CGameObject::Free();
	return dwRefCount;
}
