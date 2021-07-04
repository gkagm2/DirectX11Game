#include "pch.h"
#include "CParticleUpdateShader.h"
#include "CStructuredBuffer.h"
#include "CConstBuffer.h"

CParticleUpdateShader::CParticleUpdateShader() :
	CComputeShader(1024, 1, 1),
	m_pParticleBuffer(nullptr),
	m_fStartSpeed(0.f),
	m_fEndSpeed(0.f)
{
}

CParticleUpdateShader::~CParticleUpdateShader()
{
}

void CParticleUpdateShader::UpdateData()
{
	UINT iRegisterNum = 0;
	m_pParticleBuffer->UpdateDataRW(iRegisterNum);

	m_tInfo.iArr[0] = m_pParticleBuffer->GetElementCount();
	m_tInfo.fArr[0] = m_fStartSpeed;
	m_tInfo.fArr[1] = m_fEndSpeed;

	g_pMtrlBuffer->SetData(&m_tInfo);
	g_pMtrlBuffer->UpdateData(E_ShaderStage::Compute);
}

void CParticleUpdateShader::Clear()
{
	m_pParticleBuffer->ClearRW();
	g_pMtrlBuffer->Clear(E_ShaderStage::Compute);
}

void CParticleUpdateShader::Excute()
{
	UINT iElementCnt = m_pParticleBuffer->GetElementCount();
	UINT iThreadX = (iElementCnt / GetThreadX()) + 1;
	Dispatch(iThreadX, 1, 1);
}