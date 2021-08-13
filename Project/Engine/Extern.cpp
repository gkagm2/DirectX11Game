#include "pch.h"
#include "Extern.h"

// ���� ����ü�� ���缭 VTX�� ���ο���� �߰��� Layout�� ������� �Ѵ�.
D3D11_INPUT_ELEMENT_DESC g_arrLayoutDesc[LAYOUT_DESCRIPTION_COUNT] = {
	D3D11_INPUT_ELEMENT_DESC{
		"POSITION",						//SemanticName;
		0,								//SemanticIndex;
		DXGI_FORMAT_R32G32B32_FLOAT,	//Format; (offset 0���� 12byte�ΰ��� �������� �˷��ش�.)
		0,								//InputSlot;
		0,								//AlignedByteOffset;
		D3D11_INPUT_PER_VERTEX_DATA,	//InputSlotClass (�ø�ƽ�� �������� �����Ѵ� (��� �ܰ��))
		0								//InstanceDataStepRate;
	},

	D3D11_INPUT_ELEMENT_DESC{
		"COLOR",						//SemanticName;
		0,								//SemanticIndex;
		DXGI_FORMAT_R32G32B32A32_FLOAT,	//Format; (12byte ~28)
		0,								//InputSlot;
		12,								//AlignedByteOffset;
		D3D11_INPUT_PER_VERTEX_DATA,	//InputSlotClass
		0								//InstanceDataStepRate;
	},
	D3D11_INPUT_ELEMENT_DESC{
		"TEXCOORD",						//SemanticName;
		0,								//SemanticIndex;
		DXGI_FORMAT_R32G32_FLOAT,		//Format;
		0,								//InputSlot;
		28,								//AlignedByteOffset;
		D3D11_INPUT_PER_VERTEX_DATA,	//InputSlotClass
		0								//InstanceDataStepRate;
	}
};

TTransform g_transform = {};
TGlobalConst g_globalConst = {};

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
	case E_ComponentType::TileMap:
		strComponentType = _T("TileMap");
		break;
	case E_ComponentType::Listener:
		strComponentType = _T("Listener");
		break;
	case E_ComponentType::Audio:
		strComponentType = _T("Audio");
		break;
	default:
		assert(nullptr && _T("������Ʈ Ÿ���� String���� �ٲ� �� ����"));
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
		assert(nullptr && _T("���ҽ� Ÿ���� String���� �ٲ� �� ����"));
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
