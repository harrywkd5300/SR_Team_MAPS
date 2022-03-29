#pragma once

#include "base.h"
#include "Engine_Defines.h"
#include "Effect.h"
#include "Effect_Bullet.h"
#include "Effect_Fixed.h"

BEGIN(Engine)

class CTexture;

class DLL_EXPORT CEffect_Manager final
	:public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)
public:
	enum EFFECT_TYPE { EFFECT_DIRECT_BULLET,EFFECT_FIXED, EFFECT_END };
	enum { EFFECT_MAX_SIZE = 50, VTXEFFECT_MAX_SIZE = 1000 };
private:
	explicit CEffect_Manager();
	virtual ~CEffect_Manager() = default;


	/*FUNCTION*/
public:

	HRESULT		Ready_Effect_Renderer(LPDIRECT3DDEVICE9 pGraphicDevice);
	bool		Set_Effect(EFFECT_TYPE eType, const _tchar* TextureTag, CEffect* pEffect);
	//bool		Set_Effect_Address(EFFECT_TYPE eType, const _tchar* TextureTag, CEffect* pEffect);
	bool		Set_Effect_Output(EFFECT_TYPE eType, const _tchar* TextureTag, CEffect* pEffect, CEffect* pOutEffect);
	CEffect*	Get_Effect(EFFECT_TYPE eType, const _tchar* TextureTag);
	HRESULT		Set_Effect_Type(EFFECT_TYPE eType, const _tchar* TextureTag, CTexture* pTexture);
	void		Ready_Render();
	void		Start_Render();
	void		End_Render();
	void		Update_Effect(float fTimeDelta);

	/*MEMBER*/
private:

	map<const _tchar*, CTexture*>						m_EffectTextureMap[EFFECT_END];
	map<const _tchar*, vector<CEffect*> >				m_EffectMap[EFFECT_END];
	typedef map<const _tchar*, CTexture*>				EFFECTTEXMAP;
	typedef map<const _tchar*, vector<CEffect*> >		EFFECTMAP;

	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;

	_uint						iEffectTypeSize = VTXEFFECT_MAX_SIZE / EFFECT_MAX_SIZE;
	_uint						iEffectTypeCount = 0;

public:

private:
	virtual _ulong Free() final;
};

END
