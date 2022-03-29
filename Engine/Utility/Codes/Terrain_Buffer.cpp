#include "..\Headers\Terrain_Buffer.h"



CTerrain_Buffer::CTerrain_Buffer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{

}

CTerrain_Buffer::CTerrain_Buffer(const CTerrain_Buffer& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CTerrain_Buffer::Ready_Buffer(const _tchar* pHeightMapFilePath, const _float & fVtxInterval)
{
	/*Load HeightMap*/
	HANDLE hFile;
	hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		goto exception;

	_ulong dwByte = 0;

	BITMAPFILEHEADER fh;
	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	if (0 == dwByte)
		goto exception;

	BITMAPINFOHEADER ih;
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
	if (0 == dwByte)
		goto exception;

	_ulong PixelCnt = ih.biHeight * ih.biWidth;
	_ulong* Pixel = new _ulong[PixelCnt];

	ReadFile(hFile, Pixel, sizeof(_ulong)*PixelCnt, &dwByte, nullptr);
	if (0 == dwByte)
		goto exception;

	CloseHandle(hFile);

	/*set VertexCount*/
	const _ulong dwVtxCountX = ih.biWidth;
	const _ulong dwVtxCountZ = ih.biHeight;

	/*vertex parameters*/
	m_iVertexSize = sizeof(VTXTERTEX);
	m_iVertexCount = dwVtxCountX * dwVtxCountZ;
	m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEX2 | D3DFVF_TEX3 | D3DFVF_TEXCOORDSIZE3(1) | D3DFVF_TEXCOORDSIZE3(2);

	/*index parameters*/
	m_iIndexSize = sizeof(INDEX32);
	m_iTriCount = int(((dwVtxCountX - 1) * (dwVtxCountZ - 1)) * 2);
	m_IndexFmt = D3DFMT_INDEX32;

	/*Set Member*/
	m_pVertexPos = new _vec3[dwVtxCountX * dwVtxCountZ];
	m_iVertexCountX = dwVtxCountX;
	m_iVertexCountZ = dwVtxCountZ;
	m_fVertexItv = fVtxInterval;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		goto exception;

	if (nullptr == m_pIB || nullptr == m_pVB)
		goto exception;

	/*memory allocate done*/
	_long VertexBufferSize = dwVtxCountX*dwVtxCountZ;
	VTXTERTEX*	pVertex = new VTXTERTEX[VertexBufferSize];

	

	float fUVCount = dwVtxCountX;
	for (size_t i = 0; i < dwVtxCountZ; ++i)
	{
		for (size_t j = 0; j < dwVtxCountX; ++j)
		{
			_ulong dwIndex = i * dwVtxCountX + j;

			pVertex[dwIndex].vPosition = _vec3(j*fVtxInterval, (Pixel[dwIndex] & 0x000000ff) / 10.0f, i*fVtxInterval);
			pVertex[dwIndex].vNormal = _vec3(0.f, 0.f, 0.f);
			//pVertex[dwIndex].vTexUV = _vec2(j / _float(dwVtxCountX - 1) * (dwVtxCountX - 1) , 1- i / _float(dwVtxCountZ - 1) * (dwVtxCountZ - 1));
			//pVertex[dwIndex].vTexUV = _vec2(
			//	j / _float(dwVtxCountX - 1) * (dwVtxCountX - 1) / float(dwVtxCountX/ fUVCount),
			//	(i / _float(dwVtxCountZ - 1) * (dwVtxCountZ - 1) / float(dwVtxCountZ / fUVCount))
			//);
			pVertex[dwIndex].vTexUV = _vec2(
				j / _float(dwVtxCountX - 1) * (fUVCount),
				1 - i / _float(dwVtxCountZ - 1) * (fUVCount)
			);

			pVertex[dwIndex].vTangent = _vec3(0.f, 0.f, 0.f);
			pVertex[dwIndex].vBiNormal = _vec3(0.f, 0.f, 0.f);

			m_pVertexPos[dwIndex] = pVertex[dwIndex].vPosition;
		}
	}
	_long IndexBufferSize = (dwVtxCountX - 1)*(dwVtxCountZ - 1) * 2;

	INDEX32* pIndex = new INDEX32[IndexBufferSize];

	

	_uint		iIndex = 0;
	for (size_t i = 0; i < dwVtxCountZ - 1; ++i)
	{
		for (size_t j = 0; j < dwVtxCountX - 1; ++j)
		{
			_uint		iVtxIdx = i * dwVtxCountX + j;
			_uint		iLT, iRT, iLB, iRB;
			D3DXVECTOR4 vTangent;

			iLT = iVtxIdx + dwVtxCountX;
			iRT = iLT + 1;
			iLB = iVtxIdx;
			iRB = iLB + 1;

			pIndex[iIndex]._0 = iLT;
			pIndex[iIndex]._1 = iRT;
			pIndex[iIndex]._2 = iRB;

			_vec3 vPolygonNormal, v1, v2;

			v1 = pVertex[pIndex[iIndex]._2].vPosition - pVertex[pIndex[iIndex]._1].vPosition;
			v2 = pVertex[pIndex[iIndex]._1].vPosition - pVertex[pIndex[iIndex]._0].vPosition;
			D3DXVec3Cross(&vPolygonNormal, &v2, &v1);

			CalcTangentVector(pVertex[pIndex[iIndex]._0], pVertex[pIndex[iIndex]._1], pVertex[pIndex[iIndex]._2], vPolygonNormal);


			++iIndex;


			pIndex[iIndex]._0 = iLT;
			pIndex[iIndex]._1 = iRB;
			pIndex[iIndex]._2 = iLB;

			v1 = pVertex[pIndex[iIndex]._2].vPosition - pVertex[pIndex[iIndex]._1].vPosition;
			v2 = pVertex[pIndex[iIndex]._1].vPosition - pVertex[pIndex[iIndex]._0].vPosition;
			D3DXVec3Cross(&vPolygonNormal, &v2, &v1);

			CalcTangentVector(pVertex[pIndex[iIndex]._0], pVertex[pIndex[iIndex]._1], pVertex[pIndex[iIndex]._2], vPolygonNormal);


			++iIndex;

		}
	}

	for (size_t iIndex = 0; iIndex < VertexBufferSize; ++iIndex)
	{
		D3DXVec3Normalize(&pVertex[iIndex].vNormal, &pVertex[iIndex].vNormal);
		D3DXVec3Normalize(&pVertex[iIndex].vTangent, &pVertex[iIndex].vTangent);
		D3DXVec3Normalize(&pVertex[iIndex].vBiNormal, &pVertex[iIndex].vBiNormal);
	}

	VTXTERTEX*			pVertexBuffer = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertexBuffer, 0);
	memcpy(pVertexBuffer, pVertex, sizeof(VTXTERTEX)*VertexBufferSize);
	m_pVB->Unlock();

	INDEX32*			pIndexBuffer = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndexBuffer, 0);
	memcpy(pIndexBuffer, pIndex, sizeof(INDEX32)*IndexBufferSize);
	m_pIB->Unlock();

	Engine::Safe_Delete_Array(pVertex);
	Engine::Safe_Delete_Array(pIndex);

	return NOERROR;

exception:
	MSG_BOX("Terrain Ready Failed err:00923");
	return E_FAIL;
}

void CTerrain_Buffer::CalcTangentVector(VTXTERTEX & pVertex1, VTXTERTEX & pVertex2, VTXTERTEX & pVertex3, _vec3 vNormal)
{
	int faceCount, i;

	_vec3 vPos1, vPos2;
	D3DXVECTOR2 uv1, uv2;
	_vec3 tangent, binormal, normal = vNormal;


	vPos1 = pVertex2.vPosition - pVertex1.vPosition;
	vPos2 = pVertex3.vPosition - pVertex1.vPosition;

	uv1 = pVertex2.vTexUV - pVertex1.vTexUV;
	uv2 = pVertex3.vTexUV - pVertex1.vTexUV;


	float den;
	float length;

	// Calculate the denominator of the tangent/binormal equation.
	double tmpZeroCheck = (uv1.x * uv2.y - uv1.y * uv2.x);

	if (fabsf(tmpZeroCheck) < 1e-6f)    // almost equal to zero
	{
		tangent.x = 1.0f;
		tangent.y = 0.0f;
		tangent.z = 0.0f;

		binormal.x = 0.0f;
		binormal.y = 1.0f;
		binormal.z = 0.0f;
	}
	else
	{
		den = 1.0 / tmpZeroCheck;
		tangent = (uv2.y * vPos1 - uv1.y * vPos2) * den;
		binormal = (uv1.x * vPos2 - uv2.x * vPos1) * den;
	}



	D3DXVec3Normalize(&tangent, &tangent);
	D3DXVec3Normalize(&binormal, &binormal);
	D3DXVec3Normalize(&normal, &normal);

	D3DXVec3Cross(&binormal, &binormal, &normal);
	D3DXVec3Cross(&tangent, &tangent, &normal);

	D3DXVec3Normalize(&tangent, &tangent);
	D3DXVec3Normalize(&binormal, &binormal);
	
	pVertex3.vNormal += normal;
	pVertex3.vTangent.x += tangent.x;
	pVertex3.vTangent.y += tangent.y;
	pVertex3.vTangent.z += tangent.z;
	pVertex3.vBiNormal += binormal;

	pVertex2.vNormal += normal;
	pVertex2.vTangent.x += tangent.x;
	pVertex2.vTangent.y += tangent.y;
	pVertex2.vTangent.z += tangent.z;
	pVertex2.vBiNormal += binormal;

	pVertex1.vNormal += normal;
	pVertex1.vTangent.x += tangent.x;
	pVertex1.vTangent.y += tangent.y;
	pVertex1.vTangent.z += tangent.z;
	pVertex1.vBiNormal += binormal;
}



CTerrain_Buffer * CTerrain_Buffer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pHeightMapFilePath, const _float & fVtxInterval /*=1*/)
{
	CTerrain_Buffer* pInstance = new CTerrain_Buffer(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(pHeightMapFilePath, fVtxInterval)))
	{
		MSG_BOX("Create Terrain_Buffer failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTerrain_Buffer::Clone()
{
	return new CTerrain_Buffer(*this);
}

_ulong CTerrain_Buffer::Free()
{
	_ulong dwRefCount = 0;
	dwRefCount = CVIBuffer::Free();
	return dwRefCount;
}


