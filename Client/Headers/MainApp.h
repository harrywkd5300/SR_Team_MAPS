#pragma once

#include "Base.h"
#include "defines.h"
#include "Graphic_Device.h"

#include "management.h"
#include "component_manager.h"
#include "d3dfont.h"


namespace Engine
{
	class CRandomWell512;
}

class CSound_Manager;
class CMainApp final
	:public CBase
{

/*ctor dtor*/
private:
	explicit CMainApp();
	virtual ~CMainApp() = default;


/*function*/
public:
	HRESULT Ready_MainApp();
	_int	Update_MainApp(const _float fTimeDelta);
	void	Render_MainApp();

private:
	HRESULT Ready_DeviceSetting(HWND hWnd, CGraphic_Device::WINMODE eMode, const _uint& iBackCX, const _uint& iBackCY);
	HRESULT Ready_StaticComponent();
	HRESULT Ready_StartScene();
	HRESULT	Ready_Singleton();
	HRESULT Ready_Sound();


	void	Render_Font();

/*member*/
private:
	CGraphic_Device*		m_pDevice = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	CD3DFont*				m_pFont = nullptr;

	CManagement*			m_pManagement = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
	
	CRenderer*				m_pRenderer = nullptr;
	CParticle_Renderer*		m_pParticle_Renderer = nullptr;
	CEffect_Manager*		m_pEffect_Manager = nullptr;
	CSound_Manager*			m_pSound_Manager = nullptr;

	CRandomWell512*			m_pRandomMgr = nullptr;

	_char					m_szFPS[32] = "";
	_float					m_fFPS = 0.f;
	_float					m_fTimer = 0.f;

/*related ctor dtor*/
public:
	static CMainApp* Create();

private:
	virtual _ulong Free() final;

};


