#pragma once

namespace Engine
{

	enum CHAR_TYPE { CHAR_TYPE_MONSTER, CHAR_TYPE_PLAYER, CHAR_TYPE_END };
	enum LIGHT_TYPE { DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT, LIGHT_TYPE_END };
	enum WEAPON_TYPE { WEAPON_SUB, WEAPON_MAIN, WEAPON_HEAVY, WEAPON_SUPER, WEAPON_END};
	enum COLLBOX_TYPE{ COLLBOX_TYPE_AABB, COLLBOX_TYPE_BILLBOARD, COLLBOX_TYPE_END};
	typedef unsigned char			_ubyte;
	typedef signed char				_byte;

	typedef char					_char;

	typedef unsigned short			_ushort;
	typedef signed short			_short;

	typedef unsigned int			_uint;
	typedef signed int				_int;

	typedef unsigned long			_ulong;
	typedef signed long				_long;

	typedef float					_float;
	typedef double					_double;

	typedef wchar_t					_tchar;

	typedef bool					_bool;

	typedef D3DXVECTOR2				_vec2;
	typedef D3DXVECTOR3				_vec3;
	typedef D3DXVECTOR4				_vec4;
	
	typedef D3DXMATRIX				_matrix;

// 	typedef struct D3DXMATRIX_DERIVED : public D3DXMATRIX
// 	{
// 	public:
// 		D3DXMATRIX_DERIVED* Identity(void)
// 		{
// 			m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.0f;
// 			m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
// 
// 			return this;
// 		}
// 
// 	}_matrix;
}