#pragma once

#include "GameObject.h"


BEGIN(Engine)

class DLL_EXPORT CCamera abstract
	:public CGameObject
{

	/*ctor dtor*/
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera() = default;


/*FUNCTION*/
public:
	const _vec3& Get_CameraPos() {
		return (_vec3&)m_matCameraWorld.m[3];};
	const _matrix Get_CameraWorldMatrix() {
		return m_matCameraWorld;}
	const _vec3& Get_CameraEye() { return m_vEye; }

	void Set_CameraEye(const _vec3& v) { m_vEye = v; }

public:
	/*projection transform, view transform*/
	virtual HRESULT Ready_Camera(const _vec3& vEye, const _vec3& vAt, const _vec3& vWorldUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar);
	void	SetUp_ViewMatrix();
	void	SetUp_ProjectionMatrix();

/*MEMBER*/
protected:
	_matrix m_matCameraWorld;

	_matrix m_matView;
	_vec3	m_vEye;
	_vec3	m_vAt;
	_vec3	m_vWorldUp;

	_matrix m_matProjection;
	_float	m_fFovY;
	_float	m_fAspect;
	_float	m_fNear;
	_float	m_fFar;


/*related ctor dtor*/
protected:
	virtual _ulong Free() override;
};

END