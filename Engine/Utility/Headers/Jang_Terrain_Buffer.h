#pragma once

#include "vibuffer.h"


BEGIN(Engine)

class DLL_EXPORT Jang_CTerrain_Buffer final
	:public CVIBuffer
{
/*ctor dtor*/
private:
	explicit Jang_CTerrain_Buffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Jang_CTerrain_Buffer(const Jang_CTerrain_Buffer& rhs);
	virtual ~Jang_CTerrain_Buffer() = default;

/*FUNCTION*/
public:
	virtual HRESULT Ready_Buffer(const _tchar* pHeightMapFilePath, const _float& fVtxInterval);

/*MEMBER*/

/* related ctor dtor */
public:
	static Jang_CTerrain_Buffer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pHeightMapFilePath, const _float& fVtxInterval = 1);

private:
	virtual CComponent* Clone() final;
	virtual _ulong	Free() final;
	//--------------- Test ------------------
public:
	//void CalcTangentVector(_vec3* pos1, _vec3* pos2, _vec3* pos3, D3DXVECTOR2* texCoord1, D3DXVECTOR2* texCoord2, D3DXVECTOR2* texCoord3, _vec3* normal, D3DXVECTOR4& tangent);
	void CalcTangentVector(VTXTEX_J2* pVertex, _int BufferSize);
	void CalcTangentVector(VTXTEX_J2& pVertex1, VTXTEX_J2& pVertex2, VTXTEX_J2& pVertex3);
	void CalcTangentVector(VTXTEX_J2& pVertex1, VTXTEX_J2& pVertex2, VTXTEX_J2& pVertex3, _vec3 vNormal);

};


END