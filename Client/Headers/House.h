#pragma once

#include "Defines.h"
#include "GameObject.h"

class CHouse final : public Engine::CGameObject
{
private:
	explicit CHouse(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHouse() = default;


public:
	void	Test();

public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta);
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
	vector<CGameObject*>			m_pCollCube;

private:
	HRESULT Ready_Component(void);
	HRESULT	Ready_CubeLoad();
	HRESULT	Ready_CollCubeLoad();
public:
	static CHouse* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;

};

