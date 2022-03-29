#pragma once

#include "defines.h"
#include "gameObject.h"

namespace Engine
{
	class CAABB_Buffer;
	class CBillboard_Buffer;
	class CParticle_Effect;
}
class CCollider_Manager;

class CCharacter abstract 
	: public CGameObject
{
public:
	enum CHARWAY { NONE, LIVING1, LIVING2, LIVING3, LIVING4, LIVING5, ROOM6, ROOM7, OUTSIDE, WAY_END };

protected:
	explicit CCharacter(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCharacter() = default;


/*FUNCTION*/
public:
	CTransform*	Get_Transform() { return m_pTransformCom; }
	CVIBuffer* Get_ColliderBox_AABB();
	//_vec3* Get_ColliderBoxPos_AABB();
	CHAR_TYPE Get_CharType() { return m_eCharType; }


public:
	virtual HRESULT Ready_GameObject() PURE;
	virtual _int	Update_GameObject(const _float& fTimeDelta) PURE;
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject() PURE;
	virtual	_bool	Hit(_uint iDamage, const _float& fLifeTime);


protected:
	HRESULT Ready_Character_Component();	/*Ready Collide*/
	
	HRESULT	Load_Character_Model(const _tchar* pFilePath);
	HRESULT	Load_Character_Animate(const _tchar* pFilePath);

	void	Update_Frame(const _float& fTimeDelta);
	void	Update_Animate(const _float& fTimeDelta);
	
	virtual _bool	Compare_AnimateType() { return false; }

/*MEMBER*/
protected:
	_int					m_iHp = 0;
	//bool					m_bIsDead = false;
	CHAR_TYPE				m_eCharType;
	CTransform*				m_pTransformCom = nullptr; /*Ref Bone[0] : do not release*/
	CAABB_Buffer*			m_pColliderBox_AABB = nullptr;
	CBillboard_Buffer*		m_pColliderBox_Billboard = nullptr;
	CCollider_Manager*		m_pColliderMgr = nullptr;
	CParticle_Effect*		m_pEffectParticle = nullptr;
	CParticle_Renderer*		m_pParticleRenderer = nullptr;

	
protected:
	/*Model*/
	vector<CBone*>			m_vecBone;
	CCube_Color*			m_pBoneCubeBuffer = nullptr;

	vector<BODYINFO*>		m_vecBodyInfo;
	CCube_Texture*			m_pBodyCubeBuffer = nullptr;



	vector<vector<_float*>>	m_vvKeyFrameTime;

	/*frame*/
	_float	m_fFrameTimer = 0.f;
	_ulong	m_dwFrame = 0;

	_uint	m_iPreStateIndex = 0;
	_uint	m_iCurStateIndex = 0;
	_bool	m_bSpecialState = true;
	_bool	m_bDirectly_ChangeState = false;

	_float	m_fSoundLack = 0.f;

public:
	void Set_CharWay(CHARWAY eWay) { eCharWay = eWay; }
	CHARWAY Get_CharWay()		   { return eCharWay; }

public:
	CHARWAY eCharWay = OUTSIDE;
protected:
	virtual _ulong	Free() override;
};

