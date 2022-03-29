
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once

#include "A_Defines.h"



class CAnimationToolView;
class CMainFormView;
class CMainFrame : public CFrameWnd
{
	
/*ctor dtor*/
protected:
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
public:
	virtual ~CMainFrame();


public:



protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

private:
	CSplitterWnd	m_MainSplitter;

	CAnimationToolView*		m_pMainView = nullptr;
	CMainFormView*			m_pMainFormView = nullptr;

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


