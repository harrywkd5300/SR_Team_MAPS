#include "..\Headers\Picking.h"
#include "VIBuffer.h"

USING(Engine)

CPicking::CPicking(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CPicking::CPicking(const CPicking & rhs)
	: CComponent(rhs)
	, m_iVertexPos(rhs.m_iVertexPos)
	, eDir(rhs.eDir)
{
}

HRESULT CPicking::Ready_Picking()
{
	return NOERROR;
}

_vec3 * CPicking::Picking_ToBuffer(HWND hWnd, _uint iBackSizeX, _uint iBackSizeY, Engine::CVIBuffer * pTargetBuffer, const _matrix* pWorldMatrix/* = nullptr*/)
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	POINT			ptMouse;

	// In.Screen Space
	GetCursorPos(&ptMouse);

	// In.Client Space(Viewport Space)
	ScreenToClient(hWnd, &ptMouse);

	_vec3			vMousePos;

	// In.Projection Space
	// Compute BackBufferSize
	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);
	vMousePos.x = ptMouse.x / (ViewPort.Width  * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height  * 0.5f) + 1.f;
	vMousePos.z = 0.0f;

	// In.View Space
	_matrix			matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3			vRay, vPivot;
	vPivot = _vec3(0.0f, 0.0f, 0.0f);
	vRay = vMousePos - vPivot;

	// In.World Space
	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vPivot, &vPivot, &matView);
	D3DXVec3TransformNormal(&vRay, &vRay, &matView);

	// In.Local Space
	if (nullptr != pWorldMatrix)
	{
		_matrix		matWorld;
		D3DXMatrixInverse(&matWorld, nullptr, pWorldMatrix);
		D3DXVec3TransformCoord(&vPivot, &vPivot, &matWorld);
		D3DXVec3TransformNormal(&vRay, &vRay, &matWorld);
	}

	const _vec3* pVertexPos = pTargetBuffer->Get_VertexPos();

	//tuple<_uint, _uint, _float>	Tuple = pTargetBuffer->Get_VertexCntXZItv();

	_uint iVertexX = pTargetBuffer->Get_VertexCountX();
	_uint iVertexZ = pTargetBuffer->Get_VertexCountZ();
	_float fInterval = pTargetBuffer->Get_VertexItv();

	_float		fU = 0.f, fV = 0.f, fDist = 0.f;

	for (size_t i = 0; i <iVertexZ - 1; i++)
	{
		for (size_t j = 0; j < iVertexX - 1; j++)
		{
			_uint		iIndex = i * iVertexX + j;

			_uint		iLT, iRT, iLB, iRB;

			iLT = iIndex + iVertexX;
			iRT = iLT + 1;
			iLB = iIndex;
			iRB = iIndex + 1;

			// right top
			if (TRUE == D3DXIntersectTri(&pVertexPos[iRT], &pVertexPos[iLT], &pVertexPos[iRB], &vPivot, &vRay
				, &fU, &fV, &fDist))
			{
				//return &(vPivot + (*D3DXVec3Normalize(&vRay,&vRay) * fDist));
				//return &(vPivot + vRay * fDist);
				return &(pVertexPos[iRT] + (pVertexPos[iLT] - pVertexPos[iRT]) * fU + (pVertexPos[iRB] - pVertexPos[iRT]) * fV);
			}

			// left bottomK
			if (TRUE == D3DXIntersectTri(&pVertexPos[iLB], &pVertexPos[iRB], &pVertexPos[iLT], &vPivot, &vRay
				, &fU, &fV, &fDist))
			{
				//return &(vPivot + (*D3DXVec3Normalize(&vRay,&vRay) * fDist));
				return &(pVertexPos[iLB] + (pVertexPos[iRB] - pVertexPos[iLB]) * fU + (pVertexPos[iLT] - pVertexPos[iLB]) * fV);
			}
		}
	}

	return nullptr;
	//const _vec3* pVertexPos = pTargetBuffer->Get_VertexPos();
	//_uint iVertexX = pTargetBuffer->Get_VertexCountX();
	//_uint iVertexZ = pTargetBuffer->Get_VertexCountZ();
	//_float fInterval = pTargetBuffer->Get_VertexItv();

	//_float		fU = 0.f, fV = 0.f, fDist = 0.f;

	//for (size_t i = 0; i < iVertexZ - 1; i++)
	//{
	//	for (size_t j = 0; j < iVertexX - 1; j++)
	//	{
	//		_uint		iIndex = i * iVertexX + j;

	//		_uint		iLT, iRT, iLB, iRB;

	//		iLT = iIndex + iVertexX;
	//		iRT = iLT + 1;
	//		iLB = iIndex;
	//		iRB = iIndex + 1;

	//		// right top
	//		if (TRUE == D3DXIntersectTri(&pVertexPos[iRT], &pVertexPos[iLT], &pVertexPos[iRB], &vPivot, &vRay
	//			, &fU, &fV, &fDist))
	//		{
	//			return &(pVertexPos[iRT] + (pVertexPos[iLT] - pVertexPos[iRT]) * fU + (pVertexPos[iRB] - pVertexPos[iRT]) * fV);
	//		}

	//		// left bottomK
	//		if (TRUE == D3DXIntersectTri(&pVertexPos[iLB], &pVertexPos[iRB], &pVertexPos[iLT], &vPivot, &vRay
	//			, &fU, &fV, &fDist))
	//		{
	//			return &(pVertexPos[iLB] + (pVertexPos[iRB] - pVertexPos[iLB]) * fU + (pVertexPos[iLT] - pVertexPos[iLB]) * fV);
	//		}
	//	}
	//}
	//return nullptr;
}

const _vec3 * CPicking::Picking_ToBuffer(HWND hWnd, _vec3 *&pOut, _uint iBackSizeX, _uint iBackSizeY, Engine::CVIBuffer * pTargetBuffer, const _matrix * pWorldMatrix)
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	POINT			ptMouse;

	// In.Screen Space
	GetCursorPos(&ptMouse);

	// In.Client Space(Viewport Space)
	ScreenToClient(hWnd, &ptMouse);

	_vec3			vMousePos;

	// In.Projection Space
	// Compute BackBufferSize
	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);
	vMousePos.x = ptMouse.x / (ViewPort.Width  * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height  * 0.5f) + 1.f;
	vMousePos.z = 0.0f;

	// In.View Space
	_matrix			matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3			vRay, vPivot;
	vPivot = _vec3(0.0f, 0.0f, 0.0f);
	vRay = vMousePos - vPivot;

	// In.World Space
	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vPivot, &vPivot, &matView);
	D3DXVec3TransformNormal(&vRay, &vRay, &matView);

	// In.Local Space
	if (nullptr != pWorldMatrix)
	{
		_matrix		matWorld;
		D3DXMatrixInverse(&matWorld, nullptr, pWorldMatrix);
		D3DXVec3TransformCoord(&vPivot, &vPivot, &matWorld);
		D3DXVec3TransformNormal(&vRay, &vRay, &matWorld);
	}

	const _vec3* pVertexPos = pTargetBuffer->Get_VertexPos();

	//tuple<_uint, _uint, _float>	Tuple = pTargetBuffer->Get_VertexCntXZItv();

	_uint iVertexX = pTargetBuffer->Get_VertexCountX();
	_uint iVertexZ = pTargetBuffer->Get_VertexCountZ();
	_float fInterval = pTargetBuffer->Get_VertexItv();

	_float		fU = 0.f, fV = 0.f, fDist = 0.f;

	for (size_t i = 0; i <iVertexZ - 1; i++)
	{
		for (size_t j = 0; j < iVertexX - 1; j++)
		{
			_uint		iIndex = i * iVertexX + j;

			_uint		iLT, iRT, iLB, iRB;

			iLT = iIndex + iVertexX;
			iRT = iLT + 1;
			iLB = iIndex;
			iRB = iIndex + 1;

			// right top
			if (TRUE == D3DXIntersectTri(&pVertexPos[iRT], &pVertexPos[iLT], &pVertexPos[iRB], &vPivot, &vRay
				, &fU, &fV, &fDist))
			{
				//return &(vPivot + (*D3DXVec3Normalize(&vRay,&vRay) * fDist));
				//return &(vPivot + vRay * fDist);
				//pOut = new _vec3();
				*pOut = (pVertexPos[iRT] + (pVertexPos[iLT] - pVertexPos[iRT]) * fU + (pVertexPos[iRB] - pVertexPos[iRT]) * fV);
				return pOut;
			}

			// left bottomK
			if (TRUE == D3DXIntersectTri(&pVertexPos[iLB], &pVertexPos[iRB], &pVertexPos[iLT], &vPivot, &vRay
				, &fU, &fV, &fDist))
			{
				//return &(vPivot + (*D3DXVec3Normalize(&vRay,&vRay) * fDist));
				//pOut = new _vec3();
				*pOut = (pVertexPos[iLB] + (pVertexPos[iRB] - pVertexPos[iLB]) * fU + (pVertexPos[iLT] - pVertexPos[iLB]) * fV);
				return pOut;
			}
		}
	}
	pOut = nullptr;
	return nullptr;
}

const _vec3 * CPicking::Picking_ForWeapon(HWND hWnd, _vec3 *& pOut, _uint iBackSizeX, _uint iBackSizeY, Engine::CVIBuffer * pTargetBuffer, const _matrix * pWorldMatrix)
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	POINT			ptMouse;

	// In.Screen Space
	GetCursorPos(&ptMouse);

	// In.Client Space(Viewport Space)
	ScreenToClient(hWnd, &ptMouse);

	_vec3			vMousePos;

	// In.Projection Space
	// Compute BackBufferSize
	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);
	vMousePos.x = ptMouse.x / (ViewPort.Width  * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height  * 0.5f) + 1.f;
	vMousePos.z = 0.0f;

	// In.View Space
	_matrix			matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3			vRay, vPivot;
	vPivot = _vec3(0.0f, 0.0f, 0.0f);
	vRay = vMousePos - vPivot;

	// In.World Space
	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vPivot, &vPivot, &matView);
	D3DXVec3TransformNormal(&vRay, &vRay, &matView);

	// In.Local Space
	if (nullptr != pWorldMatrix)
	{
		_matrix		matWorld;
		D3DXMatrixInverse(&matWorld, nullptr, pWorldMatrix);
		D3DXVec3TransformCoord(&vPivot, &vPivot, &matWorld);
		D3DXVec3TransformNormal(&vRay, &vRay, &matWorld);
	}

	const _vec3* pVertexPos = pTargetBuffer->Get_VertexPos();

	//tuple<_uint, _uint, _float>	Tuple = pTargetBuffer->Get_VertexCntXZItv();

	_uint iVertexX = pTargetBuffer->Get_VertexCountX();
	_uint iVertexZ = pTargetBuffer->Get_VertexCountZ();
	_float fInterval = pTargetBuffer->Get_VertexItv();

	_float		fU = 0.f, fV = 0.f, fDist = 0.f;

	for (size_t i = 0; i <iVertexZ - 1; i++)
	{
		for (size_t j = 0; j < iVertexX - 1; j++)
		{
			_uint		iIndex = i * iVertexX + j;

			_uint		iLT, iRT, iLB, iRB;

			iLT = iIndex + iVertexX;
			iRT = iLT + 1;
			iLB = iIndex;
			iRB = iIndex + 1;

			// right top
			if (TRUE == D3DXIntersectTri(&pVertexPos[iRT], &pVertexPos[iLT], &pVertexPos[iRB], &vPivot, &vRay
				, &fU, &fV, &fDist))
			{
				//return &(vPivot + (*D3DXVec3Normalize(&vRay,&vRay) * fDist));
				//return &(vPivot + vRay * fDist);
				//pOut = new _vec3();
				*pOut = (pVertexPos[iRT] + (pVertexPos[iLT] - pVertexPos[iRT]) * fU + (pVertexPos[iRB] - pVertexPos[iRT]) * fV);
				return pOut;
			}

			// left bottomK
			if (TRUE == D3DXIntersectTri(&pVertexPos[iLB], &pVertexPos[iRB], &pVertexPos[iLT], &vPivot, &vRay
				, &fU, &fV, &fDist))
			{
				//return &(vPivot + (*D3DXVec3Normalize(&vRay,&vRay) * fDist));
				//pOut = new _vec3();
				*pOut = (pVertexPos[iLB] + (pVertexPos[iRB] - pVertexPos[iLB]) * fU + (pVertexPos[iLT] - pVertexPos[iLB]) * fV);
				return pOut;
			}
		}
	}

	*pOut = vPivot + vRay*100.f;
	return pOut;
	//pOut = nullptr;
	//return nullptr;
}



_vec3 CPicking::Picking_ToMakeBuffer(HWND hWnd, _uint iBackSizeX, _uint iBackSizeY, Engine::CVIBuffer * pTargetBuffer, const _matrix * pWorldMatrix, _float fMfCX, _float fMfCY)
{
	if (nullptr == m_pGraphicDev)
		return _vec3(0.f, 0.f, 0.f);

	_vec3			vMousePos;

	// In.Projection Space
	// Compute BackBufferSize
	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);
	vMousePos.x = fMfCX / (ViewPort.Width  * 0.5f) - 1.f;
	vMousePos.y = fMfCY / -(ViewPort.Height  * 0.5f) + 1.f;
	vMousePos.z = 0.0f;

	// In.View Space
	_matrix			matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3			vRay, vPivot;
	vPivot = _vec3(0.0f, 0.0f, 0.0f);
	vRay = vMousePos - vPivot;

	// In.World Space
	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vPivot, &vPivot, &matView);
	D3DXVec3TransformNormal(&vRay, &vRay, &matView);

	// In.Local Space
	if (nullptr != pWorldMatrix)
	{
		_matrix		matWorld;
		D3DXMatrixInverse(&matWorld, nullptr, pWorldMatrix);
		D3DXVec3TransformCoord(&vPivot, &vPivot, &matWorld);
		D3DXVec3TransformNormal(&vRay, &vRay, &matWorld);
	}

	const _vec3* pVertexPos = pTargetBuffer->Get_VertexPos();
	_uint iVertexX = pTargetBuffer->Get_VertexCountX();
	_uint iVertexZ = pTargetBuffer->Get_VertexCountZ();
	_float fInterval = pTargetBuffer->Get_VertexItv();

	_float		fU = 0.f, fV = 0.f, fDist = 0.f;

	_float fCntX = 0.f;
	_float fCntZ = 0.f;

	for (size_t i = 0; i < iVertexZ - 1; i++)
	{
		for (size_t j = 0; j < iVertexX - 1; j++)
		{
			_uint		iIndex = i * iVertexX + j;

			_uint		iLT, iRT, iLB, iRB;

			iLT = iIndex + iVertexX;
			iRT = iLT + 1;
			iLB = iIndex;
			iRB = iIndex + 1;

			// right top
			if (TRUE == D3DXIntersectTri(&pVertexPos[iRT], &pVertexPos[iLT], &pVertexPos[iRB], &vPivot, &vRay
				, &fU, &fV, &fDist))
			{
				_float fX = (pVertexPos[iRT].x - pVertexPos[iLT].x) * 0.5f + fCntX * fInterval;
				_float fZ = (pVertexPos[iRT].z - pVertexPos[iRB].z) * 0.5f + fCntZ * fInterval;
				_float fY = pVertexPos[iRT].y - ((pVertexPos[iRB].y - pVertexPos[iLT].y)) + fInterval * 0.5f;
				_vec3 vMakePos = _vec3(fX, fY, fZ);
				return vMakePos;
			}

			// left bottomK
			if (TRUE == D3DXIntersectTri(&pVertexPos[iLB], &pVertexPos[iRB], &pVertexPos[iLT], &vPivot, &vRay
				, &fU, &fV, &fDist))
			{
				_float fX = (pVertexPos[iRB].x - pVertexPos[iLB].x) * 0.5f + fCntX * fInterval;
				_float fZ = (pVertexPos[iRT].z - pVertexPos[iRB].z) * 0.5f + fCntZ * fInterval;
				_float fY = pVertexPos[iRT].y - ((pVertexPos[iRB].y - pVertexPos[iLT].y)) + fInterval * 0.5f;
				_vec3 vMakePos = _vec3(fX, fY, fZ);
				return vMakePos;
			}
			++fCntX;
		}
		fCntX = 0.f;
		++fCntZ;
	}
	return _vec3(0.f, 0.f, 0.f);
}

_bool CPicking::Picking_ToObject(HWND hWnd, Engine::CVIBuffer * pTargetBuffer, const _matrix* pWorldMatrix, _float fMfCX, _float fMfCY)
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	_vec3			vMousePos;

	// In.Projection Space
	// Compute BackBufferSize
	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);
	vMousePos.x = fMfCX / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = fMfCY / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.0f;

	// In.View Space
	_matrix			matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3			vRay, vPivot;
	vPivot = _vec3(0.0f, 0.0f, 0.0f);
	vRay = vMousePos - vPivot;

	// In.World Space
	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vPivot, &vPivot, &matView);
	D3DXVec3TransformNormal(&vRay, &vRay, &matView);

	// In.Local Space
	if (nullptr != pWorldMatrix)
	{
		_matrix		matWorld;
		D3DXMatrixInverse(&matWorld, nullptr, pWorldMatrix);
		D3DXVec3TransformCoord(&vPivot, &vPivot, &matWorld);
		D3DXVec3TransformNormal(&vRay, &vRay, &matWorld);
	}

	const _vec3*	pVertexPos = pTargetBuffer->Get_VertexPos();
	_uint	iVertexCntX = pTargetBuffer->Get_VertexCountX();
	_uint	iVertexCntZ = pTargetBuffer->Get_VertexCountZ();
	_float	iInterval = pTargetBuffer->Get_VertexItv();

	_float		fU = 0.f, fV = 0.f, fDist = 0.f;
	_float		fMaxDist = -1.f;
	m_iVertexPos = _vec3(0.f, 0.f, 0.f);

	// +x, RightUp
	if (TRUE == D3DXIntersectTri(&pVertexPos[5], &pVertexPos[1], &pVertexPos[6], &vPivot, &vRay
		, &fU, &fV, &fDist))
	{
		if (fDist > fMaxDist)
		{
			if (0 > fMaxDist)
			{
				m_iVertexPos = pVertexPos[5] + (pVertexPos[1] - pVertexPos[5]) * 0.5f + (pVertexPos[6] - pVertexPos[5]) * 0.5f;
				fMaxDist = fDist;
			}
		}
		else
		{
			m_iVertexPos = pVertexPos[5] + (pVertexPos[1] - pVertexPos[5]) * 0.5f + (pVertexPos[6] - pVertexPos[5]) * 0.5f;
			fMaxDist = fDist;
		}
	}
	// +x, LeftDown
	if (TRUE == D3DXIntersectTri(&pVertexPos[2], &pVertexPos[6], &pVertexPos[1], &vPivot, &vRay
		, &fU, &fV, &fDist))
	{
		if (fDist > fMaxDist)
		{
			if (0 > fMaxDist)
			{
				m_iVertexPos = pVertexPos[2] + (pVertexPos[6] - pVertexPos[2]) * 0.5f + (pVertexPos[1] - pVertexPos[2]) * 0.5f;
				fMaxDist = fDist;
			}
		}
		else
		{
			m_iVertexPos = pVertexPos[2] + (pVertexPos[6] - pVertexPos[2]) * 0.5f + (pVertexPos[1] - pVertexPos[2]) * 0.5f;
			fMaxDist = fDist;
		}
	}
	// -x, RightUp
	if (TRUE == D3DXIntersectTri(&pVertexPos[0], &pVertexPos[4], &pVertexPos[3], &vPivot, &vRay
		, &fU, &fV, &fDist))
	{
		if (fDist > fMaxDist)
		{
			if (0 > fMaxDist)
			{
				m_iVertexPos = pVertexPos[0] + (pVertexPos[4] - pVertexPos[0]) * 0.5f + (pVertexPos[3] - pVertexPos[0]) * 0.5f;
				fMaxDist = fDist;
			}
		}
		else
		{
			m_iVertexPos = pVertexPos[0] + (pVertexPos[4] - pVertexPos[0]) * 0.5f + (pVertexPos[3] - pVertexPos[0]) * 0.5f;
			fMaxDist = fDist;
		}
	}
	// -x, LeftDown
	if (TRUE == D3DXIntersectTri(&pVertexPos[7], &pVertexPos[3], &pVertexPos[4], &vPivot, &vRay
		, &fU, &fV, &fDist))
	{
		if (fDist > fMaxDist)
		{
			if (0 > fMaxDist)
			{
				m_iVertexPos = pVertexPos[7] + (pVertexPos[4] - pVertexPos[7]) * 0.5f + (pVertexPos[3] - pVertexPos[7]) * 0.5f;
				fMaxDist = fDist;
			}
		}
		else
		{
			m_iVertexPos = pVertexPos[7] + (pVertexPos[4] - pVertexPos[7]) * 0.5f + (pVertexPos[3] - pVertexPos[7]) * 0.5f;
			fMaxDist = fDist;
		}
	}
	// +y, RightUp
	if (TRUE == D3DXIntersectTri(&pVertexPos[5], &pVertexPos[4], &pVertexPos[1], &vPivot, &vRay
		, &fU, &fV, &fDist))
	{
		if (fDist > fMaxDist)
		{
			if (0 > fMaxDist)
			{
				m_iVertexPos = pVertexPos[5] + (pVertexPos[4] - pVertexPos[5]) * 0.5f + (pVertexPos[1] - pVertexPos[5]) * 0.5f;
				fMaxDist = fDist;
			}
		}
		else
		{
			m_iVertexPos = pVertexPos[5] + (pVertexPos[4] - pVertexPos[5]) * 0.5f + (pVertexPos[1] - pVertexPos[5]) * 0.5f;
			fMaxDist = fDist;
		}
	}
	// +y, LeftDown
	if (TRUE == D3DXIntersectTri(&pVertexPos[0], &pVertexPos[1], &pVertexPos[4], &vPivot, &vRay
		, &fU, &fV, &fDist))
	{
		if (fDist > fMaxDist)
		{
			if (0 > fMaxDist)
			{
				m_iVertexPos = pVertexPos[0] + (pVertexPos[4] - pVertexPos[0]) * 0.5f + (pVertexPos[1] - pVertexPos[0]) * 0.5f;
				fMaxDist = fDist;
			}
		}
		else
		{
			m_iVertexPos = pVertexPos[0] + (pVertexPos[4] - pVertexPos[0]) * 0.5f + (pVertexPos[1] - pVertexPos[0]) * 0.5f;
			fMaxDist = fDist;
		}
	}
	// -y, RightUp
	if (TRUE == D3DXIntersectTri(&pVertexPos[2], &pVertexPos[3], &pVertexPos[6], &vPivot, &vRay
		, &fU, &fV, &fDist))
	{
		if (fDist > fMaxDist)
		{
			if (0 > fMaxDist)
			{
				m_iVertexPos = pVertexPos[2] + (pVertexPos[3] - pVertexPos[2]) * 0.5f + (pVertexPos[6] - pVertexPos[2]) * 0.5f;
				fMaxDist = fDist;
			}
		}
		else
		{
			m_iVertexPos = pVertexPos[2] + (pVertexPos[3] - pVertexPos[2]) * 0.5f + (pVertexPos[6] - pVertexPos[2])* 0.5f;
			fMaxDist = fDist;
		}
	}
	// -y, LeftDown
	if (TRUE == D3DXIntersectTri(&pVertexPos[7], &pVertexPos[6], &pVertexPos[3], &vPivot, &vRay
		, &fU, &fV, &fDist))
	{
		if (fDist > fMaxDist)
		{
			if (0 > fMaxDist)
			{
				m_iVertexPos = pVertexPos[7] + (pVertexPos[3] - pVertexPos[7]) * 0.5f + (pVertexPos[6] - pVertexPos[7]) * 0.5f;
				fMaxDist = fDist;
			}
		}
		else
		{
			m_iVertexPos = pVertexPos[7] + (pVertexPos[3] - pVertexPos[7]) * 0.5f + (pVertexPos[6] - pVertexPos[7]) * 0.5f;
			fMaxDist = fDist;
		}
	}
	// +z, RightUp
	if (TRUE == D3DXIntersectTri(&pVertexPos[4], &pVertexPos[5], &pVertexPos[7], &vPivot, &vRay
		, &fU, &fV, &fDist))
	{
		if (fDist > fMaxDist)
		{
			if (0 > fMaxDist)
			{
				m_iVertexPos = pVertexPos[4] + (pVertexPos[5] - pVertexPos[4]) * 0.5f + (pVertexPos[7] - pVertexPos[4]) * 0.5f;
				fMaxDist = fDist;
			}
		}
		else
		{
			m_iVertexPos = pVertexPos[4] + (pVertexPos[5] - pVertexPos[4]) * 0.5f + (pVertexPos[7] - pVertexPos[4]) * 0.5f;
			fMaxDist = fDist;
		}
	}
	// +z, LeftDown
	if (TRUE == D3DXIntersectTri(&pVertexPos[6], &pVertexPos[7], &pVertexPos[5], &vPivot, &vRay
		, &fU, &fV, &fDist))
	{
		if (fDist > fMaxDist)
		{
			if (0 > fMaxDist)
			{
				m_iVertexPos = pVertexPos[6] + (pVertexPos[5] - pVertexPos[6]) * 0.5f + (pVertexPos[7] - pVertexPos[6]) * 0.5f;
				fMaxDist = fDist;
			}
		}
		else
		{
			m_iVertexPos = pVertexPos[6] + (pVertexPos[5] - pVertexPos[6]) * 0.5f + (pVertexPos[7] - pVertexPos[6]) * 0.5f;
			fMaxDist = fDist;
		}
	}
	// -z, RightUp
	if (TRUE == D3DXIntersectTri(&pVertexPos[1], &pVertexPos[0], &pVertexPos[2], &vPivot, &vRay
		, &fU, &fV, &fDist))
	{
		if (fDist > fMaxDist)
		{
			if (0 > fMaxDist)
			{
				m_iVertexPos = pVertexPos[1] + (pVertexPos[0] - pVertexPos[1]) * 0.5f + (pVertexPos[2] - pVertexPos[1]) * 0.5f;
				fMaxDist = fDist;
			}
		}
		else
		{
			m_iVertexPos = pVertexPos[1] + (pVertexPos[0] - pVertexPos[1]) * 0.5f + (pVertexPos[2] - pVertexPos[1]) * 0.5f;
			fMaxDist = fDist;
		}
	}
	// -z, LeftDown
	if (TRUE == D3DXIntersectTri(&pVertexPos[3], &pVertexPos[2], &pVertexPos[0], &vPivot, &vRay
		, &fU, &fV, &fDist))
	{
		if (-1 == fMaxDist)
		{
			m_iVertexPos = pVertexPos[3] + (pVertexPos[0] - pVertexPos[3]) * 0.5f + (pVertexPos[2] - pVertexPos[3]) * 0.5f;
			fMaxDist = fDist;
		}
		if (fDist < fMaxDist)
		{
			m_iVertexPos = pVertexPos[3] + (pVertexPos[0] - pVertexPos[3]) * 0.5f + (pVertexPos[2] - pVertexPos[3]) * 0.5f;
			fMaxDist = fDist;
		}
	}

	if (0 < fMaxDist)
	{
		m_iVertexPos *= 2.f;
		D3DXVec3TransformCoord(&m_iVertexPos, &m_iVertexPos, pWorldMatrix);
 		return true;
	}

	return false;
}

CComponent * CPicking::Clone(void)
{
	return new CPicking(*this);
}

CPicking * CPicking::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPicking* pInstance = new CPicking(pGraphicDev);

	if (FAILED(pInstance->Ready_Picking()))
	{
		MSG_BOX("CPicking Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CPicking::Free()
{
	return CComponent::Free();
}
