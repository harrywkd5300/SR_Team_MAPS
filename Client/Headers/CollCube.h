
#pragma once

#include "defines.h"
#include "gameObject.h"

class CCollider_Manager;

class CCollCube : public CGameObject
{
private:
	explicit CCollCube(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iDrawID, const TCHAR* pKey, CCube_Texture* pBufferCom);
	virtual ~CCollCube() = default;


public:
	CTransform*		Get_Transform() { return m_pTransformCom; };

public:
	virtual HRESULT Ready_GameObject(_vec3 vScale) final;

	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject() final;
	virtual void Render_Shadow(_matrix ShadowWorld);

private:
	HRESULT Ready_Component();
	/*MEMBER*/
private:
	CCube_Texture*		m_pBufferCom = nullptr;
	//CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	_int				m_iTextureNum = 0;
	const _tchar*		m_pKey;
	CCollider_Manager*		m_pColliderMgr = nullptr;

	// -------------- Test -----------------------
	CAABB_Buffer*			m_pColliderBox_AABB = nullptr;
	// -------------- Test -----------------------
public:
	/*related ctor dtor*/
public:
	void TransForm(_vec3 vPos, _vec3 vRota, _vec3 vScale);

public:
	static CCollCube* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vRota, _vec3 vScalse, const _uint& iDrawID, const TCHAR* pKey, CCube_Texture* pBufferCom);

private:
	virtual _ulong Free() final;
};

