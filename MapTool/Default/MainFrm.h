
// MainFrm.h : CMainFrame 클래스의 인터페이스
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
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
private:
	CSplitterX		m_MainSplitter;
	CMyToolForm*    m_pMapForm = nullptr;
	CMapToolView*	m_pMapView = nullptr;

// 특성입니다.
public:
	CMyToolForm*	GetMapForm()
	{
		return m_pMapForm;
	}
	CMapToolView*	GetMapView()
	{
		return m_pMapView;
	}

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};
