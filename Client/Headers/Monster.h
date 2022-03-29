#pragma once

#include "defines.h"
#include "Character.h"

class CMonster : public CCharacter
{
	enum BEHAVIOR { MON_WALK, MON_ATTACK, MON_DEAD };
protected: 
	virtual _bool Compare_AnimateType() { return true; };

protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Ready_GameObject() PURE;
	virtual _int Update_GameObject(const _float& fTimeDelta) PURE;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject() PURE;

protected:
	CHARWAY		iMoveIdx[7][7][5];
	BEHAVIOR	eMonType;


protected:
	void	Monster_beHavior(BEHAVIOR eType);

protected:
	virtual _ulong	Free() override;
};

