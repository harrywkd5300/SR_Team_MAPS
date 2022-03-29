#include "..\Headers\Resources.h"



CResources::CResources(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice)
{
}

CResources::CResources(const CResources & rhs)
	: CComponent(rhs)
{

}


CResources::~CResources()
{
}

_ulong CResources::Free()
{
	return CComponent::Free();
}
