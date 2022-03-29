#pragma once


#include "engine_defines.h"
#include "base.h"

BEGIN(Engine)

class DLL_EXPORT CInput_Device final
	:public CBase
{
	DECLARE_SINGLETON(CInput_Device)

public:
	enum DIM_BUTTON {DIM_LBUTTON, DIM_RBUTTON, DIM_WHEEL, DIM_XBUTTON};
	enum DIM_MOVE {DIM_X, DIM_Y, DIM_Z};

/*--ctor dtor--*/
private:
	explicit CInput_Device();
	virtual ~CInput_Device() = default;

/*--FUNCTION--*/
	/*get set*/
public:
	_byte	Get_DIKeyState(_ubyte byDIKey) {
		return m_KeyState[byDIKey]; }
	_byte	Get_DIMouseState(DIM_BUTTON eDIMouse) {
		return m_MouseState.rgbButtons[eDIMouse]; }
	_long	Get_DIMouseMove(DIM_MOVE eDIMouse) {
		return *(&m_MouseState.lX + eDIMouse); }

public:
	HRESULT Ready_InputDevice(HINSTANCE hInst, HWND hWnd);
	void	Update_InputState();

private:
	HRESULT Ready_Keyboard(HWND hWnd);
	HRESULT Ready_Mouse(HWND hWnd);


/*--MEMBER--*/
private:
	LPDIRECTINPUT8			m_pSDK = nullptr;
	LPDIRECTINPUTDEVICE8	m_pKeyboard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

	_byte					m_KeyState[256] = {0};
	DIMOUSESTATE			m_MouseState = { 0 };

/*--related ctor dtor--*/
	virtual _ulong Free();

};

END