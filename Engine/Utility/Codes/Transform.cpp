#include "..\Headers\Transform.h"
#include <iostream>


CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev), m_fDegreeX(0.f), m_fDegreeY(0.f), m_fDegreeZ(0.f), bIsCalcLook(false)
{
	ZeroMemory(&m_vLook, sizeof(_vec3));
}

CTransform::CTransform(const CTransform & rhs)
	:CComponent(rhs)
	, m_matWorld(rhs.m_matWorld)
	, m_matScale(rhs.m_matScale)
	, m_matRotationX(rhs.m_matRotationX)
	, m_matRotationY(rhs.m_matRotationY)
	, m_matRotationZ(rhs.m_matRotationZ)
	, m_matTrans(rhs.m_matTrans)
	, m_matParent(rhs.m_matParent)
	, m_fDegreeX(rhs.m_fDegreeX)
	, m_fDegreeY(rhs.m_fDegreeY)
	, m_fDegreeZ(rhs.m_fDegreeZ)
	, bIsCalcLook(rhs.bIsCalcLook)
	, m_vLook(rhs.m_vLook)
{
}

const _vec3& CTransform::Get_MatrixRow(ROW_INFO eInfo)
{
	return (_vec3&)m_matWorld.m[eInfo];
}

void	CTransform::Set_Degree(const _vec3& vRot)
{
	m_fDegreeX = vRot.x;
	m_fDegreeY = vRot.y;
	m_fDegreeZ = vRot.z;

	Rotation_X(0);
	Rotation_Y(0);
	Rotation_Z(0);
}

void CTransform::Set_Look(const _vec3 & vLook)
{
	m_vLook = vLook;

}

HRESULT CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matRotationX);
	D3DXMatrixIdentity(&m_matRotationY);
	D3DXMatrixIdentity(&m_matRotationZ);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matParent);
	m_vLook = _vec3(0.f, 0.f, 1.f);

	return NOERROR;
}

void CTransform::Fill_WorldMatrix()
{
	D3DXMatrixIdentity(&m_matWorld);

	if (false == bIsCalcLook)
	{
		m_matWorld = m_matScale * m_matRotationX * m_matRotationY * m_matRotationZ * m_matTrans * m_matParent;
	}
	else
	{
// 		const _float fScaleX = D3DXVec3Length(&(_vec3&)m_matScale.m[INFO_RIGHT]);
// 		const _float fScaleY = D3DXVec3Length(&(_vec3&)m_matScale.m[INFO_UP]);
// 		const _float fScaleZ = D3DXVec3Length(&(_vec3&)m_matScale.m[INFO_LOOK]);

// 		const _float fParentScaleX = D3DXVec3Length(&(_vec3&)m_matScale.m[INFO_RIGHT]);
// 		const _float fScaleY = D3DXVec3Length(&(_vec3&)m_matScale.m[INFO_UP]);
// 		const _float fScaleZ = D3DXVec3Length(&(_vec3&)m_matScale.m[INFO_LOOK]);

		_vec3& vPos = (_vec3&)m_matWorld.m[INFO_POSITION];
		vPos += (_vec3&)m_matTrans.m[INFO_POSITION];
		
		D3DXVec3Normalize(&m_vLook, &m_vLook);

		(_vec3&)m_matWorld.m[INFO_LOOK] = m_vLook;
		_vec3& vRight	= (_vec3&)m_matWorld.m[INFO_RIGHT];
		_vec3& vUp		= (_vec3&)m_matWorld.m[INFO_UP];

		D3DXVec3Cross(&vRight, &_vec3(0, 1.f, 0), &m_vLook);
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXVec3Cross(&vUp, &m_vLook, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);

		//vPos += (_vec3&)m_matParent.m[INFO_POSITION];

// 		(_vec3&)m_matWorld.m[INFO_RIGHT] *= fScaleX;
// 		(_vec3&)m_matWorld.m[INFO_UP] *= fScaleY;
// 		(_vec3&)m_matWorld.m[INFO_LOOK] *= fScaleZ;
		
		m_matWorld = m_matScale * m_matWorld * m_matParent;
	}
}

HRESULT CTransform::SetUp_MatrixToGraphicDev()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	return NOERROR;
}

void CTransform::Scaling(const _vec3 & vScale)
{
	m_matScale._11 = vScale.x;
	m_matScale._22 = vScale.y;
	m_matScale._33 = vScale.z;
}

void CTransform::Scaling(const _float & x, const _float & y, const _float & z)
{
	m_matScale._11 = x;
	m_matScale._22 = y;
	m_matScale._33 = z;
}

void CTransform::Rotation_X(const _float & fDegree)
{
	m_fDegreeX += fDegree;
	D3DXMatrixRotationX(&m_matRotationX, D3DXToRadian(m_fDegreeX));
}

void CTransform::Rotation_Y(const _float & fDegree)
{
	m_fDegreeY += fDegree;
	D3DXMatrixRotationY(&m_matRotationY, D3DXToRadian(m_fDegreeY));
}

void CTransform::Rotation_Z(const _float & fDegree)
{
	m_fDegreeZ += fDegree;
	D3DXMatrixRotationZ(&m_matRotationZ, D3DXToRadian(m_fDegreeZ));
}

void CTransform::Translation(const _vec3 & vTranslation)
{
	m_matTrans._41 = vTranslation.x;
	m_matTrans._42 = vTranslation.y;
	m_matTrans._43 = vTranslation.z;
}

void CTransform::Translation(const _float & x, const _float & y, const _float & z)
{
	m_matTrans._41 = x;
	m_matTrans._42 = y;
	m_matTrans._43 = z;
}

void CTransform::Parent(const _matrix & matParent)
{
	m_matParent = matParent;
}

void CTransform::Move_ToDirection(const _vec3& vDirection, const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3& vPos = (_vec3&)m_matTrans.m[INFO_POSITION];

	_vec3 vDir = vDirection;
	D3DXVec3Normalize(&vDir, &vDir);
	vPos += vDir * fSpeed * fTimeDelta;
}

void CTransform::Move_ToLook(const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3& vPos = (_vec3&)m_matTrans.m[INFO_POSITION];
	_vec3 vLook = (_vec3&)m_matWorld.m[INFO_LOOK];

	vLook.y = 0;
	D3DXVec3Normalize(&vLook, &vLook);

	vPos += vLook * fSpeed * fTimeDelta;
}

void CTransform::Move_ToRight(const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3& vPos = (_vec3&)m_matTrans.m[INFO_POSITION];
	_vec3 vDir = (_vec3&)m_matWorld.m[INFO_RIGHT];

	D3DXVec3Normalize(&vDir, &vDir);
	vPos += vDir * fSpeed * fTimeDelta;
}

void CTransform::Move_ToLeft(const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3& vPos = (_vec3&)m_matTrans.m[INFO_POSITION];
	_vec3 vDir = (_vec3&)m_matWorld.m[INFO_RIGHT];

	D3DXVec3Normalize(&vDir, &vDir);
	vPos -= vDir * fSpeed * fTimeDelta;
}


_vec3 CTransform::Get_IFMoveToLookVec(const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3 vPos = (_vec3&)m_matTrans.m[INFO_POSITION];
	_vec3 vLook = (_vec3&)m_matWorld.m[INFO_LOOK];

	vLook.y = 0;
	D3DXVec3Normalize(&vLook, &vLook);

	vPos += vLook * fSpeed * fTimeDelta;
	return vPos;
}

_vec3 CTransform::Get_IFMoveToRightVec(const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3 vPos = (_vec3&)m_matTrans.m[INFO_POSITION];
	_vec3 vDir = (_vec3&)m_matWorld.m[INFO_RIGHT];

	D3DXVec3Normalize(&vDir, &vDir);
	vPos += vDir * fSpeed * fTimeDelta;
	return vPos;
}

_vec3 CTransform::Get_IFMoveToLeftVec(const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3 vPos = (_vec3&)m_matTrans.m[INFO_POSITION];
	_vec3 vDir = (_vec3&)m_matWorld.m[INFO_RIGHT];

	D3DXVec3Normalize(&vDir, &vDir);
	vPos -= vDir * fSpeed * fTimeDelta;
	return vPos;
}

//void CTransform::Move_ToLook_CheckColAABB(const _float & fSpeed, const _float & fTimeDelta)
//{
//
//}

_float CTransform::On_Terrain(CVIBuffer * pTargetBuffer)
{
	_vec3& vPos = (_vec3&)m_matTrans.m[INFO_POSITION];
	
	const _vec3*	pVertexPos = pTargetBuffer->Get_VertexPos();

	const _uint& dwVtxCntX = pTargetBuffer->Get_VertexCountX();
	const _uint& dwVtxCntZ = pTargetBuffer->Get_VertexCountZ();
	const _float& fVtxItv = pTargetBuffer->Get_VertexItv();

	_uint dwIndex = _int(vPos.x / fVtxItv) + (_int(vPos.z / fVtxItv) * dwVtxCntX);

	const _vec3& v0 = pVertexPos[dwIndex + dwVtxCntX];
	const _vec3& v1 = pVertexPos[dwIndex + dwVtxCntX + 1];
	const _vec3& v3 = pVertexPos[dwIndex];
	const _vec3& v2 = pVertexPos[dwIndex + 1];

	const _float fRatioX = (vPos.x - v0.x) / fVtxItv;
	const _float fRatioZ = (v0.z - vPos.z) / fVtxItv;


	if (fRatioX > fRatioZ) /*right polygon*/
	{
		vPos.y = v0.y + ((v1.y - v0.y) * fRatioX) + ((v2.y - v1.y)*fRatioZ);
	}
	else /*left polygon*/
	{
		vPos.y = v0.y + ((v3.y - v0.y) * fRatioZ) + ((v2.y - v3.y)*fRatioX);
	}

	return vPos.y;
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform* pInstance = new CTransform(pGraphicDev);
	if (FAILED(pInstance->Ready_Transform()))
	{
		MSG_BOX("Transform creation failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTransform::Clone()
{
	return new CTransform(*this);
}

_ulong CTransform::Free()
{
	return CComponent::Free();
}

