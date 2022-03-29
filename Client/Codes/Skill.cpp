#include "stdafx.h"
#include "..\Headers\Skill.h"


CSkill::CSkill(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

_ulong CSkill::Free()
{
	return CGameObject::Free();
}
