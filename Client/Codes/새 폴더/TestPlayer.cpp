#include "stdafx.h"
#include "..\Headers\TestPlayer.h"
#include "input_device.h"
#include "Object_Manager.h"
#include "Graphic_Device.h"

CTestPlayer::CTestPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pTextureCom(nullptr)
	,m_pTerrainBufferCom(nullptr)
	,m_pBoneCom(nullptr)
	,m_fSpeed(0.f),m_fSprintSpeed(0.f)
	,m_iRecoilY(0)
	,m_fFiringRateTimer(0.f)
	,m_fRecoverTimer(0.f)
	,m_fMouseSensitive(0.f)
	/*test*/

{
	m_pInput_Device->AddRef();
}
// D3DXVec3Lerp(&v, &pos1, &pos2, t);
// D3DXMatrixTranslation(&matTMParent, v.x, v.y.v.z);

HRESULT CTestPlayer::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_Parameters()))
		return E_FAIL;

	if (FAILED(Ready_D3dFont()))
		return E_FAIL;

	return NOERROR;
}

_int CTestPlayer::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
	
	
	//FixMouse();

	Update_Input(fTimeDelta);
	Recover_Recoil(fTimeDelta);


	m_pTransformCom->Fill_WorldMatrix();

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);

	return 0;
}

_int CTestPlayer::LastUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CTestPlayer::Render_GameObject()
{
	if (nullptr == m_pBufferCom || nullptr == m_pTransformCom)
		return;

	m_pTransformCom->SetUp_MatrixToGraphicDev();
	
	//m_pBufferCom->Render_Buffer();

}

HRESULT CTestPlayer::Ready_Component()
{
	CComponent* pComponent = nullptr;

	/*------------Clone Component-----------*/

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



#ifdef __SS
	/*----------------Add Bone---------------------*/

	/*0 Heap*/
 	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Bone");
 	if (nullptr == pComponent)
 		goto exception;
 	
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Bone0", pComponent));
	m_vecBoneCom.push_back(dynamic_cast<CBone*>(pComponent));
	pComponent->AddRef();

	/*1 leftleg*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Bone");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Bone1", pComponent));
	m_vecBoneCom.push_back(dynamic_cast<CBone*>(pComponent));
	pComponent->AddRef();

	/*2 RightLeg*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Bone");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Bone2", pComponent));
	m_vecBoneCom.push_back(dynamic_cast<CBone*>(pComponent));
	pComponent->AddRef();


	/*========ADD BODY=========*/
	/*add leg*/
	/*0 Leg*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Cube_Leg");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Leg", pComponent));
	m_vecBodyCom.push_back(dynamic_cast<CCube_Color*>(pComponent));
	pComponent->AddRef();

	/*add body*/
	/*1 body*/
	pComponent = CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE, L"Component_Cube_Body");
	if (nullptr == pComponent)
		goto exception;

	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Body", pComponent));
	m_vecBodyCom.push_back(dynamic_cast<CCube_Color*>(pComponent));
	pComponent->AddRef();


	/*Add Cube_Bone*/
	pComponent = m_pBoneCubeCom = dynamic_cast<CCube_Color*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_CubeCol"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"CubeCol", pComponent));
	m_pBoneCubeCom->AddRef();





#endif 

	/*------------- Ref Component --------------*/
	/*Ref Terrain Buffer*/
	m_pTerrainBufferCom = dynamic_cast<CTerrain_Buffer*>(CObject_Manager::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain", 0, L"Buffer_Terrain"));
	m_pTerrainBufferCom->AddRef();

	return NOERROR;

exception:
	MSG_BOX("Player Component Ready Failed err: 13115");
	return E_FAIL;
}

HRESULT CTestPlayer::Ready_Parameters()
{
	ZeroMemory(&m_vRecoil, sizeof(_vec3));

	m_fRecoverTimer = 1.f;

	/*option parameters*/
	m_fMouseSensitive = 10.f;

	/*Player Parameters*/
	m_fSpeed = 2.f;
	m_fSprintSpeed = 15.f;

	/*Gun Parameters*/
	m_iRecoilY = 2;

	return NOERROR;
}

HRESULT CTestPlayer::Ready_D3dFont()
{
	m_pD3dFont = CGraphic_Device::GetInstance()->Get_Font();
	
	if (nullptr == m_pD3dFont)
	{
		MSG_BOX("Ready D3dFont Failed");
		return E_FAIL;
	}
	m_pD3dFont->AddRef();

	return NOERROR;
}


void CTestPlayer::Update_Input(const _float fTimeDelta)
{
	if (nullptr == m_pInput_Device)
		return;

#ifndef __SS
	if (m_pInput_Device->Get_DIKeyState(DIK_LSHIFT) & 0x80)
	{
		if (m_pInput_Device->Get_DIKeyState(DIK_W) & 0x80)
			m_pTransformCom->Move_ToLook(m_fSprintSpeed, fTimeDelta);

		if (m_pInput_Device->Get_DIKeyState(DIK_A) & 0x80)
			m_pTransformCom->Move_ToLeft(m_fSprintSpeed, fTimeDelta);

		if (m_pInput_Device->Get_DIKeyState(DIK_D) & 0x80)
			m_pTransformCom->Move_ToRight(m_fSprintSpeed, fTimeDelta);

		if (m_pInput_Device->Get_DIKeyState(DIK_S) & 0x80)
			m_pTransformCom->Move_ToLook(-m_fSprintSpeed, fTimeDelta);

	}
	else /*not pressed shift*/
	{
		if (m_pInput_Device->Get_DIKeyState(DIK_W) & 0x80)
			m_pTransformCom->Move_ToLook(m_fSpeed, fTimeDelta);

		if (m_pInput_Device->Get_DIKeyState(DIK_A) & 0x80)
			m_pTransformCom->Move_ToLeft(m_fSpeed, fTimeDelta);

		if (m_pInput_Device->Get_DIKeyState(DIK_D) & 0x80)
			m_pTransformCom->Move_ToRight(m_fSpeed, fTimeDelta);

		if (m_pInput_Device->Get_DIKeyState(DIK_S) & 0x80)
			m_pTransformCom->Move_ToLook(-m_fSpeed, fTimeDelta);
	}
	/*MouseMove*/
	const _long& lMoveX = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_X);
	const _long& lMoveY = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_Y);

	if (lMoveY)
	{
		_float f = lMoveY * m_fMouseSensitive * fTimeDelta;
		m_pTransformCom->Rotation_X(f);
	}

	if (lMoveX)
	{
		_float f = lMoveX * m_fMouseSensitive * fTimeDelta;
		m_pTransformCom->Rotation_Y(f);
	}


	m_fFiringRateTimer += fTimeDelta;
	if (m_pInput_Device->Get_DIMouseState(CInput_Device::DIM_LBUTTON) & 0x80
		&& m_fFiringRateTimer > 0.1f)
	{
		m_fFiringRateTimer = 0;
		Recoil(fTimeDelta, 20, m_iRecoilY);
	}
#else

	if (m_pInput_Device->Get_DIKeyState(DIK_UP) & 0x80)
	{
		m_pTransformCom->Move_ToLook(m_fSpeed, fTimeDelta);
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_LEFT) & 0x80)
		m_pTransformCom->Move_ToLeft(m_fSpeed, fTimeDelta);

	if (m_pInput_Device->Get_DIKeyState(DIK_RIGHT) & 0x80)
		m_pTransformCom->Move_ToRight(m_fSpeed, fTimeDelta);

	if (m_pInput_Device->Get_DIKeyState(DIK_DOWN) & 0x80)
		m_pTransformCom->Move_ToLook(-m_fSpeed, fTimeDelta);


	const _long& lMoveX = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_X);
	const _long& lMoveY = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_Y);

	if (lMoveY)
	{
		_float f = lMoveY * m_fMouseSensitive * fTimeDelta;
		m_pTransformCom->Rotation_X(f);
		//m_vecBoneCom[3]->Rotation_X(f);
	}

	if (lMoveX)
	{
		_float f = lMoveX * m_fMouseSensitive * fTimeDelta;
		m_pTransformCom->Rotation_Y(f);
	}


	m_fFiringRateTimer += fTimeDelta;
	if (m_pInput_Device->Get_DIMouseState(CInput_Device::DIM_LBUTTON) & 0x80
		&& m_fFiringRateTimer > 0.1f)
	{
		m_fFiringRateTimer = 0;
		Recoil(fTimeDelta, 20, m_iRecoilY);
	}

#endif


}

void CTestPlayer::Recoil(const _float & fTimeDelta, const _int & iForceX, const _int & iForceY)
{
	//for recoil recovery timer
	m_fRecoverTimer = 0.f;

	//set recoilX
	_float fMoveX = (rand() % iForceX - iForceX *0.5f )* fTimeDelta;
	m_pTransformCom->Rotation_Y(fMoveX);

	//set recoilY
	m_pTransformCom->Rotation_X(_float(-iForceY));

}

void CTestPlayer::Recover_Recoil(const _float & fTimeDelta)
{
	m_fRecoverTimer += fTimeDelta;

	if (m_fRecoverTimer < 0.5f)
	{
		m_pTransformCom->Rotation_X(_float(m_iRecoilY) * 1.6f * fTimeDelta);
	}

}

void CTestPlayer::Gravity_Test(const _float & fTimeDelta)
{

}

void CTestPlayer::FixMouse()
{
	SetCursorPos(BACKCX / 2, BACKCY / 2);
}

CTestPlayer * CTestPlayer::Create(LPDIRECT3DDEVICE9 pGrahpicDev)
{
	CTestPlayer* pInstance = new CTestPlayer(pGrahpicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CPlayer Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CTestPlayer::Free()
{
	//decrease refcount
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTerrainBufferCom);
	Safe_Release(m_pD3dFont);

	//Safe_Release(m_pBoneCom);

	for (size_t i = 0; i < m_vecBoneCom.size(); ++i)
		Safe_Release(m_vecBoneCom[i]);

	for (size_t i = 0; i < m_vecBodyCom.size(); ++i)
		Safe_Release(m_vecBodyCom[i]);

	Safe_Release(m_pBoneCubeCom);


	return CGameObject::Free();
}
