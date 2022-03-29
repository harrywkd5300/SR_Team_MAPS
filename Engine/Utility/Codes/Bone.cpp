#include "..\Headers\Bone.h"


CBone::CBone(LPDIRECT3DDEVICE9 pGrahpicDev)
	:CTransform(pGrahpicDev)
	,m_iParentIndex(-1)
	,m_dwFrame(0)
	,m_fFrameTimer(0.f)
{

}

CBone::CBone(const CBone & rhs)
	:CTransform(rhs)
{
}

void CBone::Add_State()
{
	vector<KEYFRAME*> v; 
	m_vvecKeyFrame.push_back(v);
}

void CBone::Del_State(const _uint& dwIndex)
{
	if (dwIndex>= m_vvecKeyFrame.size())
		return;


	for (size_t i = 0; i < m_vvecKeyFrame[dwIndex].size(); ++i)
	{
		Safe_Delete(m_vvecKeyFrame[dwIndex][i]);
		m_vvecKeyFrame[dwIndex].clear();
	}
	m_vvecKeyFrame.erase(m_vvecKeyFrame.begin() + dwIndex);

}

void CBone::Add_Frame(const _uint & dwStateIndex)
{
	if (dwStateIndex >= m_vvecKeyFrame.size())
		return;

	KEYFRAME*	p = new KEYFRAME;
	ZeroMemory(p, sizeof(KEYFRAME));

	m_vvecKeyFrame[dwStateIndex].push_back(p);

}

void CBone::Del_Frame(const _uint & dwStateIndex, const _uint & dwFrameIndex)
{
	if (dwStateIndex >= m_vvecKeyFrame.size())
		return;

	if (dwFrameIndex >= m_vvecKeyFrame[dwStateIndex].size())
		return;


	Safe_Delete(m_vvecKeyFrame[dwStateIndex][dwFrameIndex]);
	m_vvecKeyFrame[dwStateIndex].erase(m_vvecKeyFrame[dwStateIndex].begin() + dwFrameIndex);

}

void CBone::Set_FrameDegree(const _uint & dwStateIndex, const _uint & dwFrameIndex, const _vec3 & vStart, const _vec3 & vEnd)
{
	if (dwStateIndex >= m_vvecKeyFrame.size())
		return;

	if (dwFrameIndex >= m_vvecKeyFrame[dwStateIndex].size())
		return;

	m_vvecKeyFrame[dwStateIndex][dwFrameIndex]->vStartDegree = vStart;
	m_vvecKeyFrame[dwStateIndex][dwFrameIndex]->vEndDegree = vEnd;

}

void CBone::Set_StartDegree(const _uint & STATEINDEX, const _uint & FRAMEINDEX)
{
	/*init frame data*/
	m_dwFrame = 0;
	m_fFrameTimer = 0.f;
	
	/*Set StartDegree*/
	const _vec3& v = m_vvecKeyFrame[STATEINDEX][FRAMEINDEX]->vStartDegree;
	Set_Degree(v);
	Fill_WorldMatrix();
}

void CBone::Set_EndDegree(const _uint & STATEINDEX, const _uint & FRAMEINDEX)
{
	/*init frame data*/
	m_dwFrame = 0;
	m_fFrameTimer = 0.f;

	/*Set StartDegree*/
	const _vec3& v = m_vvecKeyFrame[STATEINDEX][FRAMEINDEX]->vEndDegree;
	Set_Degree(v);
	Fill_WorldMatrix();

}



_int CBone::Update_PreKeyFrame(const _uint & STATEINDEX, const _uint & dwFrameIndex,const _float& fTimer, const _float & fTimeDelta)
{
	if (m_bReceivedKeyFrame)
	{
		if (m_bStateChanged == true)
		{
			m_fFrameTimer = 0.f;
			m_vStartDegree = _vec3(m_fDegreeX, m_fDegreeY, m_fDegreeZ);
			m_bStateChanged = false;
		}

		m_fFrameTimer += fTimeDelta;
		const _vec3& vEnd = m_vvecKeyFrame[STATEINDEX][dwFrameIndex]->vStartDegree; //New Frame Start Degree
		const _vec3 vResult = m_vStartDegree + (((vEnd - m_vStartDegree) / fTimer) * m_fFrameTimer);

		Set_Degree(vResult);
		Fill_WorldMatrix();

		if (m_fFrameTimer > fTimer)
		{
			return 1;
		}
	}

	return 0;
}

_int CBone::Update_KeyFrame(const _uint & STATEINDEX, const _uint& dwFrameIndex, const _float& fTimer ,const _float & fTimeDelta)
{
	if (m_bReceivedKeyFrame)
	{
		/*Init*/
		if (m_bStateChanged == false)
		{
			m_bStateChanged = true;
			m_dwFrame = dwFrameIndex;
			m_fFrameTimer = 0.f;
		}

		/*changed frame*/
		if (m_dwFrame != dwFrameIndex)
		{
			m_dwFrame = dwFrameIndex;
			m_fFrameTimer = 0.f;
		}

		m_fFrameTimer += fTimeDelta;

		const _vec3& vStart = m_vvecKeyFrame[STATEINDEX][dwFrameIndex]->vStartDegree;
		const _vec3& vEnd = m_vvecKeyFrame[STATEINDEX][dwFrameIndex]->vEndDegree;

		const _vec3 vResult = vStart + (((vEnd - vStart) / fTimer) * m_fFrameTimer);


		/*수정필요*/
		Set_Degree(vResult);
		Fill_WorldMatrix();
	}


	return 0;
}

void CBone::Rotation_HeadY(const _float & fDegree)
{
	m_fDegreeY += fDegree;
	
	if (m_fDegreeY > 60)
		m_fDegreeY = 60.f;

	else if (m_fDegreeY < -60)
		m_fDegreeY = -60.f;

	D3DXMatrixRotationY(&m_matRotationY, D3DXToRadian(m_fDegreeY));
}

void CBone::Rotation_HeadX(const _float & fDegree)
{
	m_fDegreeX += fDegree;

	if (m_fDegreeX > 60)
		m_fDegreeX = 60.f;

	else if (m_fDegreeX < -60)
		m_fDegreeX = -60.f;

	D3DXMatrixRotationX(&m_matRotationX, D3DXToRadian(m_fDegreeX));
}


CBone * CBone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBone* pInstance = new CBone(pGraphicDev);
	if (FAILED(pInstance->Ready_Transform()))
	{
		MSG_BOX("Bone creation failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CBone::Clone()
{
	return new CBone(*this);
}

_ulong CBone::Free()
{
	for (size_t i = 0; i < m_vvecKeyFrame.size(); ++i)
	{
		for (size_t j = 0; j < m_vvecKeyFrame[i].size(); ++j)
			Safe_Delete(m_vvecKeyFrame[i][j]);
		m_vvecKeyFrame[i].clear();
	}
	m_vvecKeyFrame.clear();


	return CTransform::Free();
}
