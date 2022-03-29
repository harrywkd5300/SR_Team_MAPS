#pragma once

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <iostream>

#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib,"d3dx9.lib")


#define DIRECTINPUT_VERSION 0x0800 
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma	warning (disable : 4251)
#pragma	warning (disable : 4005)

#include "engine_Macro.h"
#include "engine_struct.h"
#include "engine_typedef.h"
#include "engine_function.h"
#include "engine_functor.h"


using namespace std;
using namespace Engine;