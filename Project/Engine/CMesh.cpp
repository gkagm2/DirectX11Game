#include "pch.h"
#include "CMesh.h"
#include "CDevice.h"
#pragma warning (disable:6387)

CMesh::CMesh() :
	m_pVB(nullptr),
	m_pIB(nullptr),
	m_tVtxDesc{},
	m_tIdxDesc{}
{
}

CMesh::~CMesh()
{
	if (nullptr != m_pVtxSys)
		delete m_pVtxSys;
	if (nullptr != m_pIdxSys)
		delete m_pIdxSys;
}

void CMesh::Create(void* _pVtxSys, UINT _iVtxBufferSize, void* _pIdxSys, UINT _iIdxBufferSize, D3D11_USAGE _eIdxUsage)
{
	////////////////////
	// ���ؽ� ���� �����
	////////////////////
	D3D11_BUFFER_DESC m_tVtxDesc = {};
	m_tVtxDesc.ByteWidth = sizeof(VTX) * 4; // ũ��
	m_tVtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// ���� ������ ���� ���� ����
	m_tVtxDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_tVtxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // GPU�� �ٽ� �����ؼ� ������ �� �ֵ��� write
	//tBufferDesc.MiscFlags;
	//tBufferDesc.StructureByteStride;

	D3D11_SUBRESOURCE_DATA tSubResData = {};
	tSubResData.pSysMem = _pVtxSys; // �迭�� �����ּ� (ByteWidth�� �����Ǿ��־ ũ�⼳���� �����൵ ��)

	DEVICE->CreateBuffer(&m_tVtxDesc, &tSubResData, m_pVB.GetAddressOf());


	////////////////////
	// �ε��� ���� �����
	////////////////////
	m_tIdxDesc = {};
	m_tIdxDesc.ByteWidth = sizeof(UINT) * 6; // ũ��
	m_tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// ���� ������ ���� ���� ����
	m_tIdxDesc.Usage = _eIdxUsage;
	if (D3D11_USAGE_DYNAMIC == m_tIdxDesc.Usage)
		m_tIdxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//tBufferDesc.MiscFlags;
	//tBufferDesc.StructureByteStride;

	tSubResData = {};
	tSubResData.pSysMem = _pIdxSys; // �迭�� �����ּ� (ByteWidth�� �����Ǿ��־ ũ�⼳���� �����൵ ��)

	DEVICE->CreateBuffer(&m_tIdxDesc, &tSubResData, m_pIB.GetAddressOf());


	// Alt + Tap�� �����ٺ��� GPU���� �޸𸮸� �Ҿ���� �� �����Ƿ�
	// ���� ���� �����͸� �����Ҵ����� �����Ѵ�.
	
	// ���ؽ� ���� ����
	if (nullptr == m_pVtxSys) {
		m_pVtxSys = malloc(_iVtxBufferSize);
		memcpy(m_pVtxSys, _pVtxSys, _iVtxBufferSize);
	}

	if (0 != _iIdxBufferSize) {
		// �ε��� ���� ����
		m_pIdxSys = malloc(_iIdxBufferSize);
		memcpy(m_pIdxSys, _pIdxSys, _iIdxBufferSize);
	}
}

void CMesh::UpdateData()
{
	UINT iStride = sizeof(VTX); // ���� �ϳ��� �ִ� ������ (����)
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, m_pVB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_pIB.Get(), DXGI_FORMAT_R32_UINT, 0); // 4byte unsigned intŸ������
}