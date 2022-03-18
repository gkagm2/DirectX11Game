#include "pch.h"
#include "Extern.h"


// 정점 구조체에 맞춰서 VTX에 새로운것을 추갛면 Layout도 맞춰줘야 한다.
vector< D3D11_INPUT_ELEMENT_DESC> g_vecLayoutDesc;

TTransform g_transform = {};
TGlobalConst g_globalConst = {};
TRectTransform g_rectTransform = {};

tstring ComponentTypeToStr(E_ComponentType _eType)
{
	tstring strComponentType = _T("ComponentDefaultType");
	switch (_eType) {
	case E_ComponentType::Transform:
		strComponentType = _T("Transform");
		break;
	case E_ComponentType::BoundingBox:
		strComponentType = _T("BoundingBox");
		break;
	case E_ComponentType::Collider2D:
		strComponentType = _T("Collider2D");
		break;
	case E_ComponentType::Collider3D:
		strComponentType = _T("Collider3D");
		break;
	case E_ComponentType::Rigidbody2D:
		strComponentType = _T("Rigidbody2D");
		break;
	case E_ComponentType::Rigidbody3D:
		strComponentType = _T("Rigidbody3D");
		break;
	case E_ComponentType::Animator2D:
		strComponentType = _T("Animator2D");
		break;
	case E_ComponentType::Animator3D:
		strComponentType = _T("Animator3D");
		break;
	case E_ComponentType::Light2D:
		strComponentType = _T("Light2D");
		break;
	case E_ComponentType::Light3D:
		strComponentType = _T("Light3D");
		break;
	case E_ComponentType::Camera:
		strComponentType = _T("Camera");
		break;
	case E_ComponentType::MeshRenderer:
		strComponentType = _T("MeshRenderer");
		break;
	case E_ComponentType::ParticleSystem:
		strComponentType = _T("ParticleSystem");
		break;
	case E_ComponentType::SpriteRenderer:
		strComponentType = _T("SpriteRenderer");
		break;
	case E_ComponentType::Skybox:
		strComponentType = _T("Skybox");
		break;
	case E_ComponentType::TileMap:
		strComponentType = _T("TileMap");
		break;
	case E_ComponentType::Listener:
		strComponentType = _T("Listener");
		break;
	case E_ComponentType::AudioSource:
		strComponentType = _T("AudioSource");
		break;
	case E_ComponentType::Script:
		strComponentType = _T("Script");
		break;
	case E_ComponentType::CanvasRenderer:
		strComponentType = _T("CanvasRenderer");
		break;
	case E_ComponentType::RectTransform:
		strComponentType = _T("RectTransform");
		break;
	case E_ComponentType::Decal:
		strComponentType = _T("Decal");
		break;
	case E_ComponentType::Landscape:
		strComponentType = _T("Landscape");
		break;
		// UI
	case E_ComponentType::ButtonUI:
		strComponentType = _T("ButtonUI");
		break;
	case E_ComponentType::TextUI:
		strComponentType = _T("TextUI");
		break;
	case E_ComponentType::ImageUI:
		strComponentType = _T("ImageUI");
		break;
	default:
		assert(nullptr && _T("컴포넌트 타입을 String으로 바꿀 수 없음"));
		break;
	}
	return strComponentType;
}

UINT ComponentStrToIdx(const tstring& _strComponent)
{
	UINT i = 0;
	for (; i < (UINT)E_ComponentType::End; ++i) {
		tstring str = ComponentTypeToStr((E_ComponentType)i);
		if (str == _strComponent)
			return i;
	}
	return i;
}

tstring ResourceTypeToStr(E_ResourceType _eType)
{
	tstring strResourceType = _T("ResourceDefaultType");
	switch (_eType) {
	case E_ResourceType::Prefab:
		strResourceType = _T("Prefab");
		break;
	case E_ResourceType::Material:
		strResourceType = _T("Material");
		break;
	case E_ResourceType::GraphicsShader:
		strResourceType = _T("GraphicsShader");
		break;
	case E_ResourceType::ComputeShader:
		strResourceType = _T("ComputeShader");
		break;
	case E_ResourceType::Mesh:
		strResourceType = _T("Mesh");
		break;
	case E_ResourceType::Texture:
		strResourceType = _T("Texture");
		break;
	case E_ResourceType::Sound:
		strResourceType = _T("Sound");
		break;
	default:
		assert(nullptr && _T("리소스 타입을 String으로 바꿀 수 없음"));
		break;
	}
	return strResourceType;
}


tstring CameraProjectionTypeToStr(E_ProjectionType _eType)
{
	tstring strType;
	switch (_eType) {
	case E_ProjectionType::Orthographic:
		strType = _T("Orthographic");
		break;
	case E_ProjectionType::Perspective:
		strType = _T("Perspective");
		break;
	default:
		assert(nullptr);
		break;
	}
	return strType;
}


Vector4 ChangeColorUintToVector4(UINT _iColor)
{
	BYTE br = COLOR_R_FROM_RGBA(_iColor);
	BYTE bg = COLOR_G_FROM_RGBA(_iColor);
	BYTE bb = COLOR_B_FROM_RGBA(_iColor);
	BYTE ba = COLOR_A_FROM_RGBA(_iColor);
	Vector4 colf = Vector4(float(br / 255.f), float(bg / 255.f), float(bb / 255.f), float(ba / 255.f));
	return colf;
}

UINT ChangeColorVector4ToUint(float _vColor[4])
{
	BYTE r = BYTE(_vColor[0] * 255);
	BYTE g = BYTE(_vColor[1] * 255);
	BYTE b = BYTE(_vColor[2] * 255);
	BYTE a = BYTE(_vColor[3] * 255);
	UINT iColor = COLOR_RGBA(r, g, b, a);
	return iColor;
}

const Vector3 Vector3::Left = Vector3(-1.f, 0.f, 0.f);
const Vector3 Vector3::Right = Vector3(1.f, 0.f, 0.f);
const Vector3 Vector3::Up = Vector3(0.f, 1.f, 0.f);
const Vector3 Vector3::Down = Vector3(0.f, -1.f, 0.f);
const Vector3 Vector3::Front = Vector3(0.f, 0.f, 1.f);
const Vector3 Vector3::Back = Vector3(0.f, 0.f, -1.f);
const Vector3 Vector3::One = Vector3(1.f, 1.f, 1.f);
const Vector3 Vector3::Zero = Vector3(0.f, 0.f, 0.f);
const Vector4 Vector4::Zero = Vector4(0.f, 0.f, 0.f, 0.f);