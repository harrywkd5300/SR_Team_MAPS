#pragma once

#include "gameObject.h"
#include "defines.h"


class CSkill abstract
	:public CGameObject
{

/*ctor dtor*/
protected:
	explicit CSkill(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkill() = default;



/*related ctor dtor*/
protected:
	virtual _ulong Free() override;
};

