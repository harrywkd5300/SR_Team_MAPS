#pragma once

#include "vibuffer.h"

BEGIN(Engine)

class DLL_EXPORT CJang_Cube
	:public CVIBuffer
{
private:
	explicit CJang_Cube(LPDIRECT3DDEVICE9 pGrahpicDev);
	explicit CJang_Cube(CJang_Cube& rhs);
	virtual ~CJang_Cube() = default;


/*FUNCTION*/
public:
	virtual HRESULT Ready_Buffer(const _float& fSizeX, const _float& fSizeY, const _float& fSizeZ);
	void	Render_Buffer();

private:
	_vec3	m_vSize;

/*Related ctor dtor*/
public:
	static CJang_Cube* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fSizeX, const _float& fSizeY, const _float& fSizeZ);
	void CalcTangentVector(VTXTEXCUBE_J& pVertex1, VTXTEXCUBE_J& pVertex2, VTXTEXCUBE_J& pVertex3);
	void CalcTangentVector(VTXTEXCUBE_J& pVertex1, VTXTEXCUBE_J& pVertex2, VTXTEXCUBE_J& pVertex3, _vec3 vNormal);

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
