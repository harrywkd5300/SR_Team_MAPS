#pragma once


#include "Engine_Defines.h"
#include "base.h"
#include "object_Manager.h"
#include "Component_Manager.h"
#include "camera_manager.h"

BEGIN(Engine)



class DLL_EXPORT CScene abstract
	:public CBase
{

/*ctor dtor*/
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual ~CScene() = default;

/*FUNCTION*/
public:
	virtual HRESULT Ready_Scene() =0;
	virtual HRESULT Ready_Renderer(CRenderer* pRenderer);
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene();

/*MEMBER*/
protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
	CObject_Manager*	m_pObject_Manager = nullptr;
	CComponent_Manager*	m_pComponent_Manager = nullptr;
	CCamera_Manager*	m_pCamera_Manager = nullptr;
	CRenderer*			m_pRenderer = nullptr;


/*related ctor dtor*/
protected:
	virtual _ulong Free() override;
};

END