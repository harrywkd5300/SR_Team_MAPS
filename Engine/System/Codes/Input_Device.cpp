#include "..\Headers\Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
	:m_pSDK(nullptr), m_pKeyboard(nullptr), m_pMouse(nullptr)
{
	ZeroMemory(m_KeyState, sizeof(_byte) * 256);
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));
}

HRESULT CInput_Device::Ready_InputDevice(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pSDK, nullptr)))
		goto exception;
	
	if (FAILED(Ready_Keyboard(hWnd)))
		goto exception;

	if (FAILED(Ready_Mouse(hWnd)))
		goto exception;

	return NOERROR;

exception:
	MSG_BOX("Ready_InputDevice failed err:20096");
	return E_FAIL;
}

void CInput_Device::Update_InputState()
{
	if (nullptr == m_pKeyboard || nullptr == m_pMouse)
		return;

	ZeroMemory(m_KeyState, sizeof(_byte) * 256);
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));

	m_pKeyboard->GetDeviceState(sizeof(_byte) * 256, m_KeyState);
	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);

}

HRESULT CInput_Device::Ready_Keyboard(HWND hWnd)
{
	if (FAILED(m_pSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
		return E_FAIL;

	if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;

	if (FAILED(m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	m_pKeyboard->Acquire();

	return NOERROR;
}

HRESULT CInput_Device::Ready_Mouse(HWND hWnd)
{
	if (FAILED(m_pSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	m_pMouse->Acquire();

	return NOERROR;
}

_ulong CInput_Device::Free()
{
	_ulong dwRefCount = 0;

	if (FAILED(dwRefCount = Safe_Release(m_pMouse)))
		return dwRefCount;

	if (FAILED(dwRefCount = Safe_Release(m_pKeyboard)))
		return dwRefCount;

	if (FAILED(dwRefCount = Safe_Release(m_pSDK)))
		return dwRefCount;

	return dwRefCount;
}
