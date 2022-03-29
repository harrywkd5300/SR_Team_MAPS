#pragma once

#include "Engine_Defines.h"
#include "Resources.h"

BEGIN(Engine)

class DLL_EXPORT CTexture final
	:public CResources
{
public:
	enum TEXTURETYPE {TEXTYPE_NORMAL,TEXTYPE_CUBE,TYPE_END};

/*ctor dtor*/
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;


/*FUNCTION*/
public:
	HRESULT Ready_Texture(TEXTURETYPE eType, const _tchar* pFilePath, _uint iTexCount);
	HRESULT	SetUp_TextureToGraphicDev(_uint dwStage, _uint iTextureIndex=0);
	LPDIRECT3DBASETEXTURE9 GetTexture(const _uint& iTextureIndex = 1);

/*MEMBER*/
private:
	vector<LPDIRECT3DBASETEXTURE9>			m_vecTexture;
	typedef vector<LPDIRECT3DBASETEXTURE9>	VECTEXTURE;


/*function related ctor dtor */
public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTURETYPE eType, const _tchar* pFilePath, const _uint& iTexCount = 1);
	virtual	CComponent*	Clone() final;

private:
	virtual _ulong Free() final;

};

END