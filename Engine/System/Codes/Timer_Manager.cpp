#include "Timer_Manager.h"

IMPLEMENT_SINGLETON(CTimer_Manager)

CTimer_Manager::CTimer_Manager(void)
{
}

_float CTimer_Manager::Get_TimeDelta(const TCHAR* pTimerTag) const
{
	CTimer*			pTimer = nullptr;

	pTimer = Find_Timer(pTimerTag);

	if(nullptr == pTimer)
		return 0.0f;

	return pTimer->Get_TimeDelta();
}	

HRESULT CTimer_Manager::Add_Timer(const TCHAR* pTimerTag, CTimer* pTimer)
{
	if(nullptr != Find_Timer(pTimerTag))
		return E_FAIL;

	m_mapTimer.insert(MAPTIMER::value_type(pTimerTag, pTimer));

	return NOERROR;
}

void CTimer_Manager::Compute_Timer(const TCHAR* pTimerTag)
{
	CTimer*			pTimer = nullptr;

	pTimer = Find_Timer(pTimerTag);

	if(nullptr == pTimer)
		return;

	pTimer->Compute_Timer();

}
CTimer* CTimer_Manager::Find_Timer(const TCHAR* pTimerTag) const
{
	auto iter = find_if(m_mapTimer.begin(), m_mapTimer.end(), CStringCmp(pTimerTag));
	
	if (iter == m_mapTimer.end())
		return nullptr;
	
	return iter->second;
}

_ulong CTimer_Manager::Free(void)
{
	for (auto& Pair : m_mapTimer)
	{
		Safe_Release(Pair.second);
	}
	m_mapTimer.clear();

	return 0;
}