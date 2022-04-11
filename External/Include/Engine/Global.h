#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <tchar.h>

// µð¹ö±×
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifdef _DEBUG
#include <conio.h> // Console screen
#endif

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using std::experimental::filesystem::path;

// Boost Library
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
using boost::uuids::uuid;

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
#include <set>
using std::set;
#include <bitset>
using std::bitset;

#include <functional>
using std::function;

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

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
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

#include <FBXLoader/fbxsdk.h>

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