#include "pch.h"
#include "CStructuredBuffer.h"
#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer() :
	m_tDesc{},
	m_iElementSize(0),
	m_iElementCount(0)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

void CStructuredBuffer::Create(UINT _iElementSize, UINT _iElementCount)
{
	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;

	m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_tDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_tDesc.ByteWidth = _iElementSize * _iElementCount;

	m_tDesc.StructureByteStride = _iElementSize;

	DEVICE->CreateBuffer(&m_tDesc, nullptr, m_pSB.GetAddressOf());
	assert(m_pSB.Get());

	// Shader Resouruec View »ý¼º
	D3D11_SHADER_RESOURCE_VIEW_DESC tDesc = {};
	tDesc.Format = DXGI_FORMAT_UNKNOWN;
	tDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	tDesc.BufferEx.NumElements = _iElementCount;

	DEVICE->CreateShaderResourceView(m_pSB.Get(), &tDesc, m_pSRV.GetAddressOf());
	assert(m_pSRV.Get());
}

void CStructuredBuffer::SetData(void* _pSysMem, UINT _iElementCount) const
{
	int iSize = m_iElementSize * m_iElementCount;

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(m_pSB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _pSysMem, iSize);
	CONTEXT->Unmap(m_pSB.Get(), 0);
}

void CStructuredBuffer::UpdateData(UINT _iRegisterNum, E_ShaderStage _eStage) const
{
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

void CStructuredBuffer::Release()
{
	if (m_pSB)
		m_pSB->Release();
	m_pSB = nullptr;
	if (m_pSRV)
		m_pSRV->Release();
	m_pSRV = nullptr;
}
