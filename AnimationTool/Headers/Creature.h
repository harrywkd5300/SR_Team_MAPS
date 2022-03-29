#pragma once

#include "gameObject.h"
#include "A_Defines.h"

namespace Engine {
	class CD3DFont;
}


class CCreature
	:public CGameObject
{

private:
	explicit CCreature(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCreature() = default;



public:
	void	Add_Bone();
	void	Add_Bone(const _vec3& vPos, const _int iParentIndex);
	void	Del_Bone(_int iIndex);

	void	Add_Cube();
	void	Add_Cube(TOOL_BODY* pBody);
	void	Del_Cube(_int iIndex);

	void	Add_State();
	void	Del_State(const _uint& dwIndex);

	void	Add_Frame(const _uint& dwStateIndex);
	void	Del_Frame(const _uint& dwStateIndex, const _uint& dwFrameIndex);
	
	void	Set_FrameInfo(const _uint& dwStateIndex, const _uint& dwFrameIndex, const _uint& dwBoneIndex, const _vec3& vStart, const _vec3& vEnd, const _float& fTime);

public:
	vector<CBone*>*					Get_BoneVector();
	vector<TOOL_BODY*>*				Get_BodyVector();
	vector<vector<_float*>>*		Get_vvKeyFrameTime() { return &m_vvKeyFrameTime; }

	void	Set_BonePosition(const _uint&	iIndex, const _float& fX, const _float& fY, const _float& fZ);
	void	Set_BoneParent(const _uint&	iSrcIndex, const _uint&	iDstIndex);
	void	Set_StateIndex(const _uint& iStateIndex) { m_iStateIndex = iStateIndex; }


	virtual HRESULT	Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	
	virtual void	Render_GameObject();

	void	Update_BoneCube(const _float& fTimeDelta);

	void	Render_BoneCube();
	void	Render_BoneFont();

	void	Render_BodyCube();
	void	Render_BodyFont();

	void	Update_BoneAnimate(const _float& fTimeDelta);
	void	Update_Frame(const _float& fTimeDelta);



private:
	HRESULT			Ready_Component();


/*MEMBER*/
private:
	CD3DFont*				m_pFont = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
	
	CCube_Color*			m_pBoneCubeCom = nullptr;
	CRenderer*				m_pRenderer = nullptr;

	vector<CBone*>			m_vecBone;

	CCube_Texture*			m_pCubeTex = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	vector<TOOL_BODY*>		m_vecBody;


	/*animate*/
	vector<vector<_float*>>	m_vvKeyFrameTime;
	_float					m_fFrameTimer =0.f;
	_ulong					m_dwFrame = 0;
	_uint					m_iStateIndex = 0;

/*related ctor dtor*/
public:
	static CCreature* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual _ulong Free() final;
};

