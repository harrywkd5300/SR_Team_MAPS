#pragma once


#include "Engine_Defines.h"
#include "base.h"

#include "Creature.h"

class CCreature_Manager
	:public CBase
{
	DECLARE_SINGLETON(CCreature_Manager)

/*ctor dtor*/
private:
	explicit CCreature_Manager();
	virtual ~CCreature_Manager() = default;

/*FUNCTION*/
public:
	CCreature*		Get_Creature(const _tchar* pCreatureTag);
	map<const _tchar*, CCreature*>*	Get_CreatureMap() { return &m_mapCreature; }
	CCreature*		Get_CurCreature() { return m_pCurCreature; }


	void			Set_CurCreature(CCreature*	pCreature) { m_pCurCreature = pCreature; }
	void			Set_CurCreature_StateIndex(const _uint& iStateIndex) { m_pCurCreature->Set_StateIndex(iStateIndex); }

public:
	_bool			bWIREMODE = false;
	
	_bool			bBoneRender = true;
	_bool			bBoneCoord = false;

	_bool			bCubeRender = true;
	_bool			bCubeCoord = false;

	_bool			bAnimate = false;



public:
	HRESULT		Add_Creature(const _tchar* pCreatureTag, CCreature* pCreature);
	HRESULT		Del_Creature(const _tchar* pCreatureTag);


	_int	Update_Creature(const _float& fTimeDelta);
	void	Render_Creature();

private:
	CCreature*		Find_Creature(const _tchar* pCreatureTag);
	


/*MEMBER*/
private:
	map<const _tchar*, CCreature*>		m_mapCreature;
	CCreature*							m_pCurCreature = nullptr;

public:
	typedef map<const _tchar*, CCreature*> MAPCREATURE;



/*related ctor dtor*/
private:
	virtual _ulong Free() final;
};

