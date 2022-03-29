
// MapToolView.cpp : CMapToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MapTool.h"
#endif

#include "MapToolDoc.h"
#include "MapToolView.h"
#include "MyToolForm.h"

#include "MainFrm.h"
#include "Input_Device.h"
#include "timer_Manager.h"
#include "frame_manager.h"
#include "Graphic_Device.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMapToolView

IMPLEMENT_DYNCREATE(CMapToolView, CView)

BEGIN_MESSAGE_MAP(CMapToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMapToolView 생성/소멸

CMapToolView::CMapToolView()
	:m_pDevice(CGraphic_Device::GetInstance())
	, m_pManagement(CManagement::GetInstance())
	, m_pGraphicDev(nullptr)
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pRenderer(nullptr)
	, m_pInput_Device(CInput_Device::GetInstance())
	, pTimer_Manager(CTimer_Manager::GetInstance())
	, pFrame_Manager(CFrame_Manager::GetInstance())
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_pDevice->AddRef();
	m_pManagement->AddRef();
	m_pComponent_Manager->AddRef();
	m_pInput_Device->AddRef();
	pTimer_Manager->AddRef();
	pFrame_Manager->AddRef();
}

CMapToolView::~CMapToolView()
{
	Free();
}

BOOL CMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMapToolView 그리기

HRESULT CMapToolView::Ready_DeviceSetting()
{
	if (nullptr == m_pDevice)
		goto exception;

	if (FAILED(m_pDevice->Ready_GraphicDev(&m_pGraphicDev, g_hWnd, Engine::CGraphic_Device::MODE_WIN, BACKCX, BACKCY)))
		goto exception;

	return NOERROR;

exception:
	AfxMessageBox(L"Ready_DeviceSetting failed err:04934");
	return E_FAIL;
}

HRESULT CMapToolView::Ready_StaticComponent(void)
{
	if (nullptr == m_pComponent_Manager)
	{
		AfxMessageBox(L"Component_Manager isn't allocated");
		return E_FAIL;
	}
	/*Create Triangle_Color*/
	CComponent* pComponent = nullptr;

	pComponent = CTriangle_Color::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_TriCol", pComponent)))
		goto exception;


	/*create Rect_Color*/
	pComponent = CRect_Color::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_RectCol", pComponent)))
		goto exception;


	/*Create Rect_Texture*/
	pComponent = CRect_Texture::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_RectTex", pComponent)))
		goto exception;

	/*create Renderer & reference*/
	pComponent = m_pRenderer = CRenderer::Create(m_pGraphicDev);
	m_pRenderer->AddRef();

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Renderer", pComponent)))
		goto exception;


	/*create transform*/
	pComponent = CTransform::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto exception;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Transform", pComponent)))
		goto exception;

	return NO_ERROR;

exception:
	AfxMessageBox(L"Ready_StaticComponent Failed!!");
	return E_FAIL;
}

HRESULT CMapToolView::Ready_StartScene(void)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	CScene* pScene = CScene_Tool::Create(m_pGraphicDev);

	if (nullptr == pScene)
		return E_FAIL;

	m_pScene = pScene;

	if (FAILED(m_pManagement->Set_CurScene(pScene, m_pRenderer)))
		return E_FAIL;

	return NOERROR;
}

void CMapToolView::TimeUpdate()
{
	if (nullptr == pTimer_Manager || nullptr == pFrame_Manager || nullptr == m_pInput_Device)
		AfxMessageBox(L"Manager Setting Failed!!");
	pTimer_Manager->Compute_Timer(L"Timer_Default");
	_float fTimeDelta_default = pTimer_Manager->Get_TimeDelta(L"Timer_Default");
	if (pFrame_Manager->Permit_Call(L"Frame_60", fTimeDelta_default))
	{
		m_pInput_Device->Update_InputState();
		pTimer_Manager->Compute_Timer(L"Timer_60");
		fTimeDelta = pTimer_Manager->Get_TimeDelta(L"Timer_60");
		
		Management_Update(fTimeDelta);
		Management_Render();
	}
}

void CMapToolView::Management_Update(_float& fTimeDelta)
{
	m_pManagement->Update_Management(fTimeDelta);
}

void CMapToolView::Management_Render()
{
	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	m_pGraphicDev->BeginScene();

	m_pManagement->Render_Management();

	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(nullptr, nullptr, nullptr, nullptr);
}

void CMapToolView::OnDraw(CDC* /*pDC*/)
{
	CMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMapToolView 인쇄

BOOL CMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMapToolView 진단

#ifdef _DEBUG
void CMapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

_ulong CMapToolView::Free()
{
	_ulong dwRefCnt = 0;
	/*Decrease RefCount*/
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDevice);
	Safe_Release(m_pManagement);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pInput_Device);
	Safe_Release(pTimer_Manager);
	Safe_Release(pFrame_Manager);


	/*Destroy Instance*/
	if (dwRefCnt = CManagement::GetInstance()->DestroyInstance())
		AfxMessageBox(L"Management not destroyed");
	if (dwRefCnt = CObject_Manager::GetInstance()->DestroyInstance())
		AfxMessageBox(L"CObject_Manager not destroyed");
	if (dwRefCnt = CComponent_Manager::GetInstance()->DestroyInstance())
		AfxMessageBox(L"Component_Manager not destroyed");
	if (dwRefCnt = CGraphic_Device::GetInstance()->DestroyInstance())
		AfxMessageBox(L"Device not destroyed");
	if (dwRefCnt = CInput_Device::GetInstance()->DestroyInstance())
		AfxMessageBox(L"Input_Device not destroyed");
	if (dwRefCnt = CTimer_Manager::GetInstance()->DestroyInstance())
		AfxMessageBox(L"Timer_Manager not destroyed");
	if (dwRefCnt = CFrame_Manager::GetInstance()->DestroyInstance())
		AfxMessageBox(L"Frame_Manager not destroyed");

	return dwRefCnt;
}

CMapToolDoc* CMapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolDoc)));
	return (CMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapToolView 메시지 처리기


void CMapToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	RECT rcWindow;
	
	pMainFrame->GetWindowRect(&rcWindow);
	//AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, FALSE);

	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	RECT rcMainView;
	GetClientRect(&rcMainView);

	//가로 프레임의 길이를 구하자.
	float fX = float(rcWindow.right - rcMainView.right);

	//세로 프레임의 길이를 구하자.
	float fY = float(rcWindow.bottom - rcMainView.bottom);

	pMainFrame->SetWindowPos(
		NULL,		//프레임의 크기를 계산한 상태로 출력할 위치를 결정하는 인자.
		0,
		0,
		int(BACKCX + fX),
		int(BACKCY + fY),
		SWP_NOZORDER
	);

	g_hWnd = m_hWnd;

	if (FAILED(Ready_DeviceSetting()))
	{
		AfxMessageBox(L"Device Created Failed!!");
	}
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	if (FAILED(m_pManagement->Ready_Management(SCENE_END)))
		AfxMessageBox(L"Ready_Setting Failed!!");
	if (FAILED(Ready_StaticComponent()))
		AfxMessageBox(L"Ready_Setting Failed!!");
	if (FAILED(Ready_StartScene()))
		AfxMessageBox(L"Ready_Setting Failed!!");
	//
	/* Add Timer */
	//
	CTimer*	pTimer = nullptr;
	pTimer = CTimer::Create();

	if (nullptr == pTimer)
		AfxMessageBox(L"Timer Create Failed!!");

	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_Default", pTimer)))
		AfxMessageBox(L"Timer_Default Setting Failed!!");

	pTimer = CTimer::Create();

	if (nullptr == pTimer)
		AfxMessageBox(L"Timer Create Failed!!");

	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_60", pTimer)))
		AfxMessageBox(L"Timer_60 Setting Failed!!");
	//
	/*Add Frame*/
	//
	if (FAILED(pFrame_Manager->Add_Frame(L"Frame_60", 60.f)))
		AfxMessageBox(L"Frame_60 Setting Failed!!");

	CWnd* pWnd = AfxGetMainWnd();
	HWND hWnd = pWnd->m_hWnd;
	HINSTANCE hInstance = GetModuleHandle(NULL);

	m_pInput_Device->Ready_InputDevice(hInstance, hWnd);
}

void CMapToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);

	CDialogTab2* pTab2 = ((CMainFrame*)AfxGetMainWnd())->GetMapForm()->GetTab2();

	CString strName;

	int iSelectIndex = pTab2->m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}
	pTab2->m_iObjectList.GetText(iSelectIndex, strName);

	if (dynamic_cast<CScene_Tool*>(m_pScene)->ObjectPicking(point.x, point.y, strName))
	{
		CMyToolForm* ToolForm = ((CMainFrame*)AfxGetMainWnd())->GetMapForm();
		CDialogTab2* m_Tab2 = ToolForm->GetTab2();
		if (!(m_Tab2->m_bCreateCube))
		{
			AfxMessageBox(L"Don't Select Object!!");
			return;
		}
		_uint iDrawID = ((CMainFrame*)AfxGetMainWnd())->GetMapForm()->GetTab2()->m_iDrawID;
 		if (m_Tab2->m_bPickingCube)
		{
			CString strName2;
			int iSelectIndex = m_Tab2->m_iObjectList.GetCurSel();
			m_Tab2->m_iObjectList.GetText(iSelectIndex, strName2);
			TCHAR pKey[256];
			lstrcpy(pKey, strName2);
			int iCur = m_Tab2->iSelObjectCur;
			_vec3 TerrainPos = *dynamic_cast<CScene_Tool*>(m_pScene)->Create_OnCube(iDrawID, iCur, pKey, strName2);
			AfxMessageBox(L"Create Cube!!");
		}
		m_Tab2->PickingCube();
	}
	else
	{
		_vec3 TerrainPos = dynamic_cast<CScene_Tool*>(m_pScene)->TerrainPicking(point.x, point.y);

		if (TerrainPos.x <= 0 || TerrainPos.y <= 0 || TerrainPos.z <= 0)
		{
			AfxMessageBox(L"Don't Create  Pos Cube!!");
			return;
		}

		CMyToolForm* ToolForm = ((CMainFrame*)AfxGetMainWnd())->GetMapForm();
		CDialogTab2* m_Tab2 = ToolForm->GetTab2();
		if (!(m_Tab2->m_bCreateCube))
		{
			AfxMessageBox(L"Don't Select Object!!");
			return;
		}

		_uint iDrawID = ((CMainFrame*)AfxGetMainWnd())->GetMapForm()->GetTab2()->m_iDrawID;
		if (0 > iDrawID)
		{
			AfxMessageBox(L"No Cube DrawID!!");
			return;
		}

		CString strName;

		int iSelectIndex = m_Tab2->m_iObjectList.GetCurSel();
	
		m_Tab2->m_iObjectList.GetText(iSelectIndex, strName);

		TCHAR pKey[256];
		lstrcpy(pKey, strName);

		if (FAILED(dynamic_cast<CScene_Tool*>(m_pScene)->Create_Cube(TerrainPos, _vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), iDrawID, pKey, strName)))
		{
			AfxMessageBox(L"Don't Create Cube!!");
			return;
		}
		AfxMessageBox(L"Create Cube!!");
		m_Tab2->PickingCube();
	}
}
