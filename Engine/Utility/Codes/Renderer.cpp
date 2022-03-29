#include "..\Headers\Renderer.h"
#include "GameObject.h"
#include "Light_Manager.h"
#include "Particle_Renderer.h"
#include "Effect_Manager.h"
//#include "Target_Manager.h"


CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice)
{

}

HRESULT CRenderer::Ready_Renderer()
{
	
	D3DVIEWPORT9 ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);

	m_pLightMgr = CLight_Manager::GetInstance();
	m_pLightMgr->AddRef();

	m_pParticleRenderer = CParticle_Renderer::GetInstance();
	m_pParticleRenderer->AddRef();

	m_pEffectManager = CEffect_Manager::GetInstance();
	m_pEffectManager->AddRef();
	
	m_pDepthTarget = CTarget::Create(m_pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	//m_pDepthTarget = CTarget::Create(m_pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	m_pDepthTarget->Ready_DebugBuffer(0.f, 40.f, 400.f, 400.f);

	m_pDepthTarget->AddRef();

	return NOERROR;
}

void CRenderer::Render_Renderer()
{
	Render_Priority();
	Render_BackGround();
	Render_Shadow();

	//Render_NoneAlpha();
	Render_Depth();

	m_pParticleRenderer->Start_Render();
	m_pEffectManager->Start_Render();
	Render_Alpha();
	Render_UI();

	if(m_bDebugMode)
		m_pDepthTarget->Render_DebugBuffer();

	Clear_RenderList();
	m_pLightMgr->Clear_LightList();
}

HRESULT CRenderer::Add_RenderList(RENDERTYPE eRenderType, CGameObject * pGameObject)
{
	if (RENDER_END <= eRenderType || nullptr == pGameObject)
	{
		MSG_BOX("RenderList Add Failed");
		return E_FAIL;
	}

	m_RenderObjList[eRenderType].push_back(pGameObject);
	pGameObject->AddRef();


	return NOERROR;
}

void CRenderer::Render_Priority()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	for (auto& pGameObject : m_RenderObjList[RENDER_PRIORITY])
	{
		if(pGameObject != nullptr)
			pGameObject->Render_GameObject();
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);

}

void CRenderer::Render_BackGround()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);

	for (auto& pGameObject : m_RenderObjList[RENDER_BACKGROUND])
	{
		if (pGameObject != nullptr)
			pGameObject->Render_GameObject();
	}

}

void CRenderer::Render_NoneAlpha()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);

 	for (auto& pGameObject : m_RenderObjList[RENDER_NONEALPHA])
	{
		if (pGameObject != nullptr)
			pGameObject->Render_GameObject();
	}
}


void CRenderer::Render_Alpha()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (auto& pGameObject : m_RenderObjList[RENDER_ALPHA])
	{
		if (pGameObject != nullptr)
			pGameObject->Render_GameObject();

	}

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void CRenderer::Render_UI()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	for (auto& pGameObject : m_RenderObjList[RENDER_UI])
	{
		if (pGameObject != nullptr)
			pGameObject->Render_GameObject();
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CRenderer::Render_Depth()
{
	
	// ·»´õÅ¸°Ù Swap
	_uint iRenderTargetIndex = 1;
	m_pDepthTarget->Clear_RenderTarget();
	m_pDepthTarget->Set_ToGraphicDev(iRenderTargetIndex);

	//±íÀÌ ÅØ½ºÃÄ »ý¼º
	//vDepth = vector(In.vProjPos.w * 0.001f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	Render_NoneAlpha();

	// ·»´õÅ¸°Ù Swap
	m_pDepthTarget->Release_ToGraphicDev(iRenderTargetIndex);

}

void CRenderer::Clear_RenderList()
{
	for (auto& RenderList : m_RenderObjList)
	{
		for (auto& pGameObject : RenderList)
		{
			Safe_Release(pGameObject);
		}
		RenderList.clear();
	}
}

void CRenderer::Render_Stencil()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);			
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		
	m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);	

	m_pGraphicDev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);	

	m_pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);			
	m_pGraphicDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	m_pGraphicDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	m_pGraphicDev->SetRenderState(D3DRS_STENCILREF, 0x1);		
	m_pGraphicDev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	m_pGraphicDev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	m_pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);	

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);		
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	_matrix CameraWorldMatrix;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &CameraWorldMatrix);
	D3DXMatrixInverse(&CameraWorldMatrix, nullptr, &CameraWorldMatrix);

	for (auto& pGameObject : m_RenderObjList[RENDER_NONEALPHA])
	{
		if (pGameObject != nullptr)
			pGameObject->Render_Shadow(CameraWorldMatrix);
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_Shadow()
{
	D3DVIEWPORT9			ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);

	VTXVIEWPORTCOL	pVertex[4];
	pVertex[0].vPos = D3DXVECTOR4(0, ViewPort.Height, 0.0f, 1.0f);
	pVertex[0].dwColor = 0x7f000000;
	pVertex[1].vPos = D3DXVECTOR4(0, 0, 0.0f, 1.0f);
	pVertex[1].dwColor = 0x7f000000;
	pVertex[2].vPos = D3DXVECTOR4(ViewPort.Width, ViewPort.Width, 0.0f, 1.0f);
	pVertex[2].dwColor = 0x7f000000;
	pVertex[3].vPos = D3DXVECTOR4(ViewPort.Width, 0, 0.0f, 1.0f);
	pVertex[3].dwColor = 0x7f000000;

	Render_Stencil();	

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);			
	m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);	
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_pGraphicDev->SetRenderState(D3DRS_STENCILREF, 0x1);
	m_pGraphicDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);	
	m_pGraphicDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	m_pGraphicDev->SetFVF(VTXVIEWPORTCOL::FVF);
	m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &pVertex[0], sizeof(VTXVIEWPORTCOL));

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}


CRenderer* CRenderer::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CRenderer* pInstance = new CRenderer(pGraphicDevice);
	if (FAILED(pInstance->Ready_Renderer()))
	{
		MSG_BOX("Renderer creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone()
{
	AddRef();
	return this;
}

_ulong CRenderer::Free()
{
	_ulong dwRefCount = 0;

	for (auto& RenderList : m_RenderObjList)
	{
		for (auto& pGameObject : RenderList)
		{
			Safe_Release(pGameObject);
		}
		RenderList.clear();
	}

	dwRefCount = CComponent::Free();

	Safe_Release(m_pLightMgr);
	Safe_Release(m_pParticleRenderer);
	Safe_Release(m_pEffectManager);

	Safe_Release(m_pDepthTarget);
	Safe_Release(m_pDepthTarget);

	return dwRefCount;
}



//void CRenderer::Render_Light()
//{
//	m_pTargetShadow->Clear_RenderTarget();
//	m_pTargetSpecular->Clear_RenderTarget();
//
//	m_pTargetShadow->Set_ToGraphicDev(0);
//	m_pTargetSpecular->Set_ToGraphicDev(1);
//
//	LPD3DXEFFECT	pEffect = m_pShadowShade->GetShader();
//	if (NULL == pEffect)
//		return;
//
//	m_pTargetNormal->Set_OnShader(pEffect, "NormalTexture_Tex");
//	m_pTargetDepth->Set_OnShader(pEffect, "DepthTexture_Tex");
//
//	pEffect->Begin(NULL, 0);
//
//	// --------------- Render Lights ---------------
//	_uint		numPasses = 0;
//
//	if (D3DLIGHT_DIRECTIONAL == m_LightInfo.Type)
//	{
//		numPasses = 0;
//		pEffect->SetVector("g_vLightDir", &_vec4(m_LightInfo.Direction, 0.f));
//	}
//	else if (D3DLIGHT_POINT == m_LightInfo.Type)
//	{
//		numPasses = 1;
//		pEffect->SetVector("g_vLightPos", &_vec4(m_LightInfo.Position, 1.f));
//		pEffect->SetFloat("g_fRange", m_LightInfo.Range);
//	}
//
//
//	pEffect->SetVector("g_vLightDiffuse", (_vec4*)(&m_LightInfo.Diffuse));
//	pEffect->SetVector("g_vLightAmbient", (_vec4*)(&m_LightInfo.Ambient));
//	pEffect->SetVector("g_vLightSpecular", (_vec4*)(&m_LightInfo.Specular));
//
//	_matrix			matWorld, matView, matProj;
//
//	D3DXMatrixIdentity(&matWorld);
//	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
//	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
//
//	_matrix matWorldView;
//	_matrix matWorldViewProjection;
//
//	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
//	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);
//
//
//	pEffect->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
//
//	D3DXMatrixInverse(&matView, nullptr, &matView);
//	pEffect->SetVector("g_vWorldCameraPos", (_vec4*)&matView.m[3][0]);
//
//	pEffect->BeginPass(numPasses);
//
//	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXVIEWPORT));
//	m_pGraphicDev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
//	m_pGraphicDev->SetIndices(m_pIB);
//	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
//
//	pEffect->EndPass();
//
//	// ---------------------------------------------
//	
//	pEffect->End();
//
//	m_pTargetShadow->Release_ToGraphicDev(0);
//	m_pTargetSpecular->Release_ToGraphicDev(1);
//}
