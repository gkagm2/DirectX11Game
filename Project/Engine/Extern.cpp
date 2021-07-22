#include "pch.h"
#include "Extern.h"

// 정점 구조체에 맞춰서 VTX에 새로운것을 추갛면 Layout도 맞춰줘야 한다.
D3D11_INPUT_ELEMENT_DESC g_arrLayoutDesc[LAYOUT_DESCRIPTION_COUNT] = {
	D3D11_INPUT_ELEMENT_DESC{
		"POSITION",						//SemanticName;
		0,								//SemanticIndex;
		DXGI_FORMAT_R32G32B32_FLOAT,	//Format; (offset 0부터 12byte인것을 포멧으로 알려준다.)
		0,								//InputSlot;
		0,								//AlignedByteOffset;
		D3D11_INPUT_PER_VERTEX_DATA,	//InputSlotClass (시멘틱이 정점마다 존재한다 (덩어리 단계로))
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
		strComponentType = _T("Transform");
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
	}
	return strComponentType;
}