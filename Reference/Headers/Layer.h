#pragma once


#include "Base.h"
#include "Engine_Defines.h"


BEGIN(Engine)

class CGameObject;
class CComponent;

class DLL_EXPORT CLayer
	:public CBase
{
/*ctor dtor*/
private:
	explicit CLayer();
	virtual ~CLayer() = default;

/*FUNCTION*/
public:
	const list<CGameObject*>&	Get_ObjList() {return m_ObjList;}
	CComponent*					Get_Component(const _uint& iLayerIndex, const _tchar* pComponentTag);


public:
	virtual HRESULT Ready_Layer();
	virtual _int	Update_Layer(const _float& fTimeDelta);
	virtual _int	LastUpdate_Layer(const _float& fTimeDelta);
	virtual void	Render_Layer();

	HRESULT			Add_GameObject(CGameObject* pGameObject);
	void			Release_TargetObject(const _uint& iArrayIdx);



/*MEMBER*/
private:
	list<CGameObject*>	m_ObjList;
	typedef list<CGameObject*> OBJLIST;


public:
	static CLayer*	Create();

protected:
	virtual _ulong Free() override;

};

END