#pragma once
#include "afxwin.h"

#include "A_Defines.h"

class CCreature_Manager;
class CCreature;

namespace Engine
{
	class CComponent_Manager;
}

class CPage_Body : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Body)

public:
	CPage_Body();
	virtual ~CPage_Body();

/*function*/
public:
	virtual void PreInitDialog();
	
	/*creature*/
	afx_msg void OnBnClickedCreatureAdd();
	afx_msg void OnBnClickedCreatureDel();
	afx_msg void OnEnChangeCreatureName();
	afx_msg void OnLbnSelchangeCreatureList();

	/*Bone*/
	afx_msg void OnLbnSelchangeBoneList();
	afx_msg void OnBnClickedBoneAdd();
	afx_msg void OnBnClickedBoneDel();
	afx_msg void OnEnChangeBoneLocalx();
	afx_msg void OnEnChangeBoneLocaly();
	afx_msg void OnEnChangeBoneLocalz();
	afx_msg void OnEnChangeBoneChildindex();
	afx_msg void OnEnChangeBoneParentindex();

	/*Cube*/
	afx_msg void OnBnClickedCubeDel();
	afx_msg void OnBnClickedCubeAdd();
	afx_msg void OnEnChangeCubeScaley();
	afx_msg void OnEnChangeCubeScalex();
	afx_msg void OnEnChangeCubeScalez();
	afx_msg void OnEnChangeCubePosx();
	afx_msg void OnEnChangeCubePosy();
	afx_msg void OnEnChangeCubePosz();
	afx_msg void OnEnChangeCubeParent();
	afx_msg void OnLbnSelchangeCubeList();

	/*CheckBox*/
	afx_msg void OnBnClickedCheckWireframe();
	afx_msg void OnBnClickedShowBonecube();
	afx_msg void OnBnClickedShowBonecoord();

	/*Save Load*/
	afx_msg void OnBnClickedCreatureSave();
	afx_msg void OnBnClickedCreatureLoad();

	/*Cube DDS*/
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeCubeddslist();
	afx_msg void OnBnClickedCubeTexapply();

private:
	void Refresh_Data();
	void Refresh_BoneData();
	void Refresh_CubeData();

/*member*/
private:
	CCreature_Manager*	m_pCreature_Manager = nullptr;
	CCreature*			m_pCurCreature = nullptr;

	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;

	Engine::CComponent_Manager*	m_pComponent_Manager = nullptr;

	/*creature*/
	CListBox		m_Creature_ListBox;
	CString			m_CreatureName;

	/*bone*/
	CListBox		m_Bone_ListBox;

	float			m_fBonePosX =0;
	float			m_fBonePosY =0;
	float			m_fBonePosZ =0;

	/*Parent*/
	int				m_iChildIndex = -1;
	int				m_iParentIndex = -1;

	/*Body*/
	CListBox		m_Cube_ListBox;

	float m_fCubeScaleX = 1.f;
	float m_fCubeScaleY = 1.f;
	float m_fCubeScaleZ = 1.f;

	float	m_fCubePosX = 0.f;
	float	m_fCubePosY = 0.f;
	float	m_fCubePosZ = 0.f;
	int		m_iCubeParentIndex = -1;
	int		m_iCubeTexNum = 0;


	/*CheckBox*/
	CButton m_CheckWireFrame;
	CButton m_CheckBoneCube;
	CButton m_CheckBoneCoord;


	/*Cube Image*/
	map<CString, CImage*>	m_mapCubeImage;
	CListBox				m_DDSListBox;
	CStatic					m_PictureCube;
	_int					m_iCurCubeTexNum = 0;


/*mfc*/
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_BODY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	CString	RelativePath(CString str)
	{
		TCHAR szRelativePath[MAX_PATH] = L"";
		TCHAR szDirectoryPath[MAX_PATH] = L"";
		
		GetCurrentDirectory(sizeof(szDirectoryPath), szDirectoryPath);
		PathRelativePathTo(szRelativePath, szDirectoryPath, FILE_ATTRIBUTE_DIRECTORY, str, FILE_ATTRIBUTE_NORMAL);

		return szRelativePath;
	}	

	afx_msg void OnBnClickedCheckBody();
};
