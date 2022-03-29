#include "..\Headers\Cube_Texture.h"



CCube_Texture::CCube_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CCube_Texture::CCube_Texture(const CCube_Texture & rhs)
	: CVIBuffer(rhs)//, m_vScale(rhs.m_vScale)
{
	//if (m_pColliderBoxVertex != nullptr)
	//{
		m_pColliderBoxVertex = new _vec3[rhs.m_iVertexCount];
		for (int i = 0; i < rhs.m_iVertexCount; ++i)
		{
			m_pColliderBoxVertex[i] = rhs.m_pVertexPos[i];
		}
	//}
}

// void CCube_Texture::Set_Scale(const _float & fSizeX, const _float & fSizeY, const _float & fSizeZ)
// {
// 	m_vScale = _vec3(fSizeX, fSizeY, fSizeZ);
// }

HRESULT CCube_Texture::Ready_Buffer()
{
/*	m_vScale = _vec3(1.f, 1.f, 1.f);*/

	m_iVertexCount = 8;
	m_iVertexSize = sizeof(VTXCUBETEX);
	m_dwVertexFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEX2 | D3DFVF_TEX3
		| D3DFVF_TEXCOORDSIZE3(0) | D3DFVF_TEXCOORDSIZE3(1) | D3DFVF_TEXCOORDSIZE3(2);

	m_iIndexSize = sizeof(INDEX16);
	m_iTriCount = 12;
	m_IndexFmt = D3DFMT_INDEX16;


	if (FAILED(CVIBuffer::Ready_Buffer()))
		goto exception;

	if (nullptr == m_pVB || nullptr == m_pIB)
		goto exception;

	_uint VertexBufferSize = 8;
	_uint IndexBufferSize = 12;
	_uint iIndex = 0;

	const _float x = 0.5f;
	const _float y = 0.5f;
	const _float z = 0.5f;

	VTXCUBETEX* pVertex = new VTXCUBETEX[VertexBufferSize];
	m_pColliderBoxVertex = new _vec3[8];
	m_pVertexPos = new _vec3[8];
	//VTXCUBETEX* pVertex = nullptr;


	pVertex[0].vPosition = _vec3(-x, y, -z);
	pVertex[0].vTexUV = pVertex[0].vPosition;
	pVertex[0].vNormal = _vec3(0.f, 0.f, 0.f);
	pVertex[0].vTangent = _vec3(0.f, 0.f, 0.f);
	pVertex[0].vBiNormal = _vec3(0.f, 0.f, 0.f);
	m_pVertexPos[0] = pVertex[0].vPosition;

	pVertex[1].vPosition = _vec3(x, y, -z);
	pVertex[1].vTexUV = pVertex[1].vPosition;
	pVertex[1].vNormal = _vec3(0.f, 0.f, 0.f);
	pVertex[1].vTangent = _vec3(0.f, 0.f, 0.f);
	pVertex[1].vBiNormal = _vec3(0.f, 0.f, 0.f);
	m_pVertexPos[1] = pVertex[1].vPosition;

	pVertex[2].vPosition = _vec3(x, -y, -z);
	pVertex[2].vTexUV = pVertex[2].vPosition;
	pVertex[2].vNormal = _vec3(0.f, 0.f, 0.f);
	pVertex[2].vTangent = _vec3(0.f, 0.f, 0.f);
	pVertex[2].vBiNormal = _vec3(0.f, 0.f, 0.f);
	m_pVertexPos[2] = pVertex[2].vPosition;

	pVertex[3].vPosition = _vec3(-x, -y, -z);
	pVertex[3].vTexUV = pVertex[3].vPosition;
	pVertex[3].vNormal = _vec3(0.f, 0.f, 0.f);
	pVertex[3].vTangent = _vec3(0.f, 0.f, 0.f);
	pVertex[3].vBiNormal = _vec3(0.f, 0.f, 0.f);
	m_pVertexPos[3] = pVertex[3].vPosition;

	pVertex[4].vPosition = _vec3(-x, y, z);
	pVertex[4].vTexUV = pVertex[4].vPosition;
	pVertex[4].vNormal = _vec3(0.f, 0.f, 0.f);
	pVertex[4].vTangent = _vec3(0.f, 0.f, 0.f);
	pVertex[4].vBiNormal = _vec3(0.f, 0.f, 0.f);
	m_pVertexPos[4] = pVertex[4].vPosition;

	pVertex[5].vPosition = _vec3(x, y, z);
	pVertex[5].vTexUV = pVertex[5].vPosition;
	pVertex[5].vNormal = _vec3(0.f, 0.f, 0.f);
	pVertex[5].vTangent = _vec3(0.f, 0.f, 0.f);
	pVertex[5].vBiNormal = _vec3(0.f, 0.f, 0.f);
	m_pVertexPos[5] = pVertex[5].vPosition;

	pVertex[6].vPosition = _vec3(x, -y, z);
	pVertex[6].vTexUV = pVertex[6].vPosition;
	pVertex[6].vNormal = _vec3(0.f, 0.f, 0.f);
	pVertex[6].vTangent = _vec3(0.f, 0.f, 0.f);
	pVertex[6].vBiNormal = _vec3(0.f, 0.f, 0.f);
	m_pVertexPos[6] = pVertex[6].vPosition;

	pVertex[7].vPosition = _vec3(-x, -y, z);
	pVertex[7].vTexUV = pVertex[7].vPosition;
	pVertex[7].vNormal = _vec3(0.f, 0.f, 0.f);
	pVertex[7].vTangent = _vec3(0.f, 0.f, 0.f);
	pVertex[7].vBiNormal = _vec3(0.f, 0.f, 0.f);
	m_pVertexPos[7] = pVertex[7].vPosition;


// 	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, -0.5f);
// 	pVertex[0].vTexUV = pVertex[0].vPosition;
// 	m_pVertexPos[0] = pVertex[0].vPosition;
// 
// 	pVertex[1].vPosition = _vec3(0.5f, 0.5f, -0.5f);
// 	pVertex[1].vTexUV = pVertex[1].vPosition;
// 	m_pVertexPos[1] = pVertex[1].vPosition;
// 
// 	pVertex[2].vPosition = _vec3(0.5f, -0.5f, -0.5f);
// 	pVertex[2].vTexUV = pVertex[2].vPosition;
// 	m_pVertexPos[2] = pVertex[2].vPosition;
// 
// 	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, -0.5f);
// 	pVertex[3].vTexUV = pVertex[3].vPosition;
// 	m_pVertexPos[3] = pVertex[3].vPosition;
// 
// 	pVertex[4].vPosition = _vec3(-0.5f, 0.5f, 0.5f);
// 	pVertex[4].vTexUV = pVertex[4].vPosition;
// 	m_pVertexPos[4] = pVertex[4].vPosition;
// 
// 	pVertex[5].vPosition = _vec3(0.5f, 0.5f, 0.5f);
// 	pVertex[5].vTexUV = pVertex[5].vPosition;
// 	m_pVertexPos[5] = pVertex[5].vPosition;
// 
// 	pVertex[6].vPosition = _vec3(0.5f, -0.5f, 0.5f);
// 	pVertex[6].vTexUV = pVertex[6].vPosition;
// 
// 
// 	pVertex[7].vPosition = _vec3(-0.5f, -0.5f, 0.5f);
// 	pVertex[7].vTexUV = pVertex[7].vPosition;
	


	INDEX16* pIndex = new INDEX16[IndexBufferSize];

	//INDEX16* pIndex = nullptr;
	//m_pIndexPos = new _vec3[12];


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



	for (int i = 0; i < IndexBufferSize; ++i)
	{
		_vec3 vPolygonNormal, v1, v2;
		v1 = pVertex[pIndex[i]._2].vPosition - pVertex[pIndex[i]._1].vPosition;
		v2 = pVertex[pIndex[i]._1].vPosition - pVertex[pIndex[i]._0].vPosition;
		D3DXVec3Cross(&vPolygonNormal, &v2, &v1);

		CalcTangentVector(pVertex[pIndex[i]._0], pVertex[pIndex[i]._1], pVertex[pIndex[i]._2], vPolygonNormal);

		//pVertex[pIndex[i]._0].vNormal += vPolygonNormal;
		//pVertex[pIndex[i]._1].vNormal += vPolygonNormal;
		//pVertex[pIndex[i]._2].vNormal += vPolygonNormal;
	}

	//for (int i = 0; i < VertexBufferSize; ++i)
	//{
	//	D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);
	//}

	//for (int i = 0; i < IndexBufferSize; ++i)
	//{
	//	CalcTangentVector(pVertex[pIndex[i]._0], pVertex[pIndex[i]._1], pVertex[pIndex[i]._2]);
	//}

	for (int i = 0; i < VertexBufferSize; ++i)
	{
		D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);
		D3DXVec3Normalize(&pVertex[i].vTangent, &pVertex[i].vTangent);
		D3DXVec3Normalize(&pVertex[i].vBiNormal, &pVertex[i].vBiNormal);
	}

	VTXCUBETEX*			pVertexBuffer = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertexBuffer, 0);
	memcpy(pVertexBuffer, pVertex, sizeof(VTXCUBETEX)*VertexBufferSize);
	m_pVB->Unlock();

	INDEX16*			pIndexBuffer = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndexBuffer, 0);
	memcpy(pIndexBuffer, pIndex, sizeof(INDEX16)*IndexBufferSize);
	m_pIB->Unlock();

	Engine::Safe_Delete_Array(pVertex);
	Engine::Safe_Delete_Array(pIndex);



	return NO_ERROR;

exception:
	MSG_BOX("Cube_Texture Ready Failed");
	return E_FAIL;
}

void CCube_Texture::Set_Position(const _matrix & matWorld)
{
	for (int i = 0; i < m_iVertexCount; ++i)
	{
		D3DXVec3TransformCoord(&m_pColliderBoxVertex[i], &m_pVertexPos[i], &matWorld);
	}
}

void CCube_Texture::CalcTangentVector(VTXCUBETEX & pVertex1, VTXCUBETEX & pVertex2, VTXCUBETEX & pVertex3, _vec3 vNormal)
{
	int faceCount, i;
	//TempVertexType vertex1, vertex2, vertex3;
	_vec3 vPos1, vPos2;
	D3DXVECTOR3 uv1, uv2;
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

CCube_Texture * CCube_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCube_Texture* pInstance = new CCube_Texture(pGraphicDev);
	if (FAILED(pInstance->Ready_Buffer()))
	{
		MSG_BOX("Cube_Texture creation failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCube_Texture::Clone()
{
	return new CCube_Texture(*this);
}

_ulong CCube_Texture::Free()
{
	return CVIBuffer::Free();
}
