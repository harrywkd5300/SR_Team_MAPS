#pragma once
#include "afxcmn.h"

#include "DialogTab1.h"
#include "DialogTab2.h"

// CMyToolForm �� ���Դϴ�.

class CMyToolForm : public CFormView
{
	DECLARE_DYNCREATE(CMyToolForm)

protected:
	CMyToolForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyToolForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYTOOLFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl		m_Tab;
	CDialogTab1*	m_pDialog = nullptr;
	CDialogTab2*	m_pDialog2 = nullptr;

	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);

public:

	CDialogTab1*	GetTab1()
	{
		return m_pDialog;
	}

	CDialogTab2*	GetTab2()
	{
		return m_pDialog2;
	}
};


