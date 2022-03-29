#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class CTexture;
class CLight;
class CLight_Manager;

class DLL_EXPORT CEffect
	:public CVIBuffer
{

protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect()=default;

/*FUNCTION*/
public:
	//HRESULT Ready_Particle();

	virtual void Reset_Effect(const EFTINFO& attribute) = 0;
	virtual void Update_Effect(const _float& fTimeDelta) = 0;
	virtual void Clone_Effect(CEffect* pCopyEffect);
	void Set_Light(D3DXCOLOR color, float fRange);
	const EFTINFO& Get_Info() { return m_tInfo; }
	void Set_Info(const EFTINFO& Info) { m_tInfo = Info; }
	inline bool IsAlive() { return m_tInfo._isAlive; }
	void SetIsDead() { m_tInfo._isAlive = false;  }
	void Reset_Info() { ZeroMemory(&m_tInfo, sizeof(EFTINFO)); }

	CLight*	Get_Light() { return m_pLight; }
	CLight_Manager* Get_LightManager(){ return m_pLightMgr; }
/*MEMBER*/
protected:
	EFTINFO					m_tInfo;
	CLight*					m_pLight = nullptr;
	CLight_Manager*			m_pLightMgr = nullptr;
/* related ctor&dtor */
public:
	virtual CComponent*	Clone() = 0;

protected:
	virtual _ulong Free();
};

END