#include "pch.h"
#include "CMesh.h"
#include "CDevice.h"
#pragma warning (disable:6387)

CMesh::CMesh() :
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
	// 버텍스 버퍼 만들기
	////////////////////
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

	DEVICE->CreateBuffer(&m_tVtxDesc, &tSubResData, m_pVB.GetAddressOf());


	////////////////////
	// 인덱스 버퍼 만들기
	////////////////////
	m_iIdxCount = _iIdxBufferSize / sizeof(UINT);

	m_tIdxDesc = {};
	m_tIdxDesc.ByteWidth = _iIdxBufferSize; // 크기
	m_tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// 생성 이후의 수정 여부 설정
	m_tIdxDesc.Usage = _eIdxUsage;
	if (D3D11_USAGE_DYNAMIC == m_tIdxDesc.Usage)
		m_tIdxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	tSubResData = {};
	tSubResData.pSysMem = _pIdxSys; // 배열의 시작주소 (ByteWidth가 설정되어있어서 크기설정은 안해줘도 됨)

	DEVICE->CreateBuffer(&m_tIdxDesc, &tSubResData, m_pIB.GetAddressOf());


	// Alt + Tap을 누르다보면 GPU에서 메모리를 잃어버릴 수 있으므로
	// 버퍼 원형 데이터를 동적할당으로 관리한다.
	
	// 버텍스 정보 보관
	if (nullptr == m_pVtxSys) {
		m_pVtxSys = malloc(_iVtxBufferSize);
		memcpy(m_pVtxSys, _pVtxSys, _iVtxBufferSize);
	}

	if (0 != _iIdxBufferSize) {
		// 인덱스 정보 보관
		m_pIdxSys = malloc(_iIdxBufferSize);
		memcpy(m_pIdxSys, _pIdxSys, _iIdxBufferSize);
	}
}

void CMesh::UpdateData()
{
	// Input Asselmber Stage 셋팅
	UINT iStride = sizeof(VTX); // 정점 하나의 최대 사이즈 (간격)
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, m_pVB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_pIB.Get(), DXGI_FORMAT_R32_UINT, 0); // 4byte unsigned int타입으로
}

void CMesh::Render()
{
	// 파이프라인 시작
	// 버텍스 버퍼를 사용할 경우
	//UINT iVertexCnt = 3;
	//UINT iStartVertexLocation = 0;
	// CONTEXT->Draw(iVertexCnt, iStartVertexLocation); 

	// 인덱스 버퍼를 사용
	UINT iStartIndexLocation = 0;
	UINT iBaseVertexLocation = 0;
	CONTEXT->DrawIndexed(m_iIdxCount, iStartIndexLocation, iBaseVertexLocation);
}

void CMesh::Render_Particle(UINT _iCount)
{
	CONTEXT->DrawIndexedInstanced(m_iIdxCount, _iCount, 0, 0, 0);
}

void CMesh::Reset()
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, m_pVtxSys, m_tVtxDesc.ByteWidth);
	CONTEXT->Unmap(m_pVB.Get(), 0);
}