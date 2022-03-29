#pragma once

#include "camera.h"
#include "A_defines.h"

namespace Engine
{
	class CInput_Device;
}

class CTool_Camera_Dynamic final
	:public CCamera
{
/*ctor dtor*/
private:
	explicit CTool_Camera_Dynamic(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTool_Camera_Dynamic() = default;

/*FUNCTION*/
public:
	HRESULT Ready_Camera_Dynamic(const _vec3 & vEye, const _vec3 & vAt, const _vec3 & vWorldUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar);
	
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
private:
	_int Update_InputState(const _float& fTimeDelta);

/*MEMBER*/
private:
	CInput_Device*	m_pInput_Device = nullptr;
	_float			m_fDist = 0.f;
	_float			m_fHeight = 0.f;

/*related ctor dtor*/
public:
	static CTool_Camera_Dynamic* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vEye, const _vec3 & vAt, const _vec3 & vWorldUp, const _float & fFovY = D3DXToRadian(60.f), const _float & fAspect = (_float)BACKSIZEX/ (_float)BACKSIZEY, const _float & fNear = 0.3f, const _float & fFar = 1000.f);
	
private:
	virtual _ulong Free() final;
};

