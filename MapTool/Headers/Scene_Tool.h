#pragma once

#include "Tool_Define.h"
#include "Scene.h"

#include "Tool_Terrian.h"
#include "Tool_ToolCCamera.h"
#include "Tool_Cube.h"
#include "Tool_Object.h"

class CScene_Tool : public CScene
{
private:
	CScene_Tool(LPDIRECT3DDEVICE9 pGraphicDev);
	~CScene_Tool() = default;

	//
	/*FUNCTION*/
	//
public:
	virtual HRESULT			Ready_Scene() final;
	HRESULT					Add_GameObject_ForLogo();
	HRESULT					Add_Component_ForLogo();

	virtual _int			Update_Scene(const _float& fTimeDelta);
	virtual void			Render_Scene();
public:
	void	Change_Texture_view(_bool Check);
	void	Set_WireFrame();

	const list<CGameObject*>*	Get_CubejList();


	_bool	ObjectPicking(_ulong vMx, _ulong vMy, CString strName);
	_vec3	TerrainPicking(_ulong vMx, _ulong vMy);

	HRESULT	Create_Cube(_vec3 vPos, _vec3 vRota, _vec3 vScalse, const _uint& iTextureNum, const TCHAR* pKey, CString strKey);
	_vec3*	Create_OnCube(const _uint& iDrawID, const _uint& iVecNum, const TCHAR* pKey, CString strKey);

	CGameObject*    Create_Object(CString strKey);

private:

	//
	/*MEMBER*/
	//
	CGameObject* pCamera = nullptr;
	CGameObject* pTerrian = nullptr;
	_bool		 m_bWireFrame = false;
	CPicking*	 m_pPickingCom = nullptr;
	CString		 m_key;

private:

	//
	/*FUNCTION related ctor dtor */
	//
public:
	static CScene_Tool*		Create(LPDIRECT3DDEVICE9 pGraphicDevice);

private:
	virtual _ulong Free() final;
};

