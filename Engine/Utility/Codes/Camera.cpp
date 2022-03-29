#include "..\Headers\Camera.h"



CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_fFovY(0.f), m_fAspect(0.f), m_fNear(0.f), m_fFar(0.f)
{
	ZeroMemory(&m_vEye,sizeof(_vec3));
	ZeroMemory(&m_vAt, sizeof(_vec3));
	ZeroMemory(&m_vWorldUp, sizeof(_vec3));

	D3DXMatrixIdentity(&m_matCameraWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProjection);
}

HRESULT CCamera::Ready_Camera(const _vec3 & vEye, const _vec3 & vAt, const _vec3 & vWorldUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = vEye;
	m_vAt = vAt;
	m_vWorldUp = vWorldUp;
	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	SetUp_ViewMatrix();
	SetUp_ProjectionMatrix();

	return NOERROR;
}

void CCamera::SetUp_ViewMatrix()
{
	_vec3& vCameraRight = (_vec3&)(m_matCameraWorld.m[0]);
	_vec3& vCameraUp = (_vec3&)(m_matCameraWorld.m[1]);
	_vec3& vCameraLook = (_vec3&)(m_matCameraWorld.m[2]);
	_vec3& vCameraPos = (_vec3&)(m_matCameraWorld.m[3]);

	vCameraPos = m_vEye;

	vCameraLook = m_vAt - m_vEye;
	D3DXVec3Normalize(&vCameraLook, &vCameraLook);

	D3DXVec3Cross(&vCameraRight, &m_vWorldUp, &vCameraLook);
	D3DXVec3Normalize(&vCameraRight, &vCameraRight);

	D3DXVec3Cross(&vCameraUp, &vCameraLook, &vCameraRight);
	D3DXVec3Normalize(&vCameraUp, &vCameraUp);

	D3DXMatrixInverse(&m_matView, nullptr, &m_matCameraWorld);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void CCamera::SetUp_ProjectionMatrix()
{
	m_matProjection._11 = (1 / tanf(m_fFovY*0.5f)) / m_fAspect;
	m_matProjection._22 = (1 / tanf(m_fFovY*0.5f));
	m_matProjection._33 = m_fFar / m_fFar - m_fNear;
	m_matProjection._34 = 1;
	m_matProjection._43 = -(m_fFar*m_fNear) / (m_fFar - m_fNear);
	m_matProjection._44 = 0;

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProjection);
}

_ulong CCamera::Free()
{
	return CGameObject::Free();
}
