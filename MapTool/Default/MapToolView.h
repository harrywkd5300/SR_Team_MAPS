
// MapToolView.h : CMapToolView 클래스의 인터페이스
//
#include "Graphic_Device.h"

#include "Tool_Define.h"

#include "Management.h"
#include "component_manager.h"
#include "Scene_Tool.h"

#pragma once

namespace Engine
{
	class CInput_Device;
	class CTimer_Manager;
	class CFrame_Manager;
}

class CMapToolDoc;
class CMapToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMapToolView();
	DECLARE_DYNCREATE(CMapToolView)

private:
	CGraphic_Device*		m_pDevice = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	CManagement*			m_pManagement = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
	CRenderer*				m_pRenderer = nullptr;
	CScene*					m_pScene = nullptr;

public:
	CInput_Device*			m_pInput_Device = nullptr;
	CTimer_Manager*			pTimer_Manager = nullptr;
	CFrame_Manager*			pFrame_Manager = nullptr;
	_float					fTimeDelta = 0.f;

// 특성입니다.
public:
	CMapToolDoc* GetDocument() const;

// 작업입니다.
public:
	HRESULT Ready_DeviceSetting();
	HRESULT Ready_StaticComponent(void);
	HRESULT Ready_StartScene(void);
	void	TimeUpdate();
	void    Management_Update(_float& fTimeDelta);
	void    Management_Render();
	CScene*	GetScene() { return m_pScene; }

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMapToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:
	_ulong	Free();
protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MapToolView.cpp의 디버그 버전
inline CMapToolDoc* CMapToolView::GetDocument() const
   { return reinterpret_cast<CMapToolDoc*>(m_pDocument); }
#endif

