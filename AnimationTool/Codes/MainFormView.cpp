// ../Codes/MainFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AnimationTool.h"
#include "../Headers/MainFormView.h"
#include "../headers/sheet.h"

IMPLEMENT_DYNCREATE(CMainFormView, CFormView)

CMainFormView::CMainFormView()
	: CFormView(IDD_MAINFORMVIEW)
{

}

CMainFormView::~CMainFormView()
{
}

void CMainFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainFormView, CFormView)
END_MESSAGE_MAP()


// CMainFormView 진단입니다.

#ifdef _DEBUG
void CMainFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CMainFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_pSheet = new CSheet;
	//error C2512: 'CMapSheet' : 사용할 수 있는 적절한 기본 생성자가 없습니다.
	m_pSheet->Create(this, WS_CHILD | WS_VISIBLE);
	m_pSheet->MoveWindow(0, 0, 300, 1000);

// 	m_Tab.DeleteAllItems();
// 	m_Tab.InsertItem(0, L"Body");
// 	m_Tab.InsertItem(1, L"Animate");
// 
// 	CRect rect;
// 
// 	m_pTab_Body = new CTab_Body;
// 	m_pTab_Body->Create(IDD_TAB_BODY, &m_Tab);
// 	m_pTab_Body->GetWindowRect(&rect);
// 	m_pTab_Body->MoveWindow(5, 25, rect.Width(), rect.Height());
// 	m_pTab_Body->ShowWindow(SW_SHOW);

// 	m_pTab_Animate= new CTab_Animate;
// 	m_pTab_Animate->Create(IDD_TAB_ANIMATE, &m_Tab);
// 	m_pTab_Animate->GetWindowRect(&rect);
// 	m_pTab_Animate->MoveWindow(5, 25, rect.Width(), rect.Height());
// 	m_pTab_Animate->ShowWindow(SW_HIDE);


}



// CMainFormView 메시지 처리기입니다.
