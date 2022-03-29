#pragma once

#include "Engine_Defines.h"
#include "base.h"
#include "Component_manager.h"
#include "Light_Manager.h"

BEGIN(Engine)

class CInput_Device;
class CLight_Manager;

class DLL_EXPORT CGameObject abstract
	:public CBase
{
/*ctor dtor*/
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject() = default ;

/*FUNCTION*/
public:
	CComponent*		Get_Component(const _tchar* pComponentTag);
	_bool			Get_IsDead() { return m_bIsDead; }

	void			Set_IsDead() { m_bIsDead = true; }


public:
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject();
	virtual void	Render_Shadow(_matrix CameraWorldMatrix);


private:
	CComponent*		Find_Component(const _uint& UpdateTypeIndex, const _tchar* pComponentTag);

/*MEMBER*/
protected:
	LPDIRECT3DDEVICE9							m_pGraphicDev = nullptr;
	map<const _tchar*, CComponent*>				m_mapComponent[CComponent::UPDATETYPE_END];
	typedef map<const _tchar*, CComponent*>		MAPCOMPONENT;
	
	_bool										m_bIsDead = false;
	CLight_Manager*								m_pLightMgr = nullptr;


/*related ctor dtor*/
protected:
	virtual _ulong	Free() override;

};

END