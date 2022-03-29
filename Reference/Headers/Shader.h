#pragma once

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)

class DLL_EXPORT CShader final
	:public CComponent
{
public:
	enum ShaderTYPE {SDTYPE_NORMAL,SDTYPE_END};

/*ctor dtor*/
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
	virtual ~CShader() = default;


/*FUNCTION*/
public:
	HRESULT Ready_Shader(ShaderTYPE eType, const _tchar* pFilePath, _uint iSdCount);
	//HRESULT	SetUp_ShaderToGraphicDev(_uint dwStage, _uint iShaderIndex=0);
	HRESULT LoadShader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, LPD3DXEFFECT* ppShader);
	LPD3DXEFFECT GetShader(const _uint& iSdCount = 1);
/*MEMBER*/
private:
	//LPD3DXEFFECT m_Shader = nullptr;
	vector<LPD3DXEFFECT>			m_vecShader;
	typedef vector<LPD3DXEFFECT>	VECSHADER;


/*function related ctor dtor */
public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, ShaderTYPE eType, const _tchar* pFilePath, const _uint& iSdCount = 1);
	virtual	CComponent*	Clone() final;

private:
	virtual _ulong Free() final;

};

END