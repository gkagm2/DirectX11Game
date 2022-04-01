#include "pch.h"
#include "CStructuredBuffer.h"
#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer() :
	m_desc{},
	m_iElementSize(0),
	m_iElementCount(0),
	m_eType(E_StructuredBufferType::ReadOnly),
	m_desc_cpu_read{},
	m_desc_cpu_write{},
	m_bCpuAccess{false},
	m_eRecentPipelineStage{E_ShaderStage::All},
	m_iRecentRegisterNum(0),
	m_iRecentRegisterNumRW(0)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

HRESULT CStructuredBuffer::Create(E_StructuredBufferType _eType, UINT _iElementSize, UINT _iElementCount, bool _bUseSysIO, void* _pInitial)
{
	HRESULT bResult = E_FAIL;

	// 기존에 사용중이던 구조화 버펴였다면, 참조 카운트를 감소시켜 버퍼 전부 해제
	Release();
	
	// 버퍼를 생성한다.
	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;
	m_eType = _eType;
	m_bCpuAccess = _bUseSysIO;

	// 구조화 정보
	m_desc.ByteWidth = m_iElementSize * _iElementCount;

	if (E_StructuredBufferType::ReadOnly == m_eType)
		m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	else if (E_StructuredBufferType::Read_Write == m_eType)
		m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;

	m_desc.Usage = D3D11_USAGE_DEFAULT;
	m_desc.CPUAccessFlags = 0;

	m_desc.StructureByteStride = m_iElementSize;
	m_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	
	
	if (_pInitial) {
		D3D11_SUBRESOURCE_DATA tSub = {};
		tSub.pSysMem = _pInitial;
		bResult = DEVICE->CreateBuffer(&m_desc, &tSub, m_SB.GetAddressOf());
		if (FAILED(bResult)) return E_FAIL;
	}
	else {
		bResult = DEVICE->CreateBuffer(&m_desc, nullptr, m_SB.GetAddressOf());
		if (FAILED(bResult)) return E_FAIL;
	}

	// SRV 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	tSRVDesc.BufferEx.NumElements = m_iElementCount;

	bResult = DEVICE->CreateShaderResourceView(m_SB.Get(), &tSRVDesc, m_SRV.GetAddressOf());
	if (FAILED(bResult)) return E_FAIL;


	// UAV 생성
	if (E_StructuredBufferType::Read_Write == m_eType) {
		D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
		tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		tUAVDesc.Buffer.NumElements = m_iElementCount;
		bResult = DEVICE->CreateUnorderedAccessView(m_SB.Get(), &tUAVDesc, m_UAV.GetAddressOf());
		if (FAILED(bResult)) return E_FAIL;
	}

	if (m_bCpuAccess) {
		// CPU Read
		m_desc_cpu_read.ByteWidth = m_iElementCount * m_iElementSize;
		m_desc_cpu_read.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_desc_cpu_read.Usage = D3D11_USAGE_DEFAULT;
		m_desc_cpu_read.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		m_desc_cpu_read.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		m_desc_cpu_read.StructureByteStride = m_iElementSize;

		bResult = DEVICE->CreateBuffer(&m_desc_cpu_read, nullptr, m_SB_CPU_Read.GetAddressOf());
		if (FAILED(bResult)) return E_FAIL;

		// CPU Write
		m_desc_cpu_write.ByteWidth = m_iElementCount * m_iElementSize;
		m_desc_cpu_write.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_desc_cpu_write.Usage = D3D11_USAGE_DYNAMIC;
		m_desc_cpu_write.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_desc_cpu_write.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		m_desc_cpu_write.StructureByteStride = m_iElementSize;

		bResult = DEVICE->CreateBuffer(&m_desc_cpu_write, nullptr, m_SB_CPU_Write.GetAddressOf());
		if (FAILED(bResult)) return E_FAIL;
	}

	return S_OK;
}

void CStructuredBuffer::SetData(void* _pSysMem, UINT _iSize) const
{
	// 버퍼 크기보다 더 큰 데이터가 들어오려고 한다면
	int iElementCnt = m_iElementSize / _iSize;
	if (m_iElementCount < iElementCnt) {
		assert(nullptr && _T("버퍼를 재생성하여 늘려줘야 함"));
		//Create(m_eType, _iSize , (UINT)iElementCnt, m_bCpuAccess, nullptr);
	}

	// system -> m_sb_cpu_write
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_SB_CPU_Write.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _pSysMem, (size_t)_iSize);
	CONTEXT->Unmap(m_SB_CPU_Write.Get(), 0);

	// m_sb_cpu_write -> m_SB(copyresource)
	CONTEXT->CopyResource(m_SB.Get(), m_SB_CPU_Write.Get());
}

void CStructuredBuffer::GetData(void* _pDest, UINT _iSize)
{
	// m_sb -> m_sb_cpu_read
	CONTEXT->CopyResource(m_SB_CPU_Read.Get(), m_SB.Get());

	// m_sb_cpu_read -> system mem
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_SB_CPU_Read.Get(), 0, D3D11_MAP_READ, 0, &tSub);
	memcpy(_pDest, tSub.pData, (size_t)_iSize);
	CONTEXT->Unmap(m_SB_CPU_Read.Get(), 0);
}

void CStructuredBuffer::UpdateData(UINT _iRegisterNum, E_ShaderStage _eStage)
{
	m_iRecentRegisterNum = _iRegisterNum;
	if ((UINT)_eStage & (UINT)E_ShaderStage::Vertex)
		CONTEXT->VSSetShaderResources(m_iRecentRegisterNum, 1, m_SRV.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Hull)
		CONTEXT->HSSetShaderResources(m_iRecentRegisterNum, 1, m_SRV.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Domain)
		CONTEXT->DSSetShaderResources(m_iRecentRegisterNum, 1, m_SRV.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Geometry)
		CONTEXT->GSSetShaderResources(m_iRecentRegisterNum, 1, m_SRV.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Pixel)
		CONTEXT->PSSetShaderResources(m_iRecentRegisterNum, 1, m_SRV.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Compute)
		CONTEXT->CSSetShaderResources(m_iRecentRegisterNum, 1, m_SRV.GetAddressOf());
}

void CStructuredBuffer::UpdateDataCS(UINT _iRegisterNum)
{
	if (E_StructuredBufferType::ReadOnly == m_eType)
		assert(nullptr);

	m_iRecentRegisterNumRW = _iRegisterNum;
	UINT iInitialzedCnt = -1;
	UINT iNumUAVs = 1;
	CONTEXT->CSSetUnorderedAccessViews(m_iRecentRegisterNumRW, iNumUAVs, m_UAV.GetAddressOf(), &iInitialzedCnt);
}

void CStructuredBuffer::Clear(E_ShaderStage _eStage)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	if ((UINT)_eStage & (UINT)E_ShaderStage::Vertex)
		CONTEXT->VSSetShaderResources(m_iRecentRegisterNum, 1, &pSRV);
	if ((UINT)_eStage & (UINT)E_ShaderStage::Hull)
		CONTEXT->HSSetShaderResources(m_iRecentRegisterNum, 1, &pSRV);
	if ((UINT)_eStage & (UINT)E_ShaderStage::Domain)
		CONTEXT->DSSetShaderResources(m_iRecentRegisterNum, 1, &pSRV);
	if ((UINT)_eStage & (UINT)E_ShaderStage::Geometry)
		CONTEXT->GSSetShaderResources(m_iRecentRegisterNum, 1, &pSRV);
	if ((UINT)_eStage & (UINT)E_ShaderStage::Pixel)
		CONTEXT->PSSetShaderResources(m_iRecentRegisterNum, 1, &pSRV);
	if ((UINT)_eStage & (UINT)E_ShaderStage::Compute)
		CONTEXT->CSSetShaderResources(m_iRecentRegisterNum, 1, &pSRV);
}

void CStructuredBuffer::ClearRW()
{
	ID3D11UnorderedAccessView* pUAV = nullptr;
	UINT iInitialzedCnt = -1;
	UINT iNumUAVs = 1;
	CONTEXT->CSSetUnorderedAccessViews(m_iRecentRegisterNumRW, iNumUAVs, &pUAV, &iInitialzedCnt);
}

void CStructuredBuffer::Release()
{
	if (m_SB)
		m_SB = nullptr; // ComPtr이므로 nullptr로
	if (m_SRV)
		m_SRV = nullptr;
	if (m_UAV)
		m_UAV = nullptr;
	if (m_SB_CPU_Read)
		m_SB_CPU_Read = nullptr;
	if (m_SB_CPU_Write)
		m_SB_CPU_Write = nullptr;
	if (m_SB_CPU_Write)
		m_SB_CPU_Write = nullptr;

	m_desc = D3D11_BUFFER_DESC{};
	m_desc_cpu_read = D3D11_BUFFER_DESC{};
	m_desc_cpu_write = D3D11_BUFFER_DESC{};
}
