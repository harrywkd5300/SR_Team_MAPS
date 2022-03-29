#pragma once
#include "afxwin.h"


// CDialogTab1 대화 상자입니다.

class CDialogTab1 : public CDialog
{
	DECLARE_DYNAMIC(CDialogTab1)

public:
	CDialogTab1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogTab1();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
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
