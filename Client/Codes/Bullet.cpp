#include "stdafx.h"
#include "..\Headers\Bullet.h"


CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CSkill(pGraphicDev)
{
}

HRESULT CBullet::Ready_GameObject()
{
	return E_NOTIMPL;
}

_int CBullet::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CBullet::Render_GameObject()
{
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

_ulong CBullet::Free()
{
	return _ulong();
}
