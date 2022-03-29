#pragma once

#include "scene.h"
#include "defines.h"
#include "wave_manager.h"

namespace Engine 
{
	class CInput_Device;
}

class CCollider_Manager;

class CScene_Stage final
	:public CScene
{

/*ctor dtor*/
private:
	explicit CScene_Stage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_Stage() = default;


/*FUNCTION*/
public:
	virtual HRESULT	Ready_Scene() final;
	virtual _int	Update_Scene(const _float& fTimeDelta) final;
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta) final;
	virtual void	Render_Scene() final;
	
private:
	HRESULT Add_GameObject_ForStage();
	HRESULT Add_GameObject_Monster_ForStage();
	HRESULT Add_Component_ForStage();
	HRESULT	Add_Camera_ForStage();
	HRESULT	Add_BGM_ForStage();

	void	Update_InputState(const _float& fTimeDelta);

/*MEMBER*/
private:
	CInput_Device*		m_pInput_Device = nullptr;
	CCollider_Manager*	m_pColliderMgr = nullptr;
	CWave_Manager*		m_pWave_Manager = nullptr;

/*related ctor dtor*/
public:
	static CScene_Stage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual _ulong Free() final;

};

