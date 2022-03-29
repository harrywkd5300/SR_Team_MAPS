#pragma once

#include "base.h"
#include "Engine_Defines.h"
#include "camera.h"

BEGIN(Engine)


class DLL_EXPORT CCamera_Manager
	:public CBase
{
	DECLARE_SINGLETON(CCamera_Manager)

	
	/*ctor dtor*/
private:
	explicit CCamera_Manager();
	virtual ~CCamera_Manager() = default;

/*FUNCTION*/
public:
	CCamera*	Get_Camera(const _tchar* pCameraTag);
	CCamera*	Get_CurCamera();

	void		Set_CameraPos(const _vec3& v) { m_pCurCamera->Set_CameraEye(v); }

public:
	HRESULT		Add_Camera(const _tchar* pCameraTag, CGameObject* pGameObject);
	_int		Update_Camera(const _float& fTimeDelta);
	_int		LastUpdate_Camera(const _float& fTimeDelta);

	HRESULT		Change_Camera(const _tchar* pCameraTag);

private:
	CCamera*	Find_Camera(const _tchar* pCameraTag);

/*MEMBER*/
private:
	map<const _tchar*, CCamera*> m_mapCamera;
	typedef map<const _tchar*, CCamera*> MAPCAMERA;

	CCamera*	m_pCurCamera = nullptr;


	/*related ctor dtor*/
private:
	virtual _ulong Free();
};

END