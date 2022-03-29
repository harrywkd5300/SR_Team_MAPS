#include "stdafx.h"
#include "..\Headers\Creature.h"
#include "Graphic_Device.h"

CCreature::CCreature(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pFont(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRenderer(nullptr)
{
	m_pComponent_Manager->AddRef();
}

void CCreature::Add_Bone()
{
	CBone* pBone = dynamic_cast<CBone*>(m_pComponent_Manager->Clone_Component(SCENE_TOOL, L"Component_Bone"));
	if (nullptr == pBone)
	{
		MSG_BOX("Add Bone Failed");
		return;
	}

	m_vecBone.push_back(pBone);
}

void CCreature::Add_Bone(const _vec3 & vPos, const _int iParentIndex)
{
	CBone* pBone = dynamic_cast<CBone*>(m_pComponent_Manager->Clone_Component(SCENE_TOOL, L"Component_Bone"));
	if (nullptr == pBone)
	{
		MSG_BOX("Add Bone Failed");
		return;
	}

	pBone->Set_PositionX(vPos.x);
	pBone->Set_PositionY(vPos.y);
	pBone->Set_PositionZ(vPos.z);
	pBone->Set_ParentIndex(iParentIndex);

	m_vecBone.push_back(pBone);
}

void CCreature::Del_Bone(_int iIndex)
{
	if ((size_t)iIndex > m_vecBone.size())
	{
		MSG_BOX("Del Bone Failed");
		return;
	}
	Safe_Release(m_vecBone[iIndex]);
	m_vecBone.erase(m_vecBone.begin() + iIndex);
	
}

void CCreature::Add_Cube()
{

	TOOL_BODY*	pToolBody = new TOOL_BODY;
	ZeroMemory(pToolBody, sizeof(TOOL_BODY));


	pToolBody->vScale = _vec3(1.f,1.f,1.f);
	pToolBody->vPosition = _vec3(0, 0, 0);
	pToolBody->iParentIndex = -1;
	pToolBody->iTextureNum = 0;

	m_vecBody.push_back(pToolBody);
}

void CCreature::Add_Cube(TOOL_BODY * pBody)
{
	m_vecBody.push_back(pBody);
}

void CCreature::Del_Cube(_int iIndex)
{
	if ((size_t)iIndex > m_vecBody.size())
	{
		MSG_BOX("Del Bone Failed");
		return;
	}
	Safe_Delete(m_vecBody[iIndex]);
	m_vecBody.erase(m_vecBody.begin() + iIndex);
}

void CCreature::Add_State()
{
	//vector<vector<_float>>	v;

	for (size_t i = 0; i < m_vecBone.size(); ++i)
		m_vecBone[i]->Add_State();

	vector<_float*> v;
	m_vvKeyFrameTime.push_back(v);

}

void CCreature::Del_State(const _uint& dwIndex)
{
	for (size_t i = 0; i < m_vecBone.size(); ++i)
		m_vecBone[i]->Del_State(dwIndex);

	for (size_t i = 0; i < m_vvKeyFrameTime[dwIndex].size(); ++i)
		Safe_Delete(m_vvKeyFrameTime[dwIndex][i]);
	
	m_vvKeyFrameTime.erase(m_vvKeyFrameTime.begin() + dwIndex);

}

void CCreature::Add_Frame(const _uint & dwStateIndex)
{
	for (size_t i = 0; i < m_vecBone.size(); ++i)
		m_vecBone[i]->Add_Frame(dwStateIndex);

	_float*	f = new _float;
	ZeroMemory(f, sizeof(_float));

	m_vvKeyFrameTime[dwStateIndex].push_back(f);
}

void CCreature::Del_Frame(const _uint & dwStateIndex, const _uint & dwFrameIndex)
{
	for (size_t i = 0; i < m_vecBone.size(); ++i)
		m_vecBone[i]->Del_Frame(dwStateIndex, dwFrameIndex);

	m_vvKeyFrameTime[dwStateIndex].erase(m_vvKeyFrameTime[dwStateIndex].begin() + dwFrameIndex);
}

void CCreature::Set_FrameInfo(const _uint & dwStateIndex, const _uint & dwFrameIndex,const _uint& dwBoneIndex, const _vec3 & vStart, const _vec3 & vEnd, const _float& fTime)
{
	if (dwStateIndex >= m_vvKeyFrameTime.size())
		return;
	
	if (dwFrameIndex >= m_vvKeyFrameTime[dwStateIndex].size())
		return;


	if (dwBoneIndex >= m_vecBone.size())
		return;

	*(m_vvKeyFrameTime[dwStateIndex][dwFrameIndex]) = fTime;

	m_vecBone[dwBoneIndex]->Set_FrameDegree(dwStateIndex, dwFrameIndex, vStart, vEnd);
}

vector<CBone*>* CCreature::Get_BoneVector()
{
	return &m_vecBone;
}

vector<TOOL_BODY*>* CCreature::Get_BodyVector()
{
	return &m_vecBody;
}

void CCreature::Set_BonePosition(const _uint&	iIndex, const _float& fX, const _float& fY, const _float& fZ)
{
	m_vecBone[iIndex]->Translation(fX, fY, fZ);
}

void CCreature::Set_BoneParent(const _uint&	iSrcIndex, const _uint&	iDstIndex)
{
	m_vecBone[iSrcIndex]->Parent(m_vecBone[iDstIndex]->Get_WorldMatrix());
}


HRESULT CCreature::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pFont = CGraphic_Device::GetInstance()->Get_Font();

	return NOERROR;
}

_int CCreature::Update_GameObject(const _float & fTimeDelta)
{

	Update_BoneCube(fTimeDelta);
	m_pRenderer->Add_RenderList(CRenderer::RENDER_NONEALPHA, this);
	return 0;
}

void CCreature::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CCreature::Update_BoneCube(const _float & fTimeDelta)
{
	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		_uint n = m_vecBone[i]->Get_ParentIndex();

		if(n != -1)
			m_vecBone[i]->Parent(m_vecBone[n]->Get_WorldMatrix());
		
		
		m_vecBone[i]->Fill_WorldMatrix();
	}

}

void CCreature::Render_BoneCube()
{
	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		m_vecBone[i]->SetUp_MatrixToGraphicDev();
		m_pBoneCubeCom->Render_Buffer();
	}
}

void CCreature::Render_BoneFont()
{

	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		//m_vecBone[i]->SetUp_MatrixToGraphicDev();

		/*text*/
		_matrix matScale, matParent, matWorld, matTrans;

		_matrix			matView;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, nullptr, &matView);


		_matrix mat;
		D3DXMatrixIdentity(&mat);
		(_vec3&)mat.m[0] = (_vec3&)matView.m[0];
		(_vec3&)mat.m[2] = (_vec3&)matView.m[2];

		_matrix mat2;
		mat2 = m_vecBone[i]->Get_WorldMatrix();

		(_vec3&)mat.m[3] = (_vec3&)mat2.m[3];

		matParent = mat;

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 0.05f, 0.05f, 0.05f);
		D3DXMatrixTranslation(&matTrans, 0.1f, 0.1f, 0.f);
		//matParent = m_vecBone[i]->Get_WorldMatrix();



		matWorld = matScale *matTrans* matParent;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		_int x = _int(m_vecBone[i]->Get_MatrixRow(CTransform::INFO_POSITION).x * 10);
		_int y = _int(m_vecBone[i]->Get_MatrixRow(CTransform::INFO_POSITION).y * 10);
		_int z = _int(m_vecBone[i]->Get_MatrixRow(CTransform::INFO_POSITION).z * 10);
		_int n = _int(m_vecBone[i]->Get_ParentIndex());
		
		_tchar szBuf[128];
		wsprintf(szBuf, TEXT(" BONE: %d, %d \n x: %d y: %d z: %d"), i, n, x, y, z);

		m_pFont->Render3DText(szBuf);
	}
}

void CCreature::Render_BodyCube()
{
	for (size_t i = 0; i < m_vecBody.size(); ++i)
	{
		_matrix matWorld, matScale, matTrans, matParent;
		D3DXMatrixTranslation(&matTrans, m_vecBody[i]->vPosition.x, m_vecBody[i]->vPosition.y, m_vecBody[i]->vPosition.z);
		D3DXMatrixScaling(&matScale, m_vecBody[i]->vScale.x, m_vecBody[i]->vScale.y, m_vecBody[i]->vScale.z);

		if (m_vecBody[i]->iParentIndex != -1)
		{
			matParent = m_vecBone[m_vecBody[i]->iParentIndex]->Get_WorldMatrix();

			matWorld = matScale*matTrans*matParent;
		}
		else /*has not parent*/
		{
			matWorld = matScale* matTrans;
		}

		m_pTextureCom->SetUp_TextureToGraphicDev(0, m_vecBody[i]->iTextureNum);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		m_pCubeTex->Render_Buffer();

	}
}

void CCreature::Render_BodyFont()
{
	for (size_t i = 0; i < m_vecBody.size(); ++i)
	{
		_matrix matScale, matParent, matWorld, matTrans;

		_matrix			matView;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, nullptr, &matView);


		_matrix mat;
		D3DXMatrixIdentity(&mat);
		(_vec3&)mat.m[0] = (_vec3&)matView.m[0];
		(_vec3&)mat.m[2] = (_vec3&)matView.m[2];

		_matrix mat2;
		D3DXMatrixIdentity(&mat2);
		_int n = m_vecBody[i]->iParentIndex;
		_vec3 v = _vec3(0.f, 0.f, 0.f);
		
		if (-1 != n)
		{
			_matrix matBoneWorld = m_vecBone[n]->Get_WorldMatrix();
			v = _vec3(matBoneWorld._41, matBoneWorld._42, matBoneWorld._43);

		}
		v += m_vecBody[i]->vPosition;
		
		(_vec3&)mat.m[3] = v;
		matParent = mat;

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 0.05f, 0.05f, 0.05f);
		D3DXMatrixTranslation(&matTrans, 0.1f, 0.1f, 0.f);

		matWorld = matScale *matTrans* matParent;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		_tchar szBuf[128];
		wsprintf(szBuf, TEXT("CUBE: %d, %d"),i,n);

		m_pFont->Render3DText(szBuf);
	}
}

void CCreature::Update_BoneAnimate(const _float & fTimeDelta)
{
	if (m_iStateIndex >= m_vvKeyFrameTime.size())
		return;

	if (m_dwFrame >= m_vvKeyFrameTime[m_iStateIndex].size())
		return;


	_float fTimer = *(m_vvKeyFrameTime[m_iStateIndex][m_dwFrame]);

	if (fTimer <= 0)
		return;

	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		m_vecBone[i]->Update_KeyFrame(m_iStateIndex, m_dwFrame, fTimer, fTimeDelta);
	}

}

void CCreature::Update_Frame(const _float & fTimeDelta)
{
	if (m_iStateIndex >= m_vvKeyFrameTime.size())
		return;
	
	if (m_dwFrame >= m_vvKeyFrameTime[m_iStateIndex].size())
		return;

	_float fTimer = *(m_vvKeyFrameTime[m_iStateIndex][m_dwFrame]);

	if (fTimer <= 0)
		return;

	m_fFrameTimer += fTimeDelta;


	if (m_fFrameTimer > fTimer)
	{
		m_fFrameTimer = 0;
		++m_dwFrame;
	}
	/*Frame over KeyFrameCount*/
	if (m_dwFrame >= m_vvKeyFrameTime[m_iStateIndex].size())
	{
		m_fFrameTimer = 0;
		m_dwFrame = 0;
	}

}

HRESULT CCreature::Ready_Component()
{
	CComponent* pComponent = nullptr;

	/*Renderer*/
	pComponent = m_pRenderer = dynamic_cast<CRenderer*>(m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == m_pRenderer)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));
	m_pRenderer->AddRef();
	
	/*CubeBone Buffer*/
	pComponent = m_pBoneCubeCom= dynamic_cast<CCube_Color*>(m_pComponent_Manager->Clone_Component(SCENE_TOOL, L"Component_BoneCube"));
	if (nullptr == m_pRenderer)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"BoneCube", pComponent));
	m_pBoneCubeCom->AddRef();

	/*Texture Cube Component*/
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(m_pComponent_Manager->Clone_Component(SCENE_TOOL, L"Component_Texture_Zombie"));
	if (nullptr == m_pRenderer)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
	m_pTextureCom->AddRef();

	/*CubeTex*/
	pComponent = m_pCubeTex = dynamic_cast<CCube_Texture*>(m_pComponent_Manager->Clone_Component(SCENE_TOOL, L"Component_Buffer_CubeTex"));
	if (nullptr == m_pRenderer)
		goto exception;
	m_mapComponent[CComponent::UPDATETYPE_NOTIFY].insert(MAPCOMPONENT::value_type(L"CubeTex", pComponent));
	m_pCubeTex->AddRef();

	return NOERROR;

exception:
	MSG_BOX("Creature Ready Component Failed");
	return E_FAIL;
}

CCreature * CCreature::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCreature* pInstance = new CCreature(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("Create Creature failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CCreature::Free()
{
	_ulong dwRefCount = 0;
	Safe_Release(m_pFont);
	Safe_Release(m_pBoneCubeCom);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pRenderer);



	dwRefCount = CGameObject::Free();

	return dwRefCount;
}
