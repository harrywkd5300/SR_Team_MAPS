#include "stdafx.h"
#include "..\Headers\Ui_Manager.h"

IMPLEMENT_SINGLETON(CUi_Manager)

CUi_Manager::CUi_Manager()
{
}

void CUi_Manager::Update_UIList()
{
	Sort_UIList();
}

void CUi_Manager::Sort_UIList()
{
	m_UIList.sort();
}

_ulong CUi_Manager::Free()
{
	auto& iter = m_UIList.begin();
	for (; iter != m_UIList.end(); ++iter)
	{
		Safe_Release((*iter));
	}
	return 0;
}
