#include "pch.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CConstBuffer::CConstBuffer() :
	m_tDesc{},
	m_eType(E_ConstBuffer::End)
{
}

CConstBuffer::~CConstBuffer()
{
}

void CConstBuffer::Create(const tstring& _strName, E_ConstBuffer _eType, UINT _iBufferSize)
{
	SetName(_strName);

	m_eType = _eType;
	m_tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_tDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_tDesc.ByteWidth = _iBufferSize;

	DEVICE->CreateBuffer(&m_tDesc, nullptr, m_pCB.GetAddressOf());
}


void CConstBuffer::SetData(void* _pSysMem, UINT _iSize) const
{
	// iSize에 인자값을 넣지 않으면 Desc구조체의 ByteWidth을 이용함
	if (_iSize == 0) // Default 
		_iSize = m_tDesc.ByteWidth;

	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_pCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _pSysMem, _iSize);
	CONTEXT->Unmap(m_pCB.Get(), 0);
}

void CConstBuffer::UpdateData(E_ShaderStage _eStage) const
{
	if ((UINT)_eStage & (UINT)E_ShaderStage::Vertex)
		CONTEXT->VSSetConstantBuffers((UINT)m_eType, 1, m_pCB.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Hull)
		CONTEXT->HSSetConstantBuffers((UINT)m_eType, 1, m_pCB.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Domain)
		CONTEXT->DSSetConstantBuffers((UINT)m_eType, 1, m_pCB.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Geometry)
		CONTEXT->GSSetConstantBuffers((UINT)m_eType, 1, m_pCB.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Pixel)
		CONTEXT->PSSetConstantBuffers((UINT)m_eType, 1, m_pCB.GetAddressOf());
	if ((UINT)_eStage & (UINT)E_ShaderStage::Compute)
		CONTEXT->CSSetConstantBuffers((UINT)m_eType, 1, m_pCB.GetAddressOf());
}

void CConstBuffer::Clear(E_ShaderStage _eStage) const
{
	ID3D11Buffer* pBuffer = nullptr;
	if ((UINT)_eStage & (UINT)E_ShaderStage::Vertex)
		CONTEXT->VSSetConstantBuffers((UINT)m_eType, 1, &pBuffer);
	if ((UINT)_eStage & (UINT)E_ShaderStage::Hull)
		CONTEXT->HSSetConstantBuffers((UINT)m_eType, 1, &pBuffer);
	if ((UINT)_eStage & (UINT)E_ShaderStage::Domain)
		CONTEXT->DSSetConstantBuffers((UINT)m_eType, 1, &pBuffer);
	if ((UINT)_eStage & (UINT)E_ShaderStage::Geometry)
		CONTEXT->GSSetConstantBuffers((UINT)m_eType, 1, &pBuffer);
	if ((UINT)_eStage & (UINT)E_ShaderStage::Pixel)
		CONTEXT->PSSetConstantBuffers((UINT)m_eType, 1, &pBuffer);
	if ((UINT)_eStage & (UINT)E_ShaderStage::Compute)
		CONTEXT->CSSetConstantBuffers((UINT)m_eType, 1, &pBuffer);
}
