#pragma once

#include "Defines.h"
#include "GameObject.h"

class CTestHouseTop final : public Engine::CGameObject
{
private:
	explicit CTestHouseTop(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestHouseTop() = default;

public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;

private:
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CCube_Texture*			m_pBufferCom = nullptr;

private:
	vector<CGameObject*>			m_pCubeObj;
private:
	HRESULT Ready_Component(void);
	HRESULT	Ready_CubeLoad();
public:
	static CTestHouseTop* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;

};

