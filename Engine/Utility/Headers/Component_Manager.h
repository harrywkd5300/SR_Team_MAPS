#pragma once

#include "base.h"
#include "Engine_Defines.h"

/*component*/
#include "Component.h"
#include "Triangle_Color.h"
#include "Rect_Color.h"
#include "Rect_texture.h"
#include "Renderer.h"
#include "texture.h"
#include "Shader.h"
#include "Material.h"
#include "Light.h"
#include "Transform.h"
#include "terrain_buffer.h"
#include "cube_texture.h"
#include "cube_color.h"
#include "bone.h"
#include "Target.h"
//#include "LightEffectObj.h"
#include "Picking.h"
#include "AABB_Buffer.h"
#include "Billboard_Buffer.h"
#include "RectUI_Texture.h"



/*temp*/
#include "temp_cube.h"
#include "Jang_Cube.h"
#include "jang_terrain_buffer.h"


BEGIN(Engine)

class DLL_EXPORT CComponent_Manager final 
	:public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)

/*ctor dtor*/
private:
	explicit CComponent_Manager();
	virtual ~CComponent_Manager() = default;



/*FUNCTION*/
public:
	HRESULT		Reserve_ComponentContainer(const _uint& iReserveSize);
	HRESULT		Add_Component(const _uint& iSceneIdx, const _tchar* pComponentTag, CComponent* pComponent);
	CComponent*	Clone_Component(const _uint& iSceneIdx, const _tchar* pComponentTag);

	HRESULT		Release_TargetContainer(const _uint& iSceneIndex);

private:
	CComponent*		Find_Component(const _uint& iSceneIdx, const _tchar* pComponentTag);
	virtual _ulong	Free() final;

/*MEMBER*/
private:
	map<const _tchar*, CComponent*>*		m_pMapComponents = nullptr;
	typedef map<const _tchar*, CComponent*> MAPCOMPONENT;
	_uint									m_iContainerSize = 0;

};

END