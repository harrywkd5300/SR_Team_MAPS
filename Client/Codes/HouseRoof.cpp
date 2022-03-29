#include "stdafx.h"
#include "..\Headers\HouseRoof.h"

#include "Cube.h"
#include "camera_manager.h"
#include "Input_Device.h"


CHouseRoof::CHouseRoof(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pRendererCom(nullptr)
	, m_pBufferCom(nullptr)
{
}


void CHouseRoof::Test()
{
	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_H))
	{
		for (vector<CGameObject*>::iterator iter = m_pCubeObj.begin(); iter != m_pCubeObj.end();)
		{
			CCube* pCube = dynamic_cast<CCube*>(*iter);

			if (nullptr == pCube)
				continue;

			_float fz = pCube->Get_Transform()->Get_PositionZ();

			if (fz < 60.f)
			{
				Safe_Release(*iter);
				iter = m_pCubeObj.erase(iter);
			}
			else
				++iter;
		}

	}
}

HRESULT CHouseRoof::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_CubeLoad()))
		return E_FAIL;

	return NOERROR;
}

_int CHouseRoof::Update_GameObject(const _float & fTimeDelta)
{
	if (true == m_bIsDead)
		return 1;

	if (nullptr == m_pRendererCom)
		return -1;

	Test();

	if (m_pCubeObj.empty())
		return 0;

	auto iter = m_pCubeObj.begin();
	for (; iter != m_pCubeObj.end(); ++iter)
	{
		dynamic_cast<CCube*>((*iter))->Update_GameObject(fTimeDelta);
	}

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_BACKGROUND, this);

	return 0;
}

void CHouseRoof::Render_GameObject(void)
{
	if (nullptr == m_pRendererCom)
		return;

	if (m_pCubeObj.empty())
		return;

	SetShader(1);
	StartShader(1);

	auto iter = m_pCubeObj.begin();
	for (; iter != m_pCubeObj.end(); ++iter)
	{
		dynamic_cast<CCube*>((*iter))->Render_GameObject();
	}

	EndShader(1);
}

void CHouseRoof::Render_Shadow(_matrix CameraWorldMatrix)
{
	D3DXMATRIX	matShadow;
	D3DXVECTOR3 vLightPos = (_vec3)m_pLightMgr->GetLight(LIGHT_TYPE::DIRECTIONAL_LIGHT)->Get_LightInfo().Position;
	D3DXPLANE PlaneFloor(0.f, 1.f, 0.f, 0.f);  // 평면의 방정식	
	D3DXMatrixShadow(&matShadow, &D3DXVECTOR4(0.f, 500.f, 0.f, 1.f), &PlaneFloor);

	if (nullptr == m_pRendererCom)
		return;
	//m_pTransformCom->SetUp_MatrixToGraphicDev();

	if (m_pCubeObj.empty())
		return;

	auto iter = m_pCubeObj.begin();
	for (; iter != m_pCubeObj.end(); ++iter)
	{
		dynamic_cast<CCube*>((*iter))->Render_Shadow(matShadow);
	}
}

void CHouseRoof::SetShader(_uint iShaderNum)
{
	_matrix matProj, matView, matWorld;

	_vec3 CameraPosition = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_CameraPos();

	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_vWorldCameraPos", &CameraPosition, sizeof(CameraPosition));

	CLight_Manager::GetInstance()->Set_ShaderLightList(m_pShaderCom->GetShader(iShaderNum));

	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Ambient", &m_pMatCom->Get_MaterialInfo().Ambient, sizeof(m_pMatCom->Get_MaterialInfo().Ambient));
	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Diffuse", &m_pMatCom->Get_MaterialInfo().Diffuse, sizeof(m_pMatCom->Get_MaterialInfo().Diffuse));
	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Emissive", &m_pMatCom->Get_MaterialInfo().Emissive, sizeof(m_pMatCom->Get_MaterialInfo().Emissive));
	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Specular", &m_pMatCom->Get_MaterialInfo().Specular, sizeof(m_pMatCom->Get_MaterialInfo().Specular));
	m_pShaderCom->GetShader(iShaderNum)->SetFloat("g_Material.fPower", m_pMatCom->Get_MaterialInfo().Power);


	m_pShaderCom->GetShader(iShaderNum)->SetTexture("DiffuseMap_Tex", m_pTextureCom->GetTexture(1));
	m_pShaderCom->GetShader(iShaderNum)->SetTexture("SpecularMap_Tex", m_pTextureCom->GetTexture(2));
	m_pShaderCom->GetShader(iShaderNum)->SetTexture("NormalMap_Tex", m_pTextureCom->GetTexture(3));

}

void CHouseRoof::StartShader(_uint iShaderNum)
{
	_uint numPasses = 0;
	m_pShaderCom->GetShader(iShaderNum)->Begin(&numPasses, NULL);
	m_pShaderCom->GetShader(iShaderNum)->BeginPass(0);
}

void CHouseRoof::EndShader(_uint iShaderNum)
{
	m_pShaderCom->GetShader(iShaderNum)->EndPass();
	m_pShaderCom->GetShader(iShaderNum)->End();
}

HRESULT CHouseRoof::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;

	/*Add Renderer*/
	pComponent = m_pRendererCom = dynamic_cast<CRenderer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRendererCom->AddRef();

	/*add Cube buffer*/
	pComponent = m_pBufferCom = dynamic_cast<CCube_Texture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeTex"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Buffer_Cube", pComponent));
	if (nullptr == m_pBufferCom)
		MSG_BOX("Buffer_Cube Clone Failed err:50624");
	m_pBufferCom->AddRef();

	/*add Cube Texture*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Texture_Cube"));
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture_Cube", pComponent));
	if (nullptr == m_pTextureCom)
		MSG_BOX("Texture_Cube Clone Failed err:50624");
	m_pTextureCom->AddRef();

	/*Add Shader*/
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Shader_Cube"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Shader_Cube", pComponent));
	m_pShaderCom->AddRef();

	/*Add Material*/
	pComponent = m_pMatCom = dynamic_cast<CMaterial*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Material"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Material_Player", pComponent));
	m_pMatCom->SetUp_MaterialInfo(CMaterial::MATTYPE::MATTYPE_SHINY);
	m_pMatCom->AddRef();


	return NOERROR;

exception:
	MSG_BOX("Effect Component Ready Failed err: 13115");
	return E_FAIL;
}

HRESULT CHouseRoof::Ready_CubeLoad()
{
	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../../Data/MainMapTop.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,		//기본값으로 파일을 생성하라는옵션.
		NULL
	);

	auto& iter = m_pCubeObj.begin();
	for (; iter != m_pCubeObj.end(); ++iter)
	{
		Safe_Release((*iter));
	}
	m_pCubeObj.clear();

	while (true)
	{
		STROBJECT* pObjectData = new STROBJECT;

		ZeroMemory(pObjectData, sizeof(STROBJECT));

		ReadFile(hFile, pObjectData, sizeof(STROBJECT), &dwByte, NULL);

		if (dwByte == 0)
		{
			Safe_Delete(pObjectData);
			break;
		}
		///*Create Cube*/
		//CGameObject* pObject = CCube::Create(m_pGraphicDev, pObjectData->m_vecPos, pObjectData->m_vecRot, pObjectData->m_vecScale,
		//	pObjectData->m_iTextureNum, pObjectData->m_strKey, m_pBufferCom);
		/*Create Cube*/
		CGameObject* pObject = CCube::Create(m_pGraphicDev, pObjectData->m_vecPos, pObjectData->m_vecRot, pObjectData->m_vecScale,
			3, pObjectData->m_strKey, m_pBufferCom);
		if (nullptr == pObject)
		{
			MSG_BOX("Add_GameObject failed on Scene_Logo err:50620");
			return E_FAIL;
		}
		m_pCubeObj.push_back(pObject);
	}
	CloseHandle(hFile);
	return NOERROR;
}

CHouseRoof * CHouseRoof::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHouseRoof*	pInstance = new CHouseRoof(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CHouseRoof Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CHouseRoof::Free(void)
{
	_ulong		dwRefCnt = 0;

	auto& iter = m_pCubeObj.begin();
	
	for (; iter != m_pCubeObj.end(); ++iter)
	{
		dwRefCnt += Safe_Release((*iter));
	}

	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMatCom);
	Safe_Release(m_pTextureCom);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
