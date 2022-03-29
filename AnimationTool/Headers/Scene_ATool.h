#pragma once

#include "scene.h"
#include "A_Defines.h"

namespace Engine
{
	class CInput_Device;
	class CCamera_Manager;
}
class CCreature_Manager;


class CScene_ATool final
	:public CScene
{
	/*ctor dtor*/
public:
	explicit CScene_ATool(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_ATool() = default;

/*FUNCTION*/
public:
	virtual HRESULT			Ready_Scene() final;

	virtual _int			Update_Scene(const _float& fTimeDelta);
	virtual void			Render_Scene();

private:
	HRESULT					Add_GameObject_ATool();
	HRESULT					Add_Component_ATool();
	HRESULT					Add_Camera_ATool();

	/*MEMBER*/
private:
	CCreature_Manager* m_pCreature_Manager=nullptr;
	CCamera_Manager* m_pCamera_Manager=nullptr;

	/*FUNCTION related ctor dtor */
public:
	static CScene_ATool*		Create(LPDIRECT3DDEVICE9 pGraphicDevice);

private:
	virtual _ulong Free() final;
};

