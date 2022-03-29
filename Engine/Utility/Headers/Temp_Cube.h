#pragma once

#include "vibuffer.h"

BEGIN(Engine)

class DLL_EXPORT CTemp_Cube
	:public CVIBuffer
{
private:
	explicit CTemp_Cube(LPDIRECT3DDEVICE9 pGrahpicDev);
	explicit CTemp_Cube(CTemp_Cube& rhs);
	virtual ~CTemp_Cube() = default;


/*FUNCTION*/
public:
	virtual HRESULT Ready_Buffer();
	void	Render_Buffer();

/*Related ctor dtor*/
public:
	static CTemp_Cube* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual CComponent* Clone();
	virtual _ulong Free();
};

END
/*
LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
_uint m_iVertexSize		= 0;
_uint m_iVertexCount	= 0;
_ulong m_dwVertexFVF	= 0;

LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
_uint		m_iIndexSize = 0;
_uint		m_iTriCount = 0;
D3DFORMAT	m_IndexFmt = D3DFMT_UNKNOWN;
*/
