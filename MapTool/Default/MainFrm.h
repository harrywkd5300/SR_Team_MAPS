
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once

class CSplitterX : public CSplitterWnd
{
public:
	CSplitterX()  {};
	~CSplitterX() {};
protected:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	DECLARE_MESSAGE_MAP();
};

class CMapToolView;
class CMyToolForm;
class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
private:
	CSplitterX		m_MainSplitter;
	CMyToolForm*    m_pMapForm = nullptr;
	CMapToolView*	m_pMapView = nullptr;

// Ư���Դϴ�.
public:
	CMyToolForm*	GetMapForm()
	{
		return m_pMapForm;
	}
	CMapToolView*	GetMapView()
	{
		return m_pMapView;
	}

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};
