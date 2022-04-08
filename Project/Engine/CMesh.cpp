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
	// 버텍스 버퍼 만들기
	////////////////////
	m_iVtxCount = _iVtxBufferSize / sizeof(VTX);
	m_tVtxDesc = {};
	m_tVtxDesc.ByteWidth = _iVtxBufferSize; // 크기
	m_tVtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// 생성 이후의 수정 여부 설정
	m_tVtxDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tVtxDesc.CPUAccessFlags = 0;

	//m_tVtxDesc.Usage = D3D11_USAGE_DYNAMIC;
	//m_tVtxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // GPU에 다시 접근해서 수정할 수 있도록 write
	//tBufferDesc.MiscFlags;
	//tBufferDesc.StructureByteStride;

	D3D11_SUBRESOURCE_DATA tSubResData = {};
	tSubResData.pSysMem = _pVtxSys; // 배열의 시작주소 (ByteWidth가 설정되어있어서 크기설정은 안해줘도 됨)

	HRESULT hr = DEVICE->CreateBuffer(&m_tVtxDesc, &tSubResData, m_pVB.GetAddressOf());
	if (FAILED(hr)) {
		assert(nullptr);
		return;
	}
		

	// Alt + Tap을 누르다보면 GPU에서 메모리를 잃어버릴 수 있으므로
	// 버퍼 원형 데이터를 동적할당으로 관리한다.
	// 버텍스 정보 보관
	// system copy
	m_pVtxSys = new VTX[m_iVtxCount];
	memcpy(m_pVtxSys, _pVtxSys, _iVtxBufferSize);

	//////////////////////
	// GPU Memory에 Index Buffer 만들기
	//////////////////////
	tIndexInfo info = {};
	info.iIdxCount = _iIdxBufferSize / sizeof(UINT);
	info.tIBDesc.ByteWidth = _iIdxBufferSize; // GPU 에 생성할 버퍼 메모리 크기
	info.tIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // Index Buffer 용도
	info.tIBDesc.CPUAccessFlags = 0;						// 수정 불가능 버퍼
	info.pIdxSysMem = new UINT[info.iIdxCount];

	// 생성 이후의 수정 여부 설정
	info.tIBDesc.Usage = _eIdxUsage;
	if (D3D11_USAGE_DYNAMIC == info.tIBDesc.Usage)
		info.tIBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// 인덱스 정보 보관
	memcpy(info.pIdxSysMem, _pIdxSys, _iIdxBufferSize);

	tSubResData = {};
	tSubResData.pSysMem = info.pIdxSysMem; // 배열의 시작주소 (ByteWidth가 설정되어있어서 크기설정은 안해줘도 됨)

	hr = DEVICE->CreateBuffer(&info.tIBDesc, &tSubResData, info.pIB.GetAddressOf());
	if (FAILED(hr)) {
		assert(nullptr);
		return;
	}

	m_vecIdxInfo.push_back(info);
}

void CMesh::UpdateData(UINT _iSubSet)
{
	// Input Asselmber Stage 셋팅
	UINT iStride = sizeof(VTX); // 정점 하나의 최대 사이즈 (간격)
	UINT iOffset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, m_pVB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubSet].pIB.Get(), DXGI_FORMAT_R32_UINT, 0); // 4byte unsigned int타입으로
}

void CMesh::Render(UINT _iSubset)
{
	// 파이프라인 시작
	// 버텍스 버퍼를 사용할 경우
	//UINT iVertexCnt = 3;
	//UINT iStartVertexLocation = 0;
	// CONTEXT->Draw(iVertexCnt, iStartVertexLocation); 

	// 메쉬 세팅
	UpdateData(_iSubset);
	// 인덱스 버퍼를 사용
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