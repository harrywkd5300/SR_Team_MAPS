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
}

class CEffect final : public Engine::CGameObject
{
private:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect() = default;
public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;
public:
	void Set_LightColor(D3DXCOLOR color, float fRange);
	void Set_Texture(const _tchar * pComponentTag);
private:
	Engine::CTransform*				m_pTransformCom = nullptr;	
	Engine::CRect_Texture*			m_pBufferCom = nullptr; 
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;

	CLight*			m_pLightCom = nullptr;
	CLight_Manager* m_pLightMgr = nullptr;
private:
	_float							m_fFrame = 0.f;
private:
	HRESULT Ready_Component(void);	
public:
	static CEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;

};

