#pragma once

#include "base.h"
#include "Engine_Defines.h"
#include "Particle.h"
#include "Particle_Bullet.h"
#include "Particle_Effect.h"

BEGIN(Engine)

class CTexture;

class DLL_EXPORT CParticle_Renderer final
	:public CBase
{
	DECLARE_SINGLETON(CParticle_Renderer)
public:
	enum PARTICLETYPE { PARTICLE_DIRECT_BULLET,PARTICLE_EFFECT, PARTICLE_END };
	enum { PARTICLE_MAX_SIZE = 300,VTXPTC_MAX_SIZE = 3000 };
private:
	explicit CParticle_Renderer();
	virtual ~CParticle_Renderer() = default;


	/*FUNCTION*/
public:

	//HRESULT Ready_Particle(_tchar* texFilePath);
	//HRESULT Add_Texture(_tchar* texFilePath, CTexture* TextureTag);

	//virtual void Particle_Reset();
	//virtual void Reset_Particle();
	//virtual void Add_Particle(PARTICLETYPE eType, CParticle* TextureTag);
	//void		Set_ParticleTex(CTexture* pTexture);
	//void		Add_Particle(PARTICLETYPE eType, const _tchar* ParticleTag);
	HRESULT		Ready_Particle_Renderer(LPDIRECT3DDEVICE9 pGraphicDevice);
	bool		Set_Particle(PARTICLETYPE eType, const _tchar* TextureTag, CParticle* pParticle);
	CParticle*	Get_Particle(PARTICLETYPE eType, const _tchar* TextureTag);
	HRESULT		Set_Particle_Type(PARTICLETYPE eType, const _tchar* TextureTag, CTexture* pTexture);
	void		Ready_Render(_float fParticleSize);
	void		Start_Render();
	void		End_Render();
	void		Update_Particle(float fTimeDelta);
	DWORD		FtoDw(_float f){return *((DWORD*)&f);}
	/*MEMBER*/
private:
	//map<const _tchar*, list<CParticle>*> m_ParticleTextureMap;
	//map<const _tchar*, list<CParticle>*> m_ParticleMap;
	
	map<const _tchar*, CTexture*>						m_ParticleTextureMap[PARTICLE_END];
	map<const _tchar*, vector<CParticle*> >				m_ParticleMap[PARTICLE_END];
	typedef map<const _tchar*, CTexture*>				PTCTEXMAP;
	typedef map<const _tchar*, vector<CParticle*> >		PTCMAP;

	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;

	_uint						iParticleTypeSize = VTXPTC_MAX_SIZE / PARTICLE_MAX_SIZE;
	_uint						_vbOffset = 0;
	_uint						_vbBatchSize = PARTICLE_MAX_SIZE;
	_uint						iParticleTypeCount = 0;


	//map<const _tchar*, list<CParticle>*> m_ParticleMap;
	//typedef map<const _tchar*, list<CParticle>*> PARTICLEMAP;
	//typedef list<CParticle>		PARTICLELIST;
	//list<CParticle>				m_Particles[PARTICLE_END];


	//CTexture*					curTexture;
	//D3DXVECTOR3             _origin;
	//d3d::BoundingBox        _boundingBox;
	//float                   _emitRate;   // rate new particles are added to system
	//float                   _size;       // size of particles
	//IDirect3DTexture9*      _tex;

	//std::list<Attribute>    _particles;
	//int                     _maxParticles; // max allowed particles system can have

	//DWORD _vbSize;      // size of vb
	//DWORD _vbOffset;    // offset in vb to lock   
	//DWORD _vbBatchSize;



	/* related ctor&dtor */
public:
	//static	CParticle_Renderer*	Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	//static	CParticle_Renderer*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, _tchar* texFilePath);
	//virtual CComponent*	Clone() final;

private:
	virtual _ulong Free() final;
};

END

//#pragma once
//
//#include "Component.h"
//#include "Particle.h"
//#include "Particle_Bullet.h"
//BEGIN(Engine)
//
//class CTexture;
//
//class DLL_EXPORT CParticle_Renderer final
//	:public CComponent
//{
//public:
//	enum PARTICLETYPE {PARTICLE_DIRECT_BULLET, PARTICLE_END};
//	enum {PARTICLE_MAX_SIZE = 200,VTXPTC_MAX_SIZE = 2000};
//private:
//	explicit CParticle_Renderer(LPDIRECT3DDEVICE9 pGraphicDevice);
//	virtual ~CParticle_Renderer()=default;
//
//
///*FUNCTION*/
//public:
//	
//	//HRESULT Ready_Particle(_tchar* texFilePath);
//	//HRESULT Add_Texture(_tchar* texFilePath, CTexture* TextureTag);
//
//	//virtual void Particle_Reset();
//	//virtual void Reset_Particle();
//	//virtual void Add_Particle(PARTICLETYPE eType, CParticle* TextureTag);
//	//void		Set_ParticleTex(CTexture* pTexture);
//	//void		Add_Particle(PARTICLETYPE eType, const _tchar* ParticleTag);
//	HRESULT		Ready_Particle_Renderer();
//	bool		Set_Particle(PARTICLETYPE eType, const _tchar* TextureTag, CParticle* pParticle);
//	CParticle*	Get_Particle(PARTICLETYPE eType, const _tchar* TextureTag);
//	HRESULT		Set_Particle_Type(PARTICLETYPE eType, const _tchar* TextureTag, CTexture* pTexture);
//	void		Ready_Render();
//	void		Start_Render();
//	void		End_Render();
//	void		Update_Particle(float fTimeDelta);
//
///*MEMBER*/
//private:
//	//map<const _tchar*, list<CParticle>*> m_ParticleTextureMap;
//	//map<const _tchar*, list<CParticle>*> m_ParticleMap;
//	map<const _tchar*, CTexture*>						m_ParticleTextureMap[PARTICLE_END];
//	map<const _tchar*, vector<CParticle*> >				m_ParticleMap[PARTICLE_END];
//	typedef map<const _tchar*, CTexture*>				PTCTEXMAP;
//	typedef map<const _tchar*, vector<CParticle*> >		PTCMAP;
//
//	_uint						iParticleTypeSize = VTXPTC_MAX_SIZE / PARTICLE_MAX_SIZE;
//	_uint						_vbOffset = 0;
//	_uint						_vbBatchSize = PARTICLE_MAX_SIZE;
//	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
//	_uint						iParticleTypeCount = 0;
//
//	//map<const _tchar*, list<CParticle>*> m_ParticleMap;
//	//typedef map<const _tchar*, list<CParticle>*> PARTICLEMAP;
//	//typedef list<CParticle>		PARTICLELIST;
//	//list<CParticle>				m_Particles[PARTICLE_END];
//	
//
//	//CTexture*					curTexture;
//	//D3DXVECTOR3             _origin;
//	//d3d::BoundingBox        _boundingBox;
//	//float                   _emitRate;   // rate new particles are added to system
//	//float                   _size;       // size of particles
//	//IDirect3DTexture9*      _tex;
//
//	//std::list<Attribute>    _particles;
//	//int                     _maxParticles; // max allowed particles system can have
//
//	//DWORD _vbSize;      // size of vb
//	//DWORD _vbOffset;    // offset in vb to lock   
//	//DWORD _vbBatchSize;
//	
//
//
///* related ctor&dtor */
//public:
//	static	CParticle_Renderer*	Create(LPDIRECT3DDEVICE9 pGraphicDevice);
//	//static	CParticle_Renderer*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, _tchar* texFilePath);
//	virtual CComponent*	Clone() final;
//
//private:
//	virtual _ulong Free() final;
//};
//
//END