#include "..\Headers\Effect_Manager.h"

#include "GameObject.h"

//#include "Target_Manager.h"


IMPLEMENT_SINGLETON(CEffect_Manager)


CEffect_Manager::CEffect_Manager()
	:m_pGraphicDev(nullptr), iEffectTypeSize(VTXEFFECT_MAX_SIZE / EFFECT_MAX_SIZE), iEffectTypeCount(0)
{
	
}

HRESULT CEffect_Manager::Ready_Effect_Renderer(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	m_pGraphicDev = pGraphicDevice;
	m_pGraphicDev->AddRef();

	return NOERROR;
}

bool CEffect_Manager::Set_Effect(EFFECT_TYPE eType, const _tchar * TextureTag, CEffect * pEffect)
{
	CEffect* pEffectDest = Get_Effect(eType, TextureTag);
	if (pEffect == nullptr || pEffectDest == nullptr)
	{
		return false;
	}

	pEffectDest->Clone_Effect(pEffect);
	return true;
}

//bool CEffect_Manager::Set_Effect_Address(EFFECT_TYPE eType, const _tchar * TextureTag, CEffect * pEffect)
//{
//	return false;
//}

bool CEffect_Manager::Set_Effect_Output(EFFECT_TYPE eType, const _tchar * TextureTag, CEffect * pEffect, CEffect *pOutEffect = nullptr)
{
	CEffect* pEffectDest = Get_Effect(eType, TextureTag);
	if (pEffect == nullptr || pEffectDest == nullptr)
	{
		return false;
	}

	pEffectDest->Clone_Effect(pEffect);
	if (pOutEffect != nullptr)
	{
		pOutEffect = pEffectDest;
	}
	return true;
}

CEffect * CEffect_Manager::Get_Effect(EFFECT_TYPE eType, const _tchar * TextureTag)
{
	auto iter = find_if(m_EffectMap[eType].begin(), m_EffectMap[eType].end(), [&](const EFFECTMAP::value_type Pair) {return !lstrcmp(Pair.first, TextureTag);  });

	if (iter == m_EffectMap[eType].end())
	{
		return nullptr;
	}
	else
	{
		for (auto pEffect : (*iter).second)
		{
			if (pEffect != nullptr)
			{
				if (pEffect->Get_Info()._isAlive == false)
				{
					return pEffect;
				}
			}
		}
	}

	return nullptr;
}

HRESULT CEffect_Manager::Set_Effect_Type(EFFECT_TYPE eType, const _tchar * TextureTag, CTexture * pTexture)
{
	if (iEffectTypeCount >= iEffectTypeSize)
	{
		return E_FAIL;
	}
	auto iter = find_if(m_EffectTextureMap[eType].begin(), m_EffectTextureMap[eType].end(), [&](const EFFECTTEXMAP::value_type Pair) {return !lstrcmp(Pair.first, TextureTag);  });

	if (iter == m_EffectTextureMap[eType].end())
	{
		m_EffectTextureMap[eType].insert(EFFECTTEXMAP::value_type(TextureTag, pTexture));
		pTexture->AddRef();

		vector<CEffect*> pVector;

		switch (eType)
		{
		case EFFECT_TYPE::EFFECT_DIRECT_BULLET:
			for (int i = 0; i < EFFECT_MAX_SIZE; ++i)
			{
				//CEffect_Fixed* pEffect = nullptr;
				//pEffect = CEffect_Fixed::Create(m_pGraphicDev);
				//pEffect->Ready_Buffer();

				pVector.push_back(CEffect_Bullet::Create(m_pGraphicDev));
			}
			break;
		case EFFECT_TYPE::EFFECT_FIXED:
			for (int i = 0; i < EFFECT_MAX_SIZE; ++i)
			{
				//CEffect_Fixed* pEffect = nullptr;
				//pEffect = CEffect_Fixed::Create(m_pGraphicDev);
				//pEffect->Ready_Buffer();

				pVector.push_back(CEffect_Fixed::Create(m_pGraphicDev));
			}
			break;
		default:
			return E_FAIL;
		}

		m_EffectMap[eType].insert(EFFECTMAP::value_type(TextureTag, pVector));
		++iEffectTypeCount;
	}
	return NOERROR;
}

void CEffect_Manager::Ready_Render()
{

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 50);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void CEffect_Manager::Start_Render()
{
	_matrix matWorld, matView, matScale;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	for (int i = 0; i < EFFECT_END; ++i)
	{

		if (!m_EffectMap[i].empty())
		{
			for (auto pEffectVec : m_EffectMap[i])
			{
				const _tchar *pTextureTag = nullptr;
				pTextureTag = pEffectVec.first;

				Ready_Render();

				auto iter = find_if(m_EffectTextureMap[i].begin(), m_EffectTextureMap[i].end(), [&](const EFFECTTEXMAP::value_type Pair) {return !lstrcmp(Pair.first, pTextureTag);  });
				CTexture* pTexture = iter->second;

				//m_pGraphicDev->SetTexture(0, pTexture->GetTexture());

				for (auto pEffect : pEffectVec.second)
				{
					const EFTINFO& pInfo = pEffect->Get_Info();

					if (pInfo._isAlive)
					{
						
						matWorld = matView;
						matWorld._41 = pInfo.vPos.x;
						matWorld._42 = pInfo.vPos.y;
						matWorld._43 = pInfo.vPos.z;
						D3DXMatrixScaling(&matScale, pInfo.vScale.x, pInfo.vScale.y,pInfo.vScale.z);
						//cout << "Scale: " << pInfo.vScale.x << pInfo.vScale.y << pInfo.vScale.z << endl;
						matWorld = matScale * matWorld;
						m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
						pTexture-> SetUp_TextureToGraphicDev(0, _uint(pInfo.fFrame));
						pEffect->Render_Buffer();
					}
				}
				End_Render();
			}
		}
	}
}

void CEffect_Manager::End_Render()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, true);
	//m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	//m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CEffect_Manager::Update_Effect(float fTimeDelta)
{
	for (int i = 0; i < EFFECT_END; ++i)
	{
		for (auto iter : m_EffectMap[i])
		{
			for (auto pEffect : (iter).second)
			{
				if (pEffect->IsAlive())
				{
					pEffect->Update_Effect(fTimeDelta);
				}
			}
		}
	}
}


//CEffect_Manager * CEffect_Manager::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
//{
//
//	CEffect_Manager* pInstance = new CEffect_Manager(pGraphicDevice);
//	if (FAILED(pInstance->Ready_Particle_Renderer()))
//	{
//		MSG_BOX("CEffect_Manager creation failed");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

//CComponent * CEffect_Manager::Clone()
//{
//	AddRef();
//	return this;
//}

_ulong CEffect_Manager::Free()
{
	_ulong dwRefCount = 0;

	for (int i = 0; i < EFFECT_END; ++i)
	{
		for (auto iter : m_EffectTextureMap[i])
		{
			dwRefCount += Safe_Release((iter).second);
		}
		for (auto iter : m_EffectMap[i])
		{
			for (auto pEffect : (iter).second)
			{
				dwRefCount += Safe_Release(pEffect);
			}
		}
	}

	Safe_Release(m_pGraphicDev);

	if (dwRefCount)
		int n = 0;
	return dwRefCount;
}

