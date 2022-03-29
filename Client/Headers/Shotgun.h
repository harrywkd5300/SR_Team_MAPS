#pragma once

#include "Weapon.h"

namespace Engine
{
	class CRect_Texture;
	class CRenderer;
	class CTexture;
	class CTransform;
	class CRandomWell512;
	class CParticle_Renderer;
	class CParticle_Effect;
	class CParticle_Bullet;
	class CEffect;
	class CEffect_Bullet;
	class CEffect_Manager;
	class CPicking;
	class CTransform;
}


class CShotgun  : public CWeapon
{
protected:
	explicit CShotgun(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShotgun() = default;
public:
	virtual HRESULT Ready_GameObject(void) ;
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

public:
	virtual void	Use_Weapon(const _float fTimeDelta, _vec3* vRecoverAngle);

protected:
	HRESULT Ready_Component(void);	
public:
	static CShotgun* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;

};

