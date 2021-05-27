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
		DXGI_FORMAT_R32G32B32A32_FLOAT,	//Format;
		0,								//InputSlot;
		12,								//AlignedByteOffset;
		D3D11_INPUT_PER_VERTEX_DATA,	//InputSlotClass
		0								//InstanceDataStepRate;
	}
};