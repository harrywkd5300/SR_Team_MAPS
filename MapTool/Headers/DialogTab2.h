#pragma once

#include "Tool_Define.h"
#include "afxwin.h"

#include "Scene_Tool.h"

// CDialogTab2 대화 상자입니다.

class CDialogTab2 : public CDialog
{
	DECLARE_DYNAMIC(CDialogTab2)
public:
	CDialogTab2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogTab2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGTAB2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	map<CString, CImage*>		m_mapPngImage;
	wstring						m_wstrPath;

	map<CString, CGameObject*>	m_mapGameObject;

private:
	void	HorizontalScroll();
	wstring RelativePath(wstring szFullPath);
public:
	int		m_iDrawID = -1;
	bool	m_bCreateCube = false;
	int		iSelObjectCur = -1;

public:
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	float m_fRotX;
	float m_fRotY;
	float m_fRotZ;
	float m_fScaleChX;
	float m_fScaleChY;
	float m_fScaleChZ;
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	bool  m_bPickingCube;
	CListBox m_iTextureList;
	CStatic m_Texture;
	CEdit m_EPosX;
	CString m_iObjectKey;
	CListBox m_iObjectList;
	int m_iNumX;
	int m_iNumY;
	int m_iNumZ;

public:
	virtual BOOL OnInitDialog();
	void	PickingCube();
	void	Release();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedADD();
	afx_msg void OnBnClickedTextureChange();
	afx_msg void OnBnClickedPlusX();
	afx_msg void OnBnClickedPlusY();
	afx_msg void OnBnClickedPlusZ();
	afx_msg void OnBnClickedMinusX();
	afx_msg void OnBnClickedMinusY();
	afx_msg void OnBnClickedMinusZ();
	afx_msg void OnBnClickedScaleChange();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeTextureList1();
	afx_msg void OnBnClickedSaveobj();
	afx_msg void OnBnClickedPosChange();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedLoadobj();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnBnClickedButtonDelete();

public:
	map<CString, CGameObject*>* GetMapObject() { return &m_mapGameObject; }
	afx_msg void OnBnClickedButton16();
};
