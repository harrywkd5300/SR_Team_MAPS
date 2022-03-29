#include "stdafx.h"
#include "..\Headers\Cube.h"
#include "Camera_Manager.h"
#include "Collider_Manager.h"

CCube::CCube(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iDrawID, const TCHAR* pKey, CCube_Texture* pBufferCom)
	:CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	//, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_iTextureNum(iDrawID)
	, m_pColliderMgr(CCollider_Manager::GetInstance())
{
	m_pKey = (LPWSTR)pKey;
	m_pBufferCom = pBufferCom;
	m_pBufferCom->AddRef();
	m_pColliderMgr->AddRef();
}

HRESULT CCube::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pColliderBox_AABB->Set_Position(m_pTransformCom->Get_Position());
	m_pColliderBox_AABB->Update_ColliderBox_VertexBuffer();
	//m_pBufferCom->Set_Position(m_pTransformCom->Get_WorldMatrix());
	return NOERROR;
}

_int CCube::Update_GameObject(const _float & fTimeDelta)
{
	if (true == m_bIsDead)
		return 1;
	if (nullptr == m_pTransformCom)
		return 0;
	m_pTransformCom->Fill_WorldMatrix();
	return 0;
}

_int CCube::LastUpdate_GameObject(const _float & fTimeDelta)
{
	//_matrix matWorld, matScale;
	//D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f);
	//D3DXMatrixTranslation(&matWorld, m_pTransformCom->Get_PositionX(), m_pTransformCom->Get_PositionY(), m_pTransformCom->Get_PositionZ());
	//matWorld = matScale * matWorld;
	//m_pColliderBox_AABB->Set_Position(matWorld);
	// -------------- Test -----------------------
	//m_pColliderBox_AABB->Set_Position(m_pTransformCom->Get_WorldMatrix());
	//m_pColliderMgr->Add_BackGround(m_pColliderBox_AABB);
	// -------------- Test -----------------------
	//m_pBufferCom->Set_Position(m_pTransformCom->Get_WorldMatrix());
	//m_pColliderMgr->Add_BackGround(m_pBufferCom);
	//m_pColliderBox_AABB->Set_Position(m_pTransformCom->Get_Position());
	//m_pColliderBox_AABB->Update_ColliderBox_VertexBuffer();

	//m_pColliderMgr->Add_BackGround(m_pColliderBox_AABB);

	return 0;
}

void CCube::Render_GameObject()
{
	if (nullptr == m_pBufferCom  || nullptr == m_pTransformCom)
		return;
	_matrix matProj, matView, matWorld;

	matWorld = m_pTransformCom->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_matrix matWorldView;
	_matrix matWorldViewProjection;
	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);

	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorld", &matWorld);
	m_pShaderCom->GetShader(1)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
	m_pShaderCom->GetShader(1)->CommitChanges();

	m_pBufferCom->Render_Buffer();

	//_matrix matWorld2, matScale;
	//D3DXMatrixIdentity(&matWorld2);
	//D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f);
	//D3DXMatrixTranslation(&matWorld2, m_pTransformCom->Get_PositionX(), m_pTransformCom->Get_PositionY(), m_pTransformCom->Get_PositionZ());
	//matWorld2 = matScale * matWorld2;
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld2);

	//m_pColliderBox_AABB->Render_Debug_Buffer();

	return;
}

void CCube::Render_Shadow(_matrix ShadowWorld)
{
	if (nullptr == m_pBufferCom  || nullptr == m_pTransformCom)
		return;
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pTextureCom->SetUp_TextureToGraphicDev(0, m_iTextureNum);
	//m_pTransformCom->SetUp_MatrixToGraphicDev();
	_matrix matWorld = m_pTransformCom->Get_WorldMatrix();
	matWorld = matWorld * ShadowWorld;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pBufferCom->Render_Buffer();
	m_pColliderBox_AABB->Render_Buffer();
	return;

}

HRESULT CCube::Ready_Component()
{
	CComponent* pComponent = nullptr;
	//
	/*======Add Static Component=========*/
	/*Add Transform*/
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
	if (nullptr == m_pTransformCom)
		MSG_BOX("Transform Clone Failed err:50624");
	m_pTransformCom->AddRef(); 


	///*Add Texture*/
	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Cube"));
	//m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Cube", pComponent));
	//if (nullptr == m_pTextureCom)
	//	MSG_BOX("Texture_Cube Clone Failed err:50624");
	//m_pTextureCom->AddRef();

	/*Add Shader*/
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Shader_Cube"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Shader_Cube", pComponent));
	m_pShaderCom->AddRef();


	// -------------- Test -----------------------
	/*Add ColliderBox_AABB*/
	pComponent = m_pColliderBox_AABB = dynamic_cast<CAABB_Buffer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_ColliderBox_AABB"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"ColliderBox_AABB", pComponent));
	m_pColliderBox_AABB->AddRef();
	// -------------- Test -----------------------

	return NOERROR;

exception:
	MSG_BOX("Player Component Ready Failed err: 13115");
	return E_FAIL;
}


void CCube::TransForm(_vec3 vPos, _vec3 vRota, _vec3 vScale)
{
	m_pTransformCom->Translation(vPos);
	m_pTransformCom->Scaling(vScale);
	if (vRota.x > 0)
	{
		m_pTransformCom->Rotation_X(vRota.x);
	}
	if (vRota.y > 0)
	{
		m_pTransformCom->Rotation_Y(vRota.y);
	}
	if (vRota.z > 0)
	{
		m_pTransformCom->Rotation_Z(vRota.z);
	}
}


CCube * CCube::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vPos, _vec3 vRota, _vec3 vScale, const _uint& iDrawID, const TCHAR* pKey, CCube_Texture* pBufferCom)
{
	CCube* pInstance = new CCube(pGraphicDev, iDrawID, pKey, pBufferCom);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("Create Terrain failed");
		Safe_Release(pInstance);
	}

	pInstance->TransForm(vPos, vRota, vScale);

	return pInstance;
}


_ulong CCube::Free()
{
	_ulong dwRefCount = 0;

	/*decrease refcount*/
	dwRefCount = Safe_Release(m_pBufferCom);
	//dwRefCount = Safe_Release(m_pTextureCom);
	dwRefCount = Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderMgr);
	// -------------- Test -----------------------
	Safe_Release(m_pColliderBox_AABB);
	// -------------- Test -----------------------
	dwRefCount = CGameObject::Free();
	return dwRefCount;
}
