#pragma once
#include "afxwin.h"

#include "Creature_Manager.h"
#include "A_Defines.h"

// CPage_Animate 대화 상자입니다.

class CPage_Animate : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Animate)

	/*ctor dtor*/
public:
	CPage_Animate();
	virtual ~CPage_Animate();


/*FUNCTION*/
public:
	/*creature & bone*/
	afx_msg void OnLbnSelchangeAnimateCreaturelistbox();
	afx_msg void OnLbnSelchangeAnimateBonelistbox();
	afx_msg void OnBnClickedButtonRefresh();
	
	/*state*/
	afx_msg void OnLbnSelchangeAnimateStatelistbox();
	afx_msg void OnBnClickedAnimateStateAdd();
	afx_msg void OnBnClickedAnimateStateDel();

	/*Frame*/
	afx_msg void OnLbnSelchangeAnimateFramelistbox();
	afx_msg void OnBnClickedAnimateFrameAdd();
	afx_msg void OnBnClickedAnimateFrameDel();
	afx_msg void OnBnClickedAnimateFrameApply();

	afx_msg void OnBnClickedButtonAnimateStart();
	afx_msg void OnBnClickedButtonAnimatePause();
	afx_msg void OnBnClickedButtonGetPredegree();
	
	/*save load*/
	afx_msg void OnBnClickedButtonAnimatesave();
	afx_msg void OnBnClickedButtonAnimateload();

	/*radio*/
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();


private:
	void Refresh_CurrentCreature_FromCurrentSel();
	void Refresh_CurrentInfo_FromCurrentSel();
	void Refresh_State();
	void Refresh_Frame();
	void Refresh_FrameInfo();
	void Refresh_Animate();

/*MEMBER*/
private:
	CCreature_Manager*		m_pCreature_Manager = nullptr;
	CCreature*				m_pCurCreature = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;

	CListBox m_CreatureListBox;
	CListBox m_BoneListBox;

	CListBox m_StateListBox;
	CListBox m_FrameListBox;

	float	 m_fStartDegreeX;
	float	m_fStartDegreeY;
	float	m_fStartDegreeZ;
	float	m_fEndDegreeX;
	float	m_fEndDegreeY;
	float	m_fEndDegreeZ;
	float	m_fTime;

	CButton m_Radio[3];

	/*MFC Default*/
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_ANIMATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:


	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonGetIdledegree();
	afx_msg void OnBnClickedButtonGetPredegree3();
	afx_msg void OnBnClickedButtonGetPredegree2();
};
