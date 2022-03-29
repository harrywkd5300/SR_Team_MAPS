#pragma once

#include "Defines.h"
#include "GameObject.h"
#include "Engine_Defines.h"
#include "Sound_Manager.h"

namespace Engine
{
	class CRect_Texture;
	class CRenderer;
	class CTexture;
	class CTransform;
	class CRandomWell512;
	class CParticle_Renderer;
	class CParticle_Effect;
	class CParticle_Bullet;
	class CEffect;
	class CEffect_Bullet;
	class CEffect_Fixed;
	class CEffect_Manager;
	class CPicking;
	class CTransform;
	
}

class CCharacter;
class CPlayer;
class CMonster;
class CCollider_Manager;

class CWeapon  : public Engine::CGameObject
{
public:
	enum PARTSNUMBER { PARTS_HANDLE, PARTS_MUZZLE };
	enum PLAYERBONENUMBER { PLAYERBONE_HAND = 12 };

protected:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWeapon() = default;

public:
	void Set_Equip(_bool b) { m_tInfo.bIsEquipped = b; };

public:
	virtual HRESULT Ready_GameObject(void) ;
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(void) ;

public:
	void	Set_User(CPlayer* pUser);
	void	Set_Monster(CMonster* pUser);
	void	Recoil(const _float& fTimeDelta, _vec3* vRecoverAngle); // 수평반동, 수직반동
	const WEAPONINFO& Get_WeaponInfo() { return m_tInfo; }
	virtual void	Use_Weapon(const _float fTimeDelta, _vec3* vRecoverAngle);
	virtual void	Use_Weapon_Monster(const _float fTimeDelta, const _vec3& vSourPos, const _vec3& vDestPos);
	virtual void	Reload_Weapon();

	_bool	Get_UpdateChange() { return m_bChangeUpdate; }
	
protected:
	void	Update_Bone(const _float& fTimeDelta);
	void	Render_Model();

protected:
	Engine::CRect_Texture*			m_pBufferCom = nullptr; 
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CPicking*				m_pPickingCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;

	Engine::CTransform*				m_pTransformCom = nullptr;
	CParticle_Renderer*				m_pParticleRenderer = nullptr;
	CParticle_Bullet*				m_pBulletParticle = nullptr;
	CParticle_Effect*				m_pMuzzleParticle = nullptr;
	CTexture*						m_pBulletTexture = nullptr;
	CTexture*						m_pMuzzleTexture = nullptr;
	_tchar*							m_pTextureTag = nullptr;
	_tchar*							m_pMuzzleTextureTag = nullptr;

	CEffect_Manager*				m_pEffectManager = nullptr;
	CEffect_Bullet*					m_pBulletEffect = nullptr;
	CEffect_Fixed*					m_pMuzzleEffect = nullptr;
	CTexture*						m_pBulletEffectTexture = nullptr;
	_tchar*							m_pBulletEffectTextureTag = nullptr;
	CTexture*						m_pMuzzleEffectTexture = nullptr;
	_tchar*							m_pMuzzleEffectTextureTag = nullptr;

	CCollider_Manager*				m_pColliderMgr = nullptr;

	CVIBuffer*						m_pTerrainBufferCom = nullptr;
	
	CRandomWell512*					m_pRandomMgr = nullptr;
	CCharacter*						m_pUser = nullptr;
	_bool							m_bChangeUpdate = false;
	// -------------- Shader ------------------------
public:
	void SetShader(_uint iShaderNum);
	void StartShader(_uint iShaderNum);
	void EndShader(_uint iShaderNum);
protected:
	CShader*			m_pShaderCom = nullptr;
	CMaterial*			m_pMatCom = nullptr;
	// -------------- Shader ------------------------

protected:
	WEAPONINFO		m_tInfo;
	//_float		m_fFireRate = 0.f;		//발사시간
	//_float		m_fReloadRate = 0.f;	//장전시간
	//_float		m_fXRecoilPower = 0.f;	//수평반동
	//_float		m_fYRecoilPower = 0.f;	//수직반동

	//_uint		m_iCurBullet = 0;		//현재총알
	//_uint		m_iMaxBullet = 0;		//최대총알
	//_uint		m_iDamage = 0;			//데미지



/*형태에 관련된 변수들*/
protected:
	vector<CBone*>					m_vecPartsTransform;
	vector<BODYINFO*>				m_vecBodyInfo;

	CTexture*						m_pBodyTexture = nullptr;
	CCube_Texture*					m_pCubeTexBuffer = nullptr;
	CTransform*						m_pBodyTransform = nullptr; /*ref bone[0]*/

	const vector<CBone*>*			m_pVecPlayerBone = nullptr;
protected:
	HRESULT Ready_Component(void);	
	HRESULT	Load_WeaponFile(const _tchar* pFilePath);

protected:
	virtual _ulong Free(void);

};

