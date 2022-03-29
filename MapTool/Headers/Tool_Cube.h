#pragma once

#include "Tool_Define.h"
#include "gameObject.h"

class CTool_Cube : public CGameObject
{
public:
	enum	MOVECUBE { POS, ROTATION, SCALE };
private:
	explicit CTool_Cube(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iDrawID, const TCHAR* pKey);
	virtual ~CTool_Cube() = default;

public:
	virtual HRESULT Ready_GameObject() final;

	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject() final;

private:
	HRESULT Ready_Component();
	/*MEMBER*/
private:
	CCube_Texture*		m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	STROBJECT*			m_pObjectData = nullptr;

	_int				m_iTextureNum = 0;
	_bool				m_bPickingCube = true;
	_bool				m_bWireFrameVeiew = false;
	const _tchar*		m_pKey;

public:
	/*related ctor dtor*/
public:
	void TransForm(_vec3 vPos, _vec3 vRota, _vec3 vScale);
	void MoveCube(MOVECUBE	m_MoveKey, _vec3 vData);
	void Change_Texture(int iTextureNum) { m_iTextureNum = iTextureNum; }
	int  Get_TextureID() { return m_iTextureNum; }

	void Set_PickingCube(bool PickingCube) { m_bPickingCube = PickingCube; }
	void Set_WireFrame(bool View) { m_bWireFrameVeiew = View; }

	STROBJECT*& GetObjectData() { return m_pObjectData; }

	_bool&	GetPickingCube() { return m_bPickingCube; }
	_bool&	GetWireFrame() { return m_bWireFrameVeiew; }

public:
	static CTool_Cube* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vRota, _vec3 vScalse, const _uint& iDrawID, const TCHAR* pKey);

private:
	virtual _ulong Free() final;
};

