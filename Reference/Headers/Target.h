#pragma once

#include "Component.h"

BEGIN(Engine)

class DLL_EXPORT CTarget : public CComponent
{
private:
	explicit CTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTarget() = default;
public:

	HRESULT Ready_Target(const _uint& iSizeX, const _uint& iSizeY
		, D3DFORMAT eSurfaceFormat , D3DXCOLOR Color);
	void Clear_RenderTarget(void);
	void Set_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName);
	void Set_ToGraphicDev(const _uint& iIndex);
	void Release_ToGraphicDev(const _uint& iIndex);

	HRESULT Ready_DebugBuffer(const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	void	Render_DebugBuffer(void);

private:
	LPDIRECT3DTEXTURE9			m_pTargetTexture = nullptr;
	LPDIRECT3DSURFACE9			m_pOldSurface = nullptr;
	LPDIRECT3DSURFACE9			m_pTargetSurface = nullptr;
	D3DXCOLOR					m_Color;

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

public:
	static CTarget* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY
		, D3DFORMAT eSurfaceFormat, D3DXCOLOR Color);
	virtual CComponent*	Clone() final;
private:
	virtual _ulong Free() final;
};

END
