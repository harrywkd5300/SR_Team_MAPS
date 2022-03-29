// ../codes/Page_Body.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AnimationTool.h"
#include "../headers/Page_Body.h"
#include "afxdialogex.h"
#include "Creature_Manager.h"
#include "graphic_device.h"
#include "component_manager.h"
#include "object_Manager.h"
/*
CString -> TCHAR

CString strString = _T(" 형변환");
TCHAR  *tchr =  (TCHAR*)(LPCTSTR)strSTring

배열 초기화에도 사용 가능

TCHAR -> CString

TCHAR  *tchr =  _T(" 형변환");
CString strString

1. strString.Format("%s",tchr);
2.strString =(LPCTSTR)tchr;

*/

IMPLEMENT_DYNAMIC(CPage_Body, CPropertyPage)

CPage_Body::CPage_Body()
	: CPropertyPage(IDD_PAGE_BODY)
	,m_pCreature_Manager(CCreature_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	,m_pCurCreature(nullptr)
	, m_CreatureName(_T(""))
	, m_fBonePosX(0)
	, m_fBonePosY(0)
	, m_fBonePosZ(0)
	, m_iChildIndex(-1)
	, m_iParentIndex(-1)
	, m_fCubeScaleX(1)
	, m_fCubeScaleY(1)
	, m_fCubeScaleZ(1)
	, m_fCubePosX(0)
	, m_fCubePosY(0)
	, m_fCubePosZ(0)
	, m_iCubeParentIndex(-1)
	, m_iCubeTexNum(0)
{
}
CPage_Body::~CPage_Body()
{
}


void CPage_Body::PreInitDialog()
{
	CRect rc;

	GetWindowRect(&rc);
	CPropertyPage::PreInitDialog();
	MoveWindow(0, 0, 300, 1000);

	m_pGraphicDev = CGraphic_Device::GetInstance()->Get_GraphicDev();
	
	((CButton*)GetDlgItem(IDC_CHECK_WireFrame))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_Show_BoneCube))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_Show_BoneCoord))->SetCheck(FALSE);


}


void CPage_Body::OnBnClickedCreatureAdd()
{
	UpdateData(TRUE);

	_tchar* pName = new _tchar[64];
	lstrcpy(pName, (_tchar*)(LPCTSTR)m_CreatureName);

	CCreature*	pCreature = CCreature::Create(m_pGraphicDev);
	
	if (FAILED(m_pCreature_Manager->Add_Creature(pName, pCreature)))
	{
		Safe_Release(pCreature);
		UpdateData(FALSE);
		return;
	}

	m_Creature_ListBox.AddString(m_CreatureName);
	m_pCurCreature = m_pCreature_Manager->Get_Creature(pName);
	m_pCreature_Manager->Set_CurCreature(m_pCurCreature);

	m_Creature_ListBox.SetCurSel(0);

	UpdateData(FALSE);
}


/* Creature Delete Button*/
void CPage_Body::OnBnClickedCreatureDel()
{
	UpdateData(TRUE);
	int iIndex = m_Creature_ListBox.GetCurSel();
	CString strName;

	if (iIndex < 0)
		goto exception;

	m_Creature_ListBox.GetText(iIndex, strName);
	_tchar* pName = (_tchar*)(LPCTSTR)strName;
	
	if (FAILED(m_pCreature_Manager->Del_Creature(pName)))
		goto exception;

	m_Creature_ListBox.DeleteString(iIndex);
	
	m_Creature_ListBox.SetCurSel(0);

	m_Creature_ListBox.GetText(0, strName);
	pName = (_tchar*)(LPCTSTR)strName;

	m_pCurCreature = m_pCreature_Manager->Get_Creature(pName);
	m_pCreature_Manager->Set_CurCreature(m_pCurCreature);

	UpdateData(FALSE);

exception:
	UpdateData(FALSE);
	return;

}

/*Edit Box Creature Name*/
void CPage_Body::OnEnChangeCreatureName()
{

}

/*Creature List Box*/
void CPage_Body::OnLbnSelchangeCreatureList()
{
	UpdateData(TRUE);


	Refresh_BoneData();
	Refresh_CubeData();
	
	UpdateData(FALSE);
}



/*Bone List Box*/

void CPage_Body::OnLbnSelchangeBoneList()
{
	UpdateData(TRUE);
	int iIndex = m_Bone_ListBox.GetCurSel();

	if (iIndex < 0)
	{
		UpdateData(FALSE);
		return;
	}

	vector<CBone*>*	pVecBone = m_pCurCreature->Get_BoneVector();
	
	m_fBonePosX = (*pVecBone)[iIndex]->Get_PositionX();
	m_fBonePosY = (*pVecBone)[iIndex]->Get_PositionY();
	m_fBonePosZ = (*pVecBone)[iIndex]->Get_PositionZ();

	m_iChildIndex = iIndex;
	m_iParentIndex = (*pVecBone)[iIndex]->Get_ParentIndex();



	UpdateData(FALSE);
}

/*Add Bone*/
void CPage_Body::OnBnClickedBoneAdd()
{
	if (nullptr == m_pCurCreature)
	{
		AfxMessageBox(L"CurCreature nullptr");
		return;
	}

	UpdateData(TRUE);

 	m_pCurCreature->Add_Bone();


	Refresh_Data();
	UpdateData(FALSE);
}


/*Del Bone*/
void CPage_Body::OnBnClickedBoneDel()
{
	if (nullptr == m_pCurCreature)
	{
		AfxMessageBox(L"CurCreature nullptr");
		return;
	}

	UpdateData(TRUE);
	int iIndex = m_Bone_ListBox.GetCurSel();
	m_pCurCreature->Del_Bone(iIndex);

	m_Bone_ListBox.DeleteString(iIndex);

	m_Bone_ListBox.SetCurSel(0);

	Refresh_Data();

	UpdateData(FALSE);
}


/* local x*/
void CPage_Body::OnEnChangeBoneLocalx()
{

	UpdateData(TRUE);
	
	vector<CBone*>*	pVecBone = m_pCurCreature->Get_BoneVector();
	

	int iIndex = m_Bone_ListBox.GetCurSel();

	if ((*pVecBone).size() <= (size_t)iIndex)
	{
		UpdateData(FALSE);
		return;
	}

	(*pVecBone)[iIndex]->Set_PositionX(m_fBonePosX);
	(*pVecBone)[iIndex]->Fill_WorldMatrix();
	UpdateData(FALSE);

}


/* local y */
void CPage_Body::OnEnChangeBoneLocaly()
{
	UpdateData(TRUE);
	vector<CBone*>*	pVecBone = m_pCurCreature->Get_BoneVector();
	int iIndex = m_Bone_ListBox.GetCurSel();

	if ((*pVecBone).size() <= (size_t)iIndex)
	{
		UpdateData(FALSE);
		return;
	}

	(*pVecBone)[iIndex]->Set_PositionY(m_fBonePosY);
	(*pVecBone)[iIndex]->Fill_WorldMatrix();
	UpdateData(FALSE);
}


/* local z */
void CPage_Body::OnEnChangeBoneLocalz()
{
	UpdateData(TRUE);
	vector<CBone*>*	pVecBone = m_pCurCreature->Get_BoneVector();
	int iIndex = m_Bone_ListBox.GetCurSel();

	if ((*pVecBone).size() <= (size_t)iIndex)
	{
		UpdateData(FALSE);
		return;
	}

	(*pVecBone)[iIndex]->Set_PositionZ(m_fBonePosZ);
	(*pVecBone)[iIndex]->Fill_WorldMatrix();
	UpdateData(FALSE);
}

void CPage_Body::OnEnChangeBoneChildindex()
{

}

/*bone parent index*/
void CPage_Body::OnEnChangeBoneParentindex()
{
	vector<CBone*>*	pVecBone = m_pCurCreature->Get_BoneVector();

	UpdateData(TRUE);

	if (m_iChildIndex == -1 || (size_t)m_iChildIndex >= pVecBone->size())
	{
		UpdateData(FALSE);
		return;
	}
	if (m_iParentIndex == -1 || (size_t)m_iParentIndex >= pVecBone->size())
	{
		UpdateData(FALSE);
		return;
	}

	CBone* pChild = (*pVecBone)[m_iChildIndex];
	CBone* pParent = (*pVecBone)[m_iParentIndex];

	pChild->Parent(pParent->Get_WorldMatrix());
	pChild->Set_ParentIndex(m_iParentIndex);
	pChild->Fill_WorldMatrix();

	UpdateData(FALSE);

}



void CPage_Body::Refresh_Data()
{
	UpdateData(TRUE);

	/*Refresh Current Creature*/
	int iIndex = m_Creature_ListBox.GetCurSel();

	if (iIndex < 0)
	{
		UpdateData(FALSE);
		return;
	}

	CString strName;

	m_Creature_ListBox.GetText(iIndex, strName);
	_tchar* pName = (_tchar*)(LPCTSTR)strName;

	m_pCurCreature = m_pCreature_Manager->Get_Creature(pName);
	m_pCreature_Manager->Set_CurCreature(m_pCurCreature);
	/*Get CurCreature*/


	/*Refresh Bone_List*/
	m_Bone_ListBox.ResetContent();

	vector<CBone*>*	pVecBone = m_pCurCreature->Get_BoneVector();

	for (size_t i = 0; i < pVecBone->size(); ++i)
	{
		_int n = _int(i);
		CString str;
		str.Format(_T("%d"), n);

		m_Bone_ListBox.AddString(str);
	}

	m_Bone_ListBox.SetCurSel(0);

	if (0 == pVecBone->size()) /*empty*/
	{
		m_fBonePosX = -1;
		m_fBonePosY = -1;
		m_fBonePosZ = -1;
		m_iChildIndex = -1;
		m_iParentIndex = -1;

	}
	else
	{
		m_fBonePosX = (*pVecBone)[0]->Get_PositionX();
		m_fBonePosY = (*pVecBone)[0]->Get_PositionY();
		m_fBonePosZ = (*pVecBone)[0]->Get_PositionZ();

		m_iChildIndex = m_Bone_ListBox.GetCurSel();
		(*pVecBone)[m_Bone_ListBox.GetCurSel()]->Get_ParentIndex();
	}
}

void CPage_Body::Refresh_BoneData()
{
	/*Refresh Current Creature*/
	int iIndex = m_Creature_ListBox.GetCurSel();

	if (iIndex < 0)
		return;

	CString strName;

	m_Creature_ListBox.GetText(iIndex, strName);
	_tchar* pName = (_tchar*)(LPCTSTR)strName;

	m_pCurCreature = m_pCreature_Manager->Get_Creature(pName);
	m_pCreature_Manager->Set_CurCreature(m_pCurCreature);


	/*Refresh Current Bone*/
	m_Bone_ListBox.ResetContent();

	vector<CBone*>*	pVecBone = m_pCurCreature->Get_BoneVector();

	if (pVecBone->empty())
		return;
	
	for (size_t i = 0; i < pVecBone->size(); ++i)
	{
		_int n = _int(i);
		CString str;
		str.Format(_T("%d"), n);

		m_Bone_ListBox.AddString(str);

	}

	m_Bone_ListBox.SetCurSel(0);
}


/*=========================BODY============================*/

/*Cube List*/
void CPage_Body::OnLbnSelchangeCubeList()
{
	UpdateData(TRUE);
	int iIndex = m_Cube_ListBox.GetCurSel();

	if (iIndex < 0)
	{
		UpdateData(FALSE);
		return;
	}

	vector<TOOL_BODY*>*	pVecBody = m_pCurCreature->Get_BodyVector();

	m_fCubeScaleX = (*pVecBody)[iIndex]->vScale.x;
	m_fCubeScaleY = (*pVecBody)[iIndex]->vScale.y;
	m_fCubeScaleZ = (*pVecBody)[iIndex]->vScale.z;

	m_fCubePosX = (*pVecBody)[iIndex]->vPosition.x;
	m_fCubePosY = (*pVecBody)[iIndex]->vPosition.y;
	m_fCubePosZ = (*pVecBody)[iIndex]->vPosition.z;
	
	m_iCubeParentIndex = (*pVecBody)[iIndex]->iParentIndex;
	m_iCubeTexNum = (*pVecBody)[iIndex]->iTextureNum;

	UpdateData(FALSE);
}


void CPage_Body::Refresh_CubeData()
{
	UpdateData(TRUE);

	/*Refresh Current Creature*/
	int iIndex = m_Creature_ListBox.GetCurSel();

	if (iIndex < 0)
	{
		UpdateData(FALSE);
		return;
	}

	CString strName;

	m_Creature_ListBox.GetText(iIndex, strName);
	_tchar* pName = (_tchar*)(LPCTSTR)strName;

	m_pCurCreature = m_pCreature_Manager->Get_Creature(pName);
	m_pCreature_Manager->Set_CurCreature(m_pCurCreature);
	/*Get CurCreature*/


	/*Refresh Cube_List*/
	m_Cube_ListBox.ResetContent();

	vector<TOOL_BODY*>* pVecBody = m_pCurCreature->Get_BodyVector();

	if (pVecBody->empty())
		return;

	for (size_t i = 0; i < pVecBody->size(); ++i)
	{
		_int n = _int(i);
		CString str;
		str.Format(_T("%d"), n);

		m_Cube_ListBox.AddString(str);
	}

	m_Cube_ListBox.SetCurSel(0);


	if (0 == pVecBody->size()) /*empty*/
	{
		m_fCubeScaleX = -1;
		m_fCubeScaleY = -1;
		m_fCubeScaleZ = -1;

		m_fCubePosX	=	-1;
		m_fCubePosY	=	-1;
		m_fCubePosZ	=	-1;

		m_iCubeParentIndex = -1;
	}
	else
	{
		m_fCubeScaleX = (*pVecBody)[0]->vScale.x;
		m_fCubeScaleY = (*pVecBody)[0]->vScale.y;
		m_fCubeScaleZ = (*pVecBody)[0]->vScale.z;

		m_fCubePosX = (*pVecBody)[0]->vPosition.x;
		m_fCubePosY = (*pVecBody)[0]->vPosition.y;
		m_fCubePosZ = (*pVecBody)[0]->vPosition.z;

		m_iCubeParentIndex = (*pVecBody)[0]->iParentIndex;
		m_iCubeTexNum = (*pVecBody)[0]->iTextureNum;
	}
	
	UpdateData(FALSE);

}


/* Add Cube */
void CPage_Body::OnBnClickedCubeAdd()
{

	if (nullptr == m_pCurCreature)
	{
		AfxMessageBox(L"CurCreature nullptr");
		return;
	}

	UpdateData(TRUE);

	m_pCurCreature->Add_Cube();
	Refresh_CubeData();

	UpdateData(FALSE);
}

/* Del Cube*/
void CPage_Body::OnBnClickedCubeDel()
{
	if (nullptr == m_pCurCreature)
	{
		AfxMessageBox(L"CurCreature nullptr");
		return;
	}

	UpdateData(TRUE);
	int iIndex = m_Cube_ListBox.GetCurSel();
	m_pCurCreature->Del_Cube(iIndex);

	m_Cube_ListBox.DeleteString(iIndex);

	m_Cube_ListBox.SetCurSel(0);

	Refresh_CubeData();

	UpdateData(FALSE);
}


void CPage_Body::OnEnChangeCubeScalex()
{
	vector<TOOL_BODY*>*	pVecBody = m_pCurCreature->Get_BodyVector();
	int iIndex = m_Cube_ListBox.GetCurSel();

	if ((*pVecBody).size() <= (size_t)iIndex)
		return;

	UpdateData(TRUE);

	(*pVecBody)[iIndex]->vScale.x = m_fCubeScaleX;

	UpdateData(FALSE);
}

void CPage_Body::OnEnChangeCubeScaley()
{
	vector<TOOL_BODY*>*	pVecBody = m_pCurCreature->Get_BodyVector();
	int iIndex = m_Cube_ListBox.GetCurSel();

	if ((*pVecBody).size() <= (size_t)iIndex)
		return;

	UpdateData(TRUE);

	(*pVecBody)[iIndex]->vScale.y = m_fCubeScaleY;

	UpdateData(FALSE);
}

void CPage_Body::OnEnChangeCubeScalez()
{
	vector<TOOL_BODY*>*	pVecBody = m_pCurCreature->Get_BodyVector();
	int iIndex = m_Cube_ListBox.GetCurSel();

	if ((*pVecBody).size() <= (size_t)iIndex)
		return;
	
	UpdateData(TRUE);

	(*pVecBody)[iIndex]->vScale.z = m_fCubeScaleZ;
	
	UpdateData(FALSE);
}


void CPage_Body::OnEnChangeCubePosx()
{
	vector<TOOL_BODY*>*	pVecBody = m_pCurCreature->Get_BodyVector();
	int iIndex = m_Cube_ListBox.GetCurSel();

	if ((*pVecBody).size() <= (size_t)iIndex)
		return;

	UpdateData(TRUE);

	(*pVecBody)[iIndex]->vPosition.x = m_fCubePosX;

	UpdateData(FALSE);
}


void CPage_Body::OnEnChangeCubePosy()
{
	vector<TOOL_BODY*>*	pVecBody = m_pCurCreature->Get_BodyVector();
	int iIndex = m_Cube_ListBox.GetCurSel();

	if ((*pVecBody).size() <= (size_t)iIndex)
		return;

	UpdateData(TRUE);

	(*pVecBody)[iIndex]->vPosition.y = m_fCubePosY;

	UpdateData(FALSE);
}


void CPage_Body::OnEnChangeCubePosz()
{
	vector<TOOL_BODY*>*	pVecBody = m_pCurCreature->Get_BodyVector();
	int iIndex = m_Cube_ListBox.GetCurSel();

	if ((*pVecBody).size() <= (size_t)iIndex)
		return;

	UpdateData(TRUE);

	(*pVecBody)[iIndex]->vPosition.z = m_fCubePosZ;

	UpdateData(FALSE);
}


void CPage_Body::OnEnChangeCubeParent()
{
	vector<CBone*>*	pVecBone = m_pCurCreature->Get_BoneVector();
	vector<TOOL_BODY*>*	pVecBody = m_pCurCreature->Get_BodyVector();
	int iIndex = m_Cube_ListBox.GetCurSel();

	UpdateData(TRUE);


	if (iIndex < 0)
	{
		UpdateData(FALSE);
		return;
	}

	if (m_iCubeParentIndex == -1 || (size_t)m_iCubeParentIndex >= pVecBone->size())
	{
		UpdateData(FALSE);
		return;
	}


	(*pVecBody)[iIndex]->iParentIndex = m_iCubeParentIndex;

	UpdateData(FALSE);
}

void CPage_Body::OnBnClickedCheckWireframe()
{
	UpdateData(TRUE);

	if(IsDlgButtonChecked(IDC_CHECK_WireFrame))
	{
		m_pCreature_Manager->bWIREMODE = true;
	}
	else
	{
		m_pCreature_Manager->bWIREMODE = false;
	}
	
	UpdateData(FALSE);
}


void CPage_Body::OnBnClickedShowBonecube()
{

	UpdateData(TRUE);
	if (IsDlgButtonChecked(IDC_Show_BoneCube))
	{
		m_pCreature_Manager->bBoneRender = true;
	}
	else
	{
		m_pCreature_Manager->bBoneRender = false;
	}
	UpdateData(FALSE);

}


void CPage_Body::OnBnClickedShowBonecoord()
{
	UpdateData(TRUE);
	if (IsDlgButtonChecked(IDC_Show_BoneCoord))
	{
		m_pCreature_Manager->bBoneCoord = true;
	}
	else
	{
		m_pCreature_Manager->bBoneCoord = false;
	}
	UpdateData(FALSE);
}

void CPage_Body::OnBnClickedCheckBody()
{
	UpdateData(TRUE);
	if (IsDlgButtonChecked(IDC_CHECK_Body))
	{
		m_pCreature_Manager->bCubeCoord = true;
	}
	else
	{
		m_pCreature_Manager->bCubeCoord = false;
	}
	UpdateData(FALSE);
}


/*Save*/
void CPage_Body::OnBnClickedCreatureSave()
{
	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this, 0, FALSE);

	TCHAR szDirectory[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szDirectory);
	PathRemoveFileSpec(szDirectory);

	//lstrcat(szDirectory, L"\\Test");

	Dlg.GetOFN().lpstrInitialDir = szDirectory;

	if (Dlg.DoModal() == IDOK)
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Save Failed.");
			return;
		}

		_ulong dwByte = 0;

		CCreature*	pCreature = m_pCreature_Manager->Get_CurCreature();

		/* Bone */
		/* 
		size_t	BoneSize
		_vec3	Position
		_int	ParentIndex
		*/
		vector<CBone*>* pVecBone = pCreature->Get_BoneVector();

		size_t iBoneSize= pVecBone->size();
		WriteFile(hFile, &iBoneSize, sizeof(size_t), &dwByte, NULL);

		for (size_t i = 0; i < pVecBone->size(); ++i)
		{
			_float fx = (*pVecBone)[i]->Get_PositionX();
			_float fy = (*pVecBone)[i]->Get_PositionY();
			_float fz = (*pVecBone)[i]->Get_PositionZ();
			_vec3	v = _vec3(fx, fy, fz);
			
			_int	iParentIndex = (*pVecBone)[i]->Get_ParentIndex();

			WriteFile(hFile, &v, sizeof(_vec3), &dwByte, NULL);
			WriteFile(hFile, &iParentIndex, sizeof(_int), &dwByte, NULL);

		}

		/* Body */
		/*
		size_t	BodySize
		_vec3	Scale;
		_vec3	Position;
		int		ParentIndex;
		*/

		vector<TOOL_BODY*>*	pVecBody = pCreature->Get_BodyVector();

		size_t iBodySize = pVecBody->size();
		WriteFile(hFile, &iBodySize, sizeof(size_t), &dwByte, NULL);
		
		for (size_t i = 0; i < pVecBody->size(); ++i)
		{
			_vec3 vS = (*pVecBody)[i]->vScale;
			_vec3 vP = (*pVecBody)[i]->vPosition;

			_int	iParentIndex = (*pVecBody)[i]->iParentIndex;
			_int	iTexNum = (*pVecBody)[i]->iTextureNum;

			WriteFile(hFile, &vS, sizeof(_vec3), &dwByte, NULL);
			WriteFile(hFile, &vP, sizeof(_vec3), &dwByte, NULL);
			WriteFile(hFile, &iParentIndex, sizeof(_int), &dwByte, NULL);
			WriteFile(hFile, &iTexNum, sizeof(_int), &dwByte, NULL);
		}

		AfxMessageBox(L"Successfully Saved");
		CloseHandle(hFile);


		return;
	}

}

void CPage_Body::OnBnClickedCreatureLoad()
{
	UpdateData(TRUE);
	CCreature*	pCreature = nullptr;
	CString strFileName;

	CFileDialog		Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this, 0, FALSE);

	TCHAR szDirectory[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szDirectory);
	PathRemoveFileSpec(szDirectory);

	Dlg.GetOFN().lpstrInitialDir = szDirectory;

	if (Dlg.DoModal() == IDOK)
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		strFileName = Dlg.GetPathName();
		//CString strFileName = Dlg.GetPathName();
		strFileName = RelativePath(strFileName);
		strFileName = PathFindFileName(strFileName);
		PathRemoveExtension((LPWSTR)strFileName.operator LPCWSTR());

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Load Failed.");
			return;
		}

		_ulong dwByte = 0;

		pCreature = CCreature::Create(m_pGraphicDev);

		size_t iBoneSize = 0;
		ReadFile(hFile, &iBoneSize, sizeof(size_t), &dwByte, NULL);

		for(size_t i=0; i<iBoneSize; ++i)
		{
			_vec3 vPos;
			_int iParentIndex = 0;
			
			ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, NULL);
			ReadFile(hFile, &iParentIndex, sizeof(_int), &dwByte, NULL);

			pCreature->Add_Bone(vPos, iParentIndex);
		}

		/*Link Parent Vector*/
		vector<CBone*>*	pBoneVector = pCreature->Get_BoneVector();
	
		for (size_t i = 0; i < pBoneVector->size(); ++i)
		{
			_int iIndex = (*pBoneVector)[i]->Get_ParentIndex();
			
			if (-1 == iIndex)
				continue;

			(*pBoneVector)[i]->Parent((*pBoneVector)[iIndex]->Get_WorldMatrix());
			(*pBoneVector)[i]->Fill_WorldMatrix();
		}

		size_t iBodySize = 0;
		ReadFile(hFile, &iBodySize, sizeof(size_t), &dwByte, NULL);

		for (size_t i = 0; i < iBodySize; ++i)
		{
			_vec3 vScale;
			_vec3 vPos;
			_int iParentIndex = 0;
			_int iTextureNum = 0;

			ReadFile(hFile, &vScale, sizeof(_vec3), &dwByte, NULL);
			ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, NULL);
			ReadFile(hFile, &iParentIndex, sizeof(_int), &dwByte, NULL);
			ReadFile(hFile, &iTextureNum, sizeof(_int), &dwByte, NULL);

 			TOOL_BODY* pBody = new TOOL_BODY;
 
 			pBody->vPosition = vPos;
 			pBody->vScale= vScale;
 			pBody->iParentIndex = iParentIndex;
			pBody->iTextureNum = iTextureNum;

			pCreature->Add_Cube(pBody);
		}


		CloseHandle(hFile);
	}
	_tchar* pName = new _tchar[64];
	lstrcpy(pName, (_tchar*)(LPCTSTR)strFileName);

	m_pCreature_Manager->Add_Creature(pName, pCreature);
	m_Creature_ListBox.AddString(pName);
	UpdateData(FALSE);
}



void CPage_Body::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);

	_int iFileCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	TCHAR szFullPath[MAX_PATH] = L"";

	for (_int i = 0; i < iFileCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullPath, MAX_PATH);

		std::wstring wstrRelativePath = RelativePath(szFullPath);

		CString strImageName;
		strImageName = PathFindFileName(wstrRelativePath.c_str());
		PathRemoveExtension((LPWSTR)strImageName.operator LPCWSTR());

		map<CString, CImage*>::iterator iter;

		iter = m_mapCubeImage.find(strImageName);

		if (iter == m_mapCubeImage.end())
		{
			CImage* pPngImage = new CImage;
			pPngImage->Load(wstrRelativePath.c_str());
			m_mapCubeImage.insert(make_pair(strImageName, pPngImage));
		}
		m_DDSListBox.AddString(strImageName);
	}

	CPropertyPage::OnDropFiles(hDropInfo);

	UpdateData(FALSE);

}


void CPage_Body::OnLbnSelchangeCubeddslist()
{
	UpdateData(TRUE);
	CString strName;

	int iSelectIndex = m_DDSListBox.GetCurSel();
	
	if (iSelectIndex < 0)
		return;

	m_DDSListBox.GetText(iSelectIndex, strName);

	map<CString, CImage*>::iterator iter;
	iter = m_mapCubeImage.find(strName);

	if (iter == m_mapCubeImage.end())
		return;

	m_PictureCube.SetBitmap(*iter->second);

	_int i = 0;
	for (i = 0; i < strName.GetLength(); ++i)
	{
		if (isdigit(strName[i]) != 0)	
			break;
	}
	strName.Delete(0, i);
	m_iCurCubeTexNum = _tstoi(strName);

	UpdateData(FALSE);
}

void CPage_Body::OnBnClickedCubeTexapply()
{
	_int iCubeIndex = m_Cube_ListBox.GetCurSel();

	if (iCubeIndex < 0)
		return;

	vector<TOOL_BODY*>* pb = m_pCurCreature->Get_BodyVector();

	if ((size_t)iCubeIndex >= pb->size())
		return;

	UpdateData(TRUE);

	(*pb)[iCubeIndex]->iTextureNum = m_iCurCubeTexNum;

	UpdateData(FALSE);

}


/*==========MFC default ====*/
void CPage_Body::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CREATURE_LIST, m_Creature_ListBox);
	DDX_Text(pDX, IDC_CREATURE_NAME, m_CreatureName);
	DDX_Control(pDX, IDC_Bone_List, m_Bone_ListBox);
	DDX_Text(pDX, IDC_Bone_LocalX, m_fBonePosX);
	DDX_Text(pDX, IDC_Bone_LocalY, m_fBonePosY);
	DDX_Text(pDX, IDC_Bone_LocalZ, m_fBonePosZ);
	DDX_Text(pDX, IDC_Bone_ChildIndex, m_iChildIndex);
	DDX_Text(pDX, IDC_Bone_ParentIndex, m_iParentIndex);
	DDX_Control(pDX, IDC_Cube_List, m_Cube_ListBox);
	DDX_Text(pDX, IDC_Cube_ScaleX, m_fCubeScaleX);
	DDX_Text(pDX, IDC_Cube_ScaleY, m_fCubeScaleY);
	DDX_Text(pDX, IDC_Cube_ScaleZ, m_fCubeScaleZ);
	DDX_Text(pDX, IDC_Cube_PosX, m_fCubePosX);
	DDX_Text(pDX, IDC_Cube_PosY, m_fCubePosY);
	DDX_Text(pDX, IDC_Cube_PosZ, m_fCubePosZ);
	DDX_Text(pDX, IDC_Cube_Parent, m_iCubeParentIndex);
	DDX_Control(pDX, IDC_CHECK_WireFrame, m_CheckWireFrame);
	DDX_Control(pDX, IDC_Show_BoneCube, m_CheckBoneCube);
	DDX_Control(pDX, IDC_Show_BoneCoord, m_CheckBoneCoord);
	DDX_Control(pDX, IDC_CubeDDSList, m_DDSListBox);
	DDX_Control(pDX, IDC_Picture_Cube, m_PictureCube);
	DDX_Text(pDX, IDC_Cube_TextureNum, m_iCubeTexNum);
}


BEGIN_MESSAGE_MAP(CPage_Body, CPropertyPage)
	ON_BN_CLICKED(IDC_CREATURE_ADD, &CPage_Body::OnBnClickedCreatureAdd)
	ON_BN_CLICKED(IDC_CREATURE_DEL, &CPage_Body::OnBnClickedCreatureDel)
	ON_EN_CHANGE(IDC_CREATURE_NAME, &CPage_Body::OnEnChangeCreatureName)
	ON_LBN_SELCHANGE(IDC_CREATURE_LIST, &CPage_Body::OnLbnSelchangeCreatureList)
	ON_LBN_SELCHANGE(IDC_Bone_List, &CPage_Body::OnLbnSelchangeBoneList)
	ON_BN_CLICKED(IDC_Bone_Add, &CPage_Body::OnBnClickedBoneAdd)
	ON_BN_CLICKED(IDC_Bone_Del, &CPage_Body::OnBnClickedBoneDel)
	ON_EN_CHANGE(IDC_Bone_LocalX, &CPage_Body::OnEnChangeBoneLocalx)
	ON_EN_CHANGE(IDC_Bone_LocalY, &CPage_Body::OnEnChangeBoneLocaly)
	ON_EN_CHANGE(IDC_Bone_LocalZ, &CPage_Body::OnEnChangeBoneLocalz)
	ON_EN_CHANGE(IDC_Bone_ChildIndex, &CPage_Body::OnEnChangeBoneChildindex)
	ON_EN_CHANGE(IDC_Bone_ParentIndex, &CPage_Body::OnEnChangeBoneParentindex)
	ON_BN_CLICKED(IDC_Cube_Del, &CPage_Body::OnBnClickedCubeDel)
	ON_BN_CLICKED(IDC_Cube_Add, &CPage_Body::OnBnClickedCubeAdd)
	ON_EN_CHANGE(IDC_Cube_ScaleY, &CPage_Body::OnEnChangeCubeScaley)
	ON_EN_CHANGE(IDC_Cube_ScaleX, &CPage_Body::OnEnChangeCubeScalex)
	ON_EN_CHANGE(IDC_Cube_ScaleZ, &CPage_Body::OnEnChangeCubeScalez)
	ON_EN_CHANGE(IDC_Cube_PosX, &CPage_Body::OnEnChangeCubePosx)
	ON_EN_CHANGE(IDC_Cube_PosY, &CPage_Body::OnEnChangeCubePosy)
	ON_EN_CHANGE(IDC_Cube_PosZ, &CPage_Body::OnEnChangeCubePosz)
	ON_EN_CHANGE(IDC_Cube_Parent, &CPage_Body::OnEnChangeCubeParent)
	ON_LBN_SELCHANGE(IDC_Cube_List, &CPage_Body::OnLbnSelchangeCubeList)
	ON_BN_CLICKED(IDC_CHECK_WireFrame, &CPage_Body::OnBnClickedCheckWireframe)
	ON_BN_CLICKED(IDC_Show_BoneCube, &CPage_Body::OnBnClickedShowBonecube)
	ON_BN_CLICKED(IDC_Show_BoneCoord, &CPage_Body::OnBnClickedShowBonecoord)
	ON_BN_CLICKED(IDC_CREATURE_SAVE, &CPage_Body::OnBnClickedCreatureSave)
	ON_BN_CLICKED(IDC_CREATURE_LOAD, &CPage_Body::OnBnClickedCreatureLoad)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_CubeDDSList, &CPage_Body::OnLbnSelchangeCubeddslist)
	ON_BN_CLICKED(IDC_Cube_TexApply, &CPage_Body::OnBnClickedCubeTexapply)
	ON_BN_CLICKED(IDC_CHECK_Body, &CPage_Body::OnBnClickedCheckBody)
END_MESSAGE_MAP()
