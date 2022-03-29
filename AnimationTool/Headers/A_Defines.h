#pragma once
#include "../Default/stdafx.h"


#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include <d3dx9math.h>

#include <list>
#include <vector>
#include <map>
#include <stack>
#include <queue>

#include <algorithm>
#include <string>
#include <iostream>
#include <afxdlgs.h>
#include <fstream>

#include <atlstr.h>

const int BACKSIZEX = 1920;
const int BACKSIZEY = 1080;

extern HWND	g_hWnd;
enum SCENE{SCENE_STATIC,SCENE_TOOL,SCENE_END};

using namespace std;

namespace Engine {
	class CCube_Texture;
}
struct TOOL_BODY
{
	D3DXVECTOR3		vScale;
	D3DXVECTOR3		vPosition;
	int				iParentIndex;
	int				iTextureNum;

};


//using namespace Engine;

