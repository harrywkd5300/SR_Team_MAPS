#pragma once

#include "Component.h"

BEGIN(Engine)

class CVIBuffer;
class DLL_EXPORT CPicking 
	: public CComponent
{
	/*ctor dtor*/
private:
	explicit CPicking(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPicking(const CPicking& rhs);
	virtual ~CPicking() = default;
private:
	HRESULT		Ready_Picking();
private:
	_vec3		m_iVertexPos;

public:
	enum PICKING_DIRECTION { DIR_RIGHT, DIR_LEFT, DIR_UP, DIR_DOWN, DIR_GO, DIR_BACK, DIR_END };
	PICKING_DIRECTION		eDir = DIR_END;
	/*FUNCTION*/
public:
	_vec3*		Picking_ToBuffer(HWND hWnd, _uint iBackSizeX, _uint iBackSizeY, Engine::CVIBuffer * pTargetBuffer, const _matrix* pWorldMatrix = nullptr);
	const _vec3*Picking_ToBuffer(HWND hWnd, _vec3*& pOut, _uint iBackSizeX, _uint iBackSizeY, Engine::CVIBuffer * pTargetBuffer, const _matrix* pWorldMatrix = nullptr);
	const _vec3*Picking_ForWeapon(HWND hWnd, _vec3*& pOut, _uint iBackSizeX, _uint iBackSizeY, Engine::CVIBuffer * pTargetBuffer, const _matrix* pWorldMatrix = nullptr);
	
	_vec3		Picking_ToMakeBuffer(HWND hWnd, _uint iBackSizeX, _uint iBackSizeY, Engine::CVIBuffer * pTargetBuffer, const _matrix* pWorldMatrix = nullptr, _float fMfCX = 0.f, _float fMfCY = 0.f);
	
	
	_bool		Picking_ToObject(HWND hWnd, Engine::CVIBuffer * pTargetBuffer, const _matrix* pWorldMatrix = nullptr, _float fMfCX = 0.f, _float fMfCY = 0.f);
	
	_vec3*		Get_PickingObjectPos() { return &m_iVertexPos; }

public:
	virtual CComponent* Clone(void);
	/*function related ctor dtor*/
public:
	static CPicking* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual _ulong Free();
};

END