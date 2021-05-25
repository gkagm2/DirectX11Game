#include "pch.h"
#include "RenderTest.h"
#include "CDevice.h"

ComPtr<ID3D11Buffer> g_pVB;

void Render_Test::TestInit()
{
	// Triangle
	VTX arr[3] = {};
	arr[0].vPos = Vector3(-1.f, -1.f, 0.5f);
	arr[0].vColor = Vector4{ 1.f,1.f,1.f,1.f };

	arr[1].vPos = Vector3(0.f, 1.f, 0.5f);
	arr[1].vColor = Vector4{ 1.f,1.f,1.f,1.f };

	arr[2].vPos = Vector3(1.f, -1.f, 0.5f);
	arr[2].vColor = Vector4{ 1.f,1.f,1.f,1.f };

	D3D11_BUFFER_DESC tBufferDesc = {};
	tBufferDesc.ByteWidth = sizeof(VTX) * 3; // ũ��
	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// ���� ������ ���� ���� ����
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // GPU�� �ٽ� �����ؼ� ������ �� �ֵ��� write
	tBufferDesc.MiscFlags;
	tBufferDesc.StructureByteStride;

	D3D11_SUBRESOURCE_DATA tSubResData = {};
	tSubResData.pSysMem = arr; // �迭�� �����ּ� (ByteWidth�� �����Ǿ��־ ũ�⼳���� �����൵ ��)

	DEVICE->CreateBuffer(&tBufferDesc, &tSubResData, g_pVB.GetAddressOf());
}

void Render_Test::TestUpdate()
{
}

void Render_Test::TestRender()
{
}