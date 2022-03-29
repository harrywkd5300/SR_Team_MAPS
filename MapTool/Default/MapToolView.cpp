
// MapToolView.cpp : CMapToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMapToolView ����/�Ҹ�

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
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMapToolView �׸���

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
	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMapToolView �μ�

BOOL CMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMapToolView ����

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

CMapToolDoc* CMapToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolDoc)));
	return (CMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapToolView �޽��� ó����


void CMapToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	RECT rcWindow;
	
	pMainFrame->GetWindowRect(&rcWindow);
	//AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, FALSE);

	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	RECT rcMainView;
	GetClientRect(&rcMainView);

	//���� �������� ���̸� ������.
	float fX = float(rcWindow.right - rcMainView.right);

	//���� �������� ���̸� ������.
	float fY = float(rcWindow.bottom - rcMainView.bottom);

	pMainFrame->SetWindowPos(
		NULL,		//�������� ũ�⸦ ����� ���·� ����� ��ġ�� �����ϴ� ����.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
