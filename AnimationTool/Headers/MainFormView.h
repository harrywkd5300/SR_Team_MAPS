#pragma once
#include "afxcmn.h"


// class CTab_Body;
// class CTab_Animate;
class CSheet;


class CMainFormView : public CFormView
{
	DECLARE_DYNCREATE(CMainFormView)


	/*ctor dtor*/
protected:
	CMainFormView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMainFormView();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


/*FUNCTION*/
public:
	virtual void OnInitialUpdate();

/*MEMBER*/
private:
	CSheet*			m_pSheet = nullptr;
// 	CTabCtrl		m_Tab;
// 	CTab_Animate*	m_pTab_Animate = nullptr;
// 	CTab_Body*		m_pTab_Body	= nullptr;


public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINFORMVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

};




