#include "..\Headers\Component.h"

using namespace Engine;

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev), m_bIsClone(false)
{
	m_pGraphicDev->AddRef();
}

CComponent::CComponent(const CComponent & rhs)
	:m_pGraphicDev(rhs.m_pGraphicDev), m_bIsClone(true)
{
	m_pGraphicDev->AddRef();
}


CComponent::~CComponent()
{
}

_ulong CComponent::Free()
{
	_ulong dwRefCount = 0;

	/*decrease refcnt*/
	Safe_Release(m_pGraphicDev);

	return dwRefCount;
}
