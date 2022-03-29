#include "stdafx.h"
#include "..\Headers\Creature_Manager.h"
#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CCreature_Manager)


CCreature_Manager::CCreature_Manager()
{


}

CCreature * CCreature_Manager::Get_Creature(const _tchar * pCreatureTag)
{
	auto iter = find_if(m_mapCreature.begin(), m_mapCreature.end(), [&](const MAPCREATURE::value_type Pair) {return !lstrcmp(Pair.first, pCreatureTag); });

	if (iter == m_mapCreature.end())
		return nullptr;

	return iter->second;
}

HRESULT CCreature_Manager::Add_Creature(const _tchar * pCreatureTag, CCreature * pCreature)
{
	CCreature* pFindCreature = Find_Creature(pCreatureTag);

	if (nullptr != pFindCreature)
	{
		MSG_BOX("Creature tag already used err:00995");
		return E_FAIL;
	}

	m_mapCreature.insert(MAPCREATURE::value_type(pCreatureTag, pCreature));
	//cout << m_mapCreature.size() << endl;
	return NOERROR;
}

HRESULT CCreature_Manager::Del_Creature(const _tchar * pCreatureTag)
{
	auto iter = find_if(m_mapCreature.begin(), m_mapCreature.end(), [&](const MAPCREATURE::value_type Pair) {return !lstrcmp(Pair.first, pCreatureTag); });

	if (iter == m_mapCreature.end())
	{
		MSG_BOX("CreatureTag is wrong");
		return E_FAIL;
	}

	Safe_Release(iter->second);
	m_mapCreature.erase(iter);

	return NOERROR;
}

CCreature * CCreature_Manager::Find_Creature(const _tchar * pCreatureTag)
{

	auto iter = find_if(m_mapCreature.begin(), m_mapCreature.end(), [&](const MAPCREATURE::value_type Pair) {return !lstrcmp(Pair.first, pCreatureTag); });

	if (iter == m_mapCreature.end())
		return nullptr;

	return iter->second;
}

_int CCreature_Manager::Update_Creature(const _float & fTimeDelta)
{
	if (nullptr == m_pCurCreature)
		return 0;

	if (true == bAnimate)
	{
		m_pCurCreature->Update_Frame(fTimeDelta);
		m_pCurCreature->Update_BoneAnimate(fTimeDelta);
	}

	m_pCurCreature->Update_GameObject(fTimeDelta);
	return 0;
}

void CCreature_Manager::Render_Creature()
{
	if (nullptr == m_pCurCreature)
		return;

	/*Lighting on*/
	m_pCurCreature->Render_GameObject();

	if (true == bWIREMODE)
		CGraphic_Device::GetInstance()->Get_GraphicDev()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		CGraphic_Device::GetInstance()->Get_GraphicDev()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	if (true == bBoneRender)
		m_pCurCreature->Render_BoneCube();
	
	if (true == bBoneCoord)
		m_pCurCreature->Render_BoneFont();

	if (true == bCubeRender)
		m_pCurCreature->Render_BodyCube();

	if (true == bCubeCoord)
		m_pCurCreature->Render_BodyFont();


}


_ulong CCreature_Manager::Free()
{

	_ulong dwRefCount = 0;

// 	for (auto& Pair : m_mapCreature)
// 	{
// 
// 		if (dwRefCount = Safe_Release(Pair.second))
// 			MSG_BOX("Creature Release Failed err: 90767");
// 	}
// 	m_mapCreature.clear();

	return 0;
}
