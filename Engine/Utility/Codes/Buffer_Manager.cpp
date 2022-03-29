#include "..\Headers\Buffer_Manager.h"
#include "Component_Manager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CBuffer_Manager)

Engine::CBuffer_Manager::CBuffer_Manager(void)
{
	
}

void CBuffer_Manager::Clear_BufferList()
{
	for (auto& pBuffer : m_BufferList)
	{
		Safe_Release(pBuffer);
	}
	m_BufferList.clear();
}

CVIBuffer * CBuffer_Manager::Get_Buffer(_uint iIndex)
{
	int index = 0;
	for (auto pBuffer : m_BufferList)
	{
		if (index == iIndex)
		{
			return pBuffer;
		}
		++index;
	}
	return nullptr;
}

HRESULT CBuffer_Manager::Add_Buffer(CVIBuffer * pBuffer)
{
	if (pBuffer == nullptr)
		return E_FAIL;

	m_BufferList.push_back(pBuffer);

	return NOERROR;
}

_ulong Engine::CBuffer_Manager::Free(void)
{
	_ulong dwRefCount = 0;

	for (auto& pBuffer : m_BufferList)
	{
		Safe_Release(pBuffer);
	}
	m_BufferList.clear();

	return dwRefCount;
}
