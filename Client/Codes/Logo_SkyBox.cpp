#include "stdafx.h"
#include "..\Headers\Logo_SkyBox.h"

#include "component_Manager.h"
#include "Camera_Manager.h"

CLogo_SkyBox::CLogo_SkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pCamera_Manager(CCamera_Manager::GetInstance())
{
	//m_pCamera_Manager->AddRef();
}


HRESULT CLogo_SkyBox::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CLogo_SkyBox::Update_GameObject(const _float & fTimeDelta)
{
	m_pRendererCom->Add_RenderList(CRenderer::RENDER_PRIORITY, this);

	return 0;
}

_int CLogo_SkyBox::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	const _matrix& matCamera = m_pCamera_Manager->Get_CurCamera()->Get_CameraWorldMatrix();

	const _vec3& vPos = (_vec3&)matCamera.m[3];
	m_pTransformCom->Translation(vPos);
	m_pTransformCom->Scaling(2.f, 2.f, 2.f);
	m_pTransformCom->Rotation_Y(0.1f);
	m_pTransformCom->Fill_WorldMatrix();

	return 0;
}

// _int CSkyBox::LastUpdate_GameObject(const _float & fTimeDelta)
// {
// 	if (nullptr == m_pTransformCom)
// 		return -1;
// 
// 	_matrix matCamera;
// 
// 	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamera);
// 	D3DXMatrixInverse(&matCamera, nullptr, &matCamera);
// 
// 	const _vec3& vPos = (_vec3&)matCamera.m[3];
// 	m_pTransformCom->Translation(vPos);
// 	m_pTransformCom->Scaling(2.f, 2.f, 2.f);
// 
// 
// 	m_pTransformCom->Fill_WorldMatrix();
// 
// 	return 0;
// }


void CLogo_SkyBox::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTextureCom || nullptr == m_pTransformCom)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pTextureCom->SetUp_TextureToGraphicDev(0, 0);
	m_pTransformCom->SetUp_MatrixToGraphicDev();

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CLogo_SkyBox::Ready_Component()
{
	CComponent* pComponent = nullptr;

	/*======Add Notify Component=========*/

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

	/* add Buffer_CubeTex*/
	pComponent = m_pBufferCom = dynamic_cast<CCube_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeTex"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Buffer_CubeTex", pComponent));
	m_pBufferCom->AddRef();

	/*Add Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_LOGO, L"Component_Texture_SkyBox"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_SkyBox", pComponent));
	m_pTextureCom->AddRef();

	return NOERROR;

exception:
	return E_FAIL;
}


CLogo_SkyBox * CLogo_SkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo_SkyBox* pInstance = new CLogo_SkyBox(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("Create SkyBox failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CLogo_SkyBox::Free()
{
	/*decrease refcount*/
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	//Safe_Release(m_pCamera_Manager);

	return CGameObject::Free();
}
