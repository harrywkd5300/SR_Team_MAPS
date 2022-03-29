#pragma once

namespace Engine
{
	class CStringCmp
	{
	private:
		const TCHAR* m_pKey;

	public:
		CStringCmp() {}
		CStringCmp(const TCHAR* _pKey) :m_pKey(_pKey) {}
		~CStringCmp() {}

	public:
		template<typename T>
		bool operator ()(T& pKey)
		{
			return !lstrcmp(pKey.first, m_pKey);
		}
	};

	class CReleaseMap
	{
	public:
		CReleaseMap() {};
		~CReleaseMap() {};

	public:
		template<typename T>
		void operator()(T& Pair)
		{
			Pair.second->Release();
			Pair.second = nullptr;
		}
	};
}
