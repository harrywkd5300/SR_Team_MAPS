#include "Target.h"

USING(Engine)

Engine::CTarget::CTarget(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pTargetTexture(nullptr)
	, m_pOldSurface(nullptr)
	, m_pTargetSurface(nullptr)
{
	m_pGraphicDev->AddRef();
}

HRESULT Engine::CTarget::Ready_Target(const _uint& iSizeX, const _uint& iSizeY
									  , D3DFORMAT eSurfaceFormat, D3DXCOLOR _tColor)
{
	m_Color = _tColor;

	if(FAILED(D3DXCreateTexture(m_pGraphicDev, iSizeX, iSizeY, 1, D3DUSAGE_RENDERTARGET, eSurfaceFormat, D3DPOOL_DEFAULT, &m_pTargetTexture)))
		return E_FAIL;

	m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface);

	Engine::Safe_Release(m_pTargetTexture);
	
	return NOERROR;
}


void Engine::CTarget::Clear_RenderTarget(void)
{
	m_pGraphicDev->GetRenderTarget(0, &m_pOldSurface);
	
	m_pGraphicDev->SetRenderTarget(0, m_pTargetSurface);

	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET, m_Color, 1.f, 0);
	m_pGraphicDev->SetRenderTarget(0, m_pOldSurface);

	Engine::Safe_Release(m_pOldSurface);	
}

void Engine::CTarget::Set_ToGraphicDev(const _uint& iIndex)
{
	m_pGraphicDev->GetRenderTarget(iIndex, &m_pOldSurface);
	m_pGraphicDev->SetRenderTarget(iIndex, m_pTargetSurface);
}

void Engine::CTarget::Release_ToGraphicDev(const _uint& iIndex)
{
	m_pGraphicDev->SetRenderTarget(iIndex, m_pOldSurface);

	Engine::Safe_Release(m_pOldSurface);
}

HRESULT CTarget::Ready_DebugBuffer(const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXVIEWPORT) * 4, 0, VTXVIEWPORT::FVF, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;

	VTXVIEWPORT*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(fX, fY, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(fX + fSizeX, fY, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(fX + fSizeX, fY + fSizeY, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(fX, fY + fSizeY, 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL)))
		return E_FAIL;

	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CTarget::Render_DebugBuffer(void)
{
	m_pGraphicDev->SetTexture(0, m_pTargetTexture);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXVIEWPORT));
	m_pGraphicDev->SetFVF(VTXVIEWPORT::FVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}


void Engine::CTarget::Set_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName)
{
	pEffect->SetTexture(pConstantName, m_pTargetTexture);
}

CTarget* Engine::CTarget::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY
								 , D3DFORMAT eSurfaceFormat, D3DXCOLOR Color)
{
	CTarget*	pInstance = new CTarget(pGraphicDev);

	if(FAILED(pInstance->Ready_Target(iSizeX, iSizeY, eSurfaceFormat, Color)))
	{
		MSG_BOX("Target creation failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTarget::Clone()
{
	AddRef();
	return this;
}

_ulong Engine::CTarget::Free(void)
{
	_ulong dwRefCount = 0;

	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pTargetTexture);

	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);

	CComponent::Free();

	return dwRefCount;

}

