#include "..\Headers\Jang_Terrain_Buffer.h"



Jang_CTerrain_Buffer::Jang_CTerrain_Buffer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{

}

Jang_CTerrain_Buffer::Jang_CTerrain_Buffer(const Jang_CTerrain_Buffer& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT Jang_CTerrain_Buffer::Ready_Buffer(const _tchar* pHeightMapFilePath, const _float & fVtxInterval)
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
	m_iVertexSize = sizeof(VTXTEX_J2);
	m_iVertexCount = dwVtxCountX * dwVtxCountZ;
	//m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEX2  | D3DFVF_TEXCOORDSIZE4(1) | D3DFVF_TEX3;
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
	VTXTEX_J2*	pVertex = new VTXTEX_J2[VertexBufferSize];


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
				j / _float(dwVtxCountX - 1) * (fUVCount) ,
				1 - i / _float(dwVtxCountZ - 1) * (fUVCount)
			);
			
			pVertex[dwIndex].vTangent = _vec3(0.f, 0.f, 0.f);
			pVertex[dwIndex].vBiNormal = _vec3(0.f, 0.f, 0.f);

			m_pVertexPos[dwIndex] = pVertex[dwIndex].vPosition;
		}
	}
	//		tangent.x = 1.0f;
	//		tangent.y = 0.0f;
	//		tangent.z = 0.0f;
	//
	//		bitangent.x = 0.0f;
	//		bitangent.y = 1.0f;
	//		bitangent.z = 0.0f;

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

			/*------First Polygon--------*/
			/*Set Index*/
			pIndex[iIndex]._0 = iLT;
			pIndex[iIndex]._1 = iRT;
			pIndex[iIndex]._2 = iRB;

			_vec3 vPolygonNormal, v1, v2;

			/*accumulate NormalVector First Polygon*/
			//v1 = pVertex[iRB].vPosition - pVertex[iRT].vPosition;
			//v2 = pVertex[iLT].vPosition - pVertex[iRT].vPosition;
			v1 = pVertex[pIndex[iIndex]._2].vPosition - pVertex[pIndex[iIndex]._1].vPosition;
			v2 = pVertex[pIndex[iIndex]._1].vPosition - pVertex[pIndex[iIndex]._0].vPosition;
			D3DXVec3Cross(&vPolygonNormal, &v2, &v1);

			CalcTangentVector(pVertex[pIndex[iIndex]._0], pVertex[pIndex[iIndex]._1], pVertex[pIndex[iIndex]._2], vPolygonNormal);

			//pVertex[pIndex[iIndex]._0].vNormal += vPolygonNormal;
			//pVertex[pIndex[iIndex]._1].vNormal += vPolygonNormal;
			//pVertex[pIndex[iIndex]._2].vNormal += vPolygonNormal;

			//CalcTangentVector(&pVertex[pIndex[iIndex]._0].vPosition, &pVertex[pIndex[iIndex]._1].vPosition, &pVertex[pIndex[iIndex]._2].vPosition,
			//				&pVertex[pIndex[iIndex]._0].vTexUV, &pVertex[pIndex[iIndex]._1].vTexUV, &pVertex[pIndex[iIndex]._2].vTexUV,
			//				&pVertex[pIndex[iIndex]._0].vNormal, vTangent);

			//pVertex[pIndex[iIndex]._0].vTangent = vTangent;
			//pVertex[pIndex[iIndex]._1].vTangent = vTangent;
			//pVertex[pIndex[iIndex]._2].vTangent = vTangent;
			++iIndex;

			/*------Second Polygon--------*/
			/*set Index*/
			pIndex[iIndex]._0 = iLT;
			pIndex[iIndex]._1 = iRB;
			pIndex[iIndex]._2 = iLB;

			//v1 = pVertex[iLT].vPosition - pVertex[iLB].vPosition;
			//v2 = pVertex[iRB].vPosition - pVertex[iLB].vPosition;
			v1 = pVertex[pIndex[iIndex]._2].vPosition - pVertex[pIndex[iIndex]._1].vPosition;
			v2 = pVertex[pIndex[iIndex]._1].vPosition - pVertex[pIndex[iIndex]._0].vPosition;
			D3DXVec3Cross(&vPolygonNormal, &v2, &v1);
			
			CalcTangentVector(pVertex[pIndex[iIndex]._0], pVertex[pIndex[iIndex]._1], pVertex[pIndex[iIndex]._2], vPolygonNormal);

			//pVertex[pIndex[iIndex]._0].vNormal += vPolygonNormal;
			//pVertex[pIndex[iIndex]._1].vNormal += vPolygonNormal;
			//pVertex[pIndex[iIndex]._2].vNormal += vPolygonNormal;

			//CalcTangentVector(&pVertex[pIndex[iIndex]._0].vPosition, &pVertex[pIndex[iIndex]._1].vPosition, &pVertex[pIndex[iIndex]._2].vPosition,
			//	&pVertex[pIndex[iIndex]._0].vTexUV, &pVertex[pIndex[iIndex]._1].vTexUV, &pVertex[pIndex[iIndex]._2].vTexUV,
			//	&pVertex[pIndex[iIndex]._0].vNormal, vTangent);

			//pVertex[pIndex[iIndex]._0].vTangent = vTangent;
			//pVertex[pIndex[iIndex]._1].vTangent = vTangent;
			//pVertex[pIndex[iIndex]._2].vTangent = vTangent;

			++iIndex;

		}
	}

	for (size_t iIndex = 0; iIndex < VertexBufferSize; ++iIndex)
	{
		D3DXVec3Normalize(&pVertex[iIndex].vNormal, &pVertex[iIndex].vNormal);
		D3DXVec3Normalize(&pVertex[iIndex].vTangent, &pVertex[iIndex].vTangent);
		D3DXVec3Normalize(&pVertex[iIndex].vBiNormal, &pVertex[iIndex].vBiNormal);
	}

	

	//for (size_t iIndex = 0; iIndex < VertexBufferSize; ++iIndex)
	//{
	//	D3DXVec3Normalize(&pVertex[iIndex].vNormal, &pVertex[iIndex].vNormal);
	//	//D3DXVec3Normalize(&pVertex[iIndex].vTangent, &pVertex[iIndex].vTangent);
	//	//D3DXVec3Normalize(&pVertex[iIndex].vBiNormal, &pVertex[iIndex].vBiNormal);
	//}
	////CalcTangentVector(pVertex, VertexBufferSize);
	//iIndex = 0;
	//for (size_t i = 0; i < dwVtxCountZ - 1; ++i)
	//{
	//	for (size_t j = 0; j < dwVtxCountX - 1; ++j)
	//	{
	//		_uint		iVtxIdx = i * dwVtxCountX + j;
	//		_uint		iLT, iRT, iLB, iRB;
	//		D3DXVECTOR4 vTangent;

	//		iLT = iVtxIdx + dwVtxCountX;
	//		iRT = iLT + 1;
	//		iLB = iVtxIdx;
	//		iRB = iLB + 1;

	//		/*------First Polygon--------*/
	//		/*Set Index*/
	//		pIndex[iIndex]._0 = iLT;
	//		pIndex[iIndex]._1 = iRT;
	//		pIndex[iIndex]._2 = iRB;

	//		CalcTangentVector(pVertex[pIndex[iIndex]._0], pVertex[pIndex[iIndex]._1], pVertex[pIndex[iIndex]._2]);

	//		++iIndex;

	//		/*------Second Polygon--------*/
	//		/*set Index*/
	//		pIndex[iIndex]._0 = iLT;
	//		pIndex[iIndex]._1 = iRB;
	//		pIndex[iIndex]._2 = iLB;

	//		CalcTangentVector(pVertex[pIndex[iIndex]._0], pVertex[pIndex[iIndex]._1], pVertex[pIndex[iIndex]._2]);

	//		++iIndex;

	//	}
	//}


	//for (size_t iIndex = 0; iIndex < VertexBufferSize; ++iIndex)
	//{
	//	D3DXVec3Normalize(&pVertex[iIndex].vNormal, &pVertex[iIndex].vNormal);
	//	D3DXVec3Normalize(&pVertex[iIndex].vTangent, &pVertex[iIndex].vTangent);
	//	D3DXVec3Normalize(&pVertex[iIndex].vBiNormal, &pVertex[iIndex].vBiNormal);
	//}

	//CalcTangentVector(pVertex, VertexBufferSize);

	//iIndex = 0;
	//for (size_t i = 0; i < dwVtxCountZ - 1; ++i)
	//{
	//	for (size_t j = 0; j < dwVtxCountX - 1; ++j)
	//	{
	//		_uint		iVtxIdx = i * dwVtxCountX + j;
	//		_uint		iLT, iRT, iLB, iRB;
	//		D3DXVECTOR4 vTangent;

	//		iLT = iVtxIdx + dwVtxCountX;
	//		iRT = iLT + 1;
	//		iLB = iVtxIdx;
	//		iRB = iLB + 1;

	//		/*------First Polygon--------*/
	//		pIndex[iIndex]._0 = iLT;
	//		pIndex[iIndex]._1 = iRT;
	//		pIndex[iIndex]._2 = iRB;

	//		CalcTangentVector(&pVertex[pIndex[iIndex]._0].vPosition, &pVertex[pIndex[iIndex]._1].vPosition, &pVertex[pIndex[iIndex]._2].vPosition,
	//						&pVertex[pIndex[iIndex]._0].vTexUV, &pVertex[pIndex[iIndex]._1].vTexUV, &pVertex[pIndex[iIndex]._2].vTexUV,
	//						&pVertex[pIndex[iIndex]._0].vNormal, vTangent);
	//		pVertex[pIndex[iIndex]._0].vTangent = vTangent;
	//		pVertex[pIndex[iIndex]._1].vTangent = vTangent;
	//		pVertex[pIndex[iIndex]._2].vTangent = vTangent;
	//		++iIndex;

	//		/*------Second Polygon--------*/
	//		//pIndex[iIndex]._0 = iLT;
	//		//pIndex[iIndex]._1 = iRB;
	//		//pIndex[iIndex]._2 = iLB;
	//		//CalcTangentVector(&pVertex[pIndex[iIndex]._0].vPosition, &pVertex[pIndex[iIndex]._1].vPosition, &pVertex[pIndex[iIndex]._2].vPosition,
	//		//	&pVertex[pIndex[iIndex]._0].vTexUV, &pVertex[pIndex[iIndex]._1].vTexUV, &pVertex[pIndex[iIndex]._2].vTexUV,
	//		//	&pVertex[pIndex[iIndex]._0].vNormal, vTangent);
	//		//pVertex[pIndex[iIndex]._0].vTangent = vTangent;
	//		//pVertex[pIndex[iIndex]._1].vTangent = vTangent;
	//		//pVertex[pIndex[iIndex]._2].vTangent = vTangent;
	//		++iIndex;
	//	}
	//}


	VTXTEX_J2*			pVertexBuffer = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertexBuffer, 0);
	memcpy(pVertexBuffer, pVertex, sizeof(VTXTEX_J2)*VertexBufferSize);
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

Jang_CTerrain_Buffer * Jang_CTerrain_Buffer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pHeightMapFilePath, const _float & fVtxInterval /*=1*/)
{
	Jang_CTerrain_Buffer* pInstance = new Jang_CTerrain_Buffer(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(pHeightMapFilePath, fVtxInterval)))
	{
		MSG_BOX("Create Terrain_Buffer failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * Jang_CTerrain_Buffer::Clone()
{
	return new Jang_CTerrain_Buffer(*this);
}

_ulong Jang_CTerrain_Buffer::Free()
{
	_ulong dwRefCount = 0;
	dwRefCount = CVIBuffer::Free();
	return dwRefCount;
}

void Jang_CTerrain_Buffer::CalcTangentVector(VTXTEX_J2* pVertex, _int BufferSize)
{

	//struct TempVertexType
	//{
	//	float x, y, z;
	//	float tu, tv;
	//	float nx, ny, nz;
	//};
	//struct VectorType
	//{
	//	float x, y, z;
	//};


	int faceCount, i, index;
	//TempVertexType vertex1, vertex2, vertex3;
	_vec3 vPos1, vPos2;
	D3DXVECTOR2 uv1, uv2;
	_vec3 tangent, binormal, normal;

	// Calculate the number of faces in the model.
	faceCount = BufferSize / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (i = 0; i<faceCount; i++)
	{
		// Get the three vertices for this face from the model.

		vPos1 = pVertex[index].vPosition - pVertex[index + 1].vPosition;
		vPos2 = pVertex[index+2].vPosition - pVertex[index].vPosition;

		uv1 = pVertex[index].vTexUV - pVertex[index + 1].vTexUV;
		uv2 = pVertex[index+2].vTexUV - pVertex[index].vTexUV;

		// -------------- Calculate the tangent and binormal of that face.
		
		//CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);
		
		//D3DXVec3Normalize(&vPos1, &vPos1);
		//D3DXVec3Normalize(&vPos2, &vPos2);
		//D3DXVec2Normalize(&uv1, &uv1);
		//D3DXVec2Normalize(&uv2, &uv2);

		float den;
		float length;

		// Calculate the denominator of the tangent/binormal equation.
		double tmpZeroCheck = (uv1.x * uv2.y - uv2.x * uv1.y);
		
		//if (uv1.y < 1.f || uv2.y < 1.f) 
		//{
		//	tangent.x = 1.0f;
		//	
		//}
		if (index > 1000)
		{
			tangent.x = 1.0f;
		}
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

		//D3DXVec3Normalize(&tangent, &tangent);
		//D3DXVec3Normalize(&binormal, &binormal);
		// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
		

		//// Calculate the length of this normal.
		//length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

		//// Normalize the normal and then store it
		//tangent.x = tangent.x / length;
		//tangent.y = tangent.y / length;
		//tangent.z = tangent.z / length;

		//// Calculate the length of this normal.
		//length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

		//// Normalize the normal and then store it
		//binormal.x = binormal.x / length;
		//binormal.y = binormal.y / length;
		//binormal.z = binormal.z / length;

		////  ------------- Calculate the new normal using the tangent and binormal.
		////CalculateNormal(tangent, binormal, normal);
		D3DXVec3Normalize(&tangent, &tangent);
		D3DXVec3Normalize(&binormal, &binormal);
		//// Calculate the cross product of the tangent and binormal which will give the normal vector.
		normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
		normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
		normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

		//// Calculate the length of the normal.
		//length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

		//// Normalize the normal.
		//normal.x = normal.x / length;
		//normal.y = normal.y / length;
		//normal.z = normal.z / length;

		//D3DXVec3Normalize(&tangent, &tangent);
		//D3DXVec3Normalize(&binormal, &binormal);
		D3DXVec3Normalize(&normal, &normal);

	
		// ------  Store the normal, tangent, and binormal for this face back in the model structure.
		//pVertex[index + 2].vNormal = normal;
		//pVertex[index + 2].vTangent.x = tangent.x;
		//pVertex[index + 2].vTangent.y = tangent.y;
		//pVertex[index + 2].vTangent.z = tangent.z;
		//pVertex[index + 2].vBiNormal = binormal;

		//pVertex[index + 1].vNormal = normal;
		//pVertex[index + 1].vTangent.x = tangent.x;
		//pVertex[index + 1].vTangent.y = tangent.y;
		//pVertex[index + 1].vTangent.z = tangent.z;
		//pVertex[index + 1].vBiNormal = binormal;

		//pVertex[index].vNormal = normal;
		//pVertex[index].vTangent.x = tangent.x;
		//pVertex[index].vTangent.y = tangent.y;
		//pVertex[index].vTangent.z = tangent.z;
		//pVertex[index].vBiNormal = binormal;
		//pVertex[index + 2].vNormal += normal;
		pVertex[index + 2].vTangent.x += tangent.x;
		pVertex[index + 2].vTangent.y += tangent.y;
		pVertex[index + 2].vTangent.z += tangent.z;
		pVertex[index + 2].vBiNormal += binormal;

		//pVertex[index + 1].vNormal += normal;
		pVertex[index + 1].vTangent.x += tangent.x;
		pVertex[index + 1].vTangent.y += tangent.y;
		pVertex[index + 1].vTangent.z += tangent.z;
		pVertex[index + 1].vBiNormal += binormal;

		//pVertex[index].vNormal += normal;
		pVertex[index].vTangent.x += tangent.x;
		pVertex[index].vTangent.y += tangent.y;
		pVertex[index].vTangent.z += tangent.z;
		pVertex[index].vBiNormal += binormal;


		//D3DXVECTOR3 b;
		//D3DXVec3Cross(&b, &normal, &tangent);
		//
		//if (D3DXVec3Dot(&b, &binormal) < 0.0f)
		//{
		//	pVertex[index + 2].vTangent.w = -1.f;
		//	pVertex[index + 1].vTangent.w = -1.f;
		//	pVertex[index].vTangent.w = -1.f;
		//}
		//else 
		//{
		//	pVertex[index + 2].vTangent.w = 1.f;
		//	pVertex[index + 1].vTangent.w = 1.f;
		//	pVertex[index].vTangent.w = 1.f;
		//}

		++index;
		++index;
		++index;
	}

	return;
}

void Jang_CTerrain_Buffer::CalcTangentVector(VTXTEX_J2& pVertex1, VTXTEX_J2& pVertex2, VTXTEX_J2& pVertex3)
{

	int faceCount, i;
	//TempVertexType vertex1, vertex2, vertex3;
	_vec3 vPos1, vPos2;
	D3DXVECTOR2 uv1, uv2;
	_vec3 tangent, binormal, normal;


	vPos1 = pVertex1.vPosition - pVertex2.vPosition;
	vPos2 = pVertex3.vPosition - pVertex1.vPosition;

	uv1 = pVertex1.vTexUV - pVertex2.vTexUV;
	uv2 = pVertex3.vTexUV - pVertex1.vTexUV;


	float den;
	float length;

	// Calculate the denominator of the tangent/binormal equation.
	double tmpZeroCheck = (uv1.x * uv2.y - uv2.x * uv1.y);

	//if (uv1.y < 1.f || uv2.y < 1.f) 
	//{
	//	tangent.x = 1.0f;
	//	
	//}
	//if (index > 1000)
	//{
	//	tangent.x = 1.0f;
	//}
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

	//D3DXVec3Normalize(&tangent, &tangent);
	//D3DXVec3Normalize(&binormal, &binormal);
	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.


	//// Calculate the length of this normal.
	//length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	//// Normalize the normal and then store it
	//tangent.x = tangent.x / length;
	//tangent.y = tangent.y / length;
	//tangent.z = tangent.z / length;

	//// Calculate the length of this normal.
	//length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	//// Normalize the normal and then store it
	//binormal.x = binormal.x / length;
	//binormal.y = binormal.y / length;
	//binormal.z = binormal.z / length;

	////  ------------- Calculate the new normal using the tangent and binormal.
	////CalculateNormal(tangent, binormal, normal);
	D3DXVec3Normalize(&tangent, &tangent);
	D3DXVec3Normalize(&binormal, &binormal);
	//// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	//// Calculate the length of the normal.
	//length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	//// Normalize the normal.
	//normal.x = normal.x / length;
	//normal.y = normal.y / length;
	//normal.z = normal.z / length;

	//D3DXVec3Normalize(&tangent, &tangent);
	//D3DXVec3Normalize(&binormal, &binormal);
	D3DXVec3Normalize(&normal, &normal);


	// ------  Store the normal, tangent, and binormal for this face back in the model structure.
	//pVertex3.vNormal = normal;
	//pVertex3.vTangent.x = tangent.x;
	//pVertex3.vTangent.y = tangent.y;
	//pVertex3.vTangent.z = tangent.z;
	//pVertex3.vBiNormal = binormal;

	//pVertex2.vNormal = normal;
	//pVertex2.vTangent.x = tangent.x;
	//pVertex2.vTangent.y = tangent.y;
	//pVertex2.vTangent.z = tangent.z;
	//pVertex2.vBiNormal = binormal;

	//pVertex1.vNormal = normal;
	//pVertex1.vTangent.x = tangent.x;
	//pVertex1.vTangent.y = tangent.y;
	//pVertex1.vTangent.z = tangent.z;
	//pVertex1.vBiNormal = binormal;
	//pVertex3.vNormal += normal;
	pVertex3.vTangent.x += tangent.x;
	pVertex3.vTangent.y += tangent.y;
	pVertex3.vTangent.z += tangent.z;
	pVertex3.vBiNormal += binormal;

	//pVertex2.vNormal += normal;
	pVertex2.vTangent.x += tangent.x;
	pVertex2.vTangent.y += tangent.y;
	pVertex2.vTangent.z += tangent.z;
	pVertex2.vBiNormal += binormal;

	//pVertex1.vNormal += normal;
	pVertex1.vTangent.x += tangent.x;
	pVertex1.vTangent.y += tangent.y;
	pVertex1.vTangent.z += tangent.z;
	pVertex1.vBiNormal += binormal;



	return;
}

void Jang_CTerrain_Buffer::CalcTangentVector(VTXTEX_J2 & pVertex1, VTXTEX_J2 & pVertex2, VTXTEX_J2 & pVertex3, _vec3 vNormal)
{
	int faceCount, i;
	//TempVertexType vertex1, vertex2, vertex3;
	_vec3 vPos1, vPos2;
	D3DXVECTOR2 uv1, uv2;
	_vec3 tangent, binormal, normal = vNormal;


	//vPos1 = pVertex1.vPosition - pVertex2.vPosition;
	//vPos2 = pVertex3.vPosition - pVertex1.vPosition;

	//uv1 = pVertex1.vTexUV - pVertex2.vTexUV;
	//uv2 = pVertex3.vTexUV - pVertex1.vTexUV;

	vPos1 = pVertex2.vPosition - pVertex1.vPosition;
	vPos2 = pVertex3.vPosition - pVertex1.vPosition;

	uv1 = pVertex2.vTexUV - pVertex1.vTexUV;
	uv2 = pVertex3.vTexUV - pVertex1.vTexUV;


	float den;
	float length;

	// Calculate the denominator of the tangent/binormal equation.
	double tmpZeroCheck = (uv1.x * uv2.y - uv1.y * uv2.x);

	//if (uv1.y < 1.f || uv2.y < 1.f) 
	//{
	//	tangent.x = 1.0f;
	//	
	//}
	//if (index > 1000)
	//{
	//	tangent.x = 1.0f;
	//}
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



	//D3DXVec3Normalize(&tangent, &tangent);
	//D3DXVec3Normalize(&binormal, &binormal);
	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.


	//// Calculate the length of this normal.
	//length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	//// Normalize the normal and then store it
	//tangent.x = tangent.x / length;
	//tangent.y = tangent.y / length;
	//tangent.z = tangent.z / length;

	//// Calculate the length of this normal.
	//length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	//// Normalize the normal and then store it
	//binormal.x = binormal.x / length;
	//binormal.y = binormal.y / length;
	//binormal.z = binormal.z / length;

	////  ------------- Calculate the new normal using the tangent and binormal.
	////CalculateNormal(tangent, binormal, normal);
	D3DXVec3Normalize(&tangent, &tangent);
	D3DXVec3Normalize(&binormal, &binormal);
	D3DXVec3Normalize(&normal, &normal);

	//// Calculate the cross product of the tangent and binormal which will give the normal vector.
	D3DXVec3Cross(&binormal, &binormal, &normal);
	D3DXVec3Cross(&tangent, &tangent, &normal);

	D3DXVec3Normalize(&tangent, &tangent);
	D3DXVec3Normalize(&binormal, &binormal);

	//normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	//normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	//normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	//// Calculate the length of the normal.
	//length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	//// Normalize the normal.
	//normal.x = normal.x / length;
	//normal.y = normal.y / length;
	//normal.z = normal.z / length;

	//D3DXVec3Normalize(&tangent, &tangent);
	//D3DXVec3Normalize(&binormal, &binormal);

	// ------  Store the normal, tangent, and binormal for this face back in the model structure.
	//pVertex3.vNormal = normal;
	//pVertex3.vTangent.x = tangent.x;
	//pVertex3.vTangent.y = tangent.y;
	//pVertex3.vTangent.z = tangent.z;
	//pVertex3.vBiNormal = binormal;

	//pVertex2.vNormal = normal;
	//pVertex2.vTangent.x = tangent.x;
	//pVertex2.vTangent.y = tangent.y;
	//pVertex2.vTangent.z = tangent.z;
	//pVertex2.vBiNormal = binormal;

	//pVertex1.vNormal = normal;
	//pVertex1.vTangent.x = tangent.x;
	//pVertex1.vTangent.y = tangent.y;
	//pVertex1.vTangent.z = tangent.z;
	//pVertex1.vBiNormal = binormal;
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

//void Jang_CTerrain_Buffer::CalcTangentVector(_vec3* pos1, _vec3* pos2, _vec3* pos3, D3DXVECTOR2* texCoord1, D3DXVECTOR2* texCoord2, D3DXVECTOR2* texCoord3, _vec3* normal, D3DXVECTOR4& tangent)
//{
//	D3DXVECTOR3 edge1(pos2->x - pos1->x, pos2->y - pos1->y, pos2->z - pos1->z);
//	D3DXVECTOR3 edge2(pos3->x - pos1->x, pos3->y - pos1->y, pos3->z - pos1->z);
//
//	D3DXVec3Normalize(&edge1, &edge1);
//	D3DXVec3Normalize(&edge2, &edge2);
//
//	// Create 2 vectors in tangent (texture) space that point in the same
//	// direction as edge1 and edge2 (in object space).
//	//
//	// texEdge1 is the vector from texture coordinates texCoord1 to texCoord2.
//	// texEdge2 is the vector from texture coordinates texCoord1 to texCoord3.
//	D3DXVECTOR2 texEdge1(texCoord2->x - texCoord1->x, texCoord2->y - texCoord1->y);
//	D3DXVECTOR2 texEdge2(texCoord3->x - texCoord1->x, texCoord3->y - texCoord1->y);
//
//	D3DXVec2Normalize(&texEdge1, &texEdge1);
//	D3DXVec2Normalize(&texEdge2, &texEdge2);
//
//	// These 2 sets of vectors form the following system of equations:
//	//
//	//  edge1 = (texEdge1.x * tangent) + (texEdge1.y * bitangent)
//	//  edge2 = (texEdge2.x * tangent) + (texEdge2.y * bitangent)
//	//
//	// Using matrix notation this system looks like:
//	//
//	//  [ edge1 ]     [ texEdge1.x  texEdge1.y ]  [ tangent   ]
//	//  [       ]  =  [                        ]  [           ]
//	//  [ edge2 ]     [ texEdge2.x  texEdge2.y ]  [ bitangent ]
//	//
//	// The solution is:
//	//
//	//  [ tangent   ]        1     [ texEdge2.y  -texEdge1.y ]  [ edge1 ]
//	//  [           ]  =  -------  [                         ]  [       ]
//	//  [ bitangent ]      det A   [-texEdge2.x   texEdge1.x ]  [ edge2 ]
//	//
//	//  where:
//	//        [ texEdge1.x  texEdge1.y ]
//	//    A = [                        ]
//	//        [ texEdge2.x  texEdge2.y ]
//	//
//	//    det A = (texEdge1.x * texEdge2.y) - (texEdge1.y * texEdge2.x)
//	//
//	// From this solution the tangent space basis vectors are:
//	//
//	  //  tangent = (1 / det A) * ( texEdge2.y * edge1 - texEdge1.y * edge2)
//	  //bitangent = (1 / det A) * (-texEdge2.x * edge1 + texEdge1.x * edge2)
//	  //   normal = cross(tangent, bitangent)
//
//	D3DXVECTOR3 bitangent;
//	float det = (texEdge1.x * texEdge2.y) - (texEdge1.y * texEdge2.x);
//
//	if (fabsf(det) < 1e-6f)    // almost equal to zero
//	{
//		tangent.x = 1.0f;
//		tangent.y = 0.0f;
//		tangent.z = 0.0f;
//
//		bitangent.x = 0.0f;
//		bitangent.y = 1.0f;
//		bitangent.z = 0.0f;
//	}
//	else
//	{
//		det = 1.0f / det;
//
//		tangent.x = (texEdge2.y * edge1.x - texEdge1.y * edge2.x) * det;
//		tangent.y = (texEdge2.y * edge1.y - texEdge1.y * edge2.y) * det;
//		tangent.z = (texEdge2.y * edge1.z - texEdge1.y * edge2.z) * det;
//		tangent.w = 0.0f;
//
//		bitangent.x = (-texEdge2.x * edge1.x + texEdge1.x * edge2.x) * det;
//		bitangent.y = (-texEdge2.x * edge1.y + texEdge1.x * edge2.y) * det;
//		bitangent.z = (-texEdge2.x * edge1.z + texEdge1.x * edge2.z) * det;
//
//		D3DXVec4Normalize(&tangent, &tangent);
//		D3DXVec3Normalize(&bitangent, &bitangent);
//	}
//
//	// Calculate the handedness of the local tangent space.
//	// The bitangent vector is the cross product between the triangle face
//	// normal vector and the calculated tangent vector. The resulting bitangent
//	// vector should be the same as the bitangent vector calculated from the
//	// set of linear equations above. If they point in different directions
//	// then we need to invert the cross product calculated bitangent vector. We
//	// store this scalar multiplier in the tangent vector's 'w' component so
//	// that the correct bitangent vector can be generated in the normal mapping
//	// shader's vertex shader.
//
//	D3DXVECTOR3 n(normal->x, normal->y, normal->z);
//	D3DXVECTOR3 t(tangent.x, tangent.y, tangent.z);
//	D3DXVECTOR3 b;
//
//	D3DXVec3Cross(&b, &n, &t);
//	tangent.w = (D3DXVec3Dot(&b, &bitangent) < 0.0f) ? -1.0f : 1.0f;
//}


