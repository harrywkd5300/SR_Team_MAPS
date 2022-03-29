#include "..\Headers\Graphic_Device.h"

IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
	:m_pSDK(nullptr), m_pGraphicDev(nullptr), m_pD3dFont(nullptr)
{
}

HRESULT CGraphic_Device::Ready_GraphicDev(LPDIRECT3DDEVICE9 * ppGraphicDev, HWND hWnd, WINMODE eMode, const _uint & iBackCX, const _uint & iBackCY)
{
	if (nullptr != m_pSDK || nullptr != m_pGraphicDev)
	{
		MSG_BOX("Device Already Initialized");
		return E_FAIL;
	}

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	if (nullptr == m_pSDK)
	{
		MSG_BOX("Direct3DCreate9 Created Fail");
		return E_FAIL;
	}

	D3DCAPS9	DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		MSG_BOX("Get_DeviceCaps Failed");
		return E_FAIL;
	}

	_ulong BehaviorFlag = 0;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		BehaviorFlag = D3DCREATE_HARDWARE_VERTEXPROCESSING;		
	else
		BehaviorFlag = D3DCREATE_SOFTWARE_VERTEXPROCESSING;		

	BehaviorFlag |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = iBackCX;
	d3dpp.BackBufferHeight = iBackCY;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eMode;


	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, BehaviorFlag, &d3dpp, &m_pGraphicDev)))
	{
		MSG_BOX("m_pGraphicDev Created Failed");
		return E_FAIL;
	}

	if (ppGraphicDev != nullptr)
	{
		*ppGraphicDev = m_pGraphicDev;
		m_pGraphicDev->AddRef();
	}

	/*Set Font*/
	D3DXFONT_DESC tFont;
	ZeroMemory(&tFont, sizeof(tFont));

	tFont.Height = 22;
	tFont.Width = 0;
	tFont.MipLevels = 1;

	tFont.Weight = FW_NORMAL;
	tFont.CharSet = HANGEUL_CHARSET;

	lstrcpy(tFont.FaceName, L"Arial");

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &tFont, &m_pFont)))
	{
		MSG_BOX("Create Font Failed");
		return E_FAIL;
	}
	
	m_pD3dFont = CD3DFont::Create(m_pGraphicDev, L"Arial", 15);
	
	if(nullptr == m_pD3dFont)
	{
		return E_FAIL;
	}


	return NOERROR;

}

_ulong CGraphic_Device::Free()
{
	_ulong dwRefCnt = 0;

	if (dwRefCnt = Safe_Release(m_pD3dFont))
	{
		MSG_BOX("D3dFont Release Failed");
		return dwRefCnt;
	}


	if (dwRefCnt = Safe_Release(m_pFont))
	{
		MSG_BOX("Font Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = Safe_Release(m_pGraphicDev))
	{
		MSG_BOX("Graphic Device Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = Safe_Release(m_pSDK))
	{
		MSG_BOX("SDK Release Failed");
		return dwRefCnt;
	}


	return dwRefCnt;
}
