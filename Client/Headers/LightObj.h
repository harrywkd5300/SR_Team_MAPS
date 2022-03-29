#pragma once
#include "defines.h"
#include "gameObject.h"

namespace Engine
{
	class CLight_Manager;
}
class CLightObj
	:public CGameObject
{
/*ctor dtor*/
private:
	explicit CLightObj(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightObj() = default;

/*FUNCTION*/
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject();
	CLight* GetLightCom() { return m_pLightCom; };
	CTransform* GetTransformCom() { return m_pTransformCom; };
	void SetColor(CLight::LIGHTCOLOR eType);

private:
	HRESULT Ready_Component();

/*MEMBER*/
private:
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CLight*			m_pLightCom = nullptr;
	CLight_Manager* m_pLightMgr = nullptr;
	/*Related ctor dtor*/
public:
	static CLightObj* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual _ulong Free() final;

	//----------        Test           ----------
	LPD3DXMESH				m_pSphere = NULL;
	const int LIGHT_OBJECT_SLICES = 32;
	const int LIGHT_OBJECT_STACKS = 32;
	const float LIGHT_OBJECT_RADIUS = 2.0f;
	void	Update_Input(const _float fTimeDelta);
	//LPD3DXMESH LoadModel(const _tchar * filename);
	//----------        Test           ----------
};

