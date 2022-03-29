#pragma once

#include "Skill.h"

class CBullet
	:public CSkill
{
/*ctor dtor*/
private:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBullet() = default;

/*FUNCTION*/
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject();






	/*Related ctor dtor*/
public:
	static CBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual _ulong Free() final;
};

