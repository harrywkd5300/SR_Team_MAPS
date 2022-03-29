#pragma once

#define BEGIN(NAMESPACE) namespace NAMESPACE {
#define END }

#define USING(NAMESPACE) using namespace NAMESPACE;

#define MSG_BOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Error"), MB_OK)

#define DLL_EXPORT _declspec(dllexport)

#define NO_COPY(CLASSNAME)							\
	private:										\
	CLASSNAME(const CLASSNAME&);					\
	CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)				\
	NO_COPY(CLASSNAME)								\
	private:										\
	static CLASSNAME*	m_pInstance;				\
	public:											\
	static CLASSNAME*	GetInstance( void );		\
	static unsigned long DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)						\
	CLASSNAME*	CLASSNAME::m_pInstance = nullptr;			\
	CLASSNAME*	CLASSNAME::GetInstance( void )	{			\
	if(nullptr == m_pInstance) {							\
	m_pInstance = new CLASSNAME;							\
	}														\
	return m_pInstance;										\
	}														\
	unsigned long CLASSNAME::DestroyInstance( void ) {		\
	unsigned long dwRefCnt = 0;								\
	if(nullptr != m_pInstance)	{							\
	dwRefCnt = m_pInstance->Release();						\
	if(0 == dwRefCnt)										\
	m_pInstance = nullptr;									\
	}														\
	return dwRefCnt;										\
	}


// ---------------- Jang ------------------
#define LIGHT_RANGE_DEFAULT 10.f
#define MAX_LIGHT_SIZE 20