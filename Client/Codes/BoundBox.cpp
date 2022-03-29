#include "stdafx.h"
#include "..\Headers\BoundBox.h"


CBoundBox::CBoundBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
{
}

HRESULT CBoundBox::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CBoundBox::Update_GameObject(const _float & fTimeDelta)
{
	if (true == m_bIsDead)
		return 1;
	if (nullptr == m_pTransformCom)
		return 0;
	m_pTransformCom->Fill_WorldMatrix();
	return 0;
}

void CBoundBox::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTransformCom)
		return;
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pTransformCom->SetUp_MatrixToGraphicDev();
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

HRESULT CBoundBox::Ready_Component()
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

	/*add Cube buffer*/
	pComponent = m_pBufferCom = dynamic_cast<CCube_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeTex"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Buffer_Cube", pComponent));
	if (nullptr == m_pBufferCom)
		MSG_BOX("Buffer_Cube Clone Failed err:50624");
	m_pBufferCom->AddRef();

	return NOERROR;
}

CBoundBox * CBoundBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vScalse)
{
	CBoundBox* pInstance = new CBoundBox(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("Create CBoundBox failed");
		Safe_Release(pInstance);
	}

	//pInstance->TransForm(vPos, vRota, vScale);

	return pInstance;
}

_ulong CBoundBox::Free()
{
	_ulong dwRefCount = 0;

	/*decrease refcount*/
	dwRefCount = Safe_Release(m_pBufferCom);
	dwRefCount = Safe_Release(m_pTransformCom);

	dwRefCount = CGameObject::Free();
	return dwRefCount;
}

