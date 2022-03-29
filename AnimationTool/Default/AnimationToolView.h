
// AnimationToolView.h : CAnimationToolView 클래스의 인터페이스
//

#pragma once

#include "A_Defines.h"
#include "component_Manager.h"
#include "management.h"

namespace Engine
{
	class CGraphic_Device;
	class CInput_Device;
	class CCamera_Manager;
}

class CCreature_Manager;
class CAnimationToolDoc;
class CAnimationToolView 
	: public CView
{

/*ctor dtor*/
protected:
	CAnimationToolView();
	DECLARE_DYNCREATE(CAnimationToolView)
public:
	virtual ~CAnimationToolView();

/*FUNCTION*/
public:
	CAnimationToolDoc* GetDocument() const;

public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);
	void	TimeUpdate(const _float& fTimeDelta);
	
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);



protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

private:
	HRESULT	Ready_DefaultSetting();
	HRESULT Ready_DeviceSetting();
	HRESULT Ready_StaticComponent();
	HRESULT Ready_StartScene();

	/* Free */
	_ulong	CleanUp();

/*MEMBER*/
private:
	CGraphic_Device*			m_pDevice = nullptr;
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;
	CInput_Device*				m_pInput_Device = nullptr;
	CComponent_Manager*			m_pComponent_Manager = nullptr;
	CManagement*				m_pManagement = nullptr;
	CRenderer*					m_pRenderer = nullptr;



#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // AnimationToolView.cpp의 디버그 버전
inline CAnimationToolDoc* CAnimationToolView::GetDocument() const
   { return reinterpret_cast<CAnimationToolDoc*>(m_pDocument); }
#endif

