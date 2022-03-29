#pragma once

#include "Engine_Defines.h"
#include "viBuffer.h"

BEGIN (Engine)

class DLL_EXPORT CRect_Color final
	:public CVIBuffer
{

/*ctor dtor*/
private:
	explicit CRect_Color(LPDIRECT3DDEVICE9 pGrahpicDev);
	explicit CRect_Color(const CRect_Color& rhs);
	virtual ~CRect_Color() = default;


/*FUNCTION*/
public:
	HRESULT				Ready_Buffer();
	void				Render_Buffer();

public:
	static CRect_Color* Create(LPDIRECT3DDEVICE9 pGrahpicDev);

private:
	virtual CComponent* Clone();
	virtual _ulong Free();
};

END