
// AnimationTool.h : AnimationTool 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
#include "Input_Device.h"
#include "Frame_Manager.h"
#include "Timer_Manager.h"

class CAnimationToolApp : public CWinApp
{
public:
	CAnimationToolApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);

private:
	CInput_Device*	m_pInput_Device = nullptr;
	CFrame_Manager*	m_pFrame_Manager = nullptr;
	CTimer_Manager*	m_pTimer_Manager = nullptr;

public:
	HRESULT	Ready_FrameTimer();

};

extern CAnimationToolApp theApp;
