#pragma once
#include "Engine_Defines.h"
#include "Base.h"
//#include "TargetLayer.h"

BEGIN(Engine)

class CTarget;
class CTargetLayer;
class DLL_EXPORT CTarget_Manager : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
private:
	explicit CTarget_Manager();
	virtual ~CTarget_Manager() = default;
public:
	HRESULT Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTargetTag, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT eSurfaceFormat, D3DXCOLOR Color);
	HRESULT Ready_TargetLayer(const _tchar* pTargetLayerTag, const _tchar* pTargetTag);
	void Begin_TargetLayer(const _tchar* pTargetLayerTag);
	void End_TargetLayer(const _tchar* pTargetLayerTag);
	void SetUp_OnShader(const _tchar* pTargetTag, LPD3DXEFFECT pEffect, const char* pConstantName);
private:
	//map<const _tchar*, CTarget*>					m_mapTarget;
	//typedef map<const _tchar*, CTarget*>			MAPTARGET;
	CTargetLayer*	m_pTargetLayer;
private:
	map<const _tchar*, CTargetLayer*>				m_mapTargetLayer;
	typedef map<const _tchar*, CTargetLayer*>		LAYERMAP;

private:
	CTargetLayer* Find_TargetLayer(const _tchar* pTargetTag);
	CTarget* Find_Target(const _tchar* pTargetTag);
public:
	virtual _ulong Free(void);
};

END
