#include "..\Headers\Particle_Renderer.h"

#include "GameObject.h"

//#include "Target_Manager.h"


IMPLEMENT_SINGLETON(CParticle_Renderer)


//CParticle_Renderer::CParticle_Renderer(LPDIRECT3DDEVICE9 pGraphicDevice)
//	:CComponent(pGraphicDevice), _vbBatchSize(PARTICLE_MAX_SIZE), iParticleTypeSize( VTXPTC_MAX_SIZE / PARTICLE_MAX_SIZE)
//{
//
//}

CParticle_Renderer::CParticle_Renderer()
	:m_pGraphicDev(nullptr), _vbBatchSize(PARTICLE_MAX_SIZE), iParticleTypeSize(VTXPTC_MAX_SIZE / PARTICLE_MAX_SIZE), _vbOffset(0)
{
	
}

//
//HRESULT CParticle_Renderer::Ready_Particle(_tchar * texFilePath)
//{
//
//	if (FAILED(m_pGraphicDev->CreateVertexBuffer(
//		_vbSize * sizeof(Particle),
//		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
//		Particle::FVF,
//		D3DPOOL_DEFAULT, 
//		&_vb,
//		0)))
//	{
//		MSG_BOX("ParticleBuffer Not Created");
//		return E_FAIL;
//	}
//
//	if (FAILED(D3DXCreateTextureFromFile(
//		m_pGraphicDev,
//		texFileName,
//		&_tex)))
//	{
//		MSG_BOX("ParticleTexture Not Created");
//		return E_FAIL;
//	}
//
//	return NOERROR;
//}

HRESULT CParticle_Renderer::Ready_Particle_Renderer(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	m_pGraphicDev = pGraphicDevice;
	m_pGraphicDev->AddRef();

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(
		VTXPTC_MAX_SIZE * sizeof(VTXPTC),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		VTXPTC::FVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0)))
	{
		MSG_BOX("CParticle_Renderer Buffer Not Created");
		return E_FAIL;
	}

	return NOERROR;
}

bool CParticle_Renderer::Set_Particle(PARTICLETYPE eType, const _tchar * TextureTag, CParticle* pParticle)
{
	CParticle* pParticleDest = Get_Particle(eType, TextureTag);
	if (pParticle == nullptr || pParticleDest == nullptr)
	{
		return false;
	}

	pParticleDest->Clone_Particle(pParticle);
	return true;
}

CParticle * CParticle_Renderer::Get_Particle(PARTICLETYPE eType, const _tchar * TextureTag)
{
	auto iter = find_if(m_ParticleMap[eType].begin(), m_ParticleMap[eType].end(), [&](const PTCMAP::value_type Pair) {return !lstrcmp(Pair.first, TextureTag);  });

	if (iter == m_ParticleMap[eType].end())
	{
		return nullptr;
	}
	else
	{
		for (auto pParticle : (*iter).second)
		{
			if (pParticle->Get_Info()._isAlive == false)
			{
				return pParticle;
			}
		}
	}

	return nullptr;
}

HRESULT CParticle_Renderer::Set_Particle_Type(PARTICLETYPE eType, const _tchar * TextureTag, CTexture * pTexture)
{
	if (iParticleTypeCount >= iParticleTypeSize)
	{
		return E_FAIL;
	}
	auto iter = find_if(m_ParticleTextureMap[eType].begin(), m_ParticleTextureMap[eType].end(), [&](const PTCTEXMAP::value_type Pair) {return !lstrcmp(Pair.first, TextureTag);  });

	if (iter == m_ParticleTextureMap[eType].end())
	{
		m_ParticleTextureMap[eType].insert(PTCTEXMAP::value_type(TextureTag, pTexture));
		pTexture->AddRef();

		vector<CParticle*> pVector;

		switch (eType)
		{
		case PARTICLETYPE::PARTICLE_DIRECT_BULLET:
			for (int i = 0; i < PARTICLE_MAX_SIZE; ++i)
			{
				pVector.push_back(CParticle_Bullet::Create(m_pGraphicDev));
			}
			break;
		case PARTICLETYPE::PARTICLE_EFFECT:
			for (int i = 0; i < PARTICLE_MAX_SIZE; ++i)
			{
				pVector.push_back(CParticle_Effect::Create(m_pGraphicDev));
			}
			break;
		default:
			return E_FAIL;
		}

		m_ParticleMap[eType].insert(PTCMAP::value_type(TextureTag, pVector));
		++iParticleTypeCount;
	}
	return NOERROR;
}

void CParticle_Renderer::Ready_Render(_float fParticleSize)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, FtoDw(fParticleSize));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(fParticleSize*6.f));
	
	// control the size of the particle relative to distance
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(1.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(1.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(4.0f));

	// use alpha from texture
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// read, but don't write particles to z-buffer
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}

void CParticle_Renderer::Start_Render()
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	_float fParticleSize = 1.f;

	for (int i = 0; i < PARTICLE_END; ++i)
	{
		if (i == PARTICLETYPE::PARTICLE_DIRECT_BULLET)
		{
			fParticleSize = 4.f;
		}
		else if (i == PARTICLETYPE::PARTICLE_EFFECT)
		{
			fParticleSize = 10.f;
		}

		if (!m_ParticleMap[i].empty())
		{
			for (auto pParticleVec : m_ParticleMap[i])
			{
				const _tchar *pTextureTag = nullptr;
				pTextureTag = pParticleVec.first;
				Ready_Render(fParticleSize);

				auto iter = find_if(m_ParticleTextureMap[i].begin(), m_ParticleTextureMap[i].end(), [&](const PTCTEXMAP::value_type Pair) {return !lstrcmp(Pair.first, pTextureTag);  });
				CTexture* pTexture = iter->second;

				m_pGraphicDev->SetTexture(0, pTexture->GetTexture());
				m_pGraphicDev->SetFVF(VTXPTC::FVF);
				m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXPTC));
				
				if (_vbOffset >= VTXPTC_MAX_SIZE)
					_vbOffset = 0;

				VTXPTC* pVertex = 0;

				m_pVB->Lock(
					_vbOffset * sizeof(VTXPTC),
					_vbBatchSize * sizeof(VTXPTC),
					(void**)&pVertex,
					_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

				DWORD numParticlesInBatch = 0;

				for (auto pParticle : pParticleVec.second)
				{
					const Attribute& pAtt = pParticle->Get_Info();
					if (pAtt._isAlive)
					{

						pVertex->vPos = pAtt._position;
						pVertex->dwColor = (D3DCOLOR)pAtt._color;
						++pVertex; // next element;

						numParticlesInBatch++; 

						if (numParticlesInBatch == _vbBatchSize)
						{

							m_pVB->Unlock();

							m_pGraphicDev->DrawPrimitive(
								D3DPT_POINTLIST,
								_vbOffset,
								_vbBatchSize);

							_vbOffset += _vbBatchSize;

							if (_vbOffset >= VTXPTC_MAX_SIZE)
								_vbOffset = 0;

							m_pVB->Lock(
								_vbOffset * sizeof(VTXPTC),
								_vbBatchSize * sizeof(VTXPTC),
								(void**)&pVertex,
								_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

							numParticlesInBatch = 0;
						}
					}
				}

				m_pVB->Unlock();

				if (numParticlesInBatch)
				{
					m_pGraphicDev->DrawPrimitive(
						D3DPT_POINTLIST,
						_vbOffset,
						numParticlesInBatch);
				}

				_vbOffset += _vbBatchSize;

				End_Render();
			}
		}
	}
}

void CParticle_Renderer::End_Render()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CParticle_Renderer::Update_Particle(float fTimeDelta)
{
	for (int i = 0; i < PARTICLE_END; ++i)
	{
		for (auto iter : m_ParticleMap[i])
		{
			for (auto pParticle : (iter).second)
			{
				if (pParticle->IsAlive())
				{
					pParticle->Update_Particle(fTimeDelta);
				}
			}
		}
	}
}


//CParticle_Renderer * CParticle_Renderer::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
//{
//
//	CParticle_Renderer* pInstance = new CParticle_Renderer(pGraphicDevice);
//	if (FAILED(pInstance->Ready_Particle_Renderer()))
//	{
//		MSG_BOX("CParticle_Renderer creation failed");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

//CComponent * CParticle_Renderer::Clone()
//{
//	AddRef();
//	return this;
//}

_ulong CParticle_Renderer::Free()
{
	_ulong dwRefCount = 0;

	for (int i = 0; i < PARTICLE_END; ++i)
	{
		for (auto iter : m_ParticleTextureMap[i])
		{
			dwRefCount += Safe_Release((iter).second);
		}
		for (auto iter : m_ParticleMap[i])
		{
			for (auto pParticle : (iter).second)
			{
				dwRefCount += Safe_Release(pParticle);
			}
		}
	}

	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pVB);

	return dwRefCount;
}

