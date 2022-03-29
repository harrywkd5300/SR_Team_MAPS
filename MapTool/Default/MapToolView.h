
// MapToolView.h : CMapToolView Ŭ������ �������̽�
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
protected: // serialization������ ��������ϴ�.
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

// Ư���Դϴ�.
public:
	CMapToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	HRESULT Ready_DeviceSetting();
	HRESULT Ready_StaticComponent(void);
	HRESULT Ready_StartScene(void);
	void	TimeUpdate();
	void    Management_Update(_float& fTimeDelta);
	void    Management_Render();
	CScene*	GetScene() { return m_pScene; }

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMapToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:
	_ulong	Free();
protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MapToolView.cpp�� ����� ����
inline CMapToolDoc* CMapToolView::GetDocument() const
   { return reinterpret_cast<CMapToolDoc*>(m_pDocument); }
#endif

