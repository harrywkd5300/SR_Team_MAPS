CBase::CBase()
	:m_dwRefCnt(0)
{

}

unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;
}

unsigned long CBase::Release()
{
	if (0 == m_dwRefCnt)
	{
		unsigned long dwFreeCnt = 0;

		if (dwFreeCnt = Free())
		{
			return dwFreeCnt;
		}
		else /* dwFreeCount ==0 */
		{
			delete this;
			return 0;
		}

	}
	else
		return m_dwRefCnt--;

}
