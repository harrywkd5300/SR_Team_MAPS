#include "stdafx.h"
#include "..\Headers\Tool_Cube.h"
#include "input_device.h"
#include "Camera_Manager.h"


CTool_Cube::CTool_Cube(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iDrawID, const TCHAR* pKey)
	:CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_iTextureNum(iDrawID)
	, m_bPickingCube(true)
{
	m_pObjectData = new STROBJECT;
	m_pKey = pKey;
	lstrcpy(m_pObjectData->m_strKey, m_pKey);
	m_pObjectData->m_iTextureNum = iDrawID;
}

HRESULT CTool_Cube::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CTool_Cube::Update_GameObject(const _float & fTimeDelta)
{
	if (true == m_bIsDead)
		return 1;

	m_pTransformCom->Fill_WorldMatrix();

	m_pObjectData->m_vecPos = m_pTransformCom->Get_MatrixRow(m_pTransformCom->INFO_POSITION);
	m_pObjectData->m_vecRot = _vec3(m_pTransformCom->Get_DegreeX(), m_pTransformCom->Get_DegreeY(), m_pTransformCom->Get_DegreeZ());
	m_pObjectData->m_vecScale = _vec3(m_pTransformCom->Get_ScaleMatrix()._11, m_pTransformCom->Get_ScaleMatrix()._22, m_pTransformCom->Get_ScaleMatrix()._33);


	return 0;
}

void CTool_Cube::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTextureCom || nullptr == m_pTransformCom)
		return;

	if (m_bPickingCube)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pTextureCom->SetUp_TextureToGraphicDev(0, m_iTextureNum);
	m_pTransformCom->SetUp_MatrixToGraphicDev();

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return;
}

HRESULT CTool_Cube::Ready_Component()
{
	CComponent* pComponent = nullptr;
	//
	/*======Add Static Component=========*/
	//

	/*Add Transform*/
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
	if (nullptr == m_pTransformCom)
		MSG_BOX("Transform Clone Failed err:50624");
	m_pTransformCom->AddRef(); 

	/*add terrain buffer*/
	pComponent = m_pBufferCom = dynamic_cast<CCube_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_Cube"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Buffer_Cube", pComponent));
	if (nullptr == m_pBufferCom)
		MSG_BOX("Buffer_Cube Clone Failed err:50624");
	m_pBufferCom->AddRef();

	/*Add Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Cube"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Cube", pComponent));
	if (nullptr == m_pTextureCom)
		MSG_BOX("Texture_Cube Clone Failed err:50624");
	m_pTextureCom->AddRef();

	return NOERROR;
}


void CTool_Cube::TransForm(_vec3 vPos, _vec3 vRota, _vec3 vScale)
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

 	m_pTransformCom->SetUp_MatrixToGraphicDev();
}

void CTool_Cube::MoveCube(MOVECUBE m_MoveKey, _vec3 vData)
{
	switch (m_MoveKey)
	{
	case POS:
		m_pTransformCom->Translation(vData);
		break;
	case SCALE:
		m_pTransformCom->Scaling(vData);
		break;
	case ROTATION:
		if (vData.x != 0)
		{
			m_pTransformCom->Rotation_X(vData.x);
		}
		if (vData.y != 0)
		{
			m_pTransformCom->Rotation_Y(vData.y);
		}
		if (vData.z != 0)
		{
			m_pTransformCom->Rotation_Z(vData.z);
		}
		break;
	}
}


CTool_Cube * CTool_Cube::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vPos, _vec3 vRota, _vec3 vScale, const _uint& iDrawID, const TCHAR* pKey)
{
	CTool_Cube* pInstance = new CTool_Cube(pGraphicDev, iDrawID, pKey);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("Create Terrain failed");
		Safe_Release(pInstance);
	}

	pInstance->TransForm(vPos, vRota, vScale);
	
	return pInstance;
}

_ulong CTool_Cube::Free()
{
	_ulong dwRefCount = 0;

	Safe_Delete(m_pObjectData);

	/*decrease refcount*/
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	dwRefCount = CGameObject::Free();

	return dwRefCount;
}
