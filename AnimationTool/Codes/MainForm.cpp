// ../Codes/MainForm.cpp : 구현 파일입니다.
//

#include "../default/stdafx.h"
#include "../default/AnimationTool.h"
#include "../Headers/MainForm.h"


// CMainForm

IMPLEMENT_DYNCREATE(CMainForm, CFormView)

CMainForm::CMainForm()
	: CFormView(IDD_MAINFORM)
{

}

CMainForm::~CMainForm()
{
}

void CMainForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainForm, CFormView)
END_MESSAGE_MAP()


// CMainForm 진단입니다.

#ifdef _DEBUG
void CMainForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainForm 메시지 처리기입니다.
