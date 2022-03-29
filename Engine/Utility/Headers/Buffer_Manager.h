#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

// Component들의 원본객체를 보관한다.
class CVIBuffer;
class DLL_EXPORT CBuffer_Manager : public CBase
{
	DECLARE_SINGLETON(CBuffer_Manager)

private:
	explicit CBuffer_Manager();
	virtual ~CBuffer_Manager() = default;
public:
	HRESULT		Add_Buffer(CVIBuffer* pBuffer);
	void		Clear_BufferList();
	CVIBuffer*	Get_Buffer(_uint iIndex);
	_uint		Get_BufferListSize() { return m_BufferList.size();}

private:
	list<CVIBuffer*>			m_BufferList;
	typedef	list<CVIBuffer*>	BUFFERLIST;

private:
	virtual _ulong Free();
};

END
