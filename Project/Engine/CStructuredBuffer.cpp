#include "pch.h"
#include "CStructuredBuffer.h"
#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer() :
	m_tDesc{},
	m_iElementSize(0),
	m_iElementCount(0),
	m_iRecentRegisterNum(0),
	m_eType(E_StructuredBufferType::ReadOnly)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

void CStructuredBuffer::Create(E_StructuredBufferType _eType, UINT _iElementSize, UINT _iElementCount, void* _pSysData)
{
	Release();
	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;
	m_eType = _eType;

	m_tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_tDesc.StructureByteStride = _iElementSize;
	m_tDesc.ByteWidth = _iElementSize * _iElementCount;

	// Shader Resource View Description
	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
	tSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	tSRVDesc.BufferEx.NumElements = _iElementCount;

	if (E_StructuredBufferType::ReadOnly == _eType) {
		// (m_pSB_R에만 할당)
		m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_tDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (nullptr == _pSysData)
			DEVICE->CreateBuffer(&m_tDesc, nullptr, m_pSB_R.GetAddressOf());
		else {
			D3D11_SUBRESOURCE_DATA tSub = { };
			tSub.pSysMem = _pSysData;
			DEVICE->CreateBuffer(&m_tDesc, &tSub, m_pSB_R.GetAddressOf());
		}
		assert(m_pSB_R.Get());

		// Shader Resource View 생성
		DEVICE->CreateShaderResourceView(m_pSB_R.Get(), &tSRVDesc, m_pSRV.GetAddressOf());
		assert(m_pSRV.Get());
	}
	else if (E_StructuredBufferType::Read_Write == _eType) {
		// (m_pSB_RW에만 할당)
		m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		m_tDesc.Usage = D3D11_USAGE_DEFAULT;
		m_tDesc.CPUAccessFlags = 0;

		if (nullptr == _pSysData)
			DEVICE->CreateBuffer(&m_tDesc, nullptr, m_pSB_RW.GetAddressOf());
		else {
			D3D11_SUBRESOURCE_DATA tSub = { };
			tSub.pSysMem = _pSysData;
			DEVICE->CreateBuffer(&m_tDesc, &tSub, m_pSB_RW.GetAddressOf());
		}
		assert(m_pSB_RW.Get());

		// Shader Resource View 생성
		DEVICE->CreateShaderResourceView(m_pSB_RW.Get(), &tSRVDesc, m_pSRV.GetAddressOf());
		assert(m_pSRV.Get());

		// Unorder Access View 생성
		D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
		tUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		tUAVDesc.Buffer.NumElements = _iElementCount;
		DEVICE->CreateUnorderedAccessView(m_pSB_RW.Get(), &tUAVDesc, m_pUAV.GetAddressOf());
		assert(m_pUAV.Get());
	}

	else if (E_StructuredBufferType::Dual == _eType) {
		// (m_pSB_R, m_pSB_RW 둘다 할당)
		// Read 용 버퍼
		m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_tDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (nullptr == _pSysData)
			DEVICE->CreateBuffer(&m_tDesc, nullptr, m_pSB_R.GetAddressOf());
		else {
			D3D11_SUBRESOURCE_DATA tSub = { };
			tSub.pSysMem = _pSysData;
			DEVICE->CreateBuffer(&m_tDesc, &tSub, m_pSB_R.GetAddressOf());
		}
		assert(m_pSB_R.Get());


		// Read-Write 용 버퍼
		m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		m_tDesc.Usage = D3D11_USAGE_DEFAULT;
		m_tDesc.CPUAccessFlags = 0;

		if (nullptr == _pSysData)
			DEVICE->CreateBuffer(&m_tDesc, nullptr, m_pSB_RW.GetAddressOf());
		else {
			D3D11_SUBRESOURCE_DATA tSub = { };
			tSub.pSysMem = _pSysData;
			DEVICE->CreateBuffer(&m_tDesc, &tSub, m_pSB_RW.GetAddressOf());
		}
		assert(m_pSB_RW.Get());

		// Shader Resource View 생성
		DEVICE->CreateShaderResourceView(m_pSB_RW.Get(), &tSRVDesc, m_pSRV.GetAddressOf());
		assert(m_pSRV.Get());

		// Unorder Access View 생성
		D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
		tUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		tUAVDesc.Buffer.NumElements = _iElementCount;
		DEVICE->CreateUnorderedAccessView(m_pSB_RW.Get(), &tUAVDesc, m_pUAV.GetAddressOf());
		assert(m_pUAV.Get());
	}
}

void CStructuredBuffer::SetData(void* _pSysMem, UINT _iElementCount) const
{
	assert(E_StructuredBufferType::Read_Write != m_eType && _T("Read_Write로 데이터 설정 불가능"));

	int iSize = m_iElementSize * _iElementCount;

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(m_pSB_R.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _pSysMem, iSize);
	CONTEXT->Unmap(m_pSB_R.Get(), 0);

	// Dual일 경우 m_pSB_R에서 m_pSB->RW로 버퍼 복사
	if (E_StructuredBufferType::Dual == m_eType)
		CONTEXT->CopyResource(m_pSB_RW.Get(), m_pSB_R.Get());
}

void CStructuredBuffer::GetData(void* _pDest, UINT _iElementCount)
{
	// TODO (Jang) : Get Data 만들기
}

void CStructuredBuffer::UpdateData(UINT _iRegisterNum, E_ShaderStage _eStage)
{
	m_iRecentRegisterNum = _iRegisterNum;
	if ((UINT)_eStage & (UINT)E_ShaderStage::Vertex)
		CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Hull)
		CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Domain)
		CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Geometry)
		CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Pixel)
		CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Compute)
		CONTEXT->CSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
}

void CStructuredBuffer::UpdateDataRW(UINT _iRegisterNum)
{
	// FIXED(Jang) : ??
	//// u 레지스터 바인딩이 불가능한 구조화버퍼인 경우
	//if ((UINT)E_StructuredBufferType::Read_Write == _iRegisterNum)
	//	assert(nullptr);

	m_iRecentRegisterNum = _iRegisterNum;
	UINT iInitialzedCnt = -1;
	UINT iNumUAVs = 1;
	CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, iNumUAVs, m_pUAV.GetAddressOf(), &iInitialzedCnt);
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
	CONTEXT->CSSetUnorderedAccessViews(m_iRecentRegisterNum, iNumUAVs, &pUAV, &iInitialzedCnt);
}

void CStructuredBuffer::Release()
{
	if (m_pSB_R)
		m_pSB_R = nullptr; // ComPtr이므로 nullptr로
	if (m_pSRV)
		m_pSRV = nullptr;
	if (m_pSB_R)
		m_pSB_R = nullptr;
	if (m_pSB_RW)
		m_pSB_RW = nullptr;
}
