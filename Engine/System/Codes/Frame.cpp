
#include "Frame.h"


CFrame::CFrame(void)
	: m_fTimeAcc(0.f), m_fTimePerSec(0.f)
{
}

HRESULT CFrame::Initialize(const _float& fCallCnt)
{
	m_fTimePerSec = 1.f / fCallCnt;
		
	return NOERROR;
}


_bool CFrame::Permit_Call(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if(m_fTimePerSec <= m_fTimeAcc)
	{
		m_fTimeAcc = 0.f;
		return true;
	}

	return false;
}


CFrame* CFrame::Create(const _float& fCallCnt)
{
	CFrame*		pInstance = new CFrame();

	if(FAILED(pInstance->Initialize(fCallCnt)))
	{
		MessageBox(0, L"Create Created Failed", L"System Error", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;

}

_ulong CFrame::Free()
{
	return 0;
}
