#include "pch.h"
#include "CEngineDebugScript.h"

CEngineDebugScript::CEngineDebugScript() :
	CScript(-1),
	m_fLifeTime(1.f),
	m_fAccTime(0.f),
	m_fAlpha(1.f),
	m_bUse(false)
{
}

CEngineDebugScript::~CEngineDebugScript()
{
}

void CEngineDebugScript::Update()
{
	if (!m_bUse)
		return;

	m_fAccTime += DT;
	if (m_fLifeTime < m_fAccTime)
	{
		m_fAccTime = 0.f;
		m_bUse = false;
	}
}