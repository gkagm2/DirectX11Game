#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <tchar.h>

#include <vector>
using std::vector;
#include <list>
using std::list;
#include <unordered_map>
using std::unordered_map;
#include <map>
using std::map;

#include <string>
#ifdef UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif


//Dx11
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "SimpleMath.h"
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace DirectX::SimpleMath;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#include "CSingleton.h"
#include "Define.h"

#include "Tools.h"
#include "StringTable.h"