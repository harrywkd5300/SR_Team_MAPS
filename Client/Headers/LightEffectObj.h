#pragma once

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRect_Texture;
	class CRenderer;
	class CTexture;
	class CTransform;
	class CLight_Manager;
	class CEffect;
	class CEffect_Bullet;
	class CEffect_Manager;
	class CEffect_Fixed;
}

class CLightEffectObj : public Engine::CGameObject
{
protected:
	explicit CLightEffectObj(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightEffectObj() = default;
public:
	virtual HRESULT Ready_GameObject(void) ;
	virtual _int Update_GameObject(const _float& fTimeDelta) ;
	virtual void Render_GameObject(void) ;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
public:
	void Set_LightColor(D3DXCOLOR color, float fRange);
	void Set_Texture(_tchar * pComponentTag, _float fMaxFrame);
	
	CTransform*	Get_Transform() { return m_pTransformCom; }
protected:
	Engine::CTransform*				m_pTransformCom = nullptr;	
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
	_tchar*							m_pTextureTag = nullptr;
	CEffect_Manager*				m_pEffectManager = nullptr;
	CEffect_Fixed*					m_pEffect = nullptr;
	_vec3							m_vScale;
	
	vector<BODYINFO*>      m_vecBodyInfo;
	vector<CBone*>         m_vecBone;
	CCube_Texture*         m_pBodyCubeBuffer = nullptr;
protected:
	_float							m_fFrame = 0.f;
protected:
	HRESULT Ready_Component(void);	
public:
	static CLightEffectObj* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) ;

};

