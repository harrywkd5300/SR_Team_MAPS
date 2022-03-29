#pragma once

#include "Camera.h"
#include "defines.h"
#include "component_Manager.h"

namespace Engine
{
	class CInput_Device;
}
class CPlayer;

class CCamera_Player final
	:public CCamera
{
public:
	enum EVENT{EVENT_NONE,EVENT_RELOAD,EVENT_ZOOMIN,EVENT_ZOOMOUT,EVENT_AIM};
	enum OUTEVENT{ OUTEVENT_NONE, OUTEVENT_WEAKSHAKE };
	enum EXCLUSIVEEVENT {EXCLUSIVEEVENT_NONE,EXCLUSIVEEVENT_STARTGAME,EXCLUSIVEEVENT_BOSS};

/*ctor dtor*/
private:
	explicit CCamera_Player(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera_Player() =default;


/*FUNCTION*/
public:
	void	Set_Event(EVENT eEvent) { m_eCurEvent = eEvent; }
	void	Set_OutEvent(OUTEVENT eEvent) { m_eCurOutEvent = eEvent; }
	void	Set_ExclusiveEvent(EXCLUSIVEEVENT eEvent) { m_eCurExclusiveEvent = eEvent; }

public:
	HRESULT			Ready_Camera_Player(const _vec3 & vEye, const _vec3 & vAt, const _vec3 & vWorldUp, CPlayer* pTargetPlayer, const _float & fFovY = D3DXToRadian(60.f), const _float & fAspect = (_float)BACKCX / (_float)BACKCY, const _float & fNear = 0.2f, const _float & fFar = 1000.f);
	virtual _int	Update_GameObject(const _float& fTimeDelta) final;


private:
	HRESULT	Ready_Camera_Target(CPlayer*	pTarget);
	HRESULT	Ready_Camera_Parameters();
	
	void	Update_InputState(const _float& fTimeDelta);
	void	Update_Event(const _float& fTimeDelta);
	void	Update_OutEvent(const _float& fTimeDelta);
	void	Update_ExclusiveEvent(const _float& fTimeDelta);

	void	Setting_ToPlayer();

	void	Shake(const _float& fTimeDelta, const _uint& iShakeForce);
	
	/* BACK UP RIGHT AT*/
	void	Gradually_Change_CameraSet(const _float& fTimeDelta, const _float& fTime, const _float& BACK, const _float& UP, const _float& RIGHT, const _float& AT);
	void	Gradually_Change_Parameters(const _float& f, const _float& fTime, const _float& fTimeDelta);

	void	Gradually_Change_CameraPos(const _float& fTimeDelta, const _vec3& vStartPos, const _vec3& vEndPos, const _float& fTime, const _vec3& vAt );

	void	On_Terrain();


/*MEMBER*/
private:
	CInput_Device*				m_pInput_Device = nullptr;

	/*pull data from player*/
	CPlayer*					m_pPlayer = nullptr;
	const vector<CBone*>*		m_pVecPlayerBone = nullptr;
	_float						m_fTerrainHeight = 0.f;
	CVIBuffer*					m_pTerrainBuffer = nullptr; 


	/*parameters*/

	_float			m_fBackDist = 0.f; /*dist from camera*/
	_float			m_fRightDist = 0.f;
	_float			m_fUpDist = 0.f; 
	_float			m_fAtDist = 0.f;

	_float			m_fPreBack = 0.f;
	_float			m_fPreRight = 0.f;
	_float			m_fPreUp = 0.f;
	_float			m_fPreAt = 0.f;


	_bool			b = false; /* shake*/

	EVENT			m_ePreEvent = EVENT_NONE;
	EVENT			m_eCurEvent = EVENT_NONE;
	_float			m_fEventTimer = 0.f;

	OUTEVENT		m_ePreOutEvent = OUTEVENT_NONE;
	OUTEVENT		m_eCurOutEvent = OUTEVENT_NONE;
	_float			m_fOutEventTimer = 0.f;

	EXCLUSIVEEVENT	m_ePreExclusiveEvent = EXCLUSIVEEVENT_NONE;
	EXCLUSIVEEVENT	m_eCurExclusiveEvent = EXCLUSIVEEVENT_NONE;
	_float			m_fExclusiveEventTimer = 0.f;
	_vec3			m_vEventStartPos;
	_vec3			m_vDest1;


/*related ctor dtor*/
public:
	static CCamera_Player* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vEye, const _vec3 & vAt, const _vec3 & vWorldUp, CPlayer* pTargetPlayer, const _float & fFovY = D3DXToRadian(60.f), const _float & fAspect = (_float)BACKCX / (_float)BACKCY, const _float & fNear = 0.2f, const _float & fFar = 1000.f);


private:
	virtual _ulong Free() final;
};

