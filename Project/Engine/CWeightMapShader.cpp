#include "pch.h"
#include "CWeightMapShader.h"

CWeightMapShader::CWeightMapShader() :
	CComputeShader(32, 32, 1),
	m_pWeightMapBuff{ nullptr },
	m_pRaycastInfoBuff{ nullptr },
	m_vBrushScale{ 1.f,1.f },
	m_iBrushIdx{ 0 },
	m_iWeightIdx{ 0 },
	m_iWidth{ 0 },
	m_iHeight{ 0 }
{
}

CWeightMapShader::~CWeightMapShader()
{
}

void CWeightMapShader::UpdateData()
{
}

void CWeightMapShader::Excute()
{
}
void CWeightMapShader::Clear()
{
}