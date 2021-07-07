#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <tchar.h>

// µð¹ö±×
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>


#include <vector>
using std::vector;
#include <list>
using std::list;
#include <queue>
using std::queue;
#include <unordered_map>
using std::unordered_map;
#include <map>
using std::map;
#include <bitset>
using std::bitset;

#include <memory>
using std::unique_ptr;
using std::make_unique;

#include <string>
using std::string;
using std::wstring;
#ifdef UNICODE
typedef wstring tstring;
#else
typedef string tstring;
#endif



//Dx11
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <wrl.h> // ComPtr
using namespace Microsoft::WRL;


#include "SimpleMath.h"
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace DirectX::SimpleMath;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#include "CSingleton.h"
#include "Define.h"
#include "Struct.h"
#include "Extern.h"
#include "Tools.h"
#include "StringTable.h"
#include "Function.h"