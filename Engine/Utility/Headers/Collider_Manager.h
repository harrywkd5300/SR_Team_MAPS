#pragma once

#include "base.h"
#include "Engine_Defines.h"
#include "AABB_Buffer.h"
#include "Billboard_Buffer.h"

class CCharacter;
class CGameObject;

BEGIN(Engine)

class DLL_EXPORT CCollider_Manager final
	:public CBase
{
	DECLARE_SINGLETON(CCollider_Manager)

private:
	explicit CCollider_Manager();
	virtual ~CCollider_Manager() = default;


	/*FUNCTION*/
public:
	HRESULT								Ready_CCollider_Manager(LPDIRECT3DDEVICE9 pGraphicDevice);
	void								Add_ColliderBox(CHAR_TYPE eCharType, COLLBOX_TYPE eColboxType, _float fSizeX, _float fSizeY, CCharacter* pTargetChar);
	//const _vec3*						CheckCollRay(HWND hWnd, _vec3* pOut, _uint iBackSizeX, _uint iBackSizeY, Engine::CVIBuffer * pTargetBuffer, const _matrix* pWorldMatrix = nullptr);
	CCharacter*							CheckCollBullet(HWND hWnd, CHAR_TYPE eDestCharType/*맞을캐릭터타입*/,_vec3 muzzleWorldPos/* 월드총구위치*/);

	//HRESULT		Ready_Particle_Renderer(LPDIRECT3DDEVICE9 pGraphicDevice);
	//CParticle*	Get_Collider(PARTICLETYPE eType, const _tchar* TextureTag);
	//HRESULT		Set_Particle_Type(PARTICLETYPE eType, const _tchar* TextureTag, CTexture* pTexture);
	//void		Ready_Render();
	//void		Start_Render();
	//void		End_Render();
	//void		Update_Particle(float fTimeDelta);

	/*MEMBER*/
private:
	list<CVIBuffer*>					m_ColliderBoxList[CHAR_TYPE::CHAR_TYPE_END];
	typedef list<CVIBuffer*>			COLLBOXLIST;

	list<CCharacter*>					m_CharacterList[CHAR_TYPE::CHAR_TYPE_END];
	typedef list<CCharacter*>			CHARACTERLIST;
	//map<const _tchar*, CTexture*>						m_ParticleTextureMap[PARTICLE_END];
	//map<const _tchar*, vector<CParticle*> >				m_ParticleMap[PARTICLE_END];
	//typedef map<const _tchar*, CTexture*>				PTCTEXMAP;
	//typedef map<const _tchar*, vector<CParticle*> >		PTCMAP;

	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;
	//LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;

	//_uint						iParticleTypeSize = VTXPTC_MAX_SIZE / PARTICLE_MAX_SIZE;
	//_uint						_vbOffset = 0;
	//_uint						_vbBatchSize = PARTICLE_MAX_SIZE;
	//_uint						iParticleTypeCount = 0;

private:
	virtual _ulong Free() final;
};

END