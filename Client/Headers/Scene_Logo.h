#pragma once


#include "defines.h"
#include "Scene.h"

namespace Engine
{
	class CInput_Device;
}

class CScene_Logo final
	: public CScene
{
/*ctor dtor*/
private:
	explicit CScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_Logo() =default;


//
/*FUNCTION*/
//
public:
	virtual HRESULT			Ready_Scene() final;

	virtual _int			Update_Scene(const _float& fTimeDelta);
	virtual void			Render_Scene();


private:
	HRESULT					Add_GameObject_ForLogo();
	HRESULT					Add_Component_ForLogo();
	HRESULT					Add_Camera_ForLogo();
	HRESULT					Add_Logo_BGM();

/*MEMBER*/
private:
	void					Fix_MousePointer(const _uint & iX, const _uint & iY);

private:
	CInput_Device*			m_pInput_Device = nullptr;

/*FUNCTION related ctor dtor */
public:
	static CScene_Logo*		Create(LPDIRECT3DDEVICE9 pGraphicDevice);

private:
	virtual _ulong Free() final;

};

