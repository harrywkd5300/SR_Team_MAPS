#include "stdafx.h"
#include "Camera_Player.h"
#include "Input_Device.h"
#include "player.h"
#include "Object_Manager.h"

CCamera_Player::CCamera_Player(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev), m_pInput_Device(CInput_Device::GetInstance())
	,m_pPlayer(nullptr)
	,m_pVecPlayerBone(nullptr)
	,m_pTerrainBuffer(nullptr)
	, m_fTerrainHeight(0.f)
	, m_fBackDist(0.f)
	, m_fRightDist(0.f)
	, m_fUpDist(0.f)
	, m_fAtDist(0.f)
	, m_eCurEvent(EVENT_NONE)
{
	m_pInput_Device->AddRef();
	ZeroMemory(&m_vEventStartPos, sizeof(_vec3));
}

HRESULT CCamera_Player::Ready_Camera_Player(const _vec3 & vEye, const _vec3 & vAt, const _vec3 & vWorldUp, CPlayer* pTargetPlayer, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	if (FAILED(Ready_Camera(vEye, vAt, vWorldUp, fFovY, fAspect, fNear, fFar)))
		goto exception;

	if (FAILED(Ready_Camera_Target(pTargetPlayer)))
		goto exception;

	if (FAILED(Ready_Camera_Parameters()))
		goto exception;

	return NOERROR;

exception:
	MSG_BOX("Ready Camera failed");
	return E_FAIL;
}

_int CCamera_Player::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pPlayer)
	{
		MSG_BOX("Player Camera Target Lost");
		return -1;
	}

	Update_ExclusiveEvent(fTimeDelta);

	if (EXCLUSIVEEVENT_NONE	== m_eCurExclusiveEvent)
	{
		Update_Event(fTimeDelta);
		Setting_ToPlayer();
		Update_OutEvent(fTimeDelta);

	}

	Update_InputState(fTimeDelta);

	On_Terrain();

	this->SetUp_ViewMatrix();
	this->SetUp_ProjectionMatrix();

	return 0;
}

HRESULT CCamera_Player::Ready_Camera_Target(CPlayer* pTarget)
{
	if (nullptr == pTarget)
		return E_FAIL;


	m_pPlayer = pTarget;
	pTarget->AddRef();
	m_pVecPlayerBone = m_pPlayer->Get_BoneVector();
	
	m_pTerrainBuffer = dynamic_cast<CTerrain_Buffer*>(CObject_Manager::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain", 0, L"Buffer_Terrain"));
	m_pTerrainBuffer->AddRef();

	return NOERROR;
}

HRESULT CCamera_Player::Ready_Camera_Parameters()
{
	m_fBackDist = 1.2f;
	m_fRightDist = 0.4f;
	m_fUpDist = 1.f;
	m_fAtDist = 5.f;

	return NOERROR;
}

void CCamera_Player::Update_InputState(const _float & fTimeDelta)
{
	/*Wheel Move*/
	_long lMove = m_pInput_Device->Get_DIMouseMove(CInput_Device::DIM_Z);

	if (lMove)
		m_fBackDist -= lMove*fTimeDelta;


	if (m_pInput_Device->Get_DIKeyState(DIK_T))
	{
		if (b)
		{
			b = false;
			Shake(fTimeDelta, 40);
		}
		else
			b = !b;

	}


}

void CCamera_Player::Update_Event(const _float & fTimeDelta)
{

	/*Init*/
	if (m_eCurEvent != m_ePreEvent)
	{
		m_ePreEvent = m_eCurEvent;
		
		m_fEventTimer = 0;

		m_fPreBack = m_fBackDist;
		m_fPreRight = m_fRightDist;
		m_fPreUp = m_fUpDist;
		m_fPreAt = m_fAtDist;
	}

	switch (m_eCurEvent)
	{
		/*TimeDelta, Time , Back, Up, Right, Back*/
	case EVENT_NONE:
		Gradually_Change_CameraSet(fTimeDelta, 0.5f, 1.2f, 1.f, 0.4f, 5.f);
		break;

	case EVENT_ZOOMIN:
		Gradually_Change_CameraSet(fTimeDelta, 0.5f, 1.f, 1.f, 0.4f, 5.f);
		break;

	case EVENT_ZOOMOUT:
		Gradually_Change_CameraSet(fTimeDelta, 1.f, 1.2f, 1.f, 0.4f, 5.f);
		break;

	case EVENT_AIM:
		Gradually_Change_CameraSet(fTimeDelta, 0.2f, 0.9f, 1.2f, 0.7f, 5.f);
		break;
	}



}

void CCamera_Player::Update_OutEvent(const _float & fTimeDelta)
{
	/*Init*/
	if (m_eCurOutEvent != m_ePreOutEvent)
	{
		m_ePreOutEvent = m_eCurOutEvent;
		m_fOutEventTimer = 0;
	}

	m_fOutEventTimer += fTimeDelta;
	switch (m_eCurOutEvent)
	{

	case OUTEVENT_NONE:
		
		break;

	case OUTEVENT_WEAKSHAKE:

		if (m_fOutEventTimer > 0.5f)
			m_eCurOutEvent = OUTEVENT_NONE;

		if (b)
		{
		 	b = false;
		 	Shake(fTimeDelta, 40);
		}
		else
		 	b = !b;
		break;
	}
}

void CCamera_Player::Update_ExclusiveEvent(const _float & fTimeDelta)
{

	/*Init*/
	if (m_ePreExclusiveEvent != m_eCurExclusiveEvent)
	{
		m_ePreExclusiveEvent = m_eCurExclusiveEvent;
		m_fExclusiveEventTimer = 0;
		
		switch (m_eCurExclusiveEvent)
		{

		case EXCLUSIVEEVENT_NONE:

			break;

		case EXCLUSIVEEVENT_STARTGAME:
			m_vEventStartPos = m_vEye;
			m_vDest1 = _vec3(90.5, 1.f, 72.f);
			break;

		}
	}

	m_fExclusiveEventTimer += fTimeDelta;


	switch (m_eCurExclusiveEvent)
	{

	case EXCLUSIVEEVENT_NONE:

		break;

	case EXCLUSIVEEVENT_STARTGAME:

		if (m_fExclusiveEventTimer > 0.f && m_fExclusiveEventTimer < 1.f)
		{
			Gradually_Change_CameraPos(fTimeDelta, m_vEventStartPos, m_vDest1, 1.f, m_vDest1);
		}
		else if (m_fExclusiveEventTimer >= 1.f && m_fExclusiveEventTimer < 2.f)
		{
			Gradually_Change_CameraPos(fTimeDelta, m_vDest1, _vec3(98.f, 5.f, 70.f), 0.5f, _vec3(102.f, 2.0f, 68.f));
		}
		else if (m_fExclusiveEventTimer >= 2.f && m_fExclusiveEventTimer < 3.f)
		{
			Gradually_Change_CameraPos(fTimeDelta, _vec3(98.f, 5.f, 70.f), _vec3(105.f, 7.f, 50.f), 1.f, _vec3(102.f, 2.f, 68.f));
		}
		else if (m_fExclusiveEventTimer >= 3.f && m_fExclusiveEventTimer < 5.f)
		{
		}
		else
			m_eCurExclusiveEvent = EXCLUSIVEEVENT_NONE;

		break;
	}
}

void CCamera_Player::Shake(const _float & fTimeDelta, const _uint & iShakeForce)
{
	_vec3& vCameraRight = (_vec3&)m_matCameraWorld.m[0];
	_vec3& vCameraLook = (_vec3&)m_matCameraWorld.m[2];

	_long lMoveY = rand() % iShakeForce - _uint(iShakeForce*0.5);
	_long lMoveX = rand() % iShakeForce - _uint(iShakeForce*0.5);

	_matrix matRotationX;
	D3DXMatrixRotationAxis(&matRotationX, &vCameraRight, (lMoveY*fTimeDelta * 0.1f));
	D3DXVec3TransformNormal(&vCameraLook, &vCameraLook, &matRotationX);

	_matrix matRotationY;
	D3DXMatrixRotationAxis(&matRotationY, &_vec3(0.f, 1.f, 0.f), (lMoveX*fTimeDelta * 0.1f));
	D3DXVec3TransformNormal(&vCameraLook, &vCameraLook, &matRotationY);

	m_vAt = m_vEye + vCameraLook;
}

void CCamera_Player::Gradually_Change_CameraSet(const _float & fTimeDelta, const _float & fTime, const _float & BACK, const _float & UP, const _float & RIGHT, const _float & AT)
{
	m_fEventTimer += fTimeDelta;

	if (fTime < m_fEventTimer)
		return;

	_float f1 = ((BACK - m_fPreBack) / fTime)*fTimeDelta;
	_float f2 = ((RIGHT - m_fPreRight) / fTime)*fTimeDelta;
	_float f3 = ((UP - m_fPreUp) / fTime)*fTimeDelta;
	_float f4 = ((AT - m_fPreAt) / fTime)*fTimeDelta;

	m_fBackDist += f1;
	m_fRightDist += f2;
	m_fUpDist += f3;
	m_fAtDist += f4;
}

void CCamera_Player::Gradually_Change_Parameters(const _float & f, const _float & fTime, const _float & fTimeDelta)
{
	m_fEventTimer += fTimeDelta;

	if (fTime < m_fEventTimer)
		return;


	m_fBackDist += (f / fTime) * fTimeDelta;

}

void CCamera_Player::Gradually_Change_CameraPos(const _float & fTimeDelta, const _vec3 & vStartPos, const _vec3 & vEndPos, const _float & fTime, const _vec3 & vAt)
{
	_vec3 v = (vEndPos - vStartPos) / fTime;
	m_vAt = vAt;

	m_vEye += v *fTimeDelta;

}

void CCamera_Player::On_Terrain()
{
	if (m_vEye.y < m_fTerrainHeight)
		m_vEye.y = m_fTerrainHeight;

	_vec3& vPos = m_vEye;

	const _vec3*	pVertexPos = m_pTerrainBuffer->Get_VertexPos();

	const _uint& dwVtxCntX = m_pTerrainBuffer->Get_VertexCountX();
	const _uint& dwVtxCntZ = m_pTerrainBuffer->Get_VertexCountZ();
	const _float& fVtxItv = m_pTerrainBuffer->Get_VertexItv();

	_uint dwIndex = _int(vPos.x / fVtxItv) + (_int(vPos.z / fVtxItv) * dwVtxCntX);
	if (dwIndex >= dwVtxCntX * dwVtxCntZ)
	{
		return;
	}

	const _vec3& v0 = pVertexPos[dwIndex + dwVtxCntX];
	const _vec3& v1 = pVertexPos[dwIndex + dwVtxCntX + 1];
	const _vec3& v3 = pVertexPos[dwIndex];
	const _vec3& v2 = pVertexPos[dwIndex + 1];

	const _float fRatioX = (vPos.x - v0.x) / fVtxItv;
	const _float fRatioZ = (v0.z - vPos.z) / fVtxItv;


	if (fRatioX > fRatioZ) /*right polygon*/
	{
		m_fTerrainHeight = v0.y + ((v1.y - v0.y) * fRatioX) + ((v2.y - v1.y)*fRatioZ);
	}
	else /*left polygon*/
	{
		m_fTerrainHeight = v0.y + ((v3.y - v0.y) * fRatioZ) + ((v2.y - v3.y)*fRatioX);
	}

	if (m_vEye.y < m_fTerrainHeight)
		m_vEye.y = m_fTerrainHeight;
}


void CCamera_Player::Setting_ToPlayer()
{

	const _vec3& vPlayerRight	= m_pPlayer->Get_MatrixRow(CTransform::INFO_RIGHT);
	const _vec3& vPlayerPos		= m_pPlayer->Get_MatrixRow(CTransform::INFO_POSITION);
	//const _vec3& vPlayerLook	= m_pPlayer->Get_MatrixRow(CTransform::INFO_LOOK);
	_vec3 vLook = (*m_pVecPlayerBone)[CPlayer::BONE_EYE]->Get_MatrixRow(CTransform::INFO_LOOK);


	_vec3 vRight; 

	D3DXVec3Normalize(&vRight, &vPlayerRight);
	D3DXVec3Normalize(&vLook, &vLook);

	m_vEye.x = vPlayerPos.x - vLook.x * m_fBackDist + vRight.x * m_fRightDist;
	m_vEye.y = vPlayerPos.y - vLook.y * m_fBackDist + m_fUpDist;
	m_vEye.z = vPlayerPos.z - vLook.z * m_fBackDist + vRight.z * m_fRightDist;

	m_vAt = vPlayerPos + vLook * m_fAtDist;

}

CCamera_Player * CCamera_Player::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vEye, const _vec3 & vAt, const _vec3 & vWorldUp, CPlayer* pTargetPlayer, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CCamera_Player* pInstance = new CCamera_Player(pGraphicDev);

	if (pInstance->Ready_Camera_Player(vEye, vAt, vWorldUp, pTargetPlayer, fFovY, fAspect, fNear, fFar))
	{
		MSG_BOX("PlayerCamera creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CCamera_Player::Free()
{
	_ulong dwRefCount = 0;

	/*Decrease refcount*/
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pPlayer);
	Safe_Release(m_pTerrainBuffer);

	dwRefCount = CCamera::Free();

	return dwRefCount;
}
