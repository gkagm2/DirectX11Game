#include "pch.h"
#include "CStructuredBuffer.h"
#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer() :
	m_tDesc{},
	m_iElementSize(0),
	m_iElementCount(0),
	m_iRecentRegisterNum(0)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

void CStructuredBuffer::Create(UINT _iElementSize, UINT _iElementCount, void* _pSysData)
{
	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;

	m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_tDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_tDesc.ByteWidth = _iElementSize * _iElementCount;

	m_tDesc.StructureByteStride = _iElementSize;

	if (nullptr == _pSysData)
		DEVICE->CreateBuffer(&m_tDesc, nullptr, m_pSB.GetAddressOf());
	else {
		D3D11_SUBRESOURCE_DATA tSub = { };
		tSub.pSysMem = _pSysData;
		DEVICE->CreateBuffer(&m_tDesc, &tSub, m_pSB.GetAddressOf());
	}
	assert(m_pSB.Get());
		
	// Shader Resource View 积己
	D3D11_SHADER_RESOURCE_VIEW_DESC tDesc = {};
	tDesc.Format = DXGI_FORMAT_UNKNOWN;
	tDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	tDesc.BufferEx.NumElements = _iElementCount;

	DEVICE->CreateShaderResourceView(m_pSB.Get(), &tDesc, m_pSRV.GetAddressOf());
	assert(m_pSRV.Get());
}

void CStructuredBuffer::CreateRW(UINT _iElementSize, UINT _iElementCount, void* _pSysData)
{
	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;

	m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	m_tDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tDesc.CPUAccessFlags = 0;
	m_tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_tDesc.ByteWidth = _iElementSize * _iElementCount;

	m_tDesc.StructureByteStride = _iElementSize;

	if (nullptr == _pSysData)
		DEVICE->CreateBuffer(&m_tDesc, nullptr, m_pSB.GetAddressOf());
	else {
		D3D11_SUBRESOURCE_DATA tSub = { };
		tSub.pSysMem = _pSysData;
		DEVICE->CreateBuffer(&m_tDesc, &tSub, m_pSB.GetAddressOf());
	}
	assert(m_pSB.Get());

	// Shader Resource View 积己
	D3D11_SHADER_RESOURCE_VIEW_DESC tDesc = {};
	tDesc.Format = DXGI_FORMAT_UNKNOWN;
	tDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	tDesc.BufferEx.NumElements = _iElementCount;

	DEVICE->CreateShaderResourceView(m_pSB.Get(), &tDesc, m_pSRV.GetAddressOf());
	assert(m_pSRV.Get());

	// Unorder Access View 积己
	D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
	tUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	tUAVDesc.Buffer.NumElements = _iElementCount;
	DEVICE->CreateUnorderedAccessView(m_pSB.Get(), &tUAVDesc, m_pUAV.GetAddressOf());
}

void CStructuredBuffer::SetData(void* _pSysMem, UINT _iElementCount) const
{
	int iSize = m_iElementSize * _iElementCount;

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(m_pSB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _pSysMem, iSize);
	CONTEXT->Unmap(m_pSB.Get(), 0);
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
	if (m_pSB)
		m_pSB = nullptr; // ComPtr捞骨肺 nullptr肺
	if (m_pSRV)
		m_pSRV = nullptr;
}
