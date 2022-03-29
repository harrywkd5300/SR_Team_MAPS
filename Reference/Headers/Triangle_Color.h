#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class DLL_EXPORT CTriangle_Color final
	:public CVIBuffer
{

/*ctor dtor*/
public:
	explicit CTriangle_Color(LPDIRECT3DDEVICE9 pGrahpicDev);
	explicit CTriangle_Color(const CTriangle_Color& rhs);
	virtual ~CTriangle_Color() =default;

/*FUNCTION*/
public:
	HRESULT						Ready_Buffer();
	void						Render_Buffer();


/*related ctor dtor*/
public:
	static CTriangle_Color*		Create(LPDIRECT3DDEVICE9 pGrahpicDev);

private:
	virtual	CComponent* Clone() final;
	virtual _ulong Free() final;




};

END