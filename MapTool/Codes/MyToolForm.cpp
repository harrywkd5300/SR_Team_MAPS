// ../Codes/MyToolForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "../Default/MapTool.h"
#include "MyToolForm.h"


// CMyToolForm

IMPLEMENT_DYNCREATE(CMyToolForm, CFormView)

CMyToolForm::CMyToolForm()
	: CFormView(IDD_MYTOOLFORM)
{

}

CMyToolForm::~CMyToolForm()
{
	if (NULL != m_pDialog)
	{
		delete m_pDialog;
		m_pDialog = NULL;
	}

	if (NULL != m_pDialog2)
	{
		delete m_pDialog2;
		m_pDialog2 = NULL;
	}
}

void CMyToolForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB2, m_Tab);
}

BEGIN_MESSAGE_MAP(CMyToolForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CMyToolForm::OnTcnSelchangeTab2)
END_MESSAGE_MAP()


// CMyToolForm �����Դϴ�.

#ifdef _DEBUG
void CMyToolForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyToolForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyToolForm �޽��� ó�����Դϴ�.

void CMyToolForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_Tab.DeleteAllItems();
	m_Tab.InsertItem(0, L"MapTool");
	m_Tab.InsertItem(1, L"Tool");

	CRect rect;

	m_pDialog = new CDialogTab1;
	m_pDialog->Create(IDD_DIALOG1 , &m_Tab);
	m_pDialog->GetWindowRect(&rect);
	m_pDialog->MoveWindow( 5, 25, rect.Width(), rect.Height() );
	m_pDialog->ShowWindow( SW_SHOW );

	m_pDialog2 = new CDialogTab2;
	m_pDialog2->Create(IDD_DIALOGTAB2, &m_Tab);
	m_pDialog2->GetWindowRect(&rect);
	m_pDialog2->MoveWindow(5, 25, rect.Width(), rect.Height());
	m_pDialog2->ShowWindow(SW_HIDE);
}

void CMyToolForm::OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int select = m_Tab.GetCurSel();

	switch (select)
	{
	case 0:
		m_pDialog->ShowWindow(SW_SHOW);
		m_pDialog2->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pDialog->ShowWindow(SW_HIDE);
		m_pDialog2->ShowWindow(SW_SHOW);
		break;
	}


	*pResult = 0;
}
