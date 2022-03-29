#include "..\Headers\Shader.h"



CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CShader::CShader(const CShader & rhs)
	:CComponent(rhs), m_vecShader(rhs.m_vecShader)
{
	for (auto& pShader : m_vecShader)
		pShader->AddRef();

}

HRESULT CShader::Ready_Shader(ShaderTYPE eType, const _tchar * pFilePath, _uint iSdCount)
{
	m_vecShader.reserve(iSdCount);


	for (size_t i=0; i<iSdCount; ++i)
	{
		_tchar szFilePath[128] = L"";
		wsprintf(szFilePath, pFilePath, i);

		if (SDTYPE_NORMAL == eType)
		{
			LPD3DXEFFECT pShader = nullptr;
			LoadShader(m_pGraphicDev, szFilePath, &pShader);
			m_vecShader.push_back(pShader);
		}

	}
	return NOERROR;
}

//HRESULT CShader::SetUp_ShaderToGraphicDev(_uint dwStage, _uint iShaderIndex)
//{
//	if (iShaderIndex >= m_vecShader.size())
//	{
//		MSG_BOX("Received ShaderIndex over vector size err:89518");
//		return E_FAIL;
//	}
//
//	return NOERROR;
//}

HRESULT CShader::LoadShader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pFilePath, LPD3DXEFFECT* ppShader)
{

	LPD3DXEFFECT ret = NULL;
	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(pGraphicDev, pFilePath,
		NULL, NULL, dwShaderFlags, NULL, &ret, &pError);

	// 쉐이더 로딩에 실패한 경우 output창에 쉐이더
	// 컴파일 에러를 출력한다.
	if (!ret && pError)
	{
		int size = pError->GetBufferSize();
		void *ack = pError->GetBufferPointer();

		if (ack)
		{
			_tchar* str = new _tchar[size];
			wsprintf(str, (const _tchar*)ack, size);
			MessageBox(0, str, TEXT("System Err"), MB_OK);
			delete[] str;
			return E_FAIL;
		}
	}

	*ppShader = ret;

	return NOERROR;

}

LPD3DXEFFECT CShader::GetShader(const _uint & iSdCount)
{
	if (m_vecShader.empty() || m_vecShader.size() < iSdCount || iSdCount<=0)
		return nullptr;
	
	return m_vecShader[iSdCount - 1];
}


CShader * CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, ShaderTYPE eType, const _tchar * pFilePath, const _uint & iSdCount)
{
	CShader* pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Shader(eType, pFilePath, iSdCount)))
	{
		MSG_BOX("CShader creation failed err: 994865");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CShader::Clone()
{
	return new CShader(*this);
}

_ulong CShader::Free()
{
	_ulong dwRefCount = 0;

	for (auto& pShader : m_vecShader)
		Safe_Release(pShader);
	
	m_vecShader.clear();

	CComponent::Free();

	return dwRefCount;
}

