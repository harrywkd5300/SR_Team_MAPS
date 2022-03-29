#pragma once


#include "Engine_Defines.h"
#include "base.h"

BEGIN(Engine)

class CLayer;
class CGameObject;
class CComponent;

class DLL_EXPORT CObject_Manager final
	:public CBase
{
	DECLARE_SINGLETON(CObject_Manager)

/*ctor dtor*/
private:
	explicit CObject_Manager();
	virtual ~CObject_Manager() = default;

/*FUNCTION*/
public:
	CComponent*		Get_Component(const _uint& iSceneIndex, const _tchar* pLayerTag, const _uint& iLayerIndex, const _tchar* pComponentTag);
	CLayer*			Get_Layer(const _uint& iSceneIndex, const _tchar* pLayerTag);

public:
	HRESULT	Reserve_Container(const _uint& iContainerSize);
	HRESULT	Add_GameObject(const _uint& iSceneIdx, const _tchar* pLayerTag, CGameObject* pGameObject);

	HRESULT Ready_Object_Manager();
	_int	Update_Object_Manager(const _float& fTimeDelta);
	_int	LastUpdate_Object_Manager(const _float& fTimeDelta);
	void	Render_Object_Manager();

	HRESULT	Release_TargetContainer(const _uint& iSceneIndex);
	HRESULT	Release_TargetLayer(const _uint& iSceneIndex, const _tchar* pLayerTag);
	

private:
	CLayer*	Find_Layer(const _uint& iSceneIndex, const _tchar* pLayerTag);

/*MEMBER*/
private:
	map<const _tchar*, CLayer*>*			m_pMapLayer = nullptr;
	typedef map<const _tchar*, CLayer*>		MAPLAYER;
	_uint									m_iContainerSize = 0;

/*related ctor dtor*/
private:
	virtual _ulong	Free() final;


};

END