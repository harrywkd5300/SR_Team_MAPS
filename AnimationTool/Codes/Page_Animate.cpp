// ../codes/Page_Animate.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AnimationTool.h"
#include "../headers/Page_Animate.h"
#include "afxdialogex.h"


// CPage_Animate 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage_Animate, CPropertyPage)

CPage_Animate::CPage_Animate()
	: CPropertyPage(IDD_PAGE_ANIMATE)
	, m_pCreature_Manager(CCreature_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pCurCreature(nullptr)
	, m_fStartDegreeX(0)
	, m_fStartDegreeY(0)
	, m_fStartDegreeZ(0)
	, m_fEndDegreeX(0)
	, m_fEndDegreeY(0)
	, m_fEndDegreeZ(0)
	, m_fTime(0)
{

}

CPage_Animate::~CPage_Animate()
{
}

void CPage_Animate::OnLbnSelchangeAnimateCreaturelistbox()
{

	/*Refresh Current Creature*/
	Refresh_CurrentCreature_FromCurrentSel();

	UpdateData(TRUE);

	/*Refresh Current Bone*/
	m_BoneListBox.ResetContent();

	vector<CBone*>*	pVecBone = m_pCurCreature->Get_BoneVector();

	for (size_t i = 0; i < pVecBone->size(); ++i)
	{
		_int n = _int(i);
		CString str;
		str.Format(_T("%d"), n);

		m_BoneListBox.AddString(str);

	}

	/* refresh state & frame*/
	
	
	Refresh_State();
	//Refresh_Frame();
	UpdateData(FALSE);
}


void CPage_Animate::OnLbnSelchangeAnimateBonelistbox()
{
	UpdateData(TRUE);
	Refresh_CurrentCreature_FromCurrentSel();
	Refresh_FrameInfo();
	Refresh_Animate();
	UpdateData(FALSE);
}

void CPage_Animate::OnBnClickedButtonRefresh()
{
	m_CreatureListBox.ResetContent();
	
	CCreature_Manager::MAPCREATURE* pMapCreature = m_pCreature_Manager->Get_CreatureMap();
	
	for (auto& Pair : *pMapCreature)
	{
		CString str = (LPCTSTR)Pair.first;
		m_CreatureListBox.AddString(str);
	}
	
	if ((*pMapCreature).empty())
		return;
	
	m_CreatureListBox.SetCurSel(0);

	CString strName;

	m_CreatureListBox.GetText(0, strName);
	_tchar* pName = (_tchar*)(LPCTSTR)strName;

	m_pCurCreature = m_pCreature_Manager->Get_Creature(pName);
	m_pCreature_Manager->Set_CurCreature(m_pCurCreature);


	/*Refresh Current Bone*/
	m_BoneListBox.ResetContent();

	vector<CBone*>*	pVecBone = m_pCurCreature->Get_BoneVector();

	for (size_t i = 0; i < pVecBone->size(); ++i)
	{
		_int n = _int(i);
		CString str;
		str.Format(_T("%d"), n);

		m_BoneListBox.AddString(str);
	}

	if (!pVecBone->empty())
		m_BoneListBox.SetCurSel(0);
}

void CPage_Animate::OnBnClickedAnimateStateAdd()
{
	/*Get Cur Creature*/
	Refresh_CurrentCreature_FromCurrentSel();
	

	m_pCurCreature->Add_State();
	Refresh_State();

}

void CPage_Animate::Refresh_CurrentCreature_FromCurrentSel()
{
	_int iCreatureIndex = m_CreatureListBox.GetCurSel();

	if (iCreatureIndex < 0)
		return;

	CString strName;
	m_CreatureListBox.GetText(iCreatureIndex, strName);
	_tchar* pName = (_tchar*)(LPCTSTR)strName;

	m_pCurCreature = m_pCreature_Manager->Get_Creature(pName);
	m_pCreature_Manager->Set_CurCreature(m_pCurCreature);
}

void CPage_Animate::Refresh_CurrentInfo_FromCurrentSel()
{
	_int iStateIndex = m_StateListBox.GetCurSel();
	_int iBoneIndex = m_BoneListBox.GetCurSel();
	_int iFrameIndex = m_FrameListBox.GetCurSel();

	if (iStateIndex < 0 || iBoneIndex < 0 || iFrameIndex < 0)
		return;

	/*get time vector from creature [state][frame]*/
	vector<vector<_float*>>* vt = m_pCurCreature->Get_vvKeyFrameTime();

	if ((size_t)iStateIndex >= vt->size())
	{
		AfxMessageBox(L"a");
		return;
	}

	if ((size_t)iFrameIndex >= (*vt)[iStateIndex].size())
	{
		AfxMessageBox(L"a");
		return;
	}
	m_fTime = *((*vt)[iStateIndex][iFrameIndex]);

	/*get bone vector from Creature*/
	vector<CBone*>*		vb = m_pCurCreature->Get_BoneVector();

	if ((size_t)iBoneIndex >= vb->size())
	{
		AfxMessageBox(L"a");
		return;
	}
	/*get keyframe vector from bone [state][frame]*/
	vector<vector<KEYFRAME*>>* vk = (*vb)[iBoneIndex]->Get_VvecKeyFrame();

	if ((size_t)iStateIndex >= vk->size())
	{
		AfxMessageBox(L"a");
		return;
	}
	if ((size_t)iFrameIndex >= (*vk)[iStateIndex].size())
	{
		AfxMessageBox(L"a");
		return;
	}
	m_fStartDegreeX = (*vk)[iStateIndex][iFrameIndex]->vStartDegree.x;
	m_fStartDegreeY = (*vk)[iStateIndex][iFrameIndex]->vStartDegree.y;
	m_fStartDegreeZ = (*vk)[iStateIndex][iFrameIndex]->vStartDegree.z;

	m_fEndDegreeX = (*vk)[iStateIndex][iFrameIndex]->vEndDegree.x;
	m_fEndDegreeY = (*vk)[iStateIndex][iFrameIndex]->vEndDegree.y;
	m_fEndDegreeZ = (*vk)[iStateIndex][iFrameIndex]->vEndDegree.z;

}

void CPage_Animate::Refresh_State()
{
	Refresh_CurrentCreature_FromCurrentSel();

	vector<vector<_float*>>*	pvv = m_pCurCreature->Get_vvKeyFrameTime();
	
	m_StateListBox.ResetContent();

	if (pvv->empty())
		return;

	for (size_t i = 0; i < pvv->size(); ++i)
	{
		_int n = _int(i);
		CString str;
		str.Format(_T("%d"), n);

		m_StateListBox.AddString(str);
	}

	m_StateListBox.SetCurSel(0);
}



void CPage_Animate::OnBnClickedAnimateFrameAdd()
{
	/*Check & Set Creature*/
	Refresh_CurrentCreature_FromCurrentSel();

	/*Check State*/
	_int iStateIndex = m_StateListBox.GetCurSel();
	
	if (iStateIndex < 0)
	{
		AfxMessageBox(L"StateIndex not selected");
		return;
	}

	/*check bone*/
	vector<CBone*>*	pv = m_pCurCreature->Get_BoneVector();

	if (pv->empty())
	{
		AfxMessageBox(L"Need to Bone");
		return;
	}

	m_pCurCreature->Add_Frame((_ulong)iStateIndex);
	Refresh_Frame();
	Refresh_Animate();
}

void CPage_Animate::Refresh_Frame()
{
	/*Get Current Creature*/
	Refresh_CurrentCreature_FromCurrentSel();

	/*Check State*/
	_int iStateIndex = m_StateListBox.GetCurSel();

	if (iStateIndex < 0)
		return;
	
	vector<CBone*>* pb = m_pCurCreature->Get_BoneVector();

	_int iBoneIndex = m_BoneListBox.GetCurSel();

	if (iBoneIndex < 0)
		return;

	vector<vector<KEYFRAME*>>*	pvkey = (*pb)[iBoneIndex]->Get_VvecKeyFrame();
	//vector<vector<KEYFRAME*>>*	pvkey = (*pb)[iStartIndex]->Get_VvecKeyFrame();
	
	m_FrameListBox.ResetContent();


	if (((*pvkey)[iStateIndex]).empty())
		return;

	for (size_t i = 0; i < ((*pvkey)[iStateIndex]).size(); ++i)
	{
		_int n = _int(i);
		CString str;
		str.Format(_T("%d"), n);

		m_FrameListBox.AddString(str);
	}


	m_FrameListBox.SetCurSel(0);

	m_fStartDegreeX = (*pvkey)[iStateIndex][0]->vStartDegree.x;
	m_fStartDegreeY = (*pvkey)[iStateIndex][0]->vStartDegree.y;
	m_fStartDegreeZ = (*pvkey)[iStateIndex][0]->vStartDegree.z;

	m_fEndDegreeX = (*pvkey)[iStateIndex][0]->vEndDegree.x;
	m_fEndDegreeY = (*pvkey)[iStateIndex][0]->vEndDegree.y;
	m_fEndDegreeZ = (*pvkey)[iStateIndex][0]->vEndDegree.z;

	vector < vector<_float*>>* pt = m_pCurCreature->Get_vvKeyFrameTime();
	m_fTime = *((*pt)[iStateIndex][0]);
}

void CPage_Animate::Refresh_FrameInfo()
{
	_int iStateIndex = m_StateListBox.GetCurSel();
	_int iBoneIndex = m_BoneListBox.GetCurSel();
	_int iFrameIndex = m_FrameListBox.GetCurSel();

	vector<vector<_float* >>* vt = m_pCurCreature->Get_vvKeyFrameTime();
	
	if ((size_t)iStateIndex >= vt->size())
		return;

	if ((size_t)iFrameIndex >= (*vt)[iStateIndex].size())
		return;

	m_fTime = *((*vt)[iStateIndex][iFrameIndex]);

	/*get bone vector from Creature*/
	vector<CBone*>*		vb = m_pCurCreature->Get_BoneVector();

	if ((size_t)iBoneIndex >= vb->size())
		return;
	
	/*get keyframe vector from bone [state][frame]*/
	vector<vector<KEYFRAME*>>* vk = (*vb)[iBoneIndex]->Get_VvecKeyFrame();

	if ((size_t)iStateIndex >= vk->size())
		return;

	if ((size_t)iFrameIndex >= (*vk)[iStateIndex].size())
		return;

	m_fStartDegreeX = (*vk)[iStateIndex][iFrameIndex]->vStartDegree.x;
	m_fStartDegreeY = (*vk)[iStateIndex][iFrameIndex]->vStartDegree.y;
	m_fStartDegreeZ = (*vk)[iStateIndex][iFrameIndex]->vStartDegree.z;

	m_fEndDegreeX = (*vk)[iStateIndex][iFrameIndex]->vEndDegree.x;
	m_fEndDegreeY = (*vk)[iStateIndex][iFrameIndex]->vEndDegree.y;
	m_fEndDegreeZ = (*vk)[iStateIndex][iFrameIndex]->vEndDegree.z;

}

void CPage_Animate::Refresh_Animate()
{
	if(m_Radio[0].GetCheck())
		OnBnClickedRadio1();
	else if(m_Radio[1].GetCheck())
		OnBnClickedRadio2();
	else if (m_Radio[2].GetCheck())
		OnBnClickedRadio3();
}

void CPage_Animate::OnLbnSelchangeAnimateStatelistbox()
{
	UpdateData(TRUE);

	Refresh_CurrentCreature_FromCurrentSel();
	Refresh_Frame();

	_int iStateIndex = m_StateListBox.GetCurSel();

	if (iStateIndex < 0)
		return;

	m_pCreature_Manager->Set_CurCreature_StateIndex(iStateIndex);

	Refresh_Animate();

	UpdateData(FALSE);

// 	/*Check & Set Creature*/
// 	_int iCreatureIndex = m_CreatureListBox.GetCurSel();
// 
// 	if (iCreatureIndex < 0)
// 	{
// 		AfxMessageBox(L"Creature not selected");
// 		return;
// 	}
// 
// 	CString strName;
// 	m_CreatureListBox.GetText(iCreatureIndex, strName);
// 	_tchar* pName = (_tchar*)(LPCTSTR)strName;
// 
// 	m_pCurCreature = m_pCreature_Manager->Get_Creature(pName);
// 	m_pCreature_Manager->Set_CurCreature(m_pCurCreature);
// 
// 
// 	/*Check State*/
// 	_int iStateIndex = m_StateListBox.GetCurSel();
// 
// 	if (iStateIndex < 0)
// 	{
// 		AfxMessageBox(L"StateIndex not selected");
// 		return;
// 	}
// 
// 	/*check bone*/
// 	vector<CBone*>*	pv = m_pCurCreature->Get_BoneVector();
// 
// 	if (pv->empty())
// 	{
// 		AfxMessageBox(L"Need to Bone");
// 		return;
// 	}
// 
// 	Refresh_Frame();
}


void CPage_Animate::OnLbnSelchangeAnimateFramelistbox()
{
	UpdateData(TRUE);
	Refresh_CurrentCreature_FromCurrentSel();
	Refresh_FrameInfo();
	Refresh_Animate();
	UpdateData(FALSE);
}

void CPage_Animate::OnBnClickedAnimateFrameApply()
{
	UpdateData(TRUE);

	Refresh_CurrentCreature_FromCurrentSel();

	_int iStateIndex = m_StateListBox.GetCurSel();
	_int iBoneIndex = m_BoneListBox.GetCurSel();
	_int iFrameIndex = m_FrameListBox.GetCurSel();

	if (iStateIndex < 0 || iBoneIndex < 0 || iFrameIndex < 0)
		return;

	_vec3 vs = _vec3(m_fStartDegreeX, m_fStartDegreeY, m_fStartDegreeZ);
	_vec3 ve = _vec3(m_fEndDegreeX, m_fEndDegreeY, m_fEndDegreeZ);
	_float ft = m_fTime;

	m_pCurCreature->Set_FrameInfo(_uint(iStateIndex), _uint(iFrameIndex), _uint(iBoneIndex), vs, ve, ft);

	Refresh_Animate();
	UpdateData(FALSE);

}

void CPage_Animate::OnBnClickedAnimateStateDel()
{
	UpdateData(TRUE);
	Refresh_CurrentCreature_FromCurrentSel();
	_int iStateIndex = m_StateListBox.GetCurSel();

	if (iStateIndex < 0)
		return;

	m_pCurCreature->Del_State(iStateIndex);
	Refresh_State();
	Refresh_Frame();
	Refresh_FrameInfo();

	UpdateData(FALSE);

}


void CPage_Animate::OnBnClickedAnimateFrameDel()
{
	UpdateData(TRUE);
	Refresh_CurrentCreature_FromCurrentSel();
	
	
	_int iStateIndex = m_StateListBox.GetCurSel();
	_int iFrameIndex = m_FrameListBox.GetCurSel();


	if (iStateIndex < 0 || iFrameIndex <0)
		return;

	m_pCurCreature->Del_Frame(iStateIndex,iFrameIndex);
	Refresh_Frame();
	Refresh_FrameInfo();

	UpdateData(FALSE);

}

void CPage_Animate::OnBnClickedButtonAnimateStart()
{
	m_pCreature_Manager->bAnimate = true;
	
	for (size_t i = 0; i < 3; ++i)
		m_Radio[i].SetCheck(FALSE);

	m_Radio[0].SetCheck(TRUE);
}


void CPage_Animate::OnBnClickedButtonAnimatePause()
{
	m_pCreature_Manager->bAnimate = false;
}

void CPage_Animate::OnBnClickedButtonGetPredegree()
{
	UpdateData(TRUE);
	_int iStateIndex = m_StateListBox.GetCurSel();
	_int iFrameIndex = m_FrameListBox.GetCurSel();
	_int iBoneIndex = m_BoneListBox.GetCurSel();

	if (iStateIndex < 0 || iFrameIndex <= 0 || iBoneIndex <0)
		return;

	vector<CBone*>*	vb = m_pCurCreature->Get_BoneVector();

	vector<vector<KEYFRAME*>>*	vk = (*vb)[iBoneIndex]->Get_VvecKeyFrame();
	

	for (size_t i = 0; i < (*vb).size(); ++i)
	{
		vector<vector<KEYFRAME*>>*	vk = (*vb)[i]->Get_VvecKeyFrame();

		(*vk)[iStateIndex][iFrameIndex]->vStartDegree = (*vk)[iStateIndex][iFrameIndex - 1]->vEndDegree;
		(*vk)[iStateIndex][iFrameIndex]->vEndDegree = (*vk)[iStateIndex][iFrameIndex - 1]->vEndDegree;
	}

	Refresh_FrameInfo();
	Refresh_Animate();
	UpdateData(FALSE);

}

void CPage_Animate::OnBnClickedButtonGetIdledegree()
{
	UpdateData(TRUE);
	_int iStateIndex = m_StateListBox.GetCurSel();
	_int iFrameIndex = m_FrameListBox.GetCurSel();

	if (iStateIndex < 0 || iFrameIndex < 0)
		return;

	vector<CBone*>*	vb = m_pCurCreature->Get_BoneVector();

 	for (size_t i = 0; i < vb->size(); ++i)
 	{
		vector<vector<KEYFRAME*>>*	vk = (*vb)[i]->Get_VvecKeyFrame();
		(*vk)[iStateIndex][iFrameIndex]->vStartDegree = (*vk)[0][0]->vStartDegree;
		(*vk)[iStateIndex][iFrameIndex]->vEndDegree = (*vk)[0][0]->vEndDegree;
 	}

	Refresh_Animate();
	UpdateData(FALSE);
}


void CPage_Animate::OnBnClickedButtonAnimatesave()
{
	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this, 0, FALSE);

	TCHAR szDirectory[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szDirectory);
	PathRemoveFileSpec(szDirectory);

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
		
		vector<vector<_float*>>*	vt = pCreature->Get_vvKeyFrameTime();
		size_t iStateIndex = (*vt).size();
		size_t iFrameIndex = 0;

		/*save State Index*/
		WriteFile(hFile, &iStateIndex, sizeof(size_t), &dwByte, NULL);

		for (size_t i = 0; i < iStateIndex; ++i)
		{
			/*save Frame Index*/
			iFrameIndex = (*vt)[i].size();
			WriteFile(hFile, &iFrameIndex, sizeof(size_t), &dwByte, NULL);
			
			for (size_t j = 0; j < iFrameIndex; ++j)
			{
				/*save Timer*/
				_float fTime = *((*vt)[i][j]);
				WriteFile(hFile, &fTime, sizeof(_float), &dwByte, NULL);
			}
		}


		vector<CBone*>*	pb = pCreature->Get_BoneVector();

		size_t iBoneIndex = pb->size();
		/*Save BoneIndex*/
		WriteFile(hFile, &iBoneIndex, sizeof(size_t), &dwByte, NULL);


		for (size_t i = 0; i < iStateIndex; ++i)
		{
			iFrameIndex = (*vt)[i].size();
			WriteFile(hFile, &iFrameIndex, sizeof(size_t), &dwByte, NULL);

			for (size_t j = 0; j < iFrameIndex; ++j)
			{

				for (size_t k = 0; k < iBoneIndex; ++k)
				{
					/*Save KeyFrame*/
					vector<vector<KEYFRAME*>>*	vk = (*pb)[k]->Get_VvecKeyFrame();

					_vec3 vStart = (*vk)[i][j]->vStartDegree;
					_vec3 vEnd = (*vk)[i][j]->vEndDegree;

					WriteFile(hFile, &vStart, sizeof(_vec3), &dwByte, NULL);
					WriteFile(hFile, &vEnd, sizeof(_vec3), &dwByte, NULL);
				}
			}
		}

		AfxMessageBox(L"Successfully Saved");
		CloseHandle(hFile);


		return;
	}
}


void CPage_Animate::OnBnClickedButtonAnimateload()
{
	_int iCreatureIndex = m_CreatureListBox.GetCurSel();

	if (iCreatureIndex < 0)
	{
		AfxMessageBox(L"Need to CreatureList");
		return;
	}

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

		//ReadFile(hFile, &vScale, sizeof(_vec3), &dwByte, NULL);


		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Load Failed.");
			return;
		}

		_ulong dwByte = 0;

		CCreature* pCreature = m_pCreature_Manager->Get_CurCreature();

		size_t	iStateIndex = 0;
		size_t	iFrameIndex = 0;
		ReadFile(hFile, &iStateIndex, sizeof(size_t), &dwByte, NULL);

		for (size_t i = 0; i < iStateIndex; ++i)
		{
			pCreature->Add_State();
			ReadFile(hFile, &iFrameIndex, sizeof(size_t), &dwByte, NULL);

			vector<vector<_float*>>* vt = pCreature->Get_vvKeyFrameTime();

			for (size_t j = 0; j < iFrameIndex; ++j)
			{

				pCreature->Add_Frame(i);

				_float fTime = 0.f;
				ReadFile(hFile, &fTime, sizeof(_float), &dwByte, NULL);
				
				*((*vt)[i][j]) = fTime;
			}
		}

		vector<CBone*>*	pb = pCreature->Get_BoneVector();

		size_t iBoneIndex = 0;
		ReadFile(hFile, &iBoneIndex, sizeof(size_t), &dwByte, NULL);

		for (size_t i = 0; i < iStateIndex; ++i)
		{
			ReadFile(hFile, &iFrameIndex, sizeof(size_t), &dwByte, NULL);

			for (size_t j = 0; j < iFrameIndex; ++j)
			{

				for (size_t k = 0; k < iBoneIndex; ++k)
				{
					_vec3 vStart;
					_vec3 vEnd;

					ReadFile(hFile, &vStart, sizeof(_vec3), &dwByte, NULL);
					ReadFile(hFile, &vEnd, sizeof(_vec3), &dwByte, NULL);
					
					vector<vector<KEYFRAME*>>*	vk = (*pb)[k]->Get_VvecKeyFrame();
 					(*vk)[i][j]->vStartDegree = vStart;
					(*vk)[i][j]->vEndDegree = vEnd;

				}
			}
		}


		CloseHandle(hFile);
	}

	Refresh_State();
	Refresh_Frame();
	Refresh_FrameInfo();
	Refresh_Animate();
	UpdateData(FALSE);

}

void CPage_Animate::OnBnClickedRadio1()
{

}


void CPage_Animate::OnBnClickedRadio2()
{
	m_pCreature_Manager->bAnimate = false;
	
	CCreature* pCreature = m_pCreature_Manager->Get_CurCreature();
	
	_int iSTATEINDEX = m_StateListBox.GetCurSel();
	_int iFRAMEINDEX = m_FrameListBox.GetCurSel();

	if (iSTATEINDEX < 0 || iFRAMEINDEX < 0)
		return;

	vector<CBone*>*	pb = pCreature->Get_BoneVector();
	for (size_t i = 0; i < pb->size(); ++i)
	{
		(*pb)[i]->Set_StartDegree(iSTATEINDEX, iFRAMEINDEX);
	}
}


void CPage_Animate::OnBnClickedRadio3()
{
	m_pCreature_Manager->bAnimate = false;

	CCreature* pCreature = m_pCreature_Manager->Get_CurCreature();

	_int iSTATEINDEX = m_StateListBox.GetCurSel();
	_int iFRAMEINDEX = m_FrameListBox.GetCurSel();

	if (iSTATEINDEX < 0 || iFRAMEINDEX < 0)
		return;

	vector<CBone*>*	pb = pCreature->Get_BoneVector();
	for (size_t i = 0; i < pb->size(); ++i)
	{
		(*pb)[i]->Set_EndDegree(iSTATEINDEX, iFRAMEINDEX);
	}
}

BOOL CPage_Animate::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_Radio[0].SetCheck(TRUE);

	return TRUE; 
				 
}

void CPage_Animate::OnBnClickedButtonGetPredegree3()
{
	UpdateData(TRUE);

	_int iStateIndex = m_StateListBox.GetCurSel();
	_int iFrameIndex = m_FrameListBox.GetCurSel();

	if (iStateIndex < 0 || iFrameIndex < 0)
		return;

	vector<CBone*>*	vb = m_pCurCreature->Get_BoneVector();

	for (size_t i = 0; i < vb->size(); ++i)
	{
		vector<vector<KEYFRAME*>>*	vk = (*vb)[i]->Get_VvecKeyFrame();
		(*vk)[iStateIndex][iFrameIndex]->vEndDegree = (*vk)[0][0]->vStartDegree;
	}

	Refresh_Animate();
	UpdateData(FALSE);
}


void CPage_Animate::OnBnClickedButtonGetPredegree2()
{
	UpdateData(TRUE);

	_int iStateIndex = m_StateListBox.GetCurSel();
	_int iFrameIndex = m_FrameListBox.GetCurSel();

	if (iStateIndex < 0 || iFrameIndex < 0)
		return;

	vector<CBone*>*	vb = m_pCurCreature->Get_BoneVector();

	for (size_t i = 0; i < vb->size(); ++i)
	{
		vector<vector<KEYFRAME*>>*	vk = (*vb)[i]->Get_VvecKeyFrame();
		(*vk)[iStateIndex][iFrameIndex]->vStartDegree = (*vk)[0][0]->vStartDegree;
		(*vk)[iStateIndex][iFrameIndex]->vEndDegree = (*vk)[0][0]->vStartDegree;
	}

	Refresh_Animate();
	UpdateData(FALSE);
}




void CPage_Animate::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Animate_CreatureListBox, m_CreatureListBox);
	DDX_Control(pDX, IDC_Animate_BoneListBox, m_BoneListBox);
	DDX_Control(pDX, IDC_Animate_StateListBox, m_StateListBox);
	DDX_Control(pDX, IDC_Animate_FrameListBox, m_FrameListBox);
	DDX_Text(pDX, IDC_Animate_Start_DegreeX, m_fStartDegreeX);
	DDX_Text(pDX, IDC_Animate_Start_DegreeY, m_fStartDegreeY);
	DDX_Text(pDX, IDC_Animate_Start_DegreeZ, m_fStartDegreeZ);
	DDX_Text(pDX, IDC_Animate_End_DegreeX, m_fEndDegreeX);
	DDX_Text(pDX, IDC_Animate_End_DegreeY, m_fEndDegreeY);
	DDX_Text(pDX, IDC_Animate_End_DegreeZ, m_fEndDegreeZ);
	DDX_Text(pDX, IDC_Animate_Time, m_fTime);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);

}


BEGIN_MESSAGE_MAP(CPage_Animate, CPropertyPage)
	ON_LBN_SELCHANGE(IDC_Animate_CreatureListBox, &CPage_Animate::OnLbnSelchangeAnimateCreaturelistbox)
	ON_LBN_SELCHANGE(IDC_Animate_BoneListBox, &CPage_Animate::OnLbnSelchangeAnimateBonelistbox)
	ON_BN_CLICKED(IDC_Button_Refresh, &CPage_Animate::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_Animate_State_Add, &CPage_Animate::OnBnClickedAnimateStateAdd)
	ON_BN_CLICKED(IDC_Animate_Frame_Add, &CPage_Animate::OnBnClickedAnimateFrameAdd)
	ON_LBN_SELCHANGE(IDC_Animate_StateListBox, &CPage_Animate::OnLbnSelchangeAnimateStatelistbox)
	ON_LBN_SELCHANGE(IDC_Animate_FrameListBox, &CPage_Animate::OnLbnSelchangeAnimateFramelistbox)
	ON_BN_CLICKED(IDC_Animate_Frame_Apply, &CPage_Animate::OnBnClickedAnimateFrameApply)
	ON_BN_CLICKED(IDC_Animate_State_Del, &CPage_Animate::OnBnClickedAnimateStateDel)
	ON_BN_CLICKED(IDC_Animate_Frame_Del, &CPage_Animate::OnBnClickedAnimateFrameDel)
	ON_BN_CLICKED(IDC_Button_Animate_Start, &CPage_Animate::OnBnClickedButtonAnimateStart)
	ON_BN_CLICKED(IDC_Button_Animate_Pause, &CPage_Animate::OnBnClickedButtonAnimatePause)
	ON_BN_CLICKED(IDC_Button_Get_PreDegree, &CPage_Animate::OnBnClickedButtonGetPredegree)
	ON_BN_CLICKED(IDC_Button_AnimateSave, &CPage_Animate::OnBnClickedButtonAnimatesave)
	ON_BN_CLICKED(IDC_Button_AnimateLoad, &CPage_Animate::OnBnClickedButtonAnimateload)
	ON_BN_CLICKED(IDC_RADIO1, &CPage_Animate::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CPage_Animate::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CPage_Animate::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_Button_Get_IdleDegree, &CPage_Animate::OnBnClickedButtonGetIdledegree)
	ON_BN_CLICKED(IDC_Button_Get_PreDegree3, &CPage_Animate::OnBnClickedButtonGetPredegree3)
	ON_BN_CLICKED(IDC_Button_Get_PreDegree2, &CPage_Animate::OnBnClickedButtonGetPredegree2)
END_MESSAGE_MAP()


// CPage_Animate 메시지 처리기입니다.
