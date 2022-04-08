#include "pch.h"
#include "CMesh.h"
#include "CDevice.h"
#pragma warning (disable:6387)

CMesh::CMesh() :
	CResource(E_ResourceType::Mesh),
	m_pVB(nullptr),
	m_tVtxDesc{},
	m_iVtxCount{ 0 }
{
}

CMesh::~CMesh()
{
	SAFE_DELETE(m_pVtxSys);

	for (size_t i = 0; i < m_vecIdxInfo.size(); ++i) {
		SAFE_DELETE(m_vecIdxInfo[i].pIdxSysMem);
	}
}

void CMesh::Create(void* _pVtxSys, UINT _iVtxBufferSize, void* _pIdxSys, UINT _iIdxBufferSize, D3D11_USAGE _eIdxUsage)
{

	////////////////////
	// ���ؽ� ���� �����
	////////////////////
	m_iVtxCount = _iVtxBufferSize / sizeof(VTX);
	m_tVtxDesc = {};
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

	HRESULT hr = DEVICE->CreateBuffer(&m_tVtxDesc, &tSubResData, m_pVB.GetAddressOf());
	if (FAILED(hr)) {
		assert(nullptr);
		return;
	}
		

	// Alt + Tap�� �����ٺ��� GPU���� �޸𸮸� �Ҿ���� �� �����Ƿ�
	// ���� ���� �����͸� �����Ҵ����� �����Ѵ�.
	// ���ؽ� ���� ����
	// system copy
	m_pVtxSys = new VTX[m_iVtxCount];
	memcpy(m_pVtxSys, _pVtxSys, _iVtxBufferSize);

	//////////////////////
	// GPU Memory�� Index Buffer �����
	//////////////////////
	tIndexInfo info = {};
	info.iIdxCount = _iIdxBufferSize / sizeof(UINT);
	info.tIBDesc.ByteWidth = _iIdxBufferSize; // GPU �� ������ ���� �޸� ũ��
	info.tIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // Index Buffer �뵵
	info.tIBDesc.CPUAccessFlags = 0;						// ���� �Ұ��� ����
	info.pIdxSysMem = new UINT[info.iIdxCount];

	// ���� ������ ���� ���� ����
	info.tIBDesc.Usage = _eIdxUsage;
	if (D3D11_USAGE_DYNAMIC == info.tIBDesc.Usage)
		info.tIBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// �ε��� ���� ����
	memcpy(info.pIdxSysMem, _pIdxSys, _iIdxBufferSize);

	tSubResData = {};
	tSubResData.pSysMem = info.pIdxSysMem; // �迭�� �����ּ� (ByteWidth�� �����Ǿ��־ ũ�⼳���� �����൵ ��)

	hr = DEVICE->CreateBuffer(&info.tIBDesc, &tSubResData, info.pIB.GetAddressOf());
	if (FAILED(hr)) {
		assert(nullptr);
		return;
	}

	m_vecIdxInfo.push_back(info);
}

void CMesh::UpdateData(UINT _iSubSet)
{
	// Input Asselmber Stage ����
	UINT iStride = sizeof(VTX); // ���� �ϳ��� �ִ� ������ (����)
	UINT iOffset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, m_pVB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubSet].pIB.Get(), DXGI_FORMAT_R32_UINT, 0); // 4byte unsigned intŸ������
}

void CMesh::Render(UINT _iSubset)
{
	// ���������� ����
	// ���ؽ� ���۸� ����� ���
	//UINT iVertexCnt = 3;
	//UINT iStartVertexLocation = 0;
	// CONTEXT->Draw(iVertexCnt, iStartVertexLocation); 

	// �޽� ����
	UpdateData(_iSubset);
	// �ε��� ���۸� ���
	UINT iStartIndexLocation = 0;
	UINT iBaseVertexLocation = 0;
	CONTEXT->DrawIndexed(m_vecIdxInfo[_iSubset].iIdxCount, iStartIndexLocation, iBaseVertexLocation);
}

void CMesh::Render_Particle(UINT _iCount)
{
	UpdateData(0);
	CONTEXT->DrawIndexedInstanced(m_vecIdxInfo[0].iIdxCount, _iCount, 0, 0, 0);
}

void CMesh::Reset()
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, m_pVtxSys, m_tVtxDesc.ByteWidth);
	CONTEXT->Unmap(m_pVB.Get(), 0);
}