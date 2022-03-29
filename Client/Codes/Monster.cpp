#include "stdafx.h"
#include "..\Headers\Monster.h"


CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCharacter(pGraphicDev)
{
	ZeroMemory(iMoveIdx, sizeof(int)*7*7*5);
	m_eCharType = CHAR_TYPE::CHAR_TYPE_MONSTER;
}

_int CMonster::LastUpdate_GameObject(const _float & fTimeDelta)
{
	CCharacter::LastUpdate_GameObject(fTimeDelta);

	return 0;
}

void CMonster::Monster_beHavior(BEHAVIOR eType)
{

}

_ulong CMonster::Free()
{
	return  CCharacter::Free();
}
