#pragma once

#include "base.h"
#include "Engine_Defines.h"
#include "component_Manager.h"

BEGIN(Engine)

class CScene;
class DLL_EXPORT CManagement final
	:public CBase
{
	DECLARE_SINGLETON(CManagement)

/*ctor dtor*/
private:
	explicit CManagement();
	virtual ~CManagement() =default;


/*FUNCTION*/
public:
	HRESULT	Ready_Management(const _uint& iReserveSize);
	_int	Update_Management(const _float& fTimeDelta);
	void	Render_Management();

	HRESULT	Set_CurScene(CScene* pNewScene,CRenderer* pRenderer);


/*MEMBER*/
private:
	CScene*			m_pCurScene = nullptr;
	CScene*			m_pNewScene = nullptr;

/*FUNCITON related ctor dtor */
private:
	virtual _ulong Free() final;

};

END