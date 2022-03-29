
// AnimationToolView.cpp : CAnimationToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "AnimationTool.h"
#endif

#include "AnimationToolDoc.h"
#include "AnimationToolView.h"
#include "mainfrm.h"
#include "Graphic_Device.h"
#include "timer_manager.h"
#include "Frame_Manager.h"
#include "Scene_ATool.h"
#include "Camera_Manager.h"
#include "object_manager.h"
#include "Input_Device.h"
#include "creature_manager.h"
#include "camera_manager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnimationToolView

IMPLEMENT_DYNCREATE(CAnimationToolView, CView)

BEGIN_MESSAGE_MAP(CAnimationToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CAnimationToolView 생성/소멸

CAnimationToolView::CAnimationToolView()
	:m_pDevice(CGraphic_Device::GetInstance())
	, m_pGraphicDev(nullptr)
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pManagement(CManagement::GetInstance())
	, m_pRenderer(nullptr)
	, m_pInput_Device(CInput_Device::GetInstance())
{
	m_pDevice->AddRef();
	m_pComponent_Manager->AddRef();
	m_pManagement->AddRef();
	m_pInput_Device->AddRef();
}

CAnimationToolView::~CAnimationToolView()
{
	CleanUp();
}

BOOL CAnimationToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

void CAnimationToolView::OnDraw(CDC* /*pDC*/)
{
	CAnimationToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

void CAnimationToolView::TimeUpdate(const _float& fTimeDelta)
{
	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	m_pGraphicDev->BeginScene();

	m_pManagement->Update_Management(fTimeDelta);
	m_pManagement->Render_Management();
	
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(nullptr, nullptr, nullptr, nullptr);
}


// CAnimationToolView 인쇄

BOOL CAnimationToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CAnimationToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CAnimationToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

HRESULT CAnimationToolView::Ready_DefaultSetting()
{
	g_hWnd = m_hWnd;

	return NOERROR;
}

HRESULT CAnimationToolView::Ready_DeviceSetting()
{
	if (nullptr == m_pDevice)
		goto exception;

	if (FAILED(m_pDevice->Ready_GraphicDev(&m_pGraphicDev, g_hWnd, Engine::CGraphic_Device::MODE_WIN, BACKSIZEX, BACKSIZEY)))
		goto exception;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return NOERROR;

exception:
	AfxMessageBox(L"Ready_DeviceSetting failed err:04934");
	return E_FAIL;
}



HRESULT CAnimationToolView::Ready_StaticComponent()
{
	if (nullptr == m_pComponent_Manager)
		goto exception;

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

HRESULT CAnimationToolView::Ready_StartScene()
{
	if (nullptr == m_pManagement)
		goto exception;

	CScene* pScene = CScene_ATool::Create(m_pGraphicDev);

	if (nullptr == pScene)
		goto exception;

	if (FAILED(m_pManagement->Set_CurScene(pScene, m_pRenderer)))
		goto exception;

	return NOERROR;

exception:
	AfxMessageBox(L"Ready_StartScene failed");
	return E_FAIL;
}

_ulong CAnimationToolView::CleanUp()
{
	_ulong dwRefCount = 0;
	
	/*Decrease RefCount*/
	Safe_Release(m_pDevice);
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pManagement);
	Safe_Release(m_pRenderer);

	/*Destroy Instance*/
	if (dwRefCount = CCreature_Manager::GetInstance()->DestroyInstance())
		goto exception;

	if (dwRefCount = CFrame_Manager::GetInstance()->DestroyInstance())
		goto exception;

	if (dwRefCount = CTimer_Manager::GetInstance()->DestroyInstance())
		goto exception;

 	if (dwRefCount = CCamera_Manager::GetInstance()->DestroyInstance())
		goto exception;
	
	if (dwRefCount = CManagement::GetInstance()->DestroyInstance())
		goto exception;

	if (dwRefCount = CObject_Manager::GetInstance()->DestroyInstance())
		goto exception;

	if (dwRefCount = CInput_Device::GetInstance()->DestroyInstance())
		goto exception;

	if (dwRefCount = CComponent_Manager::GetInstance()->DestroyInstance())
		goto exception;

	if (dwRefCount = CGraphic_Device::GetInstance()->DestroyInstance())
		goto exception;


	return dwRefCount;

exception:
	AfxMessageBox(L"CleanUp Failed");
	return dwRefCount;
}


// CAnimationToolView 진단

#ifdef _DEBUG
void CAnimationToolView::AssertValid() const
{
	CView::AssertValid();
}

void CAnimationToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnimationToolDoc* CAnimationToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnimationToolDoc)));
	return (CAnimationToolDoc*)m_pDocument;
}
#endif //_DEBUG


void CAnimationToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();


	/*Set FrameSize & Get g_HWnd*/
	if (FAILED(Ready_DefaultSetting()))
		goto exception;

	if (FAILED(Ready_DeviceSetting()))
		goto exception;
		
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	/*reserve container*/
	if (FAILED(m_pManagement->Ready_Management(SCENE_END)))
		goto exception;

	/*static component*/
	if (FAILED(Ready_StaticComponent()))
		goto exception;

	if (FAILED(Ready_StartScene()))
		goto exception;

// 	CWnd* pWnd = AfxGetMainWnd();
// 	HWND hWnd = pWnd->m_hWnd;
// 	HINSTANCE hInstance = GetModuleHandle(NULL);
// 
// 	if (FAILED(m_pInput_Device->Ready_InputDevice(hInstance, hWnd)))
// 		goto exception;
	
	return;

exception:
	AfxMessageBox(L"OnInitialUpdate failed");
	return;

}


void CAnimationToolView::OnMouseMove(UINT nFlags, CPoint point)
{
// 	m_pManagement->Update_Management(0.f);
// 
// 	Invalidate(FALSE);

	CView::OnMouseMove(nFlags, point);



}


void CAnimationToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
// 	m_pManagement->Update_Management(0.f);
// 
// 	Invalidate(FALSE);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);


}
