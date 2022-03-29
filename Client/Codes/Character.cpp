#include "stdafx.h"
#include "..\Headers\Character.h"
#include "Collider_Manager.h"
#include "Particle_Renderer.h"

CCharacter::CCharacter(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pColliderBox_AABB(nullptr)
	, m_pColliderBox_Billboard(nullptr)
	, m_pColliderMgr(CCollider_Manager::GetInstance())
	, m_pParticleRenderer(CParticle_Renderer::GetInstance())
	/*--sy--*/
	, m_pBoneCubeBuffer(nullptr)
	, m_pBodyCubeBuffer(nullptr)
	,m_fFrameTimer(0.f)
	,m_dwFrame(0)
	,m_iPreStateIndex(0)
	,m_iCurStateIndex(0)
	,m_bSpecialState(false)
{
	m_pColliderMgr->AddRef();
	m_pParticleRenderer->AddRef();
}

_int CCharacter::LastUpdate_GameObject(const _float & fTimeDelta)
{
	//_matrix matWorld, matScale;
	//D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixScaling(&matScale, 0.3f, 0.3f, 0.3f);
	//D3DXMatrixTranslation(&matWorld, m_pTransformCom->Get_PositionX(), m_pTransformCom->Get_PositionY(), m_pTransformCom->Get_PositionZ());
	//matWorld = matScale * matWorld;
	//m_pColliderMgr->Add_Character(this);
	//m_pColliderBox_AABB->Set_Position(matWorld);
	//m_pColliderBox_Billboard->Set_Position(matWorld);

	//m_pColliderMgr->Add_Character(this);
	//m_pColliderBox_AABB->Set_Position(m_pTransformCom->Get_WorldMatrix());
	//m_pColliderBox_Billboard->Set_Position(m_pTransformCom->Get_WorldMatrix());

	m_pColliderBox_AABB->Set_Position(m_pTransformCom->Get_Position());
	m_pColliderBox_AABB->Update_ColliderBox_VertexBuffer();
	m_pColliderMgr->Add_Character(this);

	return CGameObject::LastUpdate_GameObject(fTimeDelta);
}

bool CCharacter::Hit(_uint iDamage, const _float& fLifeTime)
{
	if (iDamage >= m_iHp)
	{
		m_iHp = 0;
		m_bIsDead = false;
		return true;
	}
	m_iHp -= iDamage;
	//cout << "피격 체력" << m_iHp << endl;
	return false;

}

CVIBuffer * CCharacter::Get_ColliderBox_AABB()
{
	// -------- Test ---------------

	return m_pColliderBox_AABB;
}

HRESULT CCharacter::Ready_Character_Component()
{
	CComponent* pComponent = nullptr;

	/*Add ColliderBox_AABB*/
	pComponent = m_pColliderBox_AABB = dynamic_cast<CAABB_Buffer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_ColliderBox_AABB"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"ColliderBox_AABB", pComponent));
	m_pColliderBox_AABB->AddRef();

	/*Add ColliderBox_Billboard*/
	pComponent = m_pColliderBox_Billboard = dynamic_cast<CBillboard_Buffer*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_ColliderBox_Billboard"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"ColliderBox_Billboard", pComponent));
	m_pColliderBox_Billboard->AddRef();

	/*Add Particle_Effect*/
	pComponent = m_pEffectParticle = dynamic_cast<CParticle_Effect*>(CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Particle_Effect"));
	if (nullptr == pComponent)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Particle_Effect", pComponent));
	m_pEffectParticle->AddRef();

	return NOERROR;

exception:
	MSG_BOX("Player Component Ready Failed err: 13115");
	return E_FAIL;
}

HRESULT CCharacter::Load_Character_Model(const _tchar * pFilePath)
{
	if (nullptr == pFilePath)
		return E_FAIL;

	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Load failed");
		return E_FAIL;
	}

	_ulong dwByte = 0;

	size_t iBoneSize = 0;
	ReadFile(hFile, &iBoneSize, sizeof(size_t), &dwByte, NULL);

	for (size_t i = 0; i < iBoneSize; ++i)
	{
		_vec3 vPos;
		_int iParentIndex = 0;

		ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, NULL);
		ReadFile(hFile, &iParentIndex, sizeof(_int), &dwByte, NULL);

		CBone* pBone = CBone::Create(m_pGraphicDev);
		pBone->Set_Position(vPos);
		pBone->Set_ParentIndex(iParentIndex);

		m_vecBone.push_back(pBone);
	}



	size_t iBodySize = 0;
	ReadFile(hFile, &iBodySize, sizeof(size_t), &dwByte, NULL);

	for (size_t i = 0; i < iBodySize; ++i)
	{
		_vec3 vScale;
		_vec3 vPos;
		_int iParentIndex = 0;
		_int iTextureNum = 0;

		ReadFile(hFile, &vScale, sizeof(_vec3), &dwByte, NULL);
		ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, NULL);
		ReadFile(hFile, &iParentIndex, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, &iTextureNum, sizeof(_int), &dwByte, NULL);

		BODYINFO* pBodyInfo = new BODYINFO;

		pBodyInfo->vPosition = vPos;
		pBodyInfo->vScale = vScale;
		pBodyInfo->iParentIndex = iParentIndex;
		pBodyInfo->iTextureNum = iTextureNum;

		m_vecBodyInfo.push_back(pBodyInfo);
	}
	CloseHandle(hFile);

	if (!m_vecBone.empty())
		m_pTransformCom = m_vecBone[0];

	return NOERROR;
}

HRESULT CCharacter::Load_Character_Animate(const _tchar * pFilePath)
{
	if (nullptr == pFilePath)
		return E_FAIL;

	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Load Failed.");
		return E_FAIL;
	}

	_ulong dwByte = 0;


	size_t	iStateIndex = 0;
	size_t	iFrameIndex = 0;
	ReadFile(hFile, &iStateIndex, sizeof(size_t), &dwByte, NULL);

	for (size_t i = 0; i < iStateIndex; ++i)
	{
		/* Add State*/

		for (size_t p = 0; p < m_vecBone.size(); ++p)
			m_vecBone[p]->Add_State();

		vector<_float*> v;
		m_vvKeyFrameTime.push_back(v);


		ReadFile(hFile, &iFrameIndex, sizeof(size_t), &dwByte, NULL);

		for (size_t j = 0; j < iFrameIndex; ++j)
		{
			for (size_t p = 0; p < m_vecBone.size(); ++p)
				m_vecBone[p]->Add_Frame(i);

			_float* f = new _float;
			*f = 0.f;
			m_vvKeyFrameTime[i].push_back(f);

			_float fTime = 0.f;
			ReadFile(hFile, &fTime, sizeof(_float), &dwByte, NULL);

			*(m_vvKeyFrameTime[i][j]) = fTime;
		}
	}

	size_t iBoneIndex = 0;
	ReadFile(hFile, &iBoneIndex, sizeof(size_t), &dwByte, NULL);

	for (size_t i = 0; i < iStateIndex; ++i)
	{
		ReadFile(hFile, &iFrameIndex, sizeof(size_t), &dwByte, NULL);

		for (size_t j = 0; j < iFrameIndex; ++j)
		{
			for (size_t k = 0; k < iBoneIndex; ++k)
			{
				_vec3 vStart;
				_vec3 vEnd;

				ReadFile(hFile, &vStart, sizeof(_vec3), &dwByte, NULL);
				ReadFile(hFile, &vEnd, sizeof(_vec3), &dwByte, NULL);

				vector<vector<KEYFRAME*>>*	vk = m_vecBone[k]->Get_VvecKeyFrame();
				(*vk)[i][j]->vStartDegree = vStart;
				(*vk)[i][j]->vEndDegree = vEnd;

			}
		}
	}

	CloseHandle(hFile);
	return NOERROR;
}

void CCharacter::Update_Frame(const _float & fTimeDelta)
{
	if (m_iCurStateIndex >= m_vvKeyFrameTime.size())
		return;

	if (m_dwFrame >= m_vvKeyFrameTime[m_iCurStateIndex].size())
		return;

	_float fTimer = *(m_vvKeyFrameTime[m_iCurStateIndex][m_dwFrame]);

	if (fTimer <= 0)
		return;

	m_fFrameTimer += fTimeDelta;

	if (m_fFrameTimer > fTimer)
	{
		m_fFrameTimer = 0;
		++m_dwFrame;
	}
	/*Frame over KeyFrameCount*/
	if (m_dwFrame >= m_vvKeyFrameTime[m_iCurStateIndex].size())
	{
		m_bSpecialState = false;
		m_fFrameTimer = 0;
		m_dwFrame = 0;
	}
}
void CCharacter::Update_Animate(const _float & fTimeDelta)
{

	if (m_iCurStateIndex >= m_vvKeyFrameTime.size())
		return;
	
	if (m_iCurStateIndex != m_iPreStateIndex && m_iPreStateIndex == 0) /*if State == Idle*/
	{

		if (m_bDirectly_ChangeState)
		{
			m_bDirectly_ChangeState = false;
			m_iPreStateIndex = m_iCurStateIndex;
			m_dwFrame = 0;
			m_fFrameTimer = 0;

			for (size_t i = 0; i < m_vecBone.size(); ++i)
				m_vecBone[i]->Set_StateChange(false);
		}
		else if(Compare_AnimateType())
		{
			m_iPreStateIndex = m_iCurStateIndex;
			m_dwFrame = 0;
			m_fFrameTimer = 0;

			for (size_t i = 0; i < m_vecBone.size(); ++i)
				m_vecBone[i]->Set_StateChange(false);
		}

		else
		{
			_int iExitCode = 0;

			for (size_t i = 0; i < m_vecBone.size(); ++i)
			{
				iExitCode += m_vecBone[i]->Update_PreKeyFrame(m_iCurStateIndex, 0, 0.2f, fTimeDelta);
				m_fFrameTimer = 0;
			}

			if (iExitCode)
			{
				m_iPreStateIndex = m_iCurStateIndex;
				m_dwFrame = 0;
			}

			return;
		}

	}
	else if (m_iCurStateIndex != m_iPreStateIndex && m_iPreStateIndex != 0) /*if State != Idle*/
	{
		m_dwFrame = 0;

		if (m_dwFrame >= m_vvKeyFrameTime[m_iCurStateIndex].size())
			return;

		_int iExitCode = 0;
		

		if (m_bDirectly_ChangeState)
		{
			m_bDirectly_ChangeState = false;
			m_iPreStateIndex = m_iCurStateIndex;
			m_dwFrame = 0;
			m_fFrameTimer = 0;

			for (size_t i = 0; i < m_vecBone.size(); ++i)
				m_vecBone[i]->Set_StateChange(false);
		}
		else if(Compare_AnimateType())
		{
			for (size_t i = 0; i < m_vecBone.size(); ++i)
			{
				iExitCode += m_vecBone[i]->Update_PreKeyFrame(m_iCurStateIndex, m_dwFrame, 0.5f, fTimeDelta);
				m_fFrameTimer = 0;
			}

			if (iExitCode)
			{
				m_iPreStateIndex = m_iCurStateIndex;
				m_dwFrame = 0;
			}
			return;
		}

		else
		{
			_int iExitCode = 0;

			for (size_t i = 0; i < m_vecBone.size(); ++i)
			{
				iExitCode += m_vecBone[i]->Update_PreKeyFrame(m_iCurStateIndex, 0, 0.2f, fTimeDelta);
				m_fFrameTimer = 0;
			}

			if (iExitCode)
			{
				m_iPreStateIndex = m_iCurStateIndex;
				m_dwFrame = 0;
			}

			return;
		}


	}

	if (m_dwFrame >= m_vvKeyFrameTime[m_iCurStateIndex].size())
		return;

	_float fTimer = *(m_vvKeyFrameTime[m_iCurStateIndex][m_dwFrame]);

	if (fTimer <= 0)
		return;

	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		m_vecBone[i]->Update_KeyFrame(m_iCurStateIndex, m_dwFrame, fTimer, fTimeDelta);
	}
}

_ulong CCharacter::Free()
{
	Safe_Release(m_pColliderBox_AABB);
	Safe_Release(m_pColliderBox_Billboard);
	Safe_Release(m_pColliderMgr);
	Safe_Release(m_pParticleRenderer);
	Safe_Release(m_pEffectParticle);

	return  Engine::CGameObject::Free();
}
