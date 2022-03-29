#pragma once

#include "Tool_Cube.h"

class CTool_Object final : public CGameObject
{
private:
	explicit CTool_Object(LPDIRECT3DDEVICE9 pGraphicDev, CString strName);
	virtual ~CTool_Object() = default;

public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;
public:
	void					Set_NotPicking();
	vector<CGameObject*>*	GetCube() {	return &m_pCubeObj;}
	HRESULT	Create_Cube(_vec3 vPos, _vec3 vRota, _vec3 vScalse, const _uint& iTextureNum, const TCHAR* pKey);
	CString&		GetStrKey() { return m_strName; }

private:
	Engine::CRenderer*				m_pRendererCom = nullptr;

	CString			m_strName;
private:
	vector<CGameObject*>			m_pCubeObj;

private:
	HRESULT Ready_Component(void);

public:
	static CTool_Object* Create(LPDIRECT3DDEVICE9 pGraphicDev, CString strName);
protected:
	virtual _ulong Free(void) final;
};

