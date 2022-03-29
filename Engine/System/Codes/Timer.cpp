#include "Timer.h"

using namespace Engine;

CTimer::CTimer(void)
: m_fTimeDelta(0.f)
{
}

HRESULT CTimer::Initialize(void)
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);	
	QueryPerformanceFrequency(&m_CpuTick);

	return NOERROR;
}


void CTimer::Compute_Timer(void)
{
	QueryPerformanceCounter(&m_FrameTime);

	if( (m_FrameTime.QuadPart - m_LastTime.QuadPart) > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	m_fTimeDelta = float( m_FrameTime.QuadPart - m_FixTime.QuadPart ) / m_CpuTick.QuadPart;

	m_FixTime	  = m_FrameTime;
}

CTimer* CTimer::Create(void)
{
	CTimer*		pInstance = new CTimer();

	if(FAILED(pInstance->Initialize()))
	{
		MessageBox(0, L"CTimer Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CTimer::Free(void)
{	
	return 0;
}
