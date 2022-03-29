#pragma once

#include "base.h"
#include "defines.h"
#include "AABB_Buffer.h"
#include "Billboard_Buffer.h"

namespace Engine
{
	class CGameObject;
}
class CCharacter;
class CColliderSkill;


class CCollider_Manager final
	:public CBase
{
	DECLARE_SINGLETON(CCollider_Manager)
private:
	enum { MAXVEC = 5, MINVEC = 3 };
private:
	explicit CCollider_Manager();
	virtual ~CCollider_Manager() = default;


	/*FUNCTION*/
public:
	HRESULT								Ready_CCollider_Manager(LPDIRECT3DDEVICE9 pGraphicDevice);
	//void								Add_ColliderBox(CHAR_TYPE eCharType, COLLBOX_TYPE eColboxType, _float fSizeX, _float fSizeY, CCharacter* pTargetChar);
	//const _vec3*						CheckCollRay(HWND hWnd, _vec3* pOut, _uint iBackSizeX, _uint iBackSizeY, Engine::CVIBuffer * pTargetBuffer, const _matrix* pWorldMatrix = nullptr);
	void								Add_Character(CCharacter* pTargetChar);
	void								Add_Target(CAABB_Buffer* pBuffer);
	void								Add_BackGround(CVIBuffer* pBuffer);
	void								Add_ColliderSkill(CColliderSkill* pColliderSkill);
	CCharacter*							CheckCollBullet(HWND hWnd, _vec3* pOut, CHAR_TYPE eSourCharType/*발사한캐릭터타입*/,_vec3 muzzleWorldPos/* 월드총구위치*/, _vec3 DestPos /*사격할좌표*/);
	void								Clear_CharacterList();
	void								Clear_TargetList();
	void								Clear_BackBufferList();
	void								Clear_SkillList();
	
	_int								Update_Manager(const _float& fTimeDelta);
	
	bool								CheckCollAABB(CVIBuffer* pSour, CVIBuffer* pDest);
	bool								CheckCollAABB_Push(_vec3* pDir, CVIBuffer* pSour, CVIBuffer* pDest);
	bool								CheckCollAABB_Slider(_vec3* pDir, CVIBuffer* pSour, CVIBuffer* pDest);
	bool								Process_AABB();
	bool								Process_AABB(_vec3* pOutDir, CCharacter* pChar, _vec3 pDestPos);
	bool								CheckCharaterColl(_vec3* pOutDir, CCharacter* pPlayer, CCharacter* pMonster);
	bool								Process_CharaterColl();
	bool								Process_Skill();
	bool								Remove_DeadSkill();

	inline _float						Max(_float Lf, _float Rf);
	inline _float						Min(_float Lf, _float Rf);

	/*MEMBER*/
private:
	list<CVIBuffer*>					m_BackBufferList;
	typedef list<CVIBuffer*>			BACKBUFLIST;

	list<CCharacter*>					m_CharacterList[CHAR_TYPE::CHAR_TYPE_END];
	typedef list<CCharacter*>			CHARACTERLIST;

	list<CAABB_Buffer*>					m_TargetBufferList;	//총기조준점타겟버퍼
	typedef list<CAABB_Buffer*>			TARGETBUFLIST;		//

	list<CColliderSkill*>				m_SkillList;
	typedef list<CColliderSkill*>			SKILLLIST;

	LPDIRECT3DDEVICE9				m_pGraphicDev = nullptr;

private:
	virtual _ulong Free() final;
};

