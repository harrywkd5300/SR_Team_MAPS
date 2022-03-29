#pragma once
#include "Resources.h"

BEGIN(Engine)

class DLL_EXPORT CVIBuffer
	:public CResources
{
	/*ctor dtor*/
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;


	/*FUNCTION*/
public:
	const _vec3* Get_VertexPos() { return m_pVertexPos; }
	const _vec3* Get_ColliderBoxVertex() { return m_pColliderBoxVertex; }
	const _uint& Get_VertexCountX() { return m_iVertexCountX; }
	const _uint& Get_VertexCountZ() { return m_iVertexCountZ; }
	const _float& Get_VertexItv() { return m_fVertexItv; }
	const _uint& Get_IndexSize() { return m_iIndexSize; }
	virtual void Set_Size(const _float & fSizeX, const _float & fSizeY, const _float & fSizeZ);
public:
	virtual HRESULT	Ready_Buffer();
	void	Render_Buffer();

	/*MEMBER*/
protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	_uint						m_iVertexSize = 0;
	_uint						m_iVertexCount = 0;
	_ulong						m_dwVertexFVF = 0;

	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
	_uint						m_iIndexSize = 0;
	_uint						m_iTriCount = 0;
	D3DFORMAT					m_IndexFmt = D3DFMT_UNKNOWN;

	_vec3*	m_pVertexPos = nullptr;
	_vec3*	m_pColliderBoxVertex = nullptr;
	_uint	m_iVertexCountX = 0;
	_uint	m_iVertexCountZ = 0;
	_float	m_fVertexItv = 0.f;
	_vec3*	m_pMax = nullptr;
	_vec3*	m_pMin = nullptr;
	


protected:
	virtual _ulong Free() override;

public:
	// -------------- test ---------------
	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer() { return m_pVB; }
};

END