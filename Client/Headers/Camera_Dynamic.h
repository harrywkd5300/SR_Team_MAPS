#pragma once

#include "camera.h"
#include "defines.h"

namespace Engine
{
	class CInput_Device;
}

class CCamera_Dynamic final
	:public CCamera
{
/*ctor dtor*/
private:
	explicit CCamera_Dynamic(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera_Dynamic() = default;



/*FUNCTION*/
public:
	HRESULT Ready_Camera_Dynamic(const _vec3 & vEye, const _vec3 & vAt, const _vec3 & vWorldUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar);
	
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual	_int	LastUpdate_GameObject(const _float& fTimeDelta);
public:
	void		 Set_CameraMove(_bool Move) { m_bMove = Move; }

private:
	_int Update_InputState(const _float& fTimeDelta);

/*MEMBER*/
private:
	CInput_Device*	m_pInput_Device = nullptr;
	_bool			m_bMove = true;

/*related ctor dtor*/
public:
	static CCamera_Dynamic* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vEye, const _vec3 & vAt, const _vec3 & vWorldUp, const _float & fFovY = D3DXToRadian(60.f), const _float & fAspect = (_float)BACKCX/ (_float)BACKCY, const _float & fNear = 0.2f, const _float & fFar = 1000.f);
	
private:
	virtual _ulong Free() final;
};

