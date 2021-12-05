#include "pch.h"
#include "CTestShader.h"
#include "CTexture.h"
#include "CConstBuffer.h"

CTestShader::CTestShader() :
	CComputeShader(32, 32, 1)
{
}

CTestShader::~CTestShader()
{
}

void CTestShader::UpdateData()
{
	assert(m_pTexture.Get());

	m_tInfo.iArr[0] = (int)m_pTexture->GetResolution().x;
	m_tInfo.iArr[1] = (int)m_pTexture->GetResolution().y;
	m_tInfo.v4Arr[0] = m_vSetColor;

	g_pMtrlBuffer->SetData(&m_tInfo);
	g_pMtrlBuffer->UpdateData(E_ShaderStage::Compute);

	m_pTexture->UpdateRWData(0);
}

void CTestShader::Clear()
{
	g_pMtrlBuffer->Clear(E_ShaderStage::Compute);

	if (nullptr != m_pTexture)
		m_pTexture->ClearRW(0);
}

void CTestShader::Excute()
{
	UINT iGroupX = ((UINT)m_pTexture->GetResolution().x / GetThreadX()) + 1;
	UINT iGroupY = ((UINT)m_pTexture->GetResolution().y / GetThreadY()) + 1;
	Dispatch(iGroupX, iGroupY, 1);
}