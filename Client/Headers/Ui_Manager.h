#pragma once

#include "Defines.h"
#include "gameObject.h"


class CUi_Manager final : public CBase
{
	DECLARE_SINGLETON(CUi_Manager)
private:
	explicit CUi_Manager();
	virtual ~CUi_Manager() = default;
private:
	list<CGameObject*>		m_UIList;

public:
	void					Add_UIList(CGameObject* pUIObject) { m_UIList.push_back(pUIObject); }

	void					Update_UIList();
	void					Sort_UIList();
	list<CGameObject*>*		Get_UIList() { return &m_UIList; }


private:
	virtual _ulong Free() final;
};

