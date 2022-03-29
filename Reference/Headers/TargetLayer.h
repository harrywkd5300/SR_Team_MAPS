#pragma once


#include "Base.h"
#include "Engine_Defines.h"


BEGIN(Engine)

class CTarget;
class CComponent;

class DLL_EXPORT CTargetLayer
	:public CBase
{
/*ctor dtor*/
private:
	explicit CTargetLayer();
	virtual ~CTargetLayer() = default;

/*FUNCTION*/

public:
	virtual HRESULT	Ready_TargetLayer();
	void			Set_ToGraphicDev();
	void			Release_ToGraphicDev();
	void			Clear_RenderTarget();

	HRESULT			Add_Target(CTarget* pTarget, const _tchar* pTargetTag);
	CTarget*		Find_Target(const _tchar* pTargetTag);

	

/*MEMBER*/
private:
	map<const _tchar*, CTarget*>					m_mapTarget;
	typedef map<const _tchar*, CTarget*>			TARGETMAP;

public:
	static CTargetLayer*	Create();

protected:
	virtual _ulong Free() override;

};

END