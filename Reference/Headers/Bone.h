#pragma once

#include "transform.h"

class DLL_EXPORT CBone final
	:public CTransform
{
private:
	explicit CBone(LPDIRECT3DDEVICE9 pGrahpicDev);
	explicit CBone(const CBone& rhs);
	virtual ~CBone() = default;

/*FUNCTION*/
public:
	const _ulong&				Get_RefCount() { return CBase::Get_RefCount(); }
	const _int&					Get_ParentIndex() { return m_iParentIndex; }
	vector<vector<KEYFRAME*>>*	Get_VvecKeyFrame() { return &m_vvecKeyFrame; } /*[state][frame]*/

	void	Set_ParentIndex(const _int& iIndex) { m_iParentIndex = iIndex; }
	void	Set_StateChange(_bool b) { m_bStateChanged = b; }
	void	Set_ReceivedKeyFrame(_bool b) { m_bReceivedKeyFrame = b; }

public:
	void	Add_State();
	void	Del_State(const _uint& dwIndex);

	void	Add_Frame(const _uint& dwStateIndex);
	void	Del_Frame(const _uint& dwStateIndex, const _uint& dwFrameIndex);
	void	Set_FrameDegree(const _uint& dwStateIndex, const _uint& dwFrameIndex, const _vec3& vStart, const _vec3& vEnd);

	void	Set_StartDegree(const _uint& STATEINDEX, const _uint& FRAMEINDEX);
	void	Set_EndDegree(const _uint& STATEINDEX, const _uint& FRAMEINDEX);

public:
	_int	Update_PreKeyFrame(const _uint& STATEINDEX, const _uint& dwFrameIndex, const _float& fTimer, const _float& fTimeDelta);
	_int	Update_KeyFrame(const _uint& STATEINDEX, const _uint& dwFrameIndex ,const _float& fTimer,const _float& fTimeDelta);

	void	Rotation_HeadY(const _float& fDegree);
	void	Rotation_HeadX(const _float& fDegree);


/*MEMBER*/
private:
	/*[state][frame]*/
	vector<vector<KEYFRAME*>>	m_vvecKeyFrame;			/*[state][frameindex]*/
	_int						m_iParentIndex = -1;	/*-1 : has not parent*/
	_uint						m_dwFrame = 0;			/*current Frame*/
	_float						m_fFrameTimer = 0.f;	/*Frame Timer*/
	
	_bool						m_bStateChanged = false;
	_vec3						m_vStartDegree;

	_bool						m_bReceivedKeyFrame = true;

/*function related ctor dtor*/
public:
	static CBone* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual CComponent* Clone() final;
	virtual _ulong Free() final;
};

