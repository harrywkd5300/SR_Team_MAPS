#pragma once

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CInput_Device;
}

class CHouseRoof final : public Engine::CGameObject
{
private:
	explicit CHouseRoof(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHouseRoof() = default;
public:
	void Test();

public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;
	virtual void Render_Shadow(_matrix CameraWorldMatrix);
public:
	void SetShader(_uint iShaderNum);
	void StartShader(_uint iShaderNum);
	void EndShader(_uint iShaderNum);

private:
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CCube_Texture*			m_pBufferCom = nullptr;
	CShader*						m_pShaderCom = nullptr;
	CMaterial*						m_pMatCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;

private:
	vector<CGameObject*>			m_pCubeObj;

private:
	HRESULT Ready_Component(void);
	HRESULT	Ready_CubeLoad();
public:
	static CHouseRoof* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;

};

