// ../Codes/Sheet.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AnimationTool.h"
#include "../Headers/Sheet.h"


IMPLEMENT_DYNAMIC(CSheet, CPropertySheet)

CSheet::CSheet()
{
	AddPage(&m_PageBody);
	AddPage(&m_PageAnimate);
}

CSheet::CSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CSheet::CSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CSheet::~CSheet()
{
}


BEGIN_MESSAGE_MAP(CSheet, CPropertySheet)
END_MESSAGE_MAP()


// CSheet �޽��� ó�����Դϴ�.
