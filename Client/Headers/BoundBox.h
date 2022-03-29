#pragma once

#include "Defines.h"
#include "gameObject.h"

class CBoundBox final : public CGameObject
{
private:
	explicit CBoundBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoundBox() = default;

private:
	CCube_Texture*		m_pBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

public:
	virtual HRESULT Ready_GameObject() final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject() final;

private:
	HRESULT Ready_Component();
public:
	static CBoundBox* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vScalse);

private:
	virtual _ulong Free() final;
};

