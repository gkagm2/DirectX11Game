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

void CStructuredBuffer::SetData(void* _pSysMem, UINT _iSize) const
{

}

void CStructuredBuffer::UpdateData(E_ShaderStage _eStage) const
{

}