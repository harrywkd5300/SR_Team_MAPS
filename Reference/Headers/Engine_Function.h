#pragma once

namespace Engine
{
	template<typename T>
	unsigned long Safe_Release(T& pInstance)
	{
		unsigned long dwRefCount = 0;
		if (nullptr != pInstance)
		{
			dwRefCount = pInstance->Release();

			if (0 == dwRefCount)
				pInstance = nullptr;

		}
		return dwRefCount;
	}

	template<typename T>
	void Safe_Delete(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete pPointer;
			pPointer = nullptr;
		}
	}

	template<typename T>
	void Safe_Delete_Array(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete[] pPointer;
			pPointer = nullptr;
		}
	}
}

