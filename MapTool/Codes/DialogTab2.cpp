// ../Codes/DialogTab2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "../Headers/DialogTab2.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MapToolView.h"

// CDialogTab2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogTab2, CDialog)

CDialogTab2::CDialogTab2(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOGTAB2, pParent)
	, m_fPosX(1.f)
	, m_fPosY(1.f)
	, m_fPosZ(1.f)
	, m_fRotX(0.f)
	, m_fRotY(0.f)
	, m_fRotZ(0.f)
	, m_fScaleChX(1.f)
	, m_fScaleChY(1.f)
	, m_fScaleChZ(1.f)
	, m_fScaleX(1.f)
	, m_fScaleY(1.f)
	, m_fScaleZ(1.f)
	, m_iObjectKey(_T(""))
	, m_bPickingCube(false)
	, m_bCreateCube(false)
	, m_iNumX(1)
	, m_iNumY(1)
	, m_iNumZ(1)
{

}

CDialogTab2::~CDialogTab2()
{
	Release();
}

void CDialogTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fPosX);
	DDX_Text(pDX, IDC_EDIT2, m_fPosY);
	DDX_Text(pDX, IDC_EDIT3, m_fPosZ);
	DDX_Text(pDX, IDC_EDIT5, m_fRotX);
	DDX_Text(pDX, IDC_EDIT6, m_fRotY);
	DDX_Text(pDX, IDC_EDIT7, m_fRotZ);
	DDX_Text(pDX, IDC_EDIT8, m_fScaleChX);
	DDX_Text(pDX, IDC_EDIT9, m_fScaleChY);
	DDX_Text(pDX, IDC_EDIT10, m_fScaleChZ);
	DDX_Text(pDX, IDC_EDIT12, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT13, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT14, m_fScaleZ);
	DDX_Control(pDX, IDC_LIST1, m_iTextureList);
	DDX_Control(pDX, IDC_PICTURE_TEXTURE, m_Texture);
	DDX_Control(pDX, IDC_EDIT1, m_EPosX);
	DDX_Text(pDX, IDC_EDIT11, m_iObjectKey);
	DDX_Control(pDX, IDC_LIST2, m_iObjectList);
	DDX_Text(pDX, IDC_EDIT15, m_iNumX);
	DDX_Text(pDX, IDC_EDIT16, m_iNumY);
	DDX_Text(pDX, IDC_EDIT17, m_iNumZ);
}


BEGIN_MESSAGE_MAP(CDialogTab2, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON12_ADD, &CDialogTab2::OnBnClickedADD)
	ON_BN_CLICKED(IDC_BUTTON11, &CDialogTab2::OnBnClickedTextureChange)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogTab2::OnBnClickedPlusX)
	ON_BN_CLICKED(IDC_BUTTON4, &CDialogTab2::OnBnClickedPlusY)
	ON_BN_CLICKED(IDC_BUTTON5, &CDialogTab2::OnBnClickedPlusZ)
	ON_BN_CLICKED(IDC_BUTTON6, &CDialogTab2::OnBnClickedMinusX)
	ON_BN_CLICKED(IDC_BUTTON7, &CDialogTab2::OnBnClickedMinusY)
	ON_BN_CLICKED(IDC_BUTTON8, &CDialogTab2::OnBnClickedMinusZ)
	ON_BN_CLICKED(IDC_BUTTON13, &CDialogTab2::OnBnClickedScaleChange)
	ON_BN_CLICKED(IDC_BUTTON14, &CDialogTab2::OnBnClickedDelete)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CDialogTab2::OnLbnSelchangeTextureList1)
	ON_BN_CLICKED(IDC_SAVEOBJ, &CDialogTab2::OnBnClickedSaveobj)
	ON_BN_CLICKED(IDC_BUTTON15, &CDialogTab2::OnBnClickedPosChange)
	ON_BN_CLICKED(IDC_CHECK2, &CDialogTab2::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_LOADOBJ, &CDialogTab2::OnBnClickedLoadobj)
	ON_BN_CLICKED(IDC_CHECK3, &CDialogTab2::OnBnClickedCheck3)
	ON_LBN_SELCHANGE(IDC_LIST2, &CDialogTab2::OnLbnSelchangeList2)
	ON_BN_CLICKED(IDC_BUTTON12_ADD2, &CDialogTab2::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON16, &CDialogTab2::OnBnClickedButton16)
END_MESSAGE_MAP()


// CDialogTab2 메시지 처리기입니다.

void CDialogTab2::HorizontalScroll()
{
	CString strName;
	CSize	Size;

	int iCX = 0;

	CDC* pDc = m_iTextureList.GetDC();
	for (int i = 0; i < m_iTextureList.GetCount(); ++i)
	{
		m_iTextureList.GetText(i, strName);
		Size = pDc->GetTextExtent(strName);
		if (Size.cx > iCX)
		{
			iCX = Size.cx;
		}
	}
	m_iTextureList.ReleaseDC(pDc);

	if (m_iTextureList.GetHorizontalExtent() < iCX)
	{
		m_iTextureList.SetHorizontalExtent(iCX);
	}
}

BOOL CDialogTab2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CheckDlgButton(IDC_RADIO3, TRUE);
	CheckDlgButton(IDC_CHECK2, TRUE);
	CheckDlgButton(IDC_CHECK3, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogTab2::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonDown(nFlags, point);
}

void CDialogTab2::OnBnClickedADD()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Add Button
	UpdateData(TRUE);

	GetDlgItemText(IDC_EDIT11, m_iObjectKey);

	m_iObjectList.AddString(m_iObjectKey);

	map<CString, CGameObject*>::iterator iter;

	iter = m_mapGameObject.find(m_iObjectKey);

	if (iter == m_mapGameObject.end())
	{
		CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

		CGameObject* pObject = dynamic_cast<CScene_Tool*>(pScene)->Create_Object(m_iObjectKey);

		AfxMessageBox(L"Create Object!!");

		m_mapGameObject.insert(make_pair(m_iObjectKey, pObject));

		return;
	}

	AfxMessageBox(L"Create Object Failed!!");

	UpdateData(FALSE);
}

void CDialogTab2::OnBnClickedTextureChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Texture Change Button

	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}

	m_iObjectList.GetText(iSelectIndex, strName);

	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

	if (nullptr == dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList())
	{
		AfxMessageBox(L"Don't Make Cube!!");
		return;
	}
	auto iter_Obj = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->begin();
	auto iter_Obj_end = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->end();

	for (; iter_Obj != iter_Obj_end;)
	{
		if (dynamic_cast<CTool_Object*>((*iter_Obj))->GetStrKey() == strName)
		{
			break;
		}
		++iter_Obj;
	}

	auto& iter = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->begin();
	auto& iter_end = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->end();

	for (; iter != iter_end; ++iter)
	{
		if (dynamic_cast<CTool_Cube*>(*iter)->GetPickingCube())
		{
			if (0 > m_iDrawID)
			{
				AfxMessageBox(L"Select Cube Texture!!");
				return;
			}
			dynamic_cast<CTool_Cube*>((*iter))->Change_Texture(m_iDrawID);
			UpdateData(FALSE);
			return;
		}
	}
}

void CDialogTab2::OnBnClickedPlusX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// + x
	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}

	m_iObjectList.GetText(iSelectIndex, strName);

	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

	if (nullptr == dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList())
	{
		AfxMessageBox(L"Don't Make Cube!!");
		return;
	}
	auto iter_Obj = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->begin();
	auto iter_Obj_end = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->end();

	for (; iter_Obj != iter_Obj_end;)
	{
		if (dynamic_cast<CTool_Object*>((*iter_Obj))->GetStrKey() == strName)
		{
			break;
		}
		++iter_Obj;
	}

	auto& iter = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->begin();
	auto& iter_end = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->end();

	for (; iter != iter_end; ++iter)
	{
		if (dynamic_cast<CTool_Cube*>(*iter)->GetPickingCube())
		{
			CComponent* pComponent = dynamic_cast<CTool_Cube*>((*iter))->Get_Component(L"Transform");
			if (IsDlgButtonChecked(IDC_RADIO3))
			{
				_vec3 vPos = dynamic_cast<CTransform*>(pComponent)->Get_MatrixRow(dynamic_cast<CTransform*>(pComponent)->INFO_POSITION);
				vPos.x += 0.5f;
				dynamic_cast<CTool_Cube*>((*iter))->MoveCube(dynamic_cast<CTool_Cube*>((*iter))->POS, vPos);

				CString strValue;
				strValue.Format(_T("%fd"), vPos.x);
				m_fPosX = vPos.x;
				SetDlgItemText(IDC_EDIT1, strValue);
			}
			else if (IsDlgButtonChecked(IDC_RADIO4))
			{
				dynamic_cast<CTool_Cube*>((*iter))->MoveCube(dynamic_cast<CTool_Cube*>((*iter))->ROTATION, _vec3(5.f, 0.f, 0.f));

				m_fRotX = dynamic_cast<CTransform*>(pComponent)->Get_DegreeX();
				SetDlgItemInt(IDC_EDIT5, _uint(m_fRotX));
			}
			UpdateData(FALSE);
			return;
		}
	}
}

void CDialogTab2::OnBnClickedPlusY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// + y
	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}

	m_iObjectList.GetText(iSelectIndex, strName);

	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

	if (nullptr == dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList())
	{
		AfxMessageBox(L"Don't Make Cube!!");
		return;
	}
	auto iter_Obj = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->begin();
	auto iter_Obj_end = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->end();

	for (; iter_Obj != iter_Obj_end;)
	{
		if (dynamic_cast<CTool_Object*>((*iter_Obj))->GetStrKey() == strName)
		{
			break;
		}
		++iter_Obj;
	}

	auto& iter = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->begin();
	auto& iter_end = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->end();

	for (; iter != iter_end; ++iter)
	{
		if (dynamic_cast<CTool_Cube*>(*iter)->GetPickingCube())
		{
			CComponent* pComponent = dynamic_cast<CTool_Cube*>((*iter))->Get_Component(L"Transform");
			if (IsDlgButtonChecked(IDC_RADIO3))
			{
				_vec3 vPos = dynamic_cast<CTransform*>(pComponent)->Get_MatrixRow(dynamic_cast<CTransform*>(pComponent)->INFO_POSITION);
				vPos.y += 0.5f;
				dynamic_cast<CTool_Cube*>((*iter))->MoveCube(dynamic_cast<CTool_Cube*>((*iter))->POS, vPos);
				CString strValue;
				strValue.Format(_T("%fd"), vPos.y);
				m_fPosY = vPos.y;
				SetDlgItemText(IDC_EDIT1, strValue);
			}
			else if (IsDlgButtonChecked(IDC_RADIO4))
			{
				dynamic_cast<CTool_Cube*>((*iter))->MoveCube(dynamic_cast<CTool_Cube*>((*iter))->ROTATION, _vec3(0.f, 5.f, 0.f));
				m_fRotX = dynamic_cast<CTransform*>(pComponent)->Get_DegreeX();
				SetDlgItemInt(IDC_EDIT5, _uint(m_fRotX));
			}
			UpdateData(FALSE);
			return;
		}
	}
}

void CDialogTab2::OnBnClickedPlusZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// ++z
	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}

	m_iObjectList.GetText(iSelectIndex, strName);

	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

	if (nullptr == dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList())
	{
		AfxMessageBox(L"Don't Make Cube!!");
		return;
	}
	auto iter_Obj = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->begin();
	auto iter_Obj_end = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->end();

	for (; iter_Obj != iter_Obj_end;)
	{
		if (dynamic_cast<CTool_Object*>((*iter_Obj))->GetStrKey() == strName)
		{
			break;
		}
		++iter_Obj;
	}

	auto& iter = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->begin();
	auto& iter_end = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->end();

	for (; iter != iter_end; ++iter)
	{
		if (dynamic_cast<CTool_Cube*>(*iter)->GetPickingCube())
		{
			CComponent* pComponent = dynamic_cast<CTool_Cube*>((*iter))->Get_Component(L"Transform");
			if (IsDlgButtonChecked(IDC_RADIO3))
			{
				_vec3 vPos = dynamic_cast<CTransform*>(pComponent)->Get_MatrixRow(dynamic_cast<CTransform*>(pComponent)->INFO_POSITION);
				vPos.z += 0.5f;
				dynamic_cast<CTool_Cube*>((*iter))->MoveCube(dynamic_cast<CTool_Cube*>((*iter))->POS, vPos);
				CString strValue;
				strValue.Format(_T("%fd"), vPos.z);
				m_fPosZ = vPos.z;
				SetDlgItemText(IDC_EDIT3, strValue);
			}
			else if (IsDlgButtonChecked(IDC_RADIO4))
			{
				dynamic_cast<CTool_Cube*>((*iter))->MoveCube(dynamic_cast<CTool_Cube*>((*iter))->ROTATION, _vec3(0.f, 0.f, 5.f));
				m_fRotZ = dynamic_cast<CTransform*>(pComponent)->Get_DegreeZ();
				SetDlgItemInt(IDC_EDIT7, _uint(m_fRotZ));
			}
			UpdateData(FALSE);
			return;
		}
	}
}

void CDialogTab2::OnBnClickedMinusX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// -x
	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}

	m_iObjectList.GetText(iSelectIndex, strName);

	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

	if (nullptr == dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList())
	{
		AfxMessageBox(L"Don't Make Cube!!");
		return;
	}
	auto iter_Obj = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->begin();
	auto iter_Obj_end = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->end();

	for (; iter_Obj != iter_Obj_end;)
	{
		if (dynamic_cast<CTool_Object*>((*iter_Obj))->GetStrKey() == strName)
		{
			break;
		}
		++iter_Obj;
	}

	auto& iter = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->begin();
	auto& iter_end = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->end();

	for (; iter != iter_end; ++iter)
	{
		if (dynamic_cast<CTool_Cube*>(*iter)->GetPickingCube())
		{
			CComponent* pComponent = dynamic_cast<CTool_Cube*>((*iter))->Get_Component(L"Transform");
			if (IsDlgButtonChecked(IDC_RADIO3))
			{
				_vec3 vPos = dynamic_cast<CTransform*>(pComponent)->Get_MatrixRow(dynamic_cast<CTransform*>(pComponent)->INFO_POSITION);
				vPos.x -= 0.5f;
				dynamic_cast<CTool_Cube*>((*iter))->MoveCube(dynamic_cast<CTool_Cube*>((*iter))->POS, vPos);
				CString strValue;
				strValue.Format(_T("%fd"), vPos.x);
				SetDlgItemText(IDC_EDIT1, strValue);
				m_fPosX = vPos.x;
			}
			else if (IsDlgButtonChecked(IDC_RADIO4))
			{
				dynamic_cast<CTool_Cube*>((*iter))->MoveCube(dynamic_cast<CTool_Cube*>((*iter))->ROTATION, _vec3(-5.f, 0.f, 0.f));
				m_fRotX = dynamic_cast<CTransform*>(pComponent)->Get_DegreeX();
				SetDlgItemInt(IDC_EDIT5, _uint(m_fRotX));
			}
			UpdateData(FALSE);
			return;
		}
	}
}

void CDialogTab2::OnBnClickedMinusY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// -y
	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}

	m_iObjectList.GetText(iSelectIndex, strName);

	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

	if (nullptr == dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList())
	{
		AfxMessageBox(L"Don't Make Cube!!");
		return;
	}
	auto iter_Obj = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->begin();
	auto iter_Obj_end = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->end();

	for (; iter_Obj != iter_Obj_end;)
	{
		if (dynamic_cast<CTool_Object*>((*iter_Obj))->GetStrKey() == strName)
		{
			break;
		}
		++iter_Obj;
	}

	auto& iter = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->begin();
	auto& iter_end = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->end();

	for (; iter != iter_end; ++iter)
	{
		if (dynamic_cast<CTool_Cube*>(*iter)->GetPickingCube())
		{
			CComponent* pComponent = dynamic_cast<CTool_Cube*>((*iter))->Get_Component(L"Transform");
			if (IsDlgButtonChecked(IDC_RADIO3))
			{
				_vec3 vPos = dynamic_cast<CTransform*>(pComponent)->Get_MatrixRow(dynamic_cast<CTransform*>(pComponent)->INFO_POSITION);
				vPos.y -= 0.5f;
				dynamic_cast<CTool_Cube*>((*iter))->MoveCube(dynamic_cast<CTool_Cube*>((*iter))->POS, vPos);
				_vec3 vPos3 = dynamic_cast<CTool_Cube*>((*iter))->GetObjectData()->m_vecPos;
				CString strValue;
				strValue.Format(_T("%fd"), vPos.y);
				m_fPosY = vPos.y;
				SetDlgItemText(IDC_EDIT2, strValue);
			}
			else if (IsDlgButtonChecked(IDC_RADIO4))
			{
				dynamic_cast<CTool_Cube*>((*iter))->MoveCube(dynamic_cast<CTool_Cube*>((*iter))->ROTATION, _vec3(0.f, -5.f, 0.f));
				m_fRotY = dynamic_cast<CTransform*>(pComponent)->Get_DegreeY();
				SetDlgItemInt(IDC_EDIT6, _uint(m_fRotY));
			}
			UpdateData(FALSE);
		}
	}
}

void CDialogTab2::OnBnClickedMinusZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// -z
	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}

	m_iObjectList.GetText(iSelectIndex, strName);

	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

	if (nullptr == dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList())
	{
		AfxMessageBox(L"Don't Make Cube!!");
		return;
	}
	auto iter_Obj = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->begin();
	auto iter_Obj_end = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->end();

	for (; iter_Obj != iter_Obj_end;)
	{
		if (dynamic_cast<CTool_Object*>((*iter_Obj))->GetStrKey() == strName)
		{
			break;
		}
		++iter_Obj;
	}

	auto& iter = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->begin();
	auto& iter_end = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->end();

	for (; iter != iter_end; ++iter)
	{
		if (dynamic_cast<CTool_Cube*>(*iter)->GetPickingCube())
		{
			CComponent* pComponent = dynamic_cast<CTool_Cube*>((*iter))->Get_Component(L"Transform");
			if (IsDlgButtonChecked(IDC_RADIO3))
			{
				_vec3 vPos = dynamic_cast<CTransform*>(pComponent)->Get_MatrixRow(dynamic_cast<CTransform*>(pComponent)->INFO_POSITION);
				vPos.z -= 0.5f;
				dynamic_cast<CTool_Cube*>((*iter))->MoveCube(dynamic_cast<CTool_Cube*>((*iter))->POS, vPos);
				CString strValue;
				strValue.Format(_T("%fd"), vPos.z);
				SetDlgItemText(IDC_EDIT3, strValue);
				m_fPosZ = vPos.z;
			}
			else if (IsDlgButtonChecked(IDC_RADIO4))
			{
				dynamic_cast<CTool_Cube*>((*iter))->MoveCube(dynamic_cast<CTool_Cube*>((*iter))->ROTATION, _vec3(0.f, 0.f, -5.f));
				m_fRotZ = dynamic_cast<CTransform*>(pComponent)->Get_DegreeZ();
				SetDlgItemInt(IDC_EDIT7, _uint(m_fRotZ));
			}
			UpdateData(FALSE);
			return;
		}
	}
}

void CDialogTab2::OnBnClickedScaleChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Scale Only Change
	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}

	m_iObjectList.GetText(iSelectIndex, strName);

	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

	if (nullptr == dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList())
	{
		AfxMessageBox(L"Don't Make Cube!!");
		return;
	}
	auto iter_Obj = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->begin();
	auto iter_Obj_end = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->end();

	for (; iter_Obj != iter_Obj_end;)
	{
		if (dynamic_cast<CTool_Object*>((*iter_Obj))->GetStrKey() == strName)
		{
			break;
		}
		++iter_Obj;
	}

	auto& iter = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->begin();
	auto& iter_end = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->end();

	for (; iter != iter_end; ++iter)
	{
		if (dynamic_cast<CTool_Cube*>(*iter)->GetPickingCube())
		{
			m_fScaleChX = float(GetDlgItemInt(IDC_EDIT8));
			m_fScaleChY = float(GetDlgItemInt(IDC_EDIT9));
			m_fScaleChZ = float(GetDlgItemInt(IDC_EDIT10));
			SetDlgItemInt(IDC_EDIT12, UINT(m_fScaleChX));
			SetDlgItemInt(IDC_EDIT13, UINT(m_fScaleChY));
			SetDlgItemInt(IDC_EDIT14, UINT(m_fScaleChZ));
			_vec3 vScale = _vec3((m_fScaleChX), (m_fScaleChY), (m_fScaleChZ));
			dynamic_cast<CTool_Cube*>((*iter))->MoveCube(dynamic_cast<CTool_Cube*>((*iter))->SCALE, vScale);
			UpdateData(FALSE);
			return;
		}
	}
}

void CDialogTab2::OnBnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Delete

	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}

	m_iObjectList.GetText(iSelectIndex, strName);

	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

	if (nullptr == dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList())
	{
		AfxMessageBox(L"Don't Make Cube!!");
		return;
	}
	auto iter_Obj = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->begin();
	auto iter_Obj_end = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->end();

	for (; iter_Obj != iter_Obj_end;)
	{
		if (dynamic_cast<CTool_Object*>((*iter_Obj))->GetStrKey() == strName)
		{
			break;
		}
		++iter_Obj;
	}

	auto& iter = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->begin();
	auto& iter_end = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->end();

	for (; iter != iter_end; ++iter)
	{
		if (dynamic_cast<CTool_Cube*>(*iter)->GetPickingCube())
		{
			dynamic_cast<CTool_Cube*>(*iter)->Set_IsDead();
			AfxMessageBox(L"Delete Cube!!");
			return;
		}
	}
}

void CDialogTab2::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	UpdateData(TRUE);

	int iFileCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	TCHAR szFullPath[MAX_PATH] = L"";

	for (int i = 0; i < iFileCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullPath, MAX_PATH);

		std::wstring wstrRelativePath = RelativePath(szFullPath);

		wstrRelativePath = wstrRelativePath.substr(0, wstrRelativePath.size() - 3) + L"png";

		CString strImageName;

		//##[17.07.18_02]
		//파일의 이름만 얻어오는 함수.
		strImageName = PathFindFileName(wstrRelativePath.c_str());

		//확장자를 제거해주는 함수.
		PathRemoveExtension((LPWSTR)strImageName.operator LPCWSTR());

		//map컨테이너 관리.
		map<CString, CImage*>::iterator iter;

		iter = m_mapPngImage.find(strImageName);

		if (iter == m_mapPngImage.end())
		{
			CImage* pPngImage = new CImage;

			//PNG이미지를 상대경로를 통해서 이미지를 로드한다.
			pPngImage->Load(wstrRelativePath.c_str());

			//키 : 파일이름(ex.Tile0)   
			m_mapPngImage.insert(make_pair(strImageName, pPngImage));
		}


		m_iTextureList.AddString(strImageName);
	}

	HorizontalScroll();

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(FALSE);

}

wstring CDialogTab2::RelativePath(wstring szFullPath)
{
	TCHAR szRelativePath[MAX_PATH] = L"";
	TCHAR szDirectoryPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirectoryPath), szDirectoryPath);

	PathRelativePathTo(szRelativePath, szDirectoryPath, FILE_ATTRIBUTE_DIRECTORY,
		szFullPath.c_str(), FILE_ATTRIBUTE_NORMAL);

	return szRelativePath;

}

void CDialogTab2::OnLbnSelchangeTextureList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_iTextureList.GetCurSel();

	if (iSelectIndex < 0)
		return;

	m_iTextureList.GetText(iSelectIndex, strName);

	map<CString, CImage*>::iterator iter;

	iter = m_mapPngImage.find(strName);

	if (iter == m_mapPngImage.end())
		return;

	m_Texture.SetBitmap(*iter->second);


	int i = 0;

	for (i = 0; i < strName.GetLength(); ++i)
	{
		//문자열을 0번 인덱스부터 검색
		//문자가 숫자인지를 검사해보자. -> 
		if (isdigit(strName[i]) != 0)		//Tile0
		{
			break;
			//함수isdigit()의 반환값이 0이 아닌 수이면 문자가 '숫자'이다.
		}
	}

	//i -> 숫자가 있는곳까지의 Index
	strName.Delete(0, i);
	//0번 인덱스부터 i까지 문자열을 삭제한다.


	//_tstoi() : string to int -> 문자열을 정수형으로 변경하는 함수.
	m_iDrawID = _tstoi(strName);	//strName = Tile0

	UpdateData(FALSE);

}

void CDialogTab2::OnBnClickedSaveobj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this, 0, FALSE);

	TCHAR szDirectory[MAX_PATH] = L"";		//드라이브경로를 받을 문자열 변수.

	GetCurrentDirectory(MAX_PATH, szDirectory);		//현재 프로젝트의 드라이브경로를 받아온다.
	PathRemoveFileSpec(szDirectory);

	lstrcat(szDirectory, L"\\Test");

	Dlg.GetOFN().lpstrInitialDir = szDirectory;

	if (Dlg.DoModal() == IDOK)
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS
			, FILE_ATTRIBUTE_NORMAL, NULL);


		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"저장실패.");
			return;
		}

		DWORD dwByte = 0;

		CString strName;

		int iSelectIndex = m_iObjectList.GetCurSel();

		if (iSelectIndex < 0)
		{
			AfxMessageBox(L"Don't Select Object!!");
			return;
		}

		m_iObjectList.GetText(iSelectIndex, strName);

		map<CString, CGameObject*>::iterator iter_obj;

		iter_obj = m_mapGameObject.find(strName);

		auto& iter = dynamic_cast<CTool_Object*>(iter_obj->second)->GetCube()->begin();
		auto& iter_end = dynamic_cast<CTool_Object*>(iter_obj->second)->GetCube()->end();

		for (; iter != iter_end; ++iter)
		{
			WriteFile(hFile, dynamic_cast<CTool_Cube*>(*iter)->GetObjectData(), sizeof(STROBJECT), &dwByte, NULL);
		}

		AfxMessageBox(L"저장성공!!");
		CloseHandle(hFile);
		return;
	}
}
void CDialogTab2::OnBnClickedLoadobj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}

	m_iObjectList.GetText(iSelectIndex, strName);

	CFileDialog		Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this, 0, FALSE);

	TCHAR szDirectory[MAX_PATH] = L"";		//드라이브경로를 받을 문자열 변수.

	GetCurrentDirectory(MAX_PATH, szDirectory);		//현재 프로젝트의 드라이브경로를 받아온다.
	PathRemoveFileSpec(szDirectory);

	lstrcat(szDirectory, L"\\Test");

	Dlg.GetOFN().lpstrInitialDir = szDirectory;

	if (Dlg.DoModal() == IDOK)
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, NULL, NULL, OPEN_EXISTING
			, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"로드실패.");
			return;
		}
		CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

		DWORD dwByte = 0;

		while (true)
		{
			STROBJECT* pObject = new STROBJECT;
			ZeroMemory(pObject, sizeof(STROBJECT));

			ReadFile(hFile, pObject, sizeof(STROBJECT), &dwByte, NULL);
			pObject;
			if (0 == dwByte)
			{
				Safe_Delete(pObject);
				break;
			}
			CString strKey;
			strKey = (LPCTSTR)pObject->m_strKey;
			dynamic_cast<CScene_Tool*>(pScene)->Create_Cube(pObject->m_vecPos, pObject->m_vecRot, pObject->m_vecScale, pObject->m_iTextureNum, pObject->m_strKey, strKey);
		}
		AfxMessageBox(L"로드성공!!");
		CloseHandle(hFile);
	}
}

void CDialogTab2::PickingCube()
{
	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}

	m_iObjectList.GetText(iSelectIndex, strName);

	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

	if (nullptr == dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList())
	{
		AfxMessageBox(L"Don't Make Cube!!");
		return;
	}
	auto iter_Obj = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->begin();
	auto iter_Obj_end = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->end();

	for (; iter_Obj != iter_Obj_end;)
	{
		if (dynamic_cast<CTool_Object*>((*iter_Obj))->GetStrKey() == strName)
		{
			break;
		}
		++iter_Obj;
	}

	auto& iter = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->begin();
	auto& iter_end = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->end();

	for (; iter != iter_end; iter++)
	{
		if (dynamic_cast<CTool_Cube*>(*iter)->GetPickingCube())
		{
			CComponent* pComponent = dynamic_cast<CTool_Cube*>(*iter)->Get_Component(L"Transform");
			_vec3 vPos = dynamic_cast<CTransform*>(pComponent)->Get_MatrixRow(dynamic_cast<CTransform*>(pComponent)->INFO_POSITION);
			_matrix vScaleMatrix = dynamic_cast<CTransform*>(pComponent)->Get_ScaleMatrix();
			_vec3 vScale = _vec3(vScaleMatrix._11, vScaleMatrix._22, vScaleMatrix._33);
			_vec3 vRot;
			vRot.x = dynamic_cast<CTransform*>(pComponent)->Get_DegreeX();
			vRot.y = dynamic_cast<CTransform*>(pComponent)->Get_DegreeY();
			vRot.z = dynamic_cast<CTransform*>(pComponent)->Get_DegreeZ();
			//10/13 ==> edit control값 으로 바꾸기..... 하고 저장불러오기 
			CString strValue;
			strValue.Format(_T("%fd"), vPos.x);
			SetDlgItemText(IDC_EDIT1, strValue);

			strValue.Format(_T("%fd"), vPos.y);
			SetDlgItemText(IDC_EDIT2, strValue);

			strValue.Format(_T("%fd"), vPos.z);
			SetDlgItemText(IDC_EDIT3, strValue);

			SetDlgItemInt(IDC_EDIT5, int(vRot.x));
			SetDlgItemInt(IDC_EDIT6, int(vRot.y));
			SetDlgItemInt(IDC_EDIT7, int(vRot.z));

			SetDlgItemInt(IDC_EDIT8, int(vScale.x));
			SetDlgItemInt(IDC_EDIT9, int(vScale.y));
			SetDlgItemInt(IDC_EDIT10, int(vScale.z));

			SetDlgItemInt(IDC_EDIT12, int(vScale.x));
			SetDlgItemInt(IDC_EDIT13, int(vScale.y));
			SetDlgItemInt(IDC_EDIT14, int(vScale.z));

			m_iDrawID = dynamic_cast<CTool_Cube*>(*iter)->Get_TextureID();

			m_iTextureList.SetCurSel(m_iDrawID);
			CString strName;

			m_iTextureList.GetText(m_iDrawID, strName);

			map<CString, CImage*>::iterator iter2;

			iter2 = m_mapPngImage.find(strName);

			if (iter2 == m_mapPngImage.end())
				return;

			m_Texture.SetBitmap(*iter2->second);

			OnBnClickedCheck2();

			UpdateData(FALSE);

			return;
			
		}
	}

	AfxMessageBox(L"Don't Picking Failed!!");

	UpdateData(FALSE);

	return;
}

void CDialogTab2::Release()
{ 
	for_each(m_mapGameObject.begin(), m_mapGameObject.end(), CReleaseMap());
	m_mapGameObject.clear();
}



void CDialogTab2::OnBnClickedPosChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// pos change
	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}

	m_iObjectList.GetText(iSelectIndex, strName);

	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

	if (nullptr == dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList())
	{
		AfxMessageBox(L"Don't Make Cube!!");
		return;
	}
	auto iter_Obj = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->begin();
	auto iter_Obj_end = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->end();

	for (; iter_Obj != iter_Obj_end;)
	{
		if (dynamic_cast<CTool_Object*>((*iter_Obj))->GetStrKey() == strName)
		{
			break;
		}
		++iter_Obj;
	}

	auto& iter = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->begin();
	auto& iter_end = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->end();

	for (; iter != iter_end; ++iter)
	{
		if (dynamic_cast<CTool_Cube*>(*iter)->GetPickingCube())
		{
			CString strValue;
			GetDlgItemText(IDC_EDIT1, strValue);
			m_fPosX = float(_tstof(strValue));

			GetDlgItemText(IDC_EDIT2, strValue);
			m_fPosY = float(_tstof(strValue));

			GetDlgItemText(IDC_EDIT3, strValue);
			m_fPosZ = float(_tstof(strValue));

			_vec3 vPos = _vec3(m_fPosX, m_fPosY, m_fPosZ);
			dynamic_cast<CTool_Cube*>((*iter))->MoveCube(dynamic_cast<CTool_Cube*>((*iter))->POS, vPos);
			UpdateData(FALSE);
			return;
		}
	}
}

void CDialogTab2::OnBnClickedCheck2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}

	m_iObjectList.GetText(iSelectIndex, strName);

	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

	if (nullptr == dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList())
	{
		AfxMessageBox(L"Don't Make Cube!!");
		return;
	}
	auto iter_Obj = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->begin();
	auto iter_Obj_end = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->end();

	for (; iter_Obj != iter_Obj_end;)
	{
		if (dynamic_cast<CTool_Object*>((*iter_Obj))->GetStrKey() == strName)
		{
			break;
		}
		++iter_Obj;
	}

	auto& iter = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->begin();
	auto& iter_end = dynamic_cast<CTool_Object*>((*iter_Obj))->GetCube()->end();

	for (; iter != iter_end; ++iter)
	{
		if (IsDlgButtonChecked(IDC_CHECK2))
		{
			dynamic_cast<CScene_Tool*>(pScene)->Set_WireFrame();
		}
		else
		{
			dynamic_cast<CScene_Tool*>(pScene)->Set_WireFrame();
			if (dynamic_cast<CTool_Cube*>(*iter)->GetPickingCube())
			{
				dynamic_cast<CTool_Cube*>((*iter))->Set_WireFrame(true);
				return;
			}
		}
	}
	UpdateData(FALSE);
	return;
}


void CDialogTab2::OnBnClickedCheck3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (IsDlgButtonChecked(IDC_CHECK3))
	{
		m_bPickingCube = true;
	}
	else
	{
		m_bPickingCube = false;
	}
}


void CDialogTab2::OnLbnSelchangeList2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString strName;

	m_bCreateCube = false;

	iSelObjectCur = m_iObjectList.GetCurSel();

	if (iSelObjectCur < 0)
		return;

	m_bCreateCube = true;
}

void CDialogTab2::OnBnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Delete
	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}
	m_iObjectList.GetText(iSelectIndex, strName);

	m_iObjectList.DeleteString(iSelectIndex);

	AfxMessageBox(L"Delete Clear!!");
}

void CDialogTab2::OnBnClickedButton16()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Delete
	CString strName;

	int iSelectIndex = m_iObjectList.GetCurSel();

	if (iSelectIndex < 0)
	{
		AfxMessageBox(L"Don't Select Object!!");
		return;
	}
	m_iObjectList.GetText(iSelectIndex, strName);

	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();

	if (nullptr == dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList())
	{
		AfxMessageBox(L"Don't Make Cube!!");
		return;
	}
	auto iter_Obj = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->begin();
	auto iter_Obj_end = dynamic_cast<CScene_Tool*>(pScene)->Get_CubejList()->end();

	for (; iter_Obj != iter_Obj_end;)
	{
		if (dynamic_cast<CTool_Object*>((*iter_Obj))->GetStrKey() == strName)
		{
			break;
		}
		++iter_Obj;
	}
	m_iNumX = float(GetDlgItemInt(IDC_EDIT15));
	m_iNumY = float(GetDlgItemInt(IDC_EDIT16));
	m_iNumZ = float(GetDlgItemInt(IDC_EDIT17));

	int iNumX = 0;
	int iNumZ = 0;
	
	for (int i = 0; i < m_iNumZ; ++i)
	{
		for (int j = 0; j < m_iNumX; ++j)
		{
			CString strValue;
			GetDlgItemText(IDC_EDIT1, strValue);
			m_fPosX = float(_tstof(strValue));
			GetDlgItemText(IDC_EDIT2, strValue);
			m_fPosY = float(_tstof(strValue));
			GetDlgItemText(IDC_EDIT3, strValue);
			m_fPosZ = float(_tstof(strValue));
			_vec3 vPos = _vec3(m_fPosX + iNumX, m_fPosY, m_fPosZ + iNumZ);
			m_fScaleX = float(GetDlgItemInt(IDC_EDIT12));
			m_fScaleY = float(GetDlgItemInt(IDC_EDIT13));
			m_fScaleZ = float(GetDlgItemInt(IDC_EDIT14));
			_vec3 vScalse = _vec3(m_fScaleX, m_fScaleY, m_fScaleZ);
			m_fRotX = float(GetDlgItemInt(IDC_EDIT5));
			m_fRotY = float(GetDlgItemInt(IDC_EDIT6));
			m_fRotZ = float(GetDlgItemInt(IDC_EDIT7));
			_vec3 vRota = _vec3(m_fRotX, m_fRotY, m_fRotZ);
			TCHAR pKey[256];
			lstrcpy(pKey, strName);
			dynamic_cast<CTool_Object*>(*iter_Obj)->Create_Cube(vPos, vRota, vScalse, m_iDrawID, pKey);
			++iNumX;
		}
		++iNumZ;
		iNumX = 0;
	}
}
