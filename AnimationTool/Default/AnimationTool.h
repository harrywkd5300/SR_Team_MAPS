
// AnimationTool.h : AnimationTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "Input_Device.h"
#include "Frame_Manager.h"
#include "Timer_Manager.h"

class CAnimationToolApp : public CWinApp
{
public:
	CAnimationToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
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
