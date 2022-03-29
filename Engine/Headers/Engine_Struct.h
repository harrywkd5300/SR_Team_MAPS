#pragma once

namespace Engine
{
	typedef struct Vertex_Collide
	{
		enum {FVF = D3DFVF_XYZ};
		D3DXVECTOR3		vPosition;
	}VTXCOLLIDE;


	typedef struct Vertex_Color
	{
		D3DXVECTOR3		vPosition;
		unsigned long	dwColor;
	}VTXCOL;

	typedef struct Vertex_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXTEX;

	typedef struct Vertex_ViewPort
	{
		enum { FVF = D3DFVF_XYZRHW | D3DFVF_TEX1};
		D3DXVECTOR4		vPos;
		D3DXVECTOR2		vTexUV;
		
	}VTXVIEWPORT;

	typedef struct Vertex_Particle
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE};
		D3DXVECTOR3		vPos;
		unsigned long	dwColor;
	}VTXPTC;

	typedef struct Vertex_EFFECT
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
		D3DXVECTOR3		vPos;
		D3DXVECTOR2		vTexUV;
	}VTXEFFECT;

	typedef struct Vertex_ViewPortColor
	{
		enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE };
		D3DXVECTOR4		vPos;
		unsigned long	dwColor;
	}VTXVIEWPORTCOL;

	
// 	typedef struct Vertex_Cube_Texture
// 	{
// 		D3DXVECTOR3		vPosition;
// 		D3DXVECTOR3		vTexUV;
// 	}VTXCUBETEX;


	typedef struct Vertex_Cube_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR3		vTexUV;
		D3DXVECTOR3		vTangent;
		D3DXVECTOR3		vBiNormal;

	}VTXCUBETEX;

	typedef struct Vertex_Texture_Normal
	{
		D3DXVECTOR3		vPosition;
 		D3DXVECTOR3		vNormal;
		unsigned long   dwDiffuse;
		unsigned long   dwSpec;
		D3DXVECTOR2		vTexUV;

	}VTXTEXNORMAL;

	typedef struct Vertex_Texture_Normal2
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vTexUV;

	}VTXTEXNORMAL2;

	struct Attribute
	{
		//Attribute()
		//{
		//	_lifeTime = 0.0f;
		//	_age = 0.0f;
		//	_isAlive = true;
		//}


		D3DXVECTOR3 _position;
		D3DXVECTOR3 _velocity;
		D3DXVECTOR3 _acceleration;
		float       _lifeTime;     // how long the particle lives for before dying  
		float       _age;          // current age of the particle  
		D3DXCOLOR   _color;        // current color of the particle   
		D3DXCOLOR   _colorFade;    // how the color fades with respect to time
		bool        _isAlive;
	};

	typedef struct EffectInfo
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vLook;
		D3DXVECTOR3 vScale;
		float       _lifeTime;     // how long the particle lives for before dying  
		float       _age;          // current age of the particle  
		bool        _isAlive;

		// ----------- Frame -----------
		float		fFrame;
		float		fMaxFrame;
		float		fFrameSpeed;
		// ----------- Frame -----------

		// ----------- Light -----------
		D3DXCOLOR	_Lightcolor;
		float		_fLightRange;
		bool		_isLight;
		// ----------- Light -----------
	}EFTINFO;

	typedef struct Vertex_Texture_Jang
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		unsigned long	dwDiffuse;
		unsigned long	dwSpec;
		D3DXVECTOR2		vTexUV;
	}VTXTEX_J;

	typedef struct Vertex_Texture_Jang2
	{
		//D3DXVECTOR3		vPosition;
		//D3DXVECTOR3		vNormal;
		//D3DXVECTOR2		vTexUV;
		//D3DXVECTOR3		vTangent;
		//D3DXVECTOR3		vBiNormal;
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vTexUV;
		D3DXVECTOR3		vTangent;
		D3DXVECTOR3		vBiNormal;
	}VTXTEX_J2;

	typedef struct Vertex_Cube_Jang
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR3		vTexUV;
		D3DXVECTOR3		vTangent;
		D3DXVECTOR3		vBiNormal;
	}VTXTEXCUBE_J;



	typedef struct Objectstruct_Kwon
	{
		D3DXVECTOR3		m_vecPos;
		D3DXVECTOR3		m_vecRot;
		D3DXVECTOR3		m_vecScale;
		TCHAR			m_strKey[256];
		int				m_iTextureNum;
	}STROBJECT;

	

	typedef struct Index_16
	{
		unsigned short _0, _1, _2;
	}INDEX16;

	typedef struct Index_32
	{
		unsigned long _0, _1, _2;
	}INDEX32;


	typedef struct tagKeyFrame
	{
		D3DXVECTOR3 vStartDegree;
		D3DXVECTOR3 vEndDegree;
	}KEYFRAME;

	typedef struct tagFrame
	{
		float fFrame;
		float fFrameCount;
		float fFramePerSecond;

	}FRAME;

	typedef struct tagBodyInfo
	{
		D3DXVECTOR3		vScale;
		D3DXVECTOR3		vPosition;
		int				iParentIndex;
		int				iTextureNum;
	}BODYINFO;

	typedef struct Vertex_Terrain_Texture
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEX2 | D3DFVF_TEX3 | D3DFVF_TEXCOORDSIZE3(1) | D3DFVF_TEXCOORDSIZE3(2) };
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vTexUV;
		D3DXVECTOR3		vTangent;
		D3DXVECTOR3		vBiNormal;
	}VTXTERTEX;
}