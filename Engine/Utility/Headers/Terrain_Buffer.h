#pragma once

#include "vibuffer.h"


BEGIN(Engine)

class DLL_EXPORT CTerrain_Buffer final
	:public CVIBuffer
{
/*ctor dtor*/
private:
	explicit CTerrain_Buffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrain_Buffer(const CTerrain_Buffer& rhs);
	virtual ~CTerrain_Buffer() = default;

/*FUNCTION*/
public:
	virtual HRESULT Ready_Buffer(const _tchar* pHeightMapFilePath, const _float& fVtxInterval);


private:
	void CalcTangentVector(VTXTERTEX& pVertex1, VTXTERTEX& pVertex2, VTXTERTEX& pVertex3, _vec3 vNormal);


/*MEMBER*/

/* related ctor dtor */
public:
	static CTerrain_Buffer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pHeightMapFilePath, const _float& fVtxInterval = 1);

private:
	virtual CComponent* Clone() final;
	virtual _ulong	Free() final;
};

END