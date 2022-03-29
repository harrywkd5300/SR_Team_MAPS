#include "stdafx.h"
#include "..\Headers\Tool_ToolCCamera.h"
#include <time.h>

ToolCCamera::ToolCCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev), m_bMouseMode(false)
	, m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
}

HRESULT ToolCCamera::Ready_Camera_Dynamic(const _vec3 & vEye, const _vec3 & vAt, const _vec3 & vWorldUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	if (FAILED(CCamera::Ready_Camera(vEye, vAt, vWorldUp, fFovY, fAspect, fNear, fFar)))
		return E_FAIL;

	srand(unsigned(time(NULL)));
	vRecoil = _vec3(0.f, 0.f, 0.f);

	return NOERROR;
}

_int ToolCCamera::Update_GameObject(const _float & fTimeDelta)
{
	if (m_MouseMove)
	{
		Fix_MousePointer((BACKCX + BACKCX / 5) >> 1, BACKCY >> 1);
	}

	Update_InputState(fTimeDelta);

	this->SetUp_ViewMatrix();
	this->SetUp_ProjectionMatrix();

	return 0;
}

_int ToolCCamera::Update_InputState(const _float& fTimeDelta)
{
	_vec3& vCameraRight = (_vec3&)m_matCameraWorld.m[0];
	_vec3& vCameraUp    = (_vec3&)m_matCameraWorld.m[1];
	_vec3& vCameraLook  = (_vec3&)m_matCameraWorld.m[2];

	const _float fSpeed = 10.f * fTimeDelta;
	//
	/*input keyboard*/
	//
	if (m_pInput_Device->Get_DIKeyState(DIK_W) & 0x80)
	{
		m_vEye += vCameraUp * fSpeed;
		m_vAt += vCameraUp * fSpeed;
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_S) & 0x80)
	{
		m_vEye += vCameraUp * -fSpeed;
		m_vAt += vCameraUp * -fSpeed;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_A) & 0x80)
	{
		m_vEye += vCameraRight * -fSpeed;
		m_vAt += vCameraRight * -fSpeed;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_D) & 0x80)
	{
		m_vEye += vCameraRight * fSpeed;
		m_vAt += vCameraRight * fSpeed;
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_Q) & 0x80)
	{
		m_vEye += vCameraLook * -fSpeed;
		m_vAt += vCameraLook * -fSpeed;
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_E) & 0x80)
	{
		m_vEye += vCameraLook * fSpeed;
		m_vAt += vCameraLook * fSpeed;
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_R) & 0x80)
	{
		Shake(fTimeDelta);
	}
	//
	/*input Mouse Move*/
	//
	if (m_MouseMove)
	{
		const _long& lMoveX = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_X);
		const _long& lMoveY = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_Y);
		if (lMoveY)
		{
			_matrix matRotationX;
			D3DXMatrixRotationAxis(&matRotationX, &vCameraRight, (lMoveY * 0.005f));
			D3DXVec3TransformNormal(&vCameraLook, &vCameraLook, &matRotationX);

			m_vAt = m_vEye + vCameraLook;
		}

		if (lMoveX)
		{
			_matrix matRotationY;
			D3DXMatrixRotationAxis(&matRotationY, &_vec3(0.f, 1.f, 0.f), (lMoveX * 0.005f));
			D3DXVec3TransformNormal(&vCameraLook, &vCameraLook, &matRotationY);

			m_vAt = m_vEye + vCameraLook;
		}
	}
	// Camera Mouse Move
	if (m_pInput_Device->Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		if (!m_MouseMove)
			m_MouseMove = true;
		else
			m_MouseMove = false;
	}
	return 0;
}

void ToolCCamera::MouseMove(CInput_Device::DIM_MOVE eMove)
{
	_vec3& vCameraRight = (_vec3&)m_matCameraWorld.m[0];
	_vec3& vCameraLook = (_vec3&)m_matCameraWorld.m[2];
	const _float fTimeDelta = 0.16f;

	const _float fSpeed = 10.f;

	if (CInput_Device::DIM_Y == eMove)
	{
		const _long& lMoveY = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_Y);
		if (lMoveY)
		{
			_matrix matRotationX;
			D3DXMatrixRotationAxis(&matRotationX, &vCameraRight, (lMoveY * 0 * 0.1f));
			D3DXVec3TransformNormal(&vCameraLook, &vCameraLook, &matRotationX);

			m_vAt = m_vEye + vCameraLook;
		}
	}
	else if (CInput_Device::DIM_X == eMove)
	{
		const _long& lMoveX = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_X);
		if (lMoveX)
		{
			_matrix matRotationY;
			D3DXMatrixRotationAxis(&matRotationY, &_vec3(0.f, 1.f, 0.f), (lMoveX*fTimeDelta * 0.1f));
			D3DXVec3TransformNormal(&vCameraLook, &vCameraLook, &matRotationY);

			m_vAt = m_vEye + vCameraLook;
		}
	}
	this->SetUp_ViewMatrix();
	this->SetUp_ProjectionMatrix();
}

void ToolCCamera::Shake(const _float & fTimeDelta)
{
	_vec3& vCameraRight = (_vec3&)m_matCameraWorld.m[0];
	_vec3& vCameraLook = (_vec3&)m_matCameraWorld.m[2];

	_long lMoveY = rand() % 20 - 10;
	_long lMoveX = rand() % 20 - 10;

	_matrix matRotationX;
	D3DXMatrixRotationAxis(&matRotationX, &vCameraRight, (lMoveY*fTimeDelta * 0.1f));
	D3DXVec3TransformNormal(&vCameraLook, &vCameraLook, &matRotationX);

	_matrix matRotationY;
	D3DXMatrixRotationAxis(&matRotationY, &_vec3(0.f, 1.f, 0.f), (lMoveX*fTimeDelta * 0.1f));
	D3DXVec3TransformNormal(&vCameraLook, &vCameraLook, &matRotationY);

	m_vAt = m_vEye + vCameraLook;
}

void ToolCCamera::Fix_MousePointer(const _uint & iX, const _uint & iY)
{
	POINT			pt = { LONG(iX), LONG(iY) };

	CWnd* pWnd = AfxGetMainWnd();
	HWND hWnd = pWnd->m_hWnd;

	ClientToScreen(hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}



ToolCCamera* ToolCCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vEye, const _vec3 & vAt, const _vec3 & vWorldUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	ToolCCamera* pInstance = new ToolCCamera(pGraphicDev);

	if (pInstance->Ready_Camera_Dynamic(vEye, vAt, vWorldUp, fFovY, fAspect, fNear, fFar))
	{
		MSG_BOX("Camera_Dynamic creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong ToolCCamera::Free()
{
	/*decrease refCount*/
	Safe_Release(m_pInput_Device);

	return CCamera::Free();
}

