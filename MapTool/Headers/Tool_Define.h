#pragma once

const unsigned int BACKCX = 1280;
const unsigned int BACKCY = 768;

enum TOOLSCENETYPE { SCENE_STATIC, SCENE_MAP, SCENE_TEXTURE, 
	                 SCENE_CUBE, SCENE_CUBE1, SCENE_CUBE2, 
					 SCENE_CUBE3, SCENE_CUBE4, SCENE_CUBE5, SCENE_END };

extern HWND g_hWnd;

#include <map>
#include <vector>
#include <iostream>

#include "../Default/stdafx.h"

