#include "stdafx.h"
#include "..\Headers\Collider_Manager.h"
#include "GameObject.h"
#include "Character.h"
#include "Graphic_Device.h"
#include "ColliderSkill.h"

//#include "Target_Manager.h"


IMPLEMENT_SINGLETON(CCollider_Manager)


CCollider_Manager::CCollider_Manager()
{

}

HRESULT CCollider_Manager::Ready_CCollider_Manager(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	if (pGraphicDevice == nullptr)
		return E_FAIL;

	m_pGraphicDev = pGraphicDevice;
	m_pGraphicDev->AddRef();

	return NOERROR;
}

void CCollider_Manager::Add_Character(CCharacter * pTargetChar)
{
	if (pTargetChar == nullptr)
		return;

	m_CharacterList[pTargetChar->Get_CharType()].push_back(pTargetChar);

	//cout <<"ColliderBox:" << 
	//	m_CharacterList[CHAR_TYPE::CHAR_TYPE_MONSTER].size() 
	//	+ m_CharacterList[CHAR_TYPE::CHAR_TYPE_PLAYER].size() << endl;
	//m_CharacterList[pTargetChar->Get_CharType()].remove()
}

void CCollider_Manager::Add_Target(CAABB_Buffer * pBuffer)
{
	if (pBuffer == nullptr)
		return;
	m_TargetBufferList.push_back(pBuffer);
}

void CCollider_Manager::Add_BackGround(CVIBuffer * pBuffer)
{
	if (pBuffer == nullptr)
		return;

	m_BackBufferList.push_back(pBuffer);

}

void CCollider_Manager::Add_ColliderSkill(CColliderSkill * pColliderSkill)
{
	if (pColliderSkill == nullptr)
		return;

	
	pColliderSkill->AddRef();
	m_SkillList.push_back(pColliderSkill);


}

CCharacter * CCollider_Manager::CheckCollBullet(HWND hWnd, _vec3* pOut, CHAR_TYPE eSourCharType, _vec3 muzzleWorldPos, _vec3 DestPos)
{
	CHAR_TYPE eDestCharType;
	if (eSourCharType == CHAR_TYPE::CHAR_TYPE_MONSTER)
	{
		eDestCharType = CHAR_TYPE::CHAR_TYPE_PLAYER;
	}
	else if(eSourCharType == CHAR_TYPE::CHAR_TYPE_PLAYER)
	{
		eDestCharType = CHAR_TYPE::CHAR_TYPE_MONSTER;
	}
	else
	{
		return nullptr;
	}

	_float fMinDist = 10000.f;
	_vec3 vReulstPos;
	CCharacter* pReulstChar = nullptr;
	CAABB_Buffer* pReulstTarget = nullptr;

	if (!m_CharacterList[eDestCharType].empty())
	{

		for (auto pChar : m_CharacterList[eDestCharType])
		{
			//_vec3			vMousePos(0.f, 0.f, 0.f); // 투영좌표상 마우스 위치(화면중앙고정)

			//_matrix	matProj;
			//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
			//D3DXMatrixInverse(&matProj, nullptr, &matProj);

			//D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj); // 뷰스페이스 상 마우스 위치

			//_matrix	matView;
			//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
			//D3DXMatrixInverse(&matView, nullptr, &matView);
			//D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matView); // 월드스페이스 상 마우스 위치

			//_matrix	matWorld = pChar->Get_Transform()->Get_WorldMatrix();	// 캐릭터 충돌 박스 월드행렬
			//D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
			//D3DXVec3TransformCoord(&vPivot, &vPivot, &matWorld);
			//D3DXVec3TransformNormal(&vMousePos, &vMousePos, &matWorld);

			_vec3 vRayDir = DestPos - muzzleWorldPos;
			//_vec3 muzzleLocalPos = muzzleWorldPos;
			//_vec3 muzzleLocalPos;

			//_matrix	matWorld = pChar->Get_Transform()->Get_WorldMatrix();	// 피격될 캐릭터 충돌 박스 월드행렬
			//D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
			//D3DXVec3TransformCoord(&muzzleLocalPos, &muzzleWorldPos, &matWorld);
			//D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

			CAABB_Buffer* pTargetBuffer = dynamic_cast<CAABB_Buffer*>(pChar->Get_ColliderBox_AABB());
			const _vec3* pVertexPos = pTargetBuffer->Get_ColliderBoxVertex();

			_float		fU = 0.f, fV = 0.f, fDist = 0.f;
			INDEX16* pIndexBuffet = pTargetBuffer->Get_IndexBuffer();

			for (_uint i = 0; i < pTargetBuffer->Get_IndexSize(); ++i)
			{

				//RT =_1 /LT = _0 / RB = _2
				// right top
				if (TRUE == D3DXIntersectTri(&pVertexPos[pIndexBuffet[i]._1], &pVertexPos[pIndexBuffet[i]._0], &pVertexPos[pIndexBuffet[i]._2], &muzzleWorldPos, &vRayDir
					, &fU, &fV, &fDist))
				{
					//return &(vPivot + (*D3DXVec3Normalize(&vRay,&vRay) * fDist));
					//return &(vPivot + vRay * fDist);
					//*pOut = (pVertexPos[iRT] + (pVertexPos[iLT] - pVertexPos[iRT]) * fU + (pVertexPos[iRB] - pVertexPos[iRT]) * fV);
					//return pChar;
					if (fDist < fMinDist)
					{
						fMinDist = fDist;
						pReulstChar = pChar;
						vReulstPos = (pVertexPos[pIndexBuffet[i]._1] + (pVertexPos[pIndexBuffet[i]._0] - pVertexPos[pIndexBuffet[i]._1]) * fU + (pVertexPos[pIndexBuffet[i]._2] - pVertexPos[pIndexBuffet[i]._1]) * fV);
						//D3DXVec3TransformCoord(&vReulstPos, &vReulstPos, &pChar->Get_Transform()->Get_WorldMatrix());
					}
				}

	
				
				// LB=_2 / RB=_1 / LT =_0
				// left bottomK
				if (TRUE == D3DXIntersectTri(&pVertexPos[pIndexBuffet[i]._2], &pVertexPos[pIndexBuffet[i]._1], &pVertexPos[pIndexBuffet[i]._0], &muzzleWorldPos, &vRayDir
					, &fU, &fV, &fDist))
				{
					if (fDist < fMinDist)
					{
						fMinDist = fDist;
						pReulstChar = pChar;
						vReulstPos = (pVertexPos[pIndexBuffet[i]._2] + (pVertexPos[pIndexBuffet[i]._1] - pVertexPos[pIndexBuffet[i]._2]) * fU + (pVertexPos[pIndexBuffet[i]._0] - pVertexPos[pIndexBuffet[i]._2]) * fV);
						//D3DXVec3TransformCoord(&vReulstPos, &vReulstPos, &pChar->Get_Transform()->Get_WorldMatrix());
						//vReulstPos = (pVertexPos[iRT] + (pVertexPos[iLT] - pVertexPos[iRT]) * fU + (pVertexPos[iRB] - pVertexPos[iRT]) * fV);
					}
				}
			}
			//_uint iVertexX = pTargetBuffer->Get_VertexCountX();
			//_uint iVertexZ = pTargetBuffer->Get_VertexCountZ();
			//_float fInterval = pTargetBuffer->Get_VertexItv();

			//_float		fU = 0.f, fV = 0.f, fDist = 0.f;

			//for (size_t i = 0; i <iVertexZ - 1; i++)
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
			//		if (TRUE == D3DXIntersectTri(&pVertexPos[iRT], &pVertexPos[iLT], &pVertexPos[iRB], &muzzleLocalPos, &vRayDir
			//			, &fU, &fV, &fDist))
			//		{
			//			//return &(vPivot + (*D3DXVec3Normalize(&vRay,&vRay) * fDist));
			//			//return &(vPivot + vRay * fDist);
			//			//*pOut = (pVertexPos[iRT] + (pVertexPos[iLT] - pVertexPos[iRT]) * fU + (pVertexPos[iRB] - pVertexPos[iRT]) * fV);
			//			//return pChar;
			//			if (fDist < fMinDist)
			//			{
			//				fMinDist = fDist;
			//				pReulstChar = pChar;
			//				vReulstPos = (pVertexPos[iRT] + (pVertexPos[iLT] - pVertexPos[iRT]) * fU + (pVertexPos[iRB] - pVertexPos[iRT]) * fV);
			//			}

			//		}

			//		// left bottomK
			//		if (TRUE == D3DXIntersectTri(&pVertexPos[iLB], &pVertexPos[iRB], &pVertexPos[iLT], &muzzleLocalPos, &vRayDir
			//			, &fU, &fV, &fDist))
			//		{
			//			if (fDist < fMinDist)
			//			{
			//				fMinDist = fDist;
			//				pReulstChar = pChar;
			//				vReulstPos = (pVertexPos[iRT] + (pVertexPos[iLT] - pVertexPos[iRT]) * fU + (pVertexPos[iRB] - pVertexPos[iRT]) * fV);
			//			}
			//		}
			//	}
			//}
		}

		if (!m_TargetBufferList.empty())
		{
			for (auto pTarget : m_TargetBufferList)
			{
				_vec3 vRayDir = DestPos - muzzleWorldPos;

				CAABB_Buffer* pTargetBuffer = pTarget;
				const _vec3* pVertexPos = pTargetBuffer->Get_ColliderBoxVertex();

				_float		fU = 0.f, fV = 0.f, fDist = 0.f;
				INDEX16* pIndexBuffet = pTargetBuffer->Get_IndexBuffer();

				for (_uint i = 0; i < pTargetBuffer->Get_IndexSize(); ++i)
				{

					//RT =_1 /LT = _0 / RB = _2
					// right top
					if (TRUE == D3DXIntersectTri(&pVertexPos[pIndexBuffet[i]._1], &pVertexPos[pIndexBuffet[i]._0], &pVertexPos[pIndexBuffet[i]._2], &muzzleWorldPos, &vRayDir
						, &fU, &fV, &fDist))
					{
	
						if (fDist < fMinDist)
						{
							fMinDist = fDist;
							pReulstChar = nullptr;
							pReulstTarget = pTargetBuffer;
							vReulstPos = (pVertexPos[pIndexBuffet[i]._1] + (pVertexPos[pIndexBuffet[i]._0] - pVertexPos[pIndexBuffet[i]._1]) * fU + (pVertexPos[pIndexBuffet[i]._2] - pVertexPos[pIndexBuffet[i]._1]) * fV);
						}
					}



					// LB=_2 / RB=_1 / LT =_0
					// left bottomK
					if (TRUE == D3DXIntersectTri(&pVertexPos[pIndexBuffet[i]._2], &pVertexPos[pIndexBuffet[i]._1], &pVertexPos[pIndexBuffet[i]._0], &muzzleWorldPos, &vRayDir
						, &fU, &fV, &fDist))
					{
						if (fDist < fMinDist)
						{
							fMinDist = fDist;
							pReulstChar = nullptr;
							pReulstTarget = pTargetBuffer;
							vReulstPos = (pVertexPos[pIndexBuffet[i]._2] + (pVertexPos[pIndexBuffet[i]._1] - pVertexPos[pIndexBuffet[i]._2]) * fU + (pVertexPos[pIndexBuffet[i]._0] - pVertexPos[pIndexBuffet[i]._2]) * fV);
						}
					}
				}
			}
		}
		// 반복문 끝
		// 충돌 했다면
		if (pReulstChar != nullptr)
		{
			//*pOut = vReulstPos;
			*pOut = vReulstPos;
			return pReulstChar;
		}
		if (pReulstTarget != nullptr)
		{
			*pOut = vReulstPos;
			return nullptr;
		}
	}
	return nullptr;
}

void CCollider_Manager::Clear_CharacterList()
{
	for (int i = 0; i < CHAR_TYPE::CHAR_TYPE_END; ++i)
	{
		m_CharacterList[i].clear();
	}
}

void CCollider_Manager::Clear_TargetList()
{
	m_TargetBufferList.clear();
}

void CCollider_Manager::Clear_BackBufferList()
{
	m_BackBufferList.clear();
}

void CCollider_Manager::Clear_SkillList()
{
	m_SkillList.clear();
}

_int CCollider_Manager::Update_Manager(const _float & fTimeDelta)
{
	//Process_AABB();
	
	
	Process_CharaterColl();
	Process_Skill();
	Remove_DeadSkill();
	//Process_AABB();
	//cout << "ColliderBox:" <<
	//	m_CharacterList[CHAR_TYPE::CHAR_TYPE_MONSTER].size()
	//	+ m_CharacterList[CHAR_TYPE::CHAR_TYPE_PLAYER].size()
	//	+ m_BackBufferList.size()
	//	<< endl;

	Clear_CharacterList();
	Clear_BackBufferList();
	Clear_TargetList();

	return 0;
}


bool CCollider_Manager::CheckCollAABB(CVIBuffer* pSour, CVIBuffer* pDest)
{
	const _vec3* pLhBuffer = pSour->Get_ColliderBoxVertex();
	const _vec3* pRhBuffer = pDest->Get_ColliderBoxVertex();

	if (pLhBuffer[MAXVEC].x < pRhBuffer[MINVEC].x || pLhBuffer[MINVEC].x > pRhBuffer[MAXVEC].x) return 0;
	if (pLhBuffer[MAXVEC].y < pRhBuffer[MINVEC].y || pLhBuffer[MINVEC].y > pRhBuffer[MAXVEC].y) return 0;
	if (pLhBuffer[MAXVEC].z < pRhBuffer[MINVEC].z || pLhBuffer[MINVEC].z > pRhBuffer[MAXVEC].z) return 0;
	return 1;
}

bool CCollider_Manager::CheckCollAABB_Push(_vec3 * pDir, CVIBuffer * pSour, CVIBuffer * pDest)
{
	const _vec3* pLhBuffer = pSour->Get_ColliderBoxVertex();
	const _vec3* pRhBuffer = pDest->Get_ColliderBoxVertex();

	//if (pLhBuffer[MINVEC].x <= pRhBuffer[MAXVEC].x && pLhBuffer[MAXVEC].x >= pRhBuffer[MINVEC].x &&
	//	pLhBuffer[MINVEC].y <= pRhBuffer[MAXVEC].y && pLhBuffer[MAXVEC].y >= pRhBuffer[MINVEC].y &&
	//	pLhBuffer[MINVEC].z <= pRhBuffer[MAXVEC].z && pLhBuffer[MAXVEC].z >= pRhBuffer[MINVEC].z)
	//{
	//	*pDir = pLhBuffer[MINVEC] - pRhBuffer[MAXVEC];
	//	return true;
	//}	
	//return false;

	if (pLhBuffer[MAXVEC].x < pRhBuffer[MINVEC].x || pLhBuffer[MINVEC].x > pRhBuffer[MAXVEC].x) return 0;
	if (pLhBuffer[MAXVEC].y < pRhBuffer[MINVEC].y || pLhBuffer[MINVEC].y > pRhBuffer[MAXVEC].y) return 0;
	if (pLhBuffer[MAXVEC].z < pRhBuffer[MINVEC].z || pLhBuffer[MINVEC].z > pRhBuffer[MAXVEC].z) return 0;

	//if(pLhBuffer[MINVEC].x >)
	//*pDir = pLhBuffer[MINVEC] - pRhBuffer[MAXVEC];
	//pDir->y = 0.f;
	//return 1;

	//_vec3 SourWidth = pLhBuffer[2] - pLhBuffer[3];
	//SourWidth.y = 0.f;
	//_vec3 SourHeight = pLhBuffer[6] - pLhBuffer[2];
	//SourHeight.y = 0.f;

	//_vec3 DestWidth = pRhBuffer[2] - pRhBuffer[3];
	//DestWidth.y = 0.f;
	//_vec3 DestHeight = pRhBuffer[6] - pRhBuffer[2];
	//DestHeight.y = 0.f;

	////// -------------------------------------------------
	//float SourWidth = pLhBuffer[2].x - pLhBuffer[3].x;
	//float SourHeight = pLhBuffer[6].z - pLhBuffer[2].z;
	//_vec3 SourPos(pLhBuffer[3].x + SourWidth / 2.f, 0.f, pLhBuffer[2].z + SourHeight / 2.f);

	//float DestWidth = pRhBuffer[2].x - pRhBuffer[3].x;
	//float DestHeight = pRhBuffer[6].z - pRhBuffer[2].z;
	//_vec3 DestPos(pRhBuffer[3].x + DestWidth / 2.f, 0.f, pRhBuffer[2].z + DestHeight / 2.f);

	//_vec3 vDist = DestPos - SourPos;

	//pDir->x = ((SourWidth + DestWidth) / 2.f - vDist.x);
	//pDir->y = 0.f;
	//pDir->z = ((SourHeight + DestHeight) / 2.f - vDist.z);
	// ////-------------------------------------------------
	// -------- 어느 방향 충돌 인지 체크---------------------

	float SourWidth = pLhBuffer[2].x - pLhBuffer[3].x;
	float SourHeight = pLhBuffer[6].z - pLhBuffer[2].z;
	_vec3 SourPos(pLhBuffer[3].x + SourWidth / 2.f, 0.f, pLhBuffer[2].z + SourHeight / 2.f);

	float DestWidth = pRhBuffer[2].x - pRhBuffer[3].x;
	float DestHeight = pRhBuffer[6].z - pRhBuffer[2].z;
	_vec3 DestPos(pRhBuffer[3].x + DestWidth / 2.f, 0.f, pRhBuffer[2].z + DestHeight / 2.f);

	_vec3 vDist = DestPos - SourPos;

	_vec3 vCollDir;

	vCollDir.x = (SourWidth + DestWidth)*0.5f - abs(vDist.x);
	vCollDir.y = 0.f;
	vCollDir.z = (SourHeight + DestHeight)*0.5f - abs(vDist.z);

	if (vCollDir.x > vCollDir.z)
	{
		vCollDir.z = 0.f;
	}
	else
	{
		vCollDir.x = 0.f;
	}

	*pDir = vCollDir;

	return true;
}

bool CCollider_Manager::CheckCollAABB_Slider(_vec3* pDir, CVIBuffer * pSour, CVIBuffer * pDest)
{
	const _vec3* pLhBuffer = pSour->Get_ColliderBoxVertex();
	const _vec3* pRhBuffer = pDest->Get_ColliderBoxVertex();

	//if (pLhBuffer[MINVEC].x <= pRhBuffer[MAXVEC].x && pLhBuffer[MAXVEC].x >= pRhBuffer[MINVEC].x &&
	//	pLhBuffer[MINVEC].y <= pRhBuffer[MAXVEC].y && pLhBuffer[MAXVEC].y >= pRhBuffer[MINVEC].y &&
	//	pLhBuffer[MINVEC].z <= pRhBuffer[MAXVEC].z && pLhBuffer[MAXVEC].z >= pRhBuffer[MINVEC].z)
	//{
	//	*pDir = pLhBuffer[MINVEC] - pRhBuffer[MAXVEC];
	//	return true;
	//}	
	//return false;

	if (pLhBuffer[MAXVEC].x < pRhBuffer[MINVEC].x || pLhBuffer[MINVEC].x > pRhBuffer[MAXVEC].x) return 0;
	if (pLhBuffer[MAXVEC].y < pRhBuffer[MINVEC].y || pLhBuffer[MINVEC].y > pRhBuffer[MAXVEC].y) return 0;
	if (pLhBuffer[MAXVEC].z < pRhBuffer[MINVEC].z || pLhBuffer[MINVEC].z > pRhBuffer[MAXVEC].z) return 0;
	
	//if(pLhBuffer[MINVEC].x >)
	//*pDir = pLhBuffer[MINVEC] - pRhBuffer[MAXVEC];
	//pDir->y = 0.f;
	//return 1;

	//_vec3 SourWidth = pLhBuffer[2] - pLhBuffer[3];
	//SourWidth.y = 0.f;
	//_vec3 SourHeight = pLhBuffer[6] - pLhBuffer[2];
	//SourHeight.y = 0.f;

	//_vec3 DestWidth = pRhBuffer[2] - pRhBuffer[3];
	//DestWidth.y = 0.f;
	//_vec3 DestHeight = pRhBuffer[6] - pRhBuffer[2];
	//DestHeight.y = 0.f;

	////// -------------------------------------------------
	//float SourWidth = pLhBuffer[2].x - pLhBuffer[3].x;
	//float SourHeight = pLhBuffer[6].z - pLhBuffer[2].z;
	//_vec3 SourPos(pLhBuffer[3].x + SourWidth / 2.f, 0.f, pLhBuffer[2].z + SourHeight / 2.f);

	//float DestWidth = pRhBuffer[2].x - pRhBuffer[3].x;
	//float DestHeight = pRhBuffer[6].z - pRhBuffer[2].z;
	//_vec3 DestPos(pRhBuffer[3].x + DestWidth / 2.f, 0.f, pRhBuffer[2].z + DestHeight / 2.f);

	//_vec3 vDist = DestPos - SourPos;

	//pDir->x = ((SourWidth + DestWidth) / 2.f - vDist.x);
	//pDir->y = 0.f;
	//pDir->z = ((SourHeight + DestHeight) / 2.f - vDist.z);
	// ////-------------------------------------------------
	// -------- 어느 방향 충돌 인지 체크---------------------

	float SourWidth = pLhBuffer[2].x - pLhBuffer[3].x;
	float SourHeight = pLhBuffer[6].z - pLhBuffer[2].z;
	_vec3 SourPos(pLhBuffer[3].x + SourWidth / 2.f, 0.f, pLhBuffer[2].z + SourHeight / 2.f);

	float DestWidth = pRhBuffer[2].x - pRhBuffer[3].x;
	float DestHeight = pRhBuffer[6].z - pRhBuffer[2].z;
	_vec3 DestPos(pRhBuffer[3].x + DestWidth / 2.f, 0.f, pRhBuffer[2].z + DestHeight / 2.f);

	_vec3 vDist = DestPos - SourPos;

	_vec3 vCollDir;

	vCollDir.x = (SourWidth + DestWidth)*0.5f - abs(vDist.x);
	vCollDir.y = 0.f;
	vCollDir.z = (SourHeight + DestHeight)*0.5f - abs(vDist.z);

	// ----- 슬라이딩 벡터 ----------------

	_vec3 vDir = *pDir;

	if (vCollDir.x > vCollDir.z)
	{
		if (vDir.x > 0)
			vDir.x = (pLhBuffer[2] - pLhBuffer[3]).x;
		else
			vDir.x = (pLhBuffer[3] - pLhBuffer[2]).x;
		vDir.y = 0.f;
		vDir.z = 0.f;
	}
	else
	{
		vDir.x = 0.f;
		vDir.y = 0.f;
		if(vDir.z > 0)
			vDir.z = (pLhBuffer[6] - pLhBuffer[2]).z;
		else
			vDir.z = (pLhBuffer[2] - pLhBuffer[6]).z;
	}
	

	D3DXVec3Normalize(pDir, &vDir);

	//cout << "vDir: "<< vDir.x << ' ' << vDir.y << ' ' << vDir.z << ' '  << endl;

	// ----- 슬라이딩 벡터 ----------------
	return true;
}

bool CCollider_Manager::Process_AABB()
{
	_vec3 vDir;
	for (int i = 0; i < CHAR_TYPE::CHAR_TYPE_END; ++i)
	{
		for (auto pChar : m_CharacterList[i])
		{
			for (auto pBackBuffer : m_BackBufferList)
			{
				
				if (CheckCollAABB_Push(&vDir, pBackBuffer, pChar->Get_ColliderBox_AABB()))
				{
					pChar->Get_Transform()->Set_Position(pChar->Get_Transform()->Get_Position() - vDir);
					//pChar->Get_Transform()->Move_ToLook(10.f,)
					//pChar->Get_Transform()->Move_ToDirection(-vDir, 1, 1.0f);
					//pChar->Get_Transform()->Fill_WorldMatrix();
					//dynamic_cast<CAABB_Buffer*>(pChar->Get_ColliderBox_AABB())->Set_Position(pChar->Get_Transform()->Get_Position());
					//pChar->Get_Transform()->Set_Position(pChar->Get_Transform()->Get)
					//cout << ":충돌!" << endl;
				}
			}
		}
	}
		return false;
}

bool CCollider_Manager::Process_AABB(_vec3 * pOutDir, CCharacter * pChar,_vec3 pDestPos)
{
	dynamic_cast<CAABB_Buffer*>(pChar->Get_ColliderBox_AABB())->Set_Position(pDestPos);
	dynamic_cast<CAABB_Buffer*>(pChar->Get_ColliderBox_AABB())->Update_ColliderBox_VertexBuffer();
	
	//cout << m_BackBufferList.size() << endl;

	for (auto pBackBuffer : m_BackBufferList)
	{

		if (CheckCollAABB_Slider(pOutDir, pBackBuffer, pChar->Get_ColliderBox_AABB()))
		{
			
			//pChar->Get_Transform()->Move_ToLook(10.f,)
			//pChar->Get_Transform()->Move_ToDirection(vDir, 1, 1.0f);
			//pChar->Get_Transform()->Fill_WorldMatrix();
			//dynamic_cast<CAABB_Buffer*>(pChar->Get_ColliderBox_AABB())->Set_Position(pChar->Get_Transform()->Get_WorldMatrix());
			////pChar->Get_Transform()->Set_Position(pChar->Get_Transform()->Get)
			//cout << ":충돌!" << endl;
			return true;
		}
	}

	return false;
}

bool CCollider_Manager::CheckCharaterColl(_vec3 * pOutDir, CCharacter * pPlayer, CCharacter * pMonster)
{
	_vec3 PlayerPos = pPlayer->Get_Transform()->Get_Position();
	_vec3 MonsterPos = pMonster->Get_Transform()->Get_Position();
	PlayerPos.y = 0.f;
	MonsterPos.y = 0.f;

	_vec3 vDir = PlayerPos - MonsterPos;
	_float fDist = D3DXVec3Length(&(vDir));

	_float PlayerLength = dynamic_cast<CAABB_Buffer*>(pPlayer->Get_ColliderBox_AABB())->Get_Scale().x;
	_float MonsterLength = dynamic_cast<CAABB_Buffer*>(pMonster->Get_ColliderBox_AABB())->Get_Scale().x;

	_float fSumLength = (MonsterLength + PlayerLength)*0.5f;

	if (fDist < fSumLength)
	{
		D3DXVec3Normalize(&vDir, &vDir);
		vDir *= (fSumLength - fDist);
		*pOutDir = vDir;
		return true;
	}
	
	return false;
}

bool CCollider_Manager::Process_CharaterColl()
{
	_vec3 vDir;

	for (auto pMonster1 : m_CharacterList[CHAR_TYPE::CHAR_TYPE_MONSTER])
	{
		for (auto pMonster2 : m_CharacterList[CHAR_TYPE::CHAR_TYPE_MONSTER])
		{
			if (pMonster1 != pMonster2)
			{
				if (CheckCharaterColl(&vDir, pMonster1, pMonster2))
				{
					pMonster1->Get_Transform()->Set_Position(pMonster1->Get_Transform()->Get_Position() + vDir);
					//pMonster2->Get_Transform()->Set_Position(pMonster2->Get_Transform()->Get_Position()  vDir*0.5f);
					//pMonster->Get_Transform()->Move_ToDirection(-vDir, 1, 1.0f);
					//pMonster->Get_Transform()->Fill_WorldMatrix();
					//cout << "충돌 " << endl;
					//dynamic_cast<CAABB_Buffer*>(pMonster->Get_ColliderBox_AABB())->Set_Position(pMonster->Get_Transform()->Get_WorldMatrix());
				}
			}
		}
	}

	for (auto pPlayer : m_CharacterList[CHAR_TYPE::CHAR_TYPE_PLAYER])
	{
		for (auto pMonster : m_CharacterList[CHAR_TYPE::CHAR_TYPE_MONSTER])
		{
			if (CheckCharaterColl(&vDir, pPlayer, pMonster))
			{
				pMonster->Get_Transform()->Set_Position(pMonster->Get_Transform()->Get_Position() - vDir);
				//pMonster->Get_Transform()->Move_ToDirection(-vDir, 1, 1.0f);
				//pMonster->Get_Transform()->Fill_WorldMatrix();
				//cout << "충돌 " << endl;
				//dynamic_cast<CAABB_Buffer*>(pMonster->Get_ColliderBox_AABB())->Set_Position(pMonster->Get_Transform()->Get_WorldMatrix());
			}
		}
	}

	
	return false;
}

bool CCollider_Manager::Process_Skill()
{
	CCharacter* pPlayer = nullptr;
	CColliderSkill* RunSKill = nullptr;

	for (auto pSKill : m_SkillList)
	{
		
		if (!pSKill->Get_IsDead())
		{
			for (auto pBackground : m_BackBufferList)
			{
				if (CheckCollAABB(pBackground, pSKill->Get_ColliderBox_AABB()))
				{
					pSKill->Run_SKill();
					RunSKill = pSKill;
					break;
				}
			}
			if (pSKill->isRun())
			{
				break;
			}
			for (auto pChar : m_CharacterList[CHAR_TYPE::CHAR_TYPE_PLAYER])
			{
				if (CheckCollAABB(pChar->Get_ColliderBox_AABB(), pSKill->Get_ColliderBox_AABB()))
				{
					pPlayer = pChar;
					pSKill->Run_SKill();
					RunSKill = pSKill;
					//cout << "Area:" << RunSKill->GetSkillArea() << endl;
					break;
				}
			}
			if (pSKill->isRun())
			{
				break;
			}
		}
		//죽었다면
		else
		{
			//m_SkillList.remove(pSKill);
		}
	}
	if (pPlayer != nullptr)
	{
		if (RunSKill != nullptr)
		{
			_float fDist = D3DXVec3Length(&(RunSKill->Get_Transform()->Get_Position() - pPlayer->Get_Transform()->Get_Position()));
			//cout << "Dist: "<<fDist << endl;
			if (RunSKill->GetSkillArea() > fDist)
			{
				pPlayer->Hit(RunSKill->GetDamage(), 0.01f);
				RunSKill->Set_IsDead();
				//cout << "파이어볼 피격 후 삭제" << endl;
				//Safe_Release(RunSKill);
			}
		}
	}
	return true;
	//for (auto pSKill : m_SkillList)
	//{
	//	if (!pSKill->Get_IsDead())
	//	{
	//		for (auto pChar : m_CharacterList[CHAR_TYPE::CHAR_TYPE_PLAYER])
	//		{
	//			if (pSKill->GetCurRange() > pSKill->GetMaxRange())
	//			{
	//				_float fDist = D3DXVec3Length(&(pSKill->Get_Transform()->Get_Position() - pChar->Get_Transform()->Get_Position()));
	//				if (pSKill->GetSkillArea() > fDist)
	//				{
	//					pChar->Hit(pSKill->GetDamage(), 0.01f);
	//					pSKill->Set_IsDead();
	//				}
	//			}
	//			else if (CheckCollAABB(pChar->Get_ColliderBox_AABB(), pSKill->Get_ColliderBox_AABB()))
	//			{
	//				_float fDist = D3DXVec3Length(&(pSKill->Get_Transform()->Get_Position() - pChar->Get_Transform()->Get_Position()));
	//				if (pSKill->GetSkillArea() > fDist)
	//				{
	//					pChar->Hit(pSKill->GetDamage(), 0.01f);
	//					pSKill->Set_IsDead();
	//				}
	//			}
	//			
	//			//if (pSKill->Get_RunType() == CColliderSkill::RUN_TYPE::AABB)
	//			//{
	//			//	if (CheckCollAABB(pChar->Get_ColliderBox_AABB(), pSKill->Get_ColliderBox_AABB()))
	//			//	{
	//			//		pChar->Hit(pSKill->GetDamage(), 0.01f);
	//			//		pSKill->Set_IsDead();
	//			//	}
	//			//}
	//			//else if(pSKill->Get_RunType() == CColliderSkill::RUN_TYPE::AREA)
	//			//{
	//			//	_float fDist = D3DXVec3Length(&(pSKill->Get_Transform()->Get_Position() - pChar->Get_Transform()->Get_Position()));
	//			//	if (pSKill->GetSkillArea() > fDist)
	//			//	{
	//			//		pChar->Hit(pSKill->GetDamage(), 0.01f);
	//			//		pSKill->Set_IsDead();
	//			//	}
	//			//}
	//		}
	//	}
	//	if (!pSKill->Get_IsDead())
	//	{
	//		for (auto pBackground : m_BackBufferList)
	//		{
	//			if (CheckCollAABB(pBackground, pSKill->Get_ColliderBox_AABB()))
	//			{
	//				_float fDist = D3DXVec3Length(&(pSKill->Get_Transform()->Get_Position() - pChar->Get_Transform()->Get_Position()));
	//				if (pSKill->GetSkillArea() > fDist)
	//				{
	//					pChar->Hit(pSKill->GetDamage(), 0.01f);
	//					pSKill->Set_IsDead();
	//				}
	//			}
	//		}
	//	}
	//}

	//return true;
}

bool CCollider_Manager::Remove_DeadSkill()
{

	SKILLLIST::iterator iter = m_SkillList.begin();
	SKILLLIST::iterator iter_end = m_SkillList.end();

	for (iter; iter != iter_end; )
	{
		if ((*iter)->Get_IsDead())
		{
			(*iter)->GetCurRange();
			Safe_Release(*iter);
			iter = m_SkillList.erase(iter);
			//cout << "스킬 삭제" << endl;
		}
		else
			++iter;
	}
	return true;
}

_float CCollider_Manager::Max(_float Lf, _float Rf)
{
	return (Lf < Rf) ? Rf : Lf;
}

inline _float CCollider_Manager::Min(_float Lf, _float Rf)
{
	return (Lf < Rf) ? Lf : Rf;
}

_ulong CCollider_Manager::Free()
{
	_ulong dwRefCount = 0;

	Clear_CharacterList();
	Clear_BackBufferList();
	Clear_TargetList();
	
	for (auto pSkill : m_SkillList)
	{
		dwRefCount += Safe_Release(pSkill);
	}

	Safe_Release(m_pGraphicDev);

	return dwRefCount;
}

