// ../Codes/DialogTab1.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "../Headers/DialogTab1.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MapToolView.h"
#include "Scene_Tool.h"


// CDialogTab1 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogTab1, CDialog)

CDialogTab1::CDialogTab1(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_iInterval(1)
	, m_strHeightName(_T(""))
{
}

CDialogTab1::~CDialogTab1()
{
}

void CDialogTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_iInterval);
	DDX_Control(pDX, IDC_CHECK1, m_bCheckButton);
	DDX_Text(pDX, IDC_EDIT11, m_strHeightName);
	DDX_Control(pDX, IDC_LIST1, m_iTextureList);
	DDX_Control(pDX, IDC_MAPTEXTURE, m_iTexture);
}


BEGIN_MESSAGE_MAP(CDialogTab1, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogTab1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &CDialogTab1::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialogTab1::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialogTab1::OnBnClickedButton3)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDialogTab1::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON9, &CDialogTab1::OnBnClickedButton9)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CDialogTab1 �޽��� ó�����Դϴ�.
BOOL CDialogTab1::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CheckDlgButton(IDC_CHECK1, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDialogTab1::HorizontalScroll()
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

wstring CDialogTab1::RelativePath(wstring szFullPath)
{
	TCHAR szRelativePath[MAX_PATH] = L"";
	TCHAR szDirectoryPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirectoryPath), szDirectoryPath);

	PathRelativePathTo(szRelativePath, szDirectoryPath, FILE_ATTRIBUTE_DIRECTORY,
		szFullPath.c_str(), FILE_ATTRIBUTE_NORMAL);

	return szRelativePath;
}

void CDialogTab1::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iInterval = GetDlgItemInt(IDC_EDIT3);
	// change

	if (_T("") == m_strHeightName)
	{
		AfxMessageBox(L"Don't Create Vertex");
		return;
	}
	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();
}
void CDialogTab1::OnBnClickedCheck1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// Frame don't view
	CScene* pScene = ((CMainFrame*)AfxGetMainWnd())->GetMapView()->GetScene();
	if (IsDlgButtonChecked(IDC_CHECK1))
	{
		dynamic_cast<CScene_Tool*>(pScene)->Change_Texture_view(false);
	}
	else
	{
		dynamic_cast<CScene_Tool*>(pScene)->Change_Texture_view(true);
	}
}

void CDialogTab1::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// SAVE

	//CFileDialog		Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this);

	//if (Dlg.DoModal() == IDCANCEL)
	//	return;

	//Dlg.m_ofn.lpstrInitialDir = L"..\\Data";


	//HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS
	//	, FILE_ATTRIBUTE_NORMAL, NULL);

	//if (INVALID_HANDLE_VALUE == hFile)
	//{
	//	AfxMessageBox(L"Save Failed!!");
	//	return;
	//}

	//DWORD dwByte = 0;
}


void CDialogTab1::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// LOAD
}

void CDialogTab1::OnLbnSelchangeList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	m_iTexture.SetBitmap(*iter->second);
	int i = 0;
	for (i = 0; i < strName.GetLength(); ++i)
	{
		//���ڿ��� 0�� �ε������� �˻�
		//���ڰ� ���������� �˻��غ���. -> 
		if (isdigit(strName[i]) != 0)		//Tile0
		{
			break;
			//�Լ�isdigit()�� ��ȯ���� 0�� �ƴ� ���̸� ���ڰ� '����'�̴�.
		}
	}
	//i -> ���ڰ� �ִ°������� Index
	strName.Delete(0, i);
	//0�� �ε������� i���� ���ڿ��� �����Ѵ�.
	//_tstoi() : string to int -> ���ڿ��� ���������� �����ϴ� �Լ�.
	//m_iDrawID = _tstoi(strName);	//strName = Tile0
	UpdateData(FALSE);
}



void CDialogTab1::OnBnClickedButton9()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

}

void CDialogTab1::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	UpdateData(TRUE);

	int iFileCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	TCHAR szFullPath[MAX_PATH] = L"";

	for (int i = 0; i < iFileCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullPath, MAX_PATH);

		std::wstring wstrRelativePath = RelativePath(szFullPath);

		CString strImageName;

		//##[17.07.18_02]
		//������ �̸��� ������ �Լ�.
		strImageName = PathFindFileName(wstrRelativePath.c_str());


		//Ȯ���ڸ� �������ִ� �Լ�.
		PathRemoveExtension((LPWSTR)strImageName.operator LPCWSTR());

		//map�����̳� ����.
		map<CString, CImage*>::iterator iter;

		iter = m_mapPngImage.find(strImageName);

		if (iter == m_mapPngImage.end())
		{
			CImage* pPngImage = new CImage;

			//PNG�̹����� ����θ� ���ؼ� �̹����� �ε��Ѵ�.
			pPngImage->Load(wstrRelativePath.c_str());

			//Ű : �����̸�(ex.Tile0)   
			m_mapPngImage.insert(make_pair(strImageName, pPngImage));
		}
		m_iTextureList.AddString(strImageName);
	}

	HorizontalScroll();


	CDialog::OnDropFiles(hDropInfo);


	UpdateData(FALSE);
}
