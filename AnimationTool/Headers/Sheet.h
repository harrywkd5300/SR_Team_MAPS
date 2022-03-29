#pragma once



// CSheet
#include "Page_Animate.h"
#include "Page_body.h"



class CSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheet)

	/*ctor dtor*/
public:
	CSheet();
	CSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheet();

private:
	CPage_Body		m_PageBody;
	CPage_Animate	m_PageAnimate;


protected:
	DECLARE_MESSAGE_MAP()
};


