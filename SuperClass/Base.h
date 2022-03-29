#pragma once


class _declspec(dllexport) CBase abstract
{

/*ctor dtor*/
protected:
	inline explicit CBase();
	inline virtual ~CBase() = default;


/*Function*/
protected:
	const unsigned long& Get_RefCount() { return m_dwRefCnt; }

public:
	inline	unsigned long AddRef();
	inline	unsigned long Release();

/*member*/
private:
	unsigned long m_dwRefCnt=0;


/*related ctor dtor*/
protected:
	virtual unsigned long Free() = 0;
};

#include "Base.inl"