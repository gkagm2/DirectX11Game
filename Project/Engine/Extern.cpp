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
		DXGI_FORMAT_R32G32B32A32_FLOAT,	//Format;
		0,								//InputSlot;
		12,								//AlignedByteOffset;
		D3D11_INPUT_PER_VERTEX_DATA,	//InputSlotClass
		0								//InstanceDataStepRate;
	}
};

TTransform g_transform = {};