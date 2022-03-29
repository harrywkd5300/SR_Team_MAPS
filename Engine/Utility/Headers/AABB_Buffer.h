#pragma once

#include "vibuffer.h"
#include "engine_defines.h"

BEGIN(Engine)

class CGameObject;

class DLL_EXPORT CAABB_Buffer
	:public CVIBuffer
{
/*ctor dtor*/
private:
	explicit CAABB_Buffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAABB_Buffer(const CAABB_Buffer& rhs);
	virtual ~CAABB_Buffer() = default;

/*FUNCTION*/
public:
	HRESULT Ready_Buffer(const _float& fSizeX, const _float& fSizeY, const _float& fSizeZ);
	void SetTarget(CGameObject* pTarget);
	virtual void Set_Size(const _float & fSizeX, const _float & fSizeY, const _float & fSizeZ);
	void Set_Position(_vec3 vPos);
	void Set_Scale(_vec3 vScale);
	void Update_ColliderBox_VertexBuffer();
	void Render_Debug_Buffer();
	INDEX16*	Get_IndexBuffer();

	const _vec3& Get_Scale() { return m_vScale; }
	const _vec3& Get_Position() { return m_vPos; }
/*MEMBER*/
private:
	CGameObject*	m_pTarget = nullptr;
	_matrix			m_matWorld;
	_vec3			m_vPos;
	_vec3			m_vScale;
	INDEX16*		m_pIndex = nullptr;
	//_vec3			m_vScale;
	//_vec3			m_vMax;
	//_vec3			m_vMin;
	
/*FUNCTION related ctor dtor*/
public:
	static CAABB_Buffer* Create(LPDIRECT3DDEVICE9 pGraphicDev,const _float& fSizeX, const _float& fSizeY, const _float& fSizeZ);
	virtual CComponent* Clone();

private:
	virtual _ulong Free();
};

END