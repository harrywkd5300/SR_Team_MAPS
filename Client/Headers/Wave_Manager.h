#pragma once

#include "base.h"
#include "defines.h"
#include "engine_defines.h"
#include "Object_Manager.h"
#include "well512a.h"

namespace Engine
{
	class CLayer;
	class CInput_Device;
	class CD3DFont;
}

class CWave_Manager final
	:public CBase
{

	DECLARE_SINGLETON(CWave_Manager)

public:
	enum WAVE{WAVE_NONE, WAVE_1, WAVE_2, WAVE_3, WAVE_END};
	enum SPAWNPOINT{SPAWN_1,SPAWN_2,SPAWN_BOSS};
	enum MONSTERTYPE{MONSTER_ZOMBIE,MONSTER_CREEPER,MONSTER_GHOST,MONSTER_BOSS};

/*ctor dtor*/
private:
	explicit CWave_Manager();
	virtual ~CWave_Manager() = default;

/*FUNCTION*/
public:
	HRESULT	Add_Monster(MONSTERTYPE eMonsterType, SPAWNPOINT eSpawnPoint);

public:
	HRESULT	Ready_Wave_Manager(LPDIRECT3DDEVICE9 pGraphicDev);
	_int	Update_Wave_Manager(const _float& fTimeDelta);
	void	Render_Wave_Manager();


	HRESULT Set_BossClearLight();

private:
	HRESULT Ready_D3dFont();
	void	Update_Input(const _float& fTimeDelta);
	void	Update_Wave1(const _float& fTimeDelta);
	void	Update_Wave2(const _float& fTimeDelta);
	void	Update_Wave3(const _float& fTimeDelta);


/*MEMBER*/
private:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
	CObject_Manager*	m_pObject_Manager = nullptr;
	CRandomWell512*		m_pWELL512	= nullptr;
	CInput_Device*		m_pInput_Device = nullptr;
	CLayer*				m_pMonsterLayer = nullptr;
	CD3DFont*			m_pD3dFont = nullptr;
	
	_uint				m_iMonsterCount = 0;
	_float				m_fTimer = 0.f;
	
	_bool				m_bStart = false;
	_bool				m_bStart2 = false;
	_bool				m_bStart3 = false;
	_bool				m_bStart4 = false;
	_bool				m_bStageEnd = false;

	_bool				m_bWave[256];
	_bool			bTest = true;
private:
	virtual _ulong Free() final;

};

