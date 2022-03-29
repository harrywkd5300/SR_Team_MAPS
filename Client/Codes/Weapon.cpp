#include "stdafx.h"
#include "..\Headers\Weapon.h"
#include "Component_Manager.h"
#include "Particle_Renderer.h"
#include "Effect_Manager.h"
//#include "Particle_Bullet.h"
#include "WELL512a.h"
#include "Player.h"
#include "Monster.h"
#include "Collider_Manager.h"
#include "camera_manager.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pPickingCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pBulletParticle(nullptr)
	, m_pParticleRenderer(CParticle_Renderer::GetInstance())
	, m_pBulletTexture(nullptr)
	, m_pTextureTag(nullptr)
	, m_pRandomMgr(CRandomWell512::GetInstance())
	, m_pCubeTexBuffer(nullptr)
	, m_pVecPlayerBone(nullptr)
	, m_pColliderMgr(CCollider_Manager::GetInstance())
	, m_pEffectManager(CEffect_Manager::GetInstance())
	, m_pShaderCom(nullptr)
	, m_pMatCom(nullptr)
{
	m_pRandomMgr->AddRef();
	m_pParticleRenderer->AddRef();
	m_pEffectManager->AddRef();
	m_pColliderMgr->AddRef();
	ZeroMemory(&m_tInfo, sizeof(WEAPONINFO));
	
}

HRESULT CWeapon::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;		

	return NOERROR;
}

_int CWeapon::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pUser)
		return 0;
	
	m_tInfo.m_fFireRateTimer += fTimeDelta;
	m_tInfo.m_fReloadRateTimer += fTimeDelta;

	if (m_tInfo.m_bReloading == true)
	{
		if (m_tInfo.m_fReloadRateTimer > m_tInfo.m_fReloadRate)
		{
			m_tInfo.m_iCurBullet = m_tInfo.m_iMaxBullet; // 탄창 교체
			m_bChangeUpdate = true;
		}
	}

	Update_Bone(fTimeDelta);

	return 0;
}

void CWeapon::Render_GameObject(void)
{
	return;
}

void CWeapon::Set_User(CPlayer * pUser)
{
	if (nullptr == pUser)
		return;

	m_pUser = pUser;
	
	m_pVecPlayerBone = pUser->Get_BoneVector();
	//m_pTargetTransformCom = (*m_pVecPlayerBone)[PLAYERBONE_HAND];
	return;
}

void CWeapon::Set_Monster(CMonster * pUser)
{
	if (nullptr == pUser)
		return;

	m_pUser = pUser;
}

void CWeapon::Recoil(const _float & fTimeDelta, _vec3* vRecoverAngle)
{
	//m_fRecoverTimer = 0.f;

	_float fRandXPower = m_pRandomMgr->GetValueF(-m_tInfo.m_fXRecoilPower, m_tInfo.m_fXRecoilPower);

	//set recoilX
	(*m_pVecPlayerBone)[CPlayer::BONE_EYE]->Rotation_HeadY(_float(-fRandXPower));

	vRecoverAngle->y += fRandXPower;

	//set recoilY
	(*m_pVecPlayerBone)[CPlayer::BONE_EYE]->Rotation_HeadX(_float(-m_tInfo.m_fYRecoilPower));

	vRecoverAngle->x += m_tInfo.m_fYRecoilPower;
}

void CWeapon::Use_Weapon(const _float fTimeDelta, _vec3* vRecoverAngle)
{
	if (nullptr == m_pUser || nullptr == m_pTextureTag)
		return;
	
	// 재장전 중이라면
	if (m_tInfo.m_bReloading == true)
	{
		// 재장전 시간 체크
		if (m_tInfo.m_fReloadRateTimer > m_tInfo.m_fReloadRate)
		{
			m_tInfo.m_iCurBullet = m_tInfo.m_iMaxBullet; // 탄창 교체
			m_tInfo.m_bReloading = false;				// 재장전 끝
			//cout << "Reload!" << m_tInfo.m_iCurBullet << endl;
		}
		else
		{
			return;
		}
	}

	// 잔탄 확인
	if (m_tInfo.m_iCurBullet<=0)
	{
		// 플레이어가 재장전 키 누르기 전까지 발사 불가능
		if (m_tInfo.m_bReloading == false)
		{
			//cout << "재장전 해 주세요." << m_tInfo.m_iCurBullet << endl;
			return;
		}
	}

	// 발사 처리
	if (m_tInfo.m_fFireRateTimer > m_tInfo.m_fFireRate)
	{

		const _matrix& mat = m_vecPartsTransform[PARTS_MUZZLE]->Get_WorldMatrix();
		//_float f = D3DXVec3Length(&(_vec3)mat.m[0]);
		D3DXVECTOR3 vPos(mat._41, mat._42, mat._43);
		//vPos *= f;
		//D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		vPos.y += 0.01f;
		_vec3 DestPos;
		_vec3 *pPickPos = &DestPos;
		m_pPickingCom->Picking_ForWeapon(g_hWnd, pPickPos, 0, 0, m_pTerrainBufferCom);

		if (pPickPos != nullptr)
		{
			
			Recoil(fTimeDelta, vRecoverAngle);//반동
			CCharacter* pDestChar = nullptr;
			if (m_tInfo.m_bBulletEffect == true)
			{
				if (nullptr == m_pBulletParticle || nullptr == m_pParticleRenderer)
					return;

				pDestChar = m_pColliderMgr->CheckCollBullet(g_hWnd, &DestPos, m_pUser->Get_CharType(), mat.m[3], *pPickPos);
				
				m_pBulletParticle->ResetBullet(
					vPos,
					D3DXVECTOR3(DestPos.x, DestPos.y, DestPos.z), //vPos,
					//D3DXVECTOR3(20.0f, 10.0f, 20.0f),
					m_tInfo.m_fBulletSpeed);
				
				if (m_pParticleRenderer->Set_Particle(
					CParticle_Renderer::PARTICLETYPE::PARTICLE_DIRECT_BULLET,
					m_pTextureTag,
					m_pBulletParticle) == false)
				{
					return;
					//cout << "Failed" << endl;
				}

				if (pDestChar != nullptr)
				{
					pDestChar->Hit(m_tInfo.m_iDamage, m_pBulletParticle->Get_Info()._lifeTime);
// 					{
// 						cout << "몬스터 사망" << endl;
// 					}
// 					cout << "몬스터 피격" << endl;
				}
			}else
			{
				if (nullptr == m_pBulletEffect || nullptr == m_pEffectManager)
					return;

				pDestChar = m_pColliderMgr->CheckCollBullet(g_hWnd, &DestPos, m_pUser->Get_CharType(), mat.m[3], *pPickPos);

				m_pBulletEffect->ResetBullet(
					vPos,
					D3DXVECTOR3(DestPos.x, DestPos.y, DestPos.z), //vPos,
					//D3DXVECTOR3(0.1f, 0.1f, 0.1f),
					D3DXVECTOR3(2.0f, 2.0f, 2.0f),
					m_tInfo.m_fBulletSpeed);

				if (m_pEffectManager->Set_Effect(
					CEffect_Manager::EFFECT_TYPE::EFFECT_DIRECT_BULLET,
					m_pBulletEffectTextureTag,
					m_pBulletEffect) == false)
				{
					return;
					//cout << "Failed" << endl;
				}

				//m_pBulletParticle->ResetBullet(
				//	vPos,
				//	D3DXVECTOR3(DestPos.x, DestPos.y, DestPos.z), //vPos,
				//	m_tInfo.m_fBulletSpeed);

				//if (m_pParticleRenderer->Set_Particle(
				//	CParticle_Renderer::PARTICLETYPE::PARTICLE_DIRECT_BULLET,
				//	m_pTextureTag,
				//	m_pBulletParticle) == false)
				//{
				//	return;
				//	//cout << "Failed" << endl;
				//}

			}
			//기타 이펙트 처리
			if (m_tInfo.m_bETCEffect == true)
			{
				if (nullptr == m_pBulletParticle || nullptr == m_pParticleRenderer)
					return;	
				if (nullptr == m_pBulletEffect || nullptr == m_pEffectManager)
					return;

				m_pMuzzleEffect->ResetEffect(
					vPos,
					_vec3(1.f, 1.f, 1.f),
					CEffect_Fixed::TYPE_TIMER,
					0.1f,
					m_vecPartsTransform[PARTS_MUZZLE]
				);
				if (m_pEffectManager->Set_Effect(
					CEffect_Manager::EFFECT_TYPE::EFFECT_FIXED,
					m_pMuzzleEffectTextureTag,
					m_pMuzzleEffect) == false)
				{
					return;
				}
				//if (FAILED(m_pEffectManager->Set_Effect_Type(
				//	CEffect_Manager::EFFECT_TYPE::EFFECT_FIXED,
				//	m_pMuzzleEffectTextureTag,
				//	m_pMuzzleEffectTexture
				//)))
				//{
				//	MSG_BOX("Effect_Manager_Texture_SizeOver : LightObj");
				//	return;
				//}

			}
			//발사후 처리
			--m_tInfo.m_iCurBullet;
			m_tInfo.m_fFireRateTimer = 0.f;

			//cout << "탄창:"<< m_tInfo.m_iCurBullet << endl;
		}
		else
		{
			//cout << "빗나감" << endl;
		}

	}
}

void CWeapon::Use_Weapon_Monster(const _float fTimeDelta, const _vec3& vSourPos,const _vec3 & vDestPos)
{
	if (nullptr == m_pUser || nullptr == m_pTextureTag)
		return;
	// 잔탄 확인
	if (m_tInfo.m_iCurBullet <= 0)
	{
		if (m_tInfo.m_bReloading == false)
		{
			m_tInfo.m_fReloadRateTimer = 0.f;
			m_tInfo.m_bReloading = true; // 리로드 시작
			return;
		}
		// 재장전 시간 체크
		if (m_tInfo.m_fReloadRateTimer > m_tInfo.m_fReloadRate)
		{
			m_tInfo.m_iCurBullet = m_tInfo.m_iMaxBullet; 
			m_tInfo.m_bReloading = false; // 리로드 끝
			return;
		}
	}
	// 발사 처리
	if (m_tInfo.m_fFireRateTimer > m_tInfo.m_fFireRate)
	{
		_vec3 SourPos = vSourPos;
		SourPos.y += 0.01f;
		_vec3 DestPos = vDestPos;

		if (m_tInfo.m_bBulletEffect == true) // 파티클 사용
		{
			if (nullptr == m_pBulletParticle || nullptr == m_pParticleRenderer)
				return;

			CCharacter* pDestChar = m_pColliderMgr->CheckCollBullet(g_hWnd, &DestPos, m_pUser->Get_CharType(), SourPos, DestPos);

			m_pBulletParticle->ResetBullet(
				SourPos,
				D3DXVECTOR3(DestPos.x, DestPos.y, DestPos.z),
				m_tInfo.m_fBulletSpeed);

			if (m_pParticleRenderer->Set_Particle(
				CParticle_Renderer::PARTICLETYPE::PARTICLE_DIRECT_BULLET,
				m_pTextureTag,
				m_pBulletParticle) == false)
			{
				return;
				//cout << "Failed" << endl;
			}

			//m_pMuzzleParticle->ResetEffect(
			//	SourPos,
			//	D3DXCOLOR(0.94f, 0.64f, 0.4f, 1.0f),
			//	0.01f
			//);
			//if (m_pParticleRenderer->Set_Particle(
			//	CParticle_Renderer::PARTICLETYPE::PARTICLE_EFFECT,
			//	m_pMuzzleTextureTag,
			//	m_pMuzzleParticle) == false)
			//{
			//	return;
			//	//cout << "Failed" << endl;
			//}
			if (pDestChar != nullptr)
			{
				if (pDestChar->Hit(m_tInfo.m_iDamage, m_pBulletParticle->Get_Info()._lifeTime))
				{
					//cout << "플레이어 사망" << endl;
				}
				//cout << "플레이어 피격" << endl;
			}
		}
		else // 이펙트 사용
		{


		}
		//발사후 처리
		--m_tInfo.m_iCurBullet;
		m_tInfo.m_fFireRateTimer = 0.f;

	}
}

void CWeapon::Reload_Weapon()
{
	// 재장전 키는 한번만 누르도록
	if (m_tInfo.m_bReloading == false)
	{
		CSound_Manager::GetInstance()->PlaySound(L"wpn_pistol32_reload.wav", CSound_Manager::Channel_Weapon, 1.f);

		//재장전 소리 추가
		m_tInfo.m_bReloading = true;		// 재장전 중
		m_tInfo.m_fReloadRateTimer = 0.f; // 재장전 타이머 초기화
		//cout << "재장전 중" << endl;
		m_bChangeUpdate = false;
	}
}

void CWeapon::Update_Bone(const _float & fTimeDelta)
{
	for (size_t i = 0; i < m_vecPartsTransform.size(); ++i)
	{
		_int iIndex = m_vecPartsTransform[i]->Get_ParentIndex();

		if (-1 == iIndex)
			continue;

		m_vecPartsTransform[i]->Parent(m_vecPartsTransform[iIndex]->Get_WorldMatrix());
		m_vecPartsTransform[i]->Fill_WorldMatrix();
	}
}

void CWeapon::Render_Model()
{
	_matrix matProj, matView;
	_matrix matWorldView;
	_matrix matWorldViewProjection;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (size_t i = 0; i < m_vecBodyInfo.size(); ++i)
	{
		_matrix matWorld, matScale, matTrans, matParent;
		D3DXMatrixTranslation(&matTrans, m_vecBodyInfo[i]->vPosition.x, m_vecBodyInfo[i]->vPosition.y, m_vecBodyInfo[i]->vPosition.z);
		D3DXMatrixScaling(&matScale, m_vecBodyInfo[i]->vScale.x, m_vecBodyInfo[i]->vScale.y, m_vecBodyInfo[i]->vScale.z);

		if (m_vecBodyInfo[i]->iParentIndex != -1)
		{
			matParent = m_vecPartsTransform[m_vecBodyInfo[i]->iParentIndex]->Get_WorldMatrix();

			matWorld = matScale*matTrans*matParent;
		}
		else /*has not parent*/
		{
			matWorld = matScale* matTrans;
		}

		D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
		D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);

		m_pShaderCom->GetShader(1)->SetTexture("DiffuseMap_Tex", m_pTextureCom->GetTexture(m_vecBodyInfo[i]->iTextureNum+1));
		m_pShaderCom->GetShader(1)->SetTexture("SpecularMap_Tex", m_pTextureCom->GetTexture(m_vecBodyInfo[i]->iTextureNum + 2));
		m_pShaderCom->GetShader(1)->SetTexture("NormalMap_Tex", m_pTextureCom->GetTexture(m_vecBodyInfo[i]->iTextureNum + 3));

		m_pShaderCom->GetShader(1)->SetMatrix("g_matWorld", &matWorld);
		m_pShaderCom->GetShader(1)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);
		m_pShaderCom->GetShader(1)->CommitChanges();

		m_pCubeTexBuffer->Render_Buffer();

	}

}

void CWeapon::SetShader(_uint iShaderNum)
{
	_matrix matProj, matView, matWorld;

	D3DXVECTOR4 gWorldCameraPosition;

	matWorld = m_pTransformCom->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_matrix matWorldView;
	_matrix matWorldViewProjection;
	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProj);


	_vec3 CameraPosition = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_CameraPos();


	m_pShaderCom->GetShader(iShaderNum)->SetMatrix("g_matWorld", &matWorld);
	m_pShaderCom->GetShader(iShaderNum)->SetMatrix("g_matWorldViewProjection", &matWorldViewProjection);

	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_vWorldCameraPos", &CameraPosition, sizeof(CameraPosition));

	CLight_Manager::GetInstance()->Set_ShaderLightList(m_pShaderCom->GetShader(iShaderNum));

	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Ambient", &m_pMatCom->Get_MaterialInfo().Ambient, sizeof(m_pMatCom->Get_MaterialInfo().Ambient));
	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Diffuse", &m_pMatCom->Get_MaterialInfo().Diffuse, sizeof(m_pMatCom->Get_MaterialInfo().Diffuse));
	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Emissive", &m_pMatCom->Get_MaterialInfo().Emissive, sizeof(m_pMatCom->Get_MaterialInfo().Emissive));
	m_pShaderCom->GetShader(iShaderNum)->SetValue("g_Material.Specular", &m_pMatCom->Get_MaterialInfo().Specular, sizeof(m_pMatCom->Get_MaterialInfo().Specular));
	m_pShaderCom->GetShader(iShaderNum)->SetFloat("g_Material.fPower", m_pMatCom->Get_MaterialInfo().Power);


	m_pShaderCom->GetShader(iShaderNum)->SetTexture("DiffuseMap_Tex", m_pTextureCom->GetTexture(1));
	m_pShaderCom->GetShader(iShaderNum)->SetTexture("SpecularMap_Tex", m_pTextureCom->GetTexture(2));
	m_pShaderCom->GetShader(iShaderNum)->SetTexture("NormalMap_Tex", m_pTextureCom->GetTexture(3));
}

void CWeapon::StartShader(_uint iShaderNum)
{
	_uint numPasses = 0;
	m_pShaderCom->GetShader(iShaderNum)->Begin(&numPasses, NULL);
	m_pShaderCom->GetShader(iShaderNum)->BeginPass(0);

}

void CWeapon::EndShader(_uint iShaderNum)
{
	m_pShaderCom->GetShader(iShaderNum)->EndPass();
	m_pShaderCom->GetShader(iShaderNum)->End();
}


HRESULT CWeapon::Ready_Component(void)
{
	return NOERROR;
}

HRESULT CWeapon::Load_WeaponFile(const _tchar * pFilePath)
{
	if (nullptr == pFilePath)
		return E_FAIL;

	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Load failed");
		return E_FAIL;
	}

	_ulong dwByte = 0;

	size_t iBoneSize = 0;
	ReadFile(hFile, &iBoneSize, sizeof(size_t), &dwByte, NULL);

	for (size_t i = 0; i < iBoneSize; ++i)
	{
		_vec3 vPos;
		_int iParentIndex = 0;

		ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, NULL);
		ReadFile(hFile, &iParentIndex, sizeof(_int), &dwByte, NULL);

		CBone* pBone = CBone::Create(m_pGraphicDev);
		pBone->Set_Position(vPos);
		pBone->Set_ParentIndex(iParentIndex);

		m_vecPartsTransform.push_back(pBone);
	}



	size_t iBodySize = 0;
	ReadFile(hFile, &iBodySize, sizeof(size_t), &dwByte, NULL);

	for (size_t i = 0; i < iBodySize; ++i)
	{
		_vec3 vScale;
		_vec3 vPos;
		_int iParentIndex = 0;
		_int iTextureNum = 0;

		ReadFile(hFile, &vScale, sizeof(_vec3), &dwByte, NULL);
		ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, NULL);
		ReadFile(hFile, &iParentIndex, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, &iTextureNum, sizeof(_int), &dwByte, NULL);

		BODYINFO* pBodyInfo = new BODYINFO;

		pBodyInfo->vPosition = vPos;
		pBodyInfo->vScale = vScale;
		pBodyInfo->iParentIndex = iParentIndex;
		pBodyInfo->iTextureNum = iTextureNum;

		m_vecBodyInfo.push_back(pBodyInfo);
	}
	CloseHandle(hFile);

	return NOERROR;
}


_ulong CWeapon::Free(void)
{
	_ulong		dwRefCnt = 0;
		
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pPickingCom);

	Safe_Release(m_pTextureCom);

	Safe_Release(m_pParticleRenderer);
	Safe_Release(m_pBulletParticle);
	Safe_Release(m_pMuzzleParticle);
	Safe_Release(m_pBulletTexture);
	Safe_Release(m_pMuzzleTexture);

	Safe_Release(m_pEffectManager);
	Safe_Release(m_pBulletEffect);
	Safe_Release(m_pMuzzleEffect);
	Safe_Release(m_pBulletEffectTexture);
	Safe_Release(m_pMuzzleEffectTexture);

	//Safe_Release(m_pBulletEffectTexture);


	Safe_Release(m_pTerrainBufferCom);
	Safe_Release(m_pRandomMgr);

	Safe_Release(m_pColliderMgr);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMatCom);

	//Safe_Release(m_pUser);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
