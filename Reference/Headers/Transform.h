#pragma once

#include "Engine_Defines.h"
#include "Component.h"
#include "vibuffer.h"

BEGIN(Engine)

class DLL_EXPORT CTransform
	:public CComponent
{
public:
	enum ROW_INFO{INFO_RIGHT,INFO_UP,INFO_LOOK,INFO_POSITION,INFO_END};

/*ctor dtor*/
protected:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;


/*FUNCTION*/
public:
	const _vec3&	Get_MatrixRow(ROW_INFO eInfo);
	const _matrix&	Get_WorldMatrix() { return m_matWorld; }
	const _matrix&	Get_ScaleMatrix() { return m_matScale; };


	const _float&	Get_DegreeX() { return m_fDegreeX; }
	const _float&	Get_DegreeY() { return m_fDegreeY; }
	const _float&	Get_DegreeZ() { return m_fDegreeZ; }

	const _float&	Get_PositionX() { return m_matTrans._41; }
	const _float&	Get_PositionY() { return m_matTrans._42; }
	const _float&	Get_PositionZ() { return m_matTrans._43; }
	_vec3	Get_Position() { return _vec3(m_matTrans._41, m_matTrans._42, m_matTrans._43); }

	void	Set_DegreeX(const _float& fx) { m_fDegreeX = fx; Rotation_X(0); }
	void	Set_DegreeY(const _float& fy) { m_fDegreeY = fy; Rotation_Y(0); }
	void	Set_DegreeZ(const _float& fz) { m_fDegreeZ = fz; Rotation_Z(0); }
	void	Set_Degree(const _vec3& vRot);

	void	Set_PositionX(const _float& fX) { m_matTrans._41 = fX; }
	void	Set_PositionY(const _float& fY) { m_matTrans._42 = fY; }
	void	Set_PositionZ(const _float& fZ) { m_matTrans._43 = fZ; }
	void	Set_Position(const _vec3& vPos) { (_vec3&)m_matTrans.m[INFO_POSITION] = vPos; }

	void	Set_MatrixRow(const _vec3& v, ROW_INFO eInfo) const {(_vec3&)m_matWorld.m[eInfo] = v;}
	void	Set_Look(const _vec3& vLook);

public:
	HRESULT Ready_Transform();


	void	Fill_WorldMatrix();
	HRESULT SetUp_MatrixToGraphicDev();
	
	void Scaling(const _vec3& vScale);
	void Scaling(const _float& x, const _float& y, const _float& z);

	void Rotation_X(const _float& fDegree);
	void Rotation_Y(const _float& fDegree);
	void Rotation_Z(const _float& fDegree);

	void Translation(const _vec3& vTranslation);
	void Translation(const _float& x, const _float& y, const _float& z);

	void Parent(const _matrix& matParent);

	void Move_ToDirection(const _vec3& vDirection, const _float& fSpeed, const _float& fTimeDelta);
	void Move_ToLook(const _float& fSpeed, const _float& fTimeDelta); //y값 고려안함
	void Move_ToRight(const _float& fSpeed, const _float& fTimeDelta);
	void Move_ToLeft(const _float& fSpeed, const _float& fTimeDelta);


	_vec3 Get_IFMoveToLookVec(const _float& fSpeed, const _float& fTimeDelta); // 만약에 이동 한다면 이동될 위치 반환
	_vec3 Get_IFMoveToRightVec(const _float& fSpeed, const _float& fTimeDelta); // 만약에 이동 한다면 이동될 위치 반환
	_vec3 Get_IFMoveToLeftVec(const _float& fSpeed, const _float& fTimeDelta); // 만약에 이동 한다면 이동될 위치 반환

	//void Move_ToLook_CheckColAABB(const _float& fSpeed, const _float& fTimeDelta);

	_float On_Terrain(CVIBuffer* pTargetBuffer);

	/*if m_bIsLookVec is true, will use LookVector instead Degree*/
	_bool	bIsCalcLook = false;

/*MEMBER*/
protected:
	_matrix m_matWorld;
	_matrix m_matScale;
	_matrix m_matRotationX;
	_matrix m_matRotationY;
	_matrix m_matRotationZ;
	_matrix m_matTrans;
	_matrix m_matParent;

	_float	m_fDegreeX =0.f;
	_float	m_fDegreeY =0.f;
	_float	m_fDegreeZ =0.f;
	_vec3	m_vLook;



/*function related ctor dtor*/
public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual CComponent* Clone() override;
	virtual _ulong Free() override;
};

END