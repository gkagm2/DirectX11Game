#include "pch.h"
#include "CMesh.h"
#include "CDevice.h"
#pragma warning (disable:6387)

CMesh::CMesh() :
	CResource(E_ResourceType::Mesh),
	m_pVB(nullptr),
	m_pIB(nullptr),
	m_tVtxDesc{},
	m_tIdxDesc{},
	m_iIdxCount(0)
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
	m_tVtxDesc.ByteWidth = _iVtxBufferSize; // ũ��
	m_tVtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// ���� ������ ���� ���� ����
	m_tVtxDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tVtxDesc.CPUAccessFlags = 0;

	//m_tVtxDesc.Usage = D3D11_USAGE_DYNAMIC;
	//m_tVtxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // GPU�� �ٽ� �����ؼ� ������ �� �ֵ��� write
	//tBufferDesc.MiscFlags;
	//tBufferDesc.StructureByteStride;

	D3D11_SUBRESOURCE_DATA tSubResData = {};
	tSubResData.pSysMem = _pVtxSys; // �迭�� �����ּ� (ByteWidth�� �����Ǿ��־ ũ�⼳���� �����൵ ��)

	DEVICE->CreateBuffer(&m_tVtxDesc, &tSubResData, m_pVB.GetAddressOf());


	////////////////////
	// �ε��� ���� �����
	////////////////////
	m_iIdxCount = _iIdxBufferSize / sizeof(UINT);

	m_tIdxDesc = {};
	m_tIdxDesc.ByteWidth = _iIdxBufferSize; // ũ��
	m_tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// ���� ������ ���� ���� ����
	m_tIdxDesc.Usage = _eIdxUsage;
	if (D3D11_USAGE_DYNAMIC == m_tIdxDesc.Usage)
		m_tIdxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

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
	// Input Asselmber Stage ����
	UINT iStride = sizeof(VTX); // ���� �ϳ��� �ִ� ������ (����)
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, m_pVB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_pIB.Get(), DXGI_FORMAT_R32_UINT, 0); // 4byte unsigned intŸ������
}

void CMesh::Render()
{
	// ���������� ����
	// ���ؽ� ���۸� ����� ���
	//UINT iVertexCnt = 3;
	//UINT iStartVertexLocation = 0;
	// CONTEXT->Draw(iVertexCnt, iStartVertexLocation); 
	
	// �޽� ����
	UpdateData();
	// �ε��� ���۸� ���
	UINT iStartIndexLocation = 0;
	UINT iBaseVertexLocation = 0;
	CONTEXT->DrawIndexed(m_iIdxCount, iStartIndexLocation, iBaseVertexLocation);
}

void CMesh::Render_Particle(UINT _iCount)
{
	UpdateData();
	CONTEXT->DrawIndexedInstanced(m_iIdxCount, _iCount, 0, 0, 0);
}

void CMesh::Reset()
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, m_pVtxSys, m_tVtxDesc.ByteWidth);
	CONTEXT->Unmap(m_pVB.Get(), 0);
}