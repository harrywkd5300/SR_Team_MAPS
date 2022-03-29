#include "stdafx.h"
#include "..\Headers\Wave_Manager.h"
#include "gameObject.h"

#include "baseMonster.h"
#include "creeper.h"
#include "ghost.h"
#include "boss.h"

#include "input_device.h"
#include "Graphic_Device.h"
#include "camera_manager.h"
#include "camera_player.h"
#include "layer.h"

#include "Light_Manager.h"
#include "LightEffectObj.h"
#include "Sound_Manager.h"

IMPLEMENT_SINGLETON(CWave_Manager)

CWave_Manager::CWave_Manager()
{
}

HRESULT CWave_Manager::Ready_Wave_Manager(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	pGraphicDev->AddRef();

	m_pObject_Manager = CObject_Manager::GetInstance();
	m_pObject_Manager->AddRef();


	m_pWELL512 = CRandomWell512::GetInstance();
	m_pWELL512->AddRef();

	m_pInput_Device = CInput_Device::GetInstance();
	m_pInput_Device->AddRef();

	if (FAILED(Ready_D3dFont()))
		return E_FAIL;

	for (_int i = 0; i < 256; ++i)
		m_bWave[i] = true;

	return NOERROR;
}

_int CWave_Manager::Update_Wave_Manager(const _float & fTimeDelta)
{
	if (m_pGraphicDev == nullptr)
		return -1;

	Update_Input(fTimeDelta);

	if (m_bStart)
		Update_Wave1(fTimeDelta);
	
	if (m_bStart2)
		Update_Wave2(fTimeDelta);
	
	if (m_bStart3)
		Update_Wave3(fTimeDelta);


	return 0;
}

void CWave_Manager::Render_Wave_Manager()
{
}

HRESULT CWave_Manager::Set_BossClearLight()
{

		CGameObject* pGameObject = nullptr;

		//4-5번방 오른쪽벽
		D3DXCOLOR Organge(248.f / 255.f, 188.f / 255.f, 80.f / 255.f, 1.f);
		D3DXCOLOR WhiteColor(1.f, 1.f, 1.f, 1.f);


		//4-5번방 가운데
		pGameObject = CLightEffectObj::Create(m_pGraphicDev);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
			return E_FAIL;
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
		//dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 4.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(WhiteColor, 10.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(62.f, 1.5f, 70.5f));


		////6번방 입구
		//pGameObject = CLightEffectObj::Create(m_pGraphicDev);
		//if (nullptr == pGameObject)
		//	return E_FAIL;
		//if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
		//	return E_FAIL;
		//dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
		//dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(WhiteColor, 7.f);
		//dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(72.f, 2.5f, 79.5f));


		// bossR
		pGameObject = CLightEffectObj::Create(m_pGraphicDev);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
			return E_FAIL;
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 10.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(75.f, 15.5f, 7.5f));

		// bossL
		pGameObject = CLightEffectObj::Create(m_pGraphicDev);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
			return E_FAIL;
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 10.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(53.f, 15.5f, 7.5f));


		// --------------------------------------------------------------------------------------------------
		// bossR
		pGameObject = CLightEffectObj::Create(m_pGraphicDev);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
			return E_FAIL;
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 3.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(75.f, 2.0f, 30.5f));
		// bossL
		pGameObject = CLightEffectObj::Create(m_pGraphicDev);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
			return E_FAIL;
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 3.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(53.f, 2.0f, 30.5f));

		// bossR
		pGameObject = CLightEffectObj::Create(m_pGraphicDev);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
			return E_FAIL;
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 3.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(75.f, 2.0f, 36.5f));
		// bossL
		pGameObject = CLightEffectObj::Create(m_pGraphicDev);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
			return E_FAIL;
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 3.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(53.f, 2.0f, 36.5f));

		// bossR
		pGameObject = CLightEffectObj::Create(m_pGraphicDev);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
			return E_FAIL;
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 3.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(75.f, 1.0f, 42.5f));
		// bossL
		pGameObject = CLightEffectObj::Create(m_pGraphicDev);
		if (nullptr == pGameObject)
			return E_FAIL;
		if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Light", pGameObject)))
			return E_FAIL;
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_Texture(L"Component_Texture_Effect_FlameB", 30.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Set_LightColor(Organge, 3.f);
		dynamic_cast<CLightEffectObj*>(pGameObject)->Get_Transform()->Set_Position(_vec3(53.f, 1.0f, 42.5f));



}

HRESULT CWave_Manager::Ready_D3dFont()
{
	m_pD3dFont = CGraphic_Device::GetInstance()->Get_Font();

	if (nullptr == m_pD3dFont)
	{
		MSG_BOX("Ready D3dFont Failed");
		return E_FAIL;
	}
	m_pD3dFont->AddRef();

	return NOERROR;

}

void CWave_Manager::Update_Input(const _float & fTimeDelta)
{
	if (m_pInput_Device->Get_DIKeyState(DIK_P) & 0x80)
	{
		m_bStart = true;
		if (dynamic_cast<CCamera_Player*>(CCamera_Manager::GetInstance()->Get_CurCamera()))
			dynamic_cast<CCamera_Player*>(CCamera_Manager::GetInstance()->Get_CurCamera())->Set_ExclusiveEvent(CCamera_Player::EXCLUSIVEEVENT_STARTGAME);
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_O) & 0x80 && bTest)
	{
		Add_Monster(MONSTER_BOSS, SPAWN_BOSS);
		CSound_Manager::GetInstance()->StopSound(CSound_Manager::Channel_BGM);
		CSound_Manager::GetInstance()->PlayBGM(L"Dead By Daylight-No Place To Hide.mp3", CSound_Manager::Channel_BGM, 0.8f);
		bTest = false;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_5) & 0x80 )
	{
		m_bStart2 = true;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_6) & 0x80 )
	{
		m_bStart3 = true;
	}
}

void CWave_Manager::Update_Wave1(const _float & fTimeDelta)
{
	m_fTimer += fTimeDelta;


	if (m_fTimer > 1.0f && m_bWave[0] == true)
	{
		m_bWave[0] = false;

		for (size_t i = 0; i < 2; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);

		for (size_t i = 0; i < 2; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);

	}

	if (m_fTimer > 4.f && m_bWave[1] == true)
	{
		m_bWave[1] = false;

		for (size_t i = 0; i < 2; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);

		for (size_t i = 0; i < 2; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);
	}

	if (m_fTimer > 7.f && m_bWave[2] == true)
	{
		m_bWave[2] = false;

		for (size_t i = 0; i < 2; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);

		for (size_t i = 0; i < 2; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);
	}

	if (m_fTimer > 13.f && m_bWave[3] == true)
	{
		m_bWave[3] = false;

		for(size_t i=0; i< 2; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);

		for (size_t i = 0; i < 2; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);


	}
	
	if (m_fTimer > 15.f && m_bWave[4] == true)
	{
		m_bWave[4] = false;

		for (size_t i = 0; i < 2; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);

		for (size_t i = 0; i < 2; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);

		m_bStageEnd = true;
	}

	if (nullptr != m_pMonsterLayer && m_bStageEnd ==true)
	{
		m_pMonsterLayer = CObject_Manager::GetInstance()->Get_Layer(SCENE_STAGE, L"Layer_Monster");
		const list<CGameObject*>	MonsterList = m_pMonsterLayer->Get_ObjList();
		if (MonsterList.size() == 0)
		{
			m_bStart2 = true;
			m_bStart = false;
			m_bStageEnd = false;
			m_fTimer = 0.f;

			for (size_t i = 0; i < 256; ++i)
				m_bWave[i] = true;
		}
	}




}

void CWave_Manager::Update_Wave2(const _float & fTimeDelta)
{
	m_fTimer += fTimeDelta;

	if (m_fTimer > 1.0f && m_bWave[0] == true)
	{
		m_bWave[0] = false;

		for (size_t i = 0; i < 2; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);

		for (size_t i = 0; i < 2; ++i)
		{ 	
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);
			Add_Monster(MONSTER_CREEPER, SPAWN_2);
		}
	}

	if (m_fTimer > 4.f && m_bWave[1] == true)
	{
		m_bWave[1] = false;

		for (size_t i = 0; i < 2; ++i)
		{
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);
			Add_Monster(MONSTER_CREEPER, SPAWN_1);
		}

		for (size_t i = 0; i < 2; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);
	}

	if (m_fTimer > 7.f && m_bWave[2] == true)
	{
		m_bWave[2] = false;

		for (size_t i = 0; i < 3; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);

		for (size_t i = 0; i < 3; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);
	}

	if (m_fTimer > 13.f && m_bWave[3] == true)
	{
		m_bWave[3] = false;

		for (size_t i = 0; i < 3; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);

		for (size_t i = 0; i < 3; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);


	}

	if (m_fTimer > 15.f && m_bWave[4] == true)
	{
		m_bWave[4] = false;

		for (size_t i = 0; i < 3; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);

		for (size_t i = 0; i < 3; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);

		m_bStageEnd = true;
	}


	if (nullptr != m_pMonsterLayer && m_bStageEnd == true)
	{
		m_pMonsterLayer = CObject_Manager::GetInstance()->Get_Layer(SCENE_STAGE, L"Layer_Monster");
		const list<CGameObject*>	MonsterList = m_pMonsterLayer->Get_ObjList();
		
		if (MonsterList.size() == 0)
		{
			m_bStart3 = true;
			m_bStart2 = false;
			m_fTimer = 0.f;
			m_bStageEnd = false;


			for (size_t i = 0; i < 256; ++i)
				m_bWave[i] = true;
		}
	}


}

void CWave_Manager::Update_Wave3(const _float & fTimeDelta)
{
	m_fTimer += fTimeDelta;

	if (m_fTimer > 1.0f && m_bWave[0] == true)
	{
		m_bWave[0] = false;

		for (size_t i = 0; i < 2; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);

		for (size_t i = 0; i < 2; ++i)
		{
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);
			Add_Monster(MONSTER_CREEPER, SPAWN_2);
		}
		Add_Monster(MONSTER_GHOST, SPAWN_2);
	}

	if (m_fTimer > 4.f && m_bWave[1] == true)
	{
		m_bWave[1] = false;

		for (size_t i = 0; i < 2; ++i)
		{
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);
			Add_Monster(MONSTER_CREEPER, SPAWN_1);
		}

		for (size_t i = 0; i < 2; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);
		Add_Monster(MONSTER_GHOST, SPAWN_1);
	}

	if (m_fTimer > 7.f && m_bWave[2] == true)
	{
		m_bWave[2] = false;

		for (size_t i = 0; i < 3; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);

		for (size_t i = 0; i < 3; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);
	}

	if (m_fTimer > 13.f && m_bWave[3] == true)
	{
		m_bWave[3] = false;

		for (size_t i = 0; i < 3; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);

		for (size_t i = 0; i < 3; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);


	}

	if (m_fTimer > 15.f && m_bWave[4] == true)
	{
		m_bWave[4] = false;

		for (size_t i = 0; i < 3; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_1);

		for (size_t i = 0; i < 3; ++i)
			Add_Monster(MONSTER_ZOMBIE, SPAWN_2);

		m_bStageEnd = true;
	}
}

HRESULT CWave_Manager::Add_Monster(MONSTERTYPE eMonsterType, SPAWNPOINT eSpawnPoint)
{
	CGameObject* pGameObject = nullptr;


	_float fx = 0.f;
	_float fz = 0.f;
	_vec3 vSpawn;
	ZeroMemory(&vSpawn, sizeof(_vec3));


	//1: right bottom vSpawnPosition = _vec3(98.5f, 0.5f, 49.5f);
	//2: vSpawnPosition = _vec3(40.5f, 0.5f, 99.5f);

	switch (eSpawnPoint)
	{
	case SPAWN_1:
		fx = m_pWELL512->GetValueF(98.f, 113.f);
		fz = m_pWELL512->GetValueF(55.f, 80.f);

		vSpawn = _vec3(fx, 0.f, fz);
		break;

	case SPAWN_2:
		fx = m_pWELL512->GetValueF(48.f, 72.f);
		fz = m_pWELL512->GetValueF(100.f, 115.f);
		
		vSpawn = _vec3(fx, 0.f, fz);
		break;

	case SPAWN_BOSS:
		vSpawn = _vec3(64.f, 0.f, 0.f);

		break;
	}

	switch (eMonsterType)
	{
	case MONSTER_ZOMBIE:
		pGameObject = CBaseMonster::Create(m_pGraphicDev, vSpawn);
		break;
	case MONSTER_CREEPER:
		pGameObject = CCreeper::Create(m_pGraphicDev, &vSpawn);
		break;
	case MONSTER_GHOST:
		pGameObject = CGhost::Create(m_pGraphicDev, &vSpawn);
		break;
	case MONSTER_BOSS:
		pGameObject = CBoss::Create(m_pGraphicDev, &vSpawn);
		break;
	}

	if (nullptr == pGameObject)
		goto exception;

	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_STAGE, L"Layer_Monster", pGameObject)))
		goto exception;

	m_pMonsterLayer = m_pObject_Manager->Get_Layer(SCENE_STAGE, L"Layer_Monster");
	

	return NOERROR;

exception:
	MSG_BOX("Add Monster Failed");
	return E_FAIL;
}

_ulong CWave_Manager::Free()
{
	/*decrease refcount*/
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pWELL512);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pD3dFont);

	return 0;
}
