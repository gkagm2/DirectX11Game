#include "pch.h"
#include "CAnimation3DShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

CAnimation3DShader::CAnimation3DShader()
	: CComputeShader(256, 1, 1)
	, m_pFrameDataBuffer(nullptr)
	, m_pOffsetMatBuffer(nullptr)
	, m_pOutputBuffer(nullptr)
{
}

CAnimation3DShader::~CAnimation3DShader()
{
}

void CAnimation3DShader::UpdateData()
{
	// 구조화버퍼 전달
	m_pFrameDataBuffer->UpdateData(16, E_ShaderStage::Compute); // t16
	m_pOffsetMatBuffer->UpdateData(17, E_ShaderStage::Compute); // t17
	m_pOutputBuffer->UpdateDataCS(0);   // u0
}

void CAnimation3DShader::Excute()
{
	UINT iGrounX = (m_tInfo.iArr[0] / GetGroupPerThreadX()) + 1;
	Dispatch(iGrounX, 1, 1);
}

void CAnimation3DShader::Clear()
{
	// 전달한 구조화버퍼 클리어	
	if (nullptr != m_pFrameDataBuffer)
	{
		m_pFrameDataBuffer->Clear();
		m_pFrameDataBuffer = nullptr;
	}

	if (nullptr != m_pOffsetMatBuffer)
	{
		m_pOffsetMatBuffer->Clear();
		m_pOffsetMatBuffer = nullptr;
	}

	if (nullptr != m_pOutputBuffer)
	{
		m_pOutputBuffer->ClearRW();
		m_pOutputBuffer = nullptr;
	}
}