#include "stdafx.h"
#include "..\Headers\Tool_Camera_Dynamic.h"
#include "Input_Device.h"
#include <time.h>
#include <iostream>

CTool_Camera_Dynamic::CTool_Camera_Dynamic(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev)
	,m_pInput_Device(CInput_Device::GetInstance())
	,m_fDist(0.f)
{
	m_pInput_Device->AddRef();
}

HRESULT CTool_Camera_Dynamic::Ready_Camera_Dynamic(const _vec3 & vEye, const _vec3 & vAt, const _vec3 & vWorldUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	if (FAILED(CCamera::Ready_Camera(vEye, vAt, vWorldUp, fFovY, fAspect, fNear, fFar)))
		return E_FAIL;

	m_fDist = 5.f;

	return NOERROR;
}

_int CTool_Camera_Dynamic::Update_GameObject(const _float & fTimeDelta)
{
	Update_InputState(fTimeDelta);

	this->SetUp_ViewMatrix();
	this->SetUp_ProjectionMatrix();

	return 0;
}

_int CTool_Camera_Dynamic::LastUpdate_GameObject(const _float & fTimeDelta)
{
	Update_InputState(fTimeDelta);

	this->SetUp_ViewMatrix();
	this->SetUp_ProjectionMatrix();
	return 0;
}

_int CTool_Camera_Dynamic::Update_InputState(const _float& fTimeDelta)
{
	_vec3& vCameraRight = (_vec3&)m_matCameraWorld.m[0];
	_vec3& vCameraLook = (_vec3&)m_matCameraWorld.m[2];

	const _float fSpeed = 10.f;

	//
	/*input keyboard*/
	//
 	if (m_pInput_Device->Get_DIKeyState(DIK_W) & 0x80)
 	{
		m_fDist -= fSpeed*fTimeDelta;
		_vec3 v = m_vEye;
		_vec3 v2 = _vec3(0, m_fHeight, 0);

		v.y = m_fHeight;
		v = v2 - v;

		D3DXVec3Normalize(&v, &v);

		m_vEye = v*m_fDist;
		m_vEye.y = m_fHeight;

		m_vAt = v2;

 	}
 	if (m_pInput_Device->Get_DIKeyState(DIK_S) & 0x80)
 	{
		m_fDist += fSpeed*fTimeDelta;
		_vec3 v = m_vEye;
		_vec3 v2 = _vec3(0, m_fHeight, 0);

		v.y = m_fHeight;
		v = v2 - v;

		D3DXVec3Normalize(&v, &v);

		m_vEye = v*m_fDist;
		m_vEye.y = m_fHeight;

		m_vAt = v2;
	}
 
 	if (m_pInput_Device->Get_DIKeyState(DIK_A) & 0x80)
 	{
		_vec3 v = m_vEye + vCameraRight * - fSpeed*fTimeDelta;
		_vec3 v2 = _vec3(0, m_fHeight, 0);

		v.y = m_fHeight;
		v = v2 - v;

		D3DXVec3Normalize(&v, &v);

		m_vEye = v*m_fDist;
		m_vEye.y = m_fHeight;

		m_vAt = v2;

	}
 
 	if (m_pInput_Device->Get_DIKeyState(DIK_D) & 0x80)
 	{

		_vec3 v = m_vEye + vCameraRight* +fSpeed*fTimeDelta;
		_vec3 v2 = _vec3(0, m_fHeight, 0);
		
		v.y = m_fHeight;
		v = v2 - v;

		D3DXVec3Normalize(&v, &v);

		m_vEye = v*m_fDist;
		m_vEye.y = m_fHeight;

		m_vAt = v2;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_R) & 0x80)
	{
		m_fHeight += fSpeed*fTimeDelta;
		_vec3 v2 = _vec3(0, m_fHeight, 0);
		
		m_vEye.y = m_fHeight;
		cout << m_vEye.y << endl;
		m_vAt = v2;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_F) & 0x80)
	{
		m_fHeight -= fSpeed*fTimeDelta;
		_vec3 v2 = _vec3(0, m_fHeight, 0);

		m_vEye.y = m_fHeight;
		//cout << m_vEye.y << endl;
		m_vAt = v2;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_Z) & 0x80)
	{
		m_vEye = _vec3(0.f, 0.f, -5.f);
		m_vAt = _vec3(0.f, 0.f, 0.f);
		m_fDist = 5.f;
		m_fHeight = 0.f;
	}

 	//
 	/*input Mouse Move*/
 	//
 
//  	const _long& lMoveX = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_X);
//  	const _long& lMoveY = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_Y);
//  
//  	if (lMoveY)
//  	{
//  		_matrix matRotationX;
//  		D3DXMatrixRotationAxis(&matRotationX, &vCameraRight, (lMoveY*fTimeDelta * 0.1f));
//  		D3DXVec3TransformNormal(&vCameraLook, &vCameraLook, &matRotationX);
//  
//  		m_vAt = m_vEye + vCameraLook;
//  	}
//  
//  	if (lMoveX)
//  	{
//  		_matrix matRotationY;
//  		D3DXMatrixRotationAxis(&matRotationY, &_vec3(0.f, 1.f, 0.f), (lMoveX*fTimeDelta * 0.1f));
//  		D3DXVec3TransformNormal(&vCameraLook, &vCameraLook, &matRotationY);
//  
//  		m_vAt = m_vEye + vCameraLook;
//  	}

	return 0;
}

CTool_Camera_Dynamic * CTool_Camera_Dynamic::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vEye, const _vec3 & vAt, const _vec3 & vWorldUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CTool_Camera_Dynamic* pInstance = new CTool_Camera_Dynamic(pGraphicDev);

	if (pInstance->Ready_Camera_Dynamic(vEye, vAt, vWorldUp, fFovY, fAspect, fNear, fFar))
	{
		MSG_BOX("Camera_Dynamic creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CTool_Camera_Dynamic::Free()
{
	/*decrease refCount*/
	Safe_Release(m_pInput_Device);
	
	return CCamera::Free();
}

