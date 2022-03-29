
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "AnimationTool.h"
#include "MainFrm.h"
#include "MainFormView.h"
#include "AnimationToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*Global*/
HWND g_hWnd;
extern CAnimationToolView*	g_pMainView;


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.cx = BACKSIZEX ;
	cs.cy = BACKSIZEX ;

	return TRUE;
}

// CMainFrame ����




// CMainFrame �޽��� ó����



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_MainSplitter.CreateStatic(this, 1, 2);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CAnimationToolView), CSize(BACKSIZEX, BACKSIZEY), pContext);
	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMainFormView), CSize(300, 300), pContext);
	
	g_pMainView = m_pMainView = (CAnimationToolView*)m_MainSplitter.GetPane(0, 1);
	m_pMainFormView = (CMainFormView*)m_MainSplitter.GetPane(0, 0);

	m_MainSplitter.SetColumnInfo(0, 300, 20);

	//g_pView = m_pMapView = (CMapToolView*)m_MainSplitter.GetPane(0, 1);

	return true;
}


#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG