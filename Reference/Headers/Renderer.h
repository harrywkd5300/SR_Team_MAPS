#pragma once

#include "Component.h"

BEGIN(Engine)

class CGameObject;

class CLight_Manager;
class CTarget;
class CShader;
class CParticle_Renderer;
class CEffect_Manager;

class DLL_EXPORT CRenderer final
	:public CComponent
{
public:
	enum RENDERTYPE {RENDER_PRIORITY, RENDER_BACKGROUND, RENDER_NONEALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END};

	/*ctor dtor*/
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual ~CRenderer()=default;


/*FUNCTION*/
public:
	HRESULT Ready_Renderer();
	void	Render_Renderer();
	HRESULT Add_RenderList(RENDERTYPE eRenderType, CGameObject* pGameObject);
	inline void	DebugModeOn() { m_bDebugMode = true; }
	inline void	DebugModeOff() { m_bDebugMode = false; }

private:
	void	Render_Priority();
	void	Render_BackGround();
	void	Render_NoneAlpha();
	void	Render_Alpha();
	//void	Render_Light();
	void	Render_UI();
	void	Render_Depth();
	void	Clear_RenderList();
	void	Render_Stencil();
	void	Render_Shadow();


/*MEMBER*/
private:
	list<CGameObject*>			m_RenderObjList[RENDER_END];
	typedef list<CGameObject*>	RENDEROBJLIST;
	CLight_Manager* m_pLightMgr = nullptr;
	CParticle_Renderer* m_pParticleRenderer = nullptr;
	CEffect_Manager*	m_pEffectManager = nullptr;
	CTarget*			m_pDepthTarget = nullptr;
	bool				m_bDebugMode = false;
/* related ctor&dtor */
public:
	static	CRenderer*	Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CComponent*	Clone() final;

private:
	virtual _ulong Free() final;
};

END