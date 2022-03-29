#pragma once
#include "afxwin.h"


// CDialogTab1 ��ȭ �����Դϴ�.

class CDialogTab1 : public CDialog
{
	DECLARE_DYNAMIC(CDialogTab1)

public:
	CDialogTab1(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogTab1();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()

private:
	map<CString, CImage*>		m_mapPngImage;
	wstring						m_wstrPath;

private:
	void	HorizontalScroll();
	wstring RelativePath(wstring szFullPath);


public:
	int m_iInterval;
	CButton m_bCheckButton;
	afx_msg void OnBnClickedButton1();  // change
	afx_msg void OnBnClickedCheck1();   // frameview
	virtual BOOL OnInitDialog();
	CString m_strHeightName;
	afx_msg void OnBnClickedButton2();  // save
	afx_msg void OnBnClickedButton3();  // load
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButton9();
	CListBox m_iTextureList;
	CStatic m_iTexture;
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
