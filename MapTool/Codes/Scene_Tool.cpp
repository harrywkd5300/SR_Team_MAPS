#include "stdafx.h"
#include "..\Headers\Scene_Tool.h"

#include "Layer.h"

CScene_Tool::CScene_Tool(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
	, pCamera(nullptr)
	, pTerrian(nullptr)
	, m_bWireFrame(false)
	, m_pPickingCom(nullptr)
{
}
HRESULT CScene_Tool::Ready_Scene()
{
	if (FAILED(Add_Component_ForLogo()))
		return E_FAIL;

	if (FAILED(Add_GameObject_ForLogo()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Tool::Add_GameObject_ForLogo()
{
	if (nullptr == m_pObject_Manager)
		goto exception;

	/*Create Back_Logo & Add on ObjectManager*/
	CGameObject* pObject = pTerrian = CTerrian::Create(m_pGraphicDev);

	if (nullptr == pObject)
		goto exception;

	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_MAP, L"Layer_BackGround", pObject)))
		goto exception;

	/*Create Camera_Dynamic*/
	pObject = pCamera = ToolCCamera::Create(m_pGraphicDev, _vec3(0.f, 20.f, -20.f), _vec3(0.f, 0.f, 0.f), _vec3(0.f, 1.f, 0.f));

	if (nullptr == pObject)
		goto exception;

	if (FAILED(m_pCamera_Manager->Add_Camera(L"Layer_Camera", pObject)))
		goto exception;

	m_pCamera_Manager->Change_Camera(L"Layer_Camera");

	return NOERROR;

exception:
	MSG_BOX("Add_GameObject failed on Scene_Logo err:50624");
	return E_FAIL;
}

HRESULT CScene_Tool::Add_Component_ForLogo()
{
	if (nullptr == m_pComponent_Manager)
		goto AddSceneCom;

	/*Create&Add Texture*/
	CComponent* pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_NORMAL, L"../Bin/Resources/Textures/Terrain/Tile0.jpg");

	if (nullptr == pComponent)
		goto AddSceneCom;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Texture_Terrain", pComponent)))
		goto AddSceneCom;

	/*Cube Texture*/
	pComponent = CTexture::Create(m_pGraphicDev, CTexture::TEXTYPE_CUBE, L"../Bin/Resources/Textures/CubeTile/CubeDDS/CubeTile_%d.dds", 103);

	if (nullptr == pComponent)
		goto AddSceneCom;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Texture_Cube", pComponent)))
		goto AddSceneCom;

	/*Create&Add Terrain_Buffer */
	pComponent = CTerrain_Buffer::Create(m_pGraphicDev,L"../Bin/Resources/Textures/Terrain/Height1.bmp", 1);

	if (nullptr == pComponent)
		goto AddSceneCom;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_Terrain", pComponent)))
		goto AddSceneCom;

	pComponent = CCube_Texture::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto AddSceneCom;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_Cube", pComponent)))
		goto AddSceneCom;

	/*Create&Add Shader_Terrian */
	pComponent = CShader::Create(m_pGraphicDev, CShader::SDTYPE_NORMAL, L"../Bin/Resources/Textures/Shader/Terrain_%d.fx", 1);
	if (nullptr == pComponent)
		goto AddSceneCom;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Shader_Terrain", pComponent)))
		goto AddSceneCom;

	/*create Picking*/
	pComponent = m_pPickingCom = CPicking::Create(m_pGraphicDev);

	if (nullptr == pComponent)
		goto AddSceneCom;

	if (FAILED(m_pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Picking", pComponent)))
		goto AddSceneCom;

	return NO_ERROR;

AddSceneCom:
	MSG_BOX("Add Component_ForLogo failed err:12854");
	return E_FAIL;
}

_int CScene_Tool::Update_Scene(const _float & fTimeDelta)
{
	_int iExitCode = CScene::Update_Scene(fTimeDelta);

	return iExitCode;
}

void CScene_Tool::Render_Scene()
{
	if (m_bWireFrame)
	{
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	CScene::Render_Scene();
}

void CScene_Tool::Change_Texture_view(_bool Check)
{
	m_bWireFrame = Check;
}

void CScene_Tool::Set_WireFrame()
{
	CLayer* pLayer = m_pObject_Manager->Get_Layer(SCENE_CUBE, L"Layer_Cube");
	if (nullptr == pLayer)
		return;
	if (pLayer->Get_ObjList().empty())
		return;

	auto& iter = pLayer->Get_ObjList().begin();
	for (; iter != pLayer->Get_ObjList().end(); ++iter)
	{
		auto& iter_obj = dynamic_cast<CTool_Object*>(*iter)->GetCube()->begin();
		auto& iter_obj_end = dynamic_cast<CTool_Object*>(*iter)->GetCube()->end();

		for (; iter_obj != iter_obj_end; ++iter_obj)
		{
			dynamic_cast<CTool_Cube*>(*iter_obj)->Set_WireFrame(false);
		}
	}
}

const list<CGameObject*>* CScene_Tool::Get_CubejList()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	CLayer* pLayer = m_pObject_Manager->Get_Layer(SCENE_CUBE, L"Layer_Cube");
	if (nullptr == pLayer)
		return nullptr;

	if (pLayer->Get_ObjList().empty())
		return nullptr;

	return &(pLayer->Get_ObjList());
}

_bool CScene_Tool::ObjectPicking(_ulong vMx, _ulong vMy, CString strName)
{
	CWnd* pWnd = AfxGetMainWnd();
	HWND hWnd = pWnd->m_hWnd;

	CLayer* pLayer = m_pObject_Manager->Get_Layer(SCENE_CUBE, L"Layer_Cube");
	if (nullptr == pLayer)
		return false;

	if (!pLayer->Get_ObjList().empty())
	{
		auto& iter = Get_CubejList()->begin();
		auto& iter_end = Get_CubejList()->end();

		for (; iter != iter_end;)
		{
			if (dynamic_cast<CTool_Object*>((*iter))->GetStrKey() == strName)
				break;

			++iter;
		}
		dynamic_cast<CTool_Object*>((*iter))->Set_NotPicking();
		auto& iter_obj = dynamic_cast<CTool_Object*>(*iter)->GetCube()->begin();
		auto& iter_obj_end = dynamic_cast<CTool_Object*>(*iter)->GetCube()->end();

		for (; iter_obj != iter_obj_end; ++iter_obj)
		{
			if (m_pPickingCom->Picking_ToObject(hWnd,
				(CVIBuffer*)(*iter_obj)->Get_Component(L"Buffer_Cube")
				, &dynamic_cast<CTransform*>((*iter_obj)->Get_Component(L"Transform"))->Get_WorldMatrix(), float(vMx), float(vMy)))
			{
				dynamic_cast<CTool_Cube*>(*iter_obj)->Set_PickingCube(true);
				return true;
			}
		}
	}
	return false;
}

_vec3 CScene_Tool::TerrainPicking(_ulong vMx, _ulong vMy)
{
	CWnd* pWnd = AfxGetMainWnd();
	HWND hWnd = pWnd->m_hWnd;

	CLayer* pLayer = m_pObject_Manager->Get_Layer(SCENE_MAP, L"Layer_BackGround");
	if (!pLayer->Get_ObjList().empty())
	{
		CLayer* pLayer2 = m_pObject_Manager->Get_Layer(SCENE_CUBE, L"Layer_Cube");

		auto& iter2 = pLayer2->Get_ObjList().begin();
		auto& iter_cube = dynamic_cast<CTool_Object*>((*iter2))->GetCube()->begin();
		auto& iter_cube_end = dynamic_cast<CTool_Object*>((*iter2))->GetCube()->end();
		for (; iter_cube != iter_cube_end; ++iter_cube)
		{
			dynamic_cast<CTool_Cube*>((*iter_cube))->Set_PickingCube(false);
		}

		auto& iter = pLayer->Get_ObjList().begin();

		_vec3 vPickPos = m_pPickingCom->Picking_ToMakeBuffer(hWnd, BACKCX, BACKCY,
			(CVIBuffer*)(*iter)->Get_Component(L"Buffer_Terrain"), nullptr, float(vMx), float(vMy));
		if (nullptr == vPickPos)
			return _vec3(0.f, 0.f, 0.f);

		return vPickPos;
	}
	return _vec3(0.f, 0.f, 0.f);
}

HRESULT CScene_Tool::Create_Cube(_vec3 vPos, _vec3 vRota, _vec3 vScalse, const _uint & iTextureNum, const TCHAR * pKey, CString strKey)
{
	CLayer* pLayer = m_pObject_Manager->Get_Layer(SCENE_CUBE, L"Layer_Cube");
	if (nullptr == pLayer)
		return false;

	if (!pLayer->Get_ObjList().empty())
	{
		auto& iter = pLayer->Get_ObjList().begin();
		for (; iter != pLayer->Get_ObjList().end(); ++iter)
		{
			if (dynamic_cast<CTool_Object*>(*iter)->GetStrKey() == strKey)
			{
				dynamic_cast<CTool_Object*>(*iter)->Create_Cube(vPos, vRota, vScalse, iTextureNum, pKey);
				return NOERROR;
			}
		}
	}
	return E_FAIL;
}

_vec3* CScene_Tool::Create_OnCube(const _uint& iDrawID, const _uint& iVecNum, const TCHAR* pKey, CString strKey)
{
	_vec3 vPickPos = *m_pPickingCom->Get_PickingObjectPos();

	if (0 > iDrawID)
	{
		AfxMessageBox(L"Don't Create Cube!!");
		return &vPickPos;
	}
	Create_Cube(vPickPos, _vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), iDrawID, pKey, strKey);

	return &vPickPos;
}

CGameObject * CScene_Tool::Create_Object(CString strKey)
{
	if (nullptr == m_pObject_Manager)
		MSG_BOX("Object failed");

	CGameObject* pObject = CTool_Object::Create(m_pGraphicDev, strKey);

	if (FAILED(pObject->Ready_GameObject()))
	{
		MSG_BOX("Object failed");
		Safe_Release(pObject);
	}

	if (FAILED(m_pObject_Manager->Add_GameObject(SCENE_CUBE, L"Layer_Cube", pObject)))
		MSG_BOX("Object failed");

	return pObject;
}

CScene_Tool * CScene_Tool::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CScene_Tool* pInstance = new CScene_Tool(pGraphicDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("Logo creation failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CScene_Tool::Free()
{
	m_pCamera_Manager->Release();

	m_pComponent_Manager->Release_TargetContainer(SCENE_STATIC);

	m_pObject_Manager->Release_TargetContainer(SCENE_MAP);
	m_pObject_Manager->Release_TargetContainer(SCENE_CUBE);

	return CScene::Free();
}
