#include "..\Headers\ColliderBox.h"


CColliderBox::CColliderBox(LPDIRECT3DDEVICE9 pGraphicDev)
: CComponent(pGraphicDev)
{

}

CColliderBox::CColliderBox(const CColliderBox& rhs)
: CComponent(rhs)
{

}

HRESULT CColliderBox::Ready_ColliderBox(COLTYPE eType)
{
	if (eType == COLTYPE::COL_BILLBOARD)
	{
		_uint m_iVertexCount = 4;
		_uint m_iVertexSize = sizeof(VTXCOLLIDE);
		DWORD m_dwVertexFVF = VTXCOLLIDE::FVF;

		_uint m_iIndexSize = sizeof(INDEX16);
		_uint m_iTriCount = 2;
		D3DFORMAT m_IndexFmt = D3DFMT_INDEX16;

		if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_iVertexSize*m_iVertexCount, 0, m_dwVertexFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		{
			MSG_BOX("VertexBuffer Not Created");
			return E_FAIL;
		}
		if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_iIndexSize*m_iTriCount, 0, m_IndexFmt, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		{
			MSG_BOX("IndexBuffer creation failed");
			return E_FAIL;
		}

		if (nullptr == m_pVB || nullptr == m_pIB)
			return E_FAIL;

		VTXCOLLIDE* pVertex = nullptr;

		m_pVB->Lock(0, 0, (void**)&pVertex, 0);

		pVertex[0].vPosition = _vec3(-1.f, 1.f, 0.f);

		pVertex[1].vPosition = _vec3(1.f, 1.f, 0.f);

		pVertex[2].vPosition = _vec3(1.f, -1.f, 0.f);

		pVertex[3].vPosition = _vec3(-1.f, -1.f, 0.f);


		m_pVB->Unlock();

		INDEX16* pIndex = nullptr;

		m_pIB->Lock(0, 0, (void**)&pIndex, 0);

		pIndex[0]._0 = 0;
		pIndex[0]._1 = 1;
		pIndex[0]._2 = 2;

		pIndex[1]._0 = 0;
		pIndex[1]._1 = 2;
		pIndex[1]._2 = 3;

		m_pIB->Unlock();
	}
	else if (eType == COLTYPE::COL_AABB)
	{
		_uint m_iVertexCount = 8;
		_uint m_iVertexSize = sizeof(VTXCOLLIDE);
		DWORD m_dwVertexFVF = VTXCOLLIDE::FVF;

		_uint m_iIndexSize = sizeof(INDEX16);
		_uint m_iTriCount = 12;
		D3DFORMAT m_IndexFmt = D3DFMT_INDEX16;

		if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_iVertexSize*m_iVertexCount, 0, m_dwVertexFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		{
			MSG_BOX("VertexBuffer Not Created");
			return E_FAIL;
		}
		if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_iIndexSize*m_iTriCount, 0, m_IndexFmt, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		{
			MSG_BOX("IndexBuffer creation failed");
			return E_FAIL;
		}
		if (nullptr == m_pVB || nullptr == m_pIB)
			return E_FAIL;

		_uint VertexBufferSize = 8;
		_uint IndexBufferSize = 12;
		_uint iIndex = 0;

		const _float x = 0.5f;
		const _float y = 0.5f;
		const _float z = 0.5f;

		VTXCOLLIDE* pVertex = new VTXCOLLIDE[VertexBufferSize];

		pVertex[0].vPosition = _vec3(-x, y, -z);
		//m_pVertexPos[0] = pVertex[0].vPosition;

		pVertex[1].vPosition = _vec3(x, y, -z);
		//m_pVertexPos[1] = pVertex[1].vPosition;

		pVertex[2].vPosition = _vec3(x, -y, -z);
		//m_pVertexPos[2] = pVertex[2].vPosition;

		pVertex[3].vPosition = _vec3(-x, -y, -z);
		//m_pVertexPos[3] = pVertex[3].vPosition;

		pVertex[4].vPosition = _vec3(-x, y, z);
		//m_pVertexPos[4] = pVertex[4].vPosition;

		pVertex[5].vPosition = _vec3(x, y, z);
		//m_pVertexPos[5] = pVertex[5].vPosition;

		pVertex[6].vPosition = _vec3(x, -y, z);
		//m_pVertexPos[6] = pVertex[6].vPosition;

		pVertex[7].vPosition = _vec3(-x, -y, z);
		//m_pVertexPos[7] = pVertex[7].vPosition;

		INDEX16* pIndex = new INDEX16[IndexBufferSize];

		//+x
		pIndex[0]._0 = 1; pIndex[0]._1 = 5; pIndex[0]._2 = 6;
		pIndex[1]._0 = 1; pIndex[1]._1 = 6; pIndex[1]._2 = 2;
		//-x
		pIndex[2]._0 = 4; pIndex[2]._1 = 0; pIndex[2]._2 = 3;
		pIndex[3]._0 = 4; pIndex[3]._1 = 3; pIndex[3]._2 = 7;

		//+y
		pIndex[4]._0 = 4; pIndex[4]._1 = 5; pIndex[4]._2 = 1;
		pIndex[5]._0 = 4; pIndex[5]._1 = 1; pIndex[5]._2 = 0;

		//-y
		pIndex[6]._0 = 3; pIndex[6]._1 = 2; pIndex[6]._2 = 6;
		pIndex[7]._0 = 3; pIndex[7]._1 = 6; pIndex[7]._2 = 7;

		//+z
		pIndex[8]._0 = 5; pIndex[8]._1 = 4; pIndex[8]._2 = 7;
		pIndex[9]._0 = 5; pIndex[9]._1 = 7; pIndex[9]._2 = 6;

		//-z
		pIndex[10]._0 = 0; pIndex[10]._1 = 1; pIndex[10]._2 = 2;
		pIndex[11]._0 = 0; pIndex[11]._1 = 2; pIndex[11]._2 = 3;


		VTXCOLLIDE*			pVertexBuffer = nullptr;
		m_pVB->Lock(0, 0, (void**)&pVertexBuffer, 0);
		memcpy(pVertexBuffer, pVertex, sizeof(VTXCOLLIDE)*VertexBufferSize);
		m_pVB->Unlock();

		INDEX16*			pIndexBuffer = nullptr;
		m_pIB->Lock(0, 0, (void**)&pIndexBuffer, 0);
		memcpy(pIndexBuffer, pIndex, sizeof(INDEX16)*IndexBufferSize);
		m_pIB->Unlock();

		Engine::Safe_Delete_Array(pVertex);
		Engine::Safe_Delete_Array(pIndex);

	}
	else
	{
		return E_FAIL;
	}

	return NOERROR;
}


CComponent* CColliderBox::Clone(void)
{
	return new CColliderBox(*this);
}

CColliderBox * CColliderBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, COLTYPE eType, _float fSizeX, _float fSizeY)
{
	CColliderBox*	pInstance = new CColliderBox(pGraphicDev);

	//if (FAILED(pInstance->Ready_ColliderBox()))
	//{
	//	MessageBox(NULL, L"CColliderBox Created Failed", NULL, MB_OK);
	//	Safe_Release(pInstance);
	//}
	return pInstance;
}


_ulong CColliderBox::Free(void)
{
	_ulong dwRefCount = 0;


	dwRefCount = Safe_Release(m_pVB);
	dwRefCount=	Safe_Release(m_pIB);
	
	CComponent::Free();

	return dwRefCount;

}

