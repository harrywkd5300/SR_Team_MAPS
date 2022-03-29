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
	class CParticle_Bullet;
}


class CPistol  : public CWeapon
{
protected:
	explicit CPistol(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPistol() = default;
public:
	virtual HRESULT Ready_GameObject(void) ;
	//virtual _int Update_GameObject(const _float& fTimeDelta, _vec3* vRecoverAngle) ;
	//virtual void Render_GameObject(void) ;

public:
	//void	Reload_Weapon();
	//virtual void	Use_Weapon(const _float fTimeDelta);

protected:
	HRESULT Ready_Component(void);	
public:
	static CPistol* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;

};

