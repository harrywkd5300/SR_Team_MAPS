#include "stdafx.h"
#include "..\Headers\LightObj.h"
#include "Input_Device.h"
#include "Light_Manager.h"


CLightObj::CLightObj(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pLightCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pSphere(nullptr)
{
}

HRESULT CLightObj::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(D3DXCreateSphere(m_pGraphicDev, LIGHT_OBJECT_RADIUS,
		LIGHT_OBJECT_SLICES, LIGHT_OBJECT_STACKS, &m_pSphere, 0)))
		return E_FAIL;

	m_pLightMgr = CLight_Manager::GetInstance();
	m_pLightMgr->AddRef();
	//m_pTransformCom->Set_MatrixRow(_vec3(20.0f, 50.0f, 20.0f), Engine::CTransform::INFO_POSITION);
	//m_pTransformCom->Move_ToDirection(_vec3(100.0f, 50.0f , 100.0f), 20.f, 1);
	//m_pTransformCom->Move_ToLook(10.f, 1);
	//m_pTransformCom->Move_ToDirection(_vec3(100.0f, 50.0f, 100.0f), 30.f, 1);
	//m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);
	return NOERROR;
}

_int CLightObj::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom || nullptr == m_pTransformCom)
		return -1;

	//Update_Input(fTimeDelta);
	//m_pTransformCom->Move_ToDirection(_vec3(-100.0f, -100.f, -100.0f), 2.f, fTimeDelta);
	//m_pTransformCom->Move_ToDirection(_vec3(100.0f, 50.0f, 100.0f), 20.f, 1);

	m_pTransformCom->Fill_WorldMatrix();
	m_pLightCom->SetUp_LightPosition(m_pTransformCom->Get_MatrixRow(CTransform::ROW_INFO::INFO_POSITION));
	//m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);
	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);
	m_pLightMgr->Add_RenderLight(m_pLightCom, LIGHT_TYPE::POINT_LIGHT);

	return 0;
}

void CLightObj::Render_GameObject()
{
	if (nullptr == m_pTransformCom)
		return;

	_matrix matProj, matView, matWorld;

	D3DXMatrixIdentity(&matWorld);
	matWorld = m_pTransformCom->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMATRIXA16 matWorldView;
	D3DXMATRIXA16 matWorldViewProjection;
	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);


	m_pShaderCom->GetShader(1)->SetMatrix("worldViewProjectionMatrix", &matWorldViewProjection);
	m_pShaderCom->GetShader(1)->SetFloat("ambientIntensity", 1.0f);
	m_pShaderCom->GetShader(1)->SetValue("ambientColor", &m_pLightCom->Get_LightInfo().Diffuse, sizeof(m_pLightCom->Get_LightInfo().Diffuse));

	_uint numPasses = 0;

	m_pShaderCom->GetShader(1)->Begin(&numPasses, NULL);
	{
		for (_uint i = 0; i < numPasses; ++i)
		{
			m_pShaderCom->GetShader(1)->BeginPass(i);
			{
				m_pTransformCom->SetUp_MatrixToGraphicDev();
				//m_pBufferCom->Render_Buffer();
				m_pSphere->DrawSubset(0);
			}
			m_pShaderCom->GetShader(1)->EndPass();
		}
	}
	m_pShaderCom->GetShader(1)->End();
}

void CLightObj::SetColor(CLight::LIGHTCOLOR eType)
{
	m_pLightCom->SetUp_LightInfo(eType);
}

HRESULT CLightObj::Ready_Component()
{
	CComponent* pComponent = nullptr;

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

	/*Add Shader*/
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Shader_Light"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Shader_Light", pComponent));
	m_pShaderCom->AddRef();

	/*Add Light*/
	pComponent = m_pLightCom = dynamic_cast<CLight*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Light"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Light", pComponent));
	m_pLightCom->AddRef();

	return NOERROR;

exception:
	MSG_BOX("Light Component Ready Failed err: 13116");
	return E_FAIL;

}

CLightObj * CLightObj::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLightObj* pInstance = new CLightObj(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CLightObj Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CLightObj::Free()
{

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pLightCom);
	Safe_Release(m_pLightMgr);
	Safe_Release(m_pSphere);

	return CGameObject::Free();
}

void CLightObj::Update_Input(const _float fTimeDelta)
{
	CInput_Device* m_pInput_Device = CInput_Device::GetInstance();;


	if (m_pInput_Device->Get_DIKeyState(DIK_UP) & 0x80)
		m_pTransformCom->Move_ToLook(10.f, fTimeDelta);

	if (m_pInput_Device->Get_DIKeyState(DIK_LEFT) & 0x80)
		m_pTransformCom->Move_ToLeft(10.f, fTimeDelta);

	if (m_pInput_Device->Get_DIKeyState(DIK_RIGHT) & 0x80)
		m_pTransformCom->Move_ToRight(10.f, fTimeDelta);

	if (m_pInput_Device->Get_DIKeyState(DIK_DOWN) & 0x80)
		m_pTransformCom->Move_ToLook(-10.f, fTimeDelta);

	if (m_pInput_Device->Get_DIKeyState(DIK_Q) & 0x80)
	{
		_float f = 10.f * fTimeDelta;
		m_pTransformCom->Rotation_Y(f);
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_E) & 0x80)
	{
		_float f = +10.f * fTimeDelta;
		m_pTransformCom->Rotation_Z(f);
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_R) & 0x80)
	{
		_float f = +10.f * fTimeDelta;
		m_pTransformCom->Rotation_X(f);
	}

}
