
#include "Frame_Manager.h"


IMPLEMENT_SINGLETON(CFrame_Manager)

CFrame_Manager::CFrame_Manager(void)
{
}

HRESULT CFrame_Manager::Add_Frame(const TCHAR* pFrameTag, const _float& fCallCnt)
{
	if(nullptr != Find_Frame(pFrameTag))
		return E_FAIL;

	CFrame*		pFrame = CFrame::Create(fCallCnt);
	if(nullptr == pFrame)
		return E_FAIL;

	m_mapFrame.insert(MAPFRAME::value_type(pFrameTag, pFrame));
	
	return NOERROR;
}

_bool CFrame_Manager::Permit_Call(const TCHAR* pFrameTag, const _float& fTimeDelta)
{
	CFrame*			pFrame = NULL;

	pFrame = Find_Frame(pFrameTag);

	if(nullptr == pFrame)
		return false;

	return pFrame->Permit_Call(fTimeDelta);
}

CFrame* CFrame_Manager::Find_Frame(const TCHAR* pFrameTag)
{
	/*using lambda*/
	auto iter = find_if(m_mapFrame.begin(), m_mapFrame.end(), [&](const MAPFRAME::value_type& Pair) {return !lstrcmp(Pair.first, pFrameTag); });

	if (iter == m_mapFrame.end())
		return nullptr;

	return iter->second;
}

_ulong CFrame_Manager::Free()
{
	for (auto& Pair : m_mapFrame)
	{
		_ulong dwRefCount = 0;

		if (dwRefCount = Safe_Release(Pair.second))
		{
			MSG_BOX("Frame Release Failed");
			return dwRefCount;
		}
	}
	m_mapFrame.clear();

	return 0;
}
