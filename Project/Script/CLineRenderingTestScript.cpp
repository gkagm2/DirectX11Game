#include "pch.h"
#include "CLineRenderingTestScript.h"
#include <Engine\CLine.h>

CLineRenderingTestScript::CLineRenderingTestScript() :
	CScript((UINT)SCRIPT_TYPE::LINERENDERINGTESTSCRIPT)
{
	m_pLine = new CLine;
}

CLineRenderingTestScript::~CLineRenderingTestScript()
{
	SAFE_DELETE(m_pLine);
}

void CLineRenderingTestScript::Awake()
{
	m_pLine->AddLine(Vector3(0.f, 0.f, 0.f), Vector3(0.f,  0.f, 0.f));
	m_pLine->AddLine(Vector3(0.f, 0.0f, 0.f), Vector3(1.f, 1.f, 0.f));
	m_pLine->AddLine(Vector3(0.f, 0.1f, 0.f), Vector3(0.f, 0.8f, 0.f));
	m_pLine->AddLine(Vector3(0.f, 0.2f, 0.f), Vector3(0.f, 0.7f, 0.f));
	m_pLine->AddLine(Vector3(0.f, 0.3f, 0.f), Vector3(0.f, 0.6f, 0.f));
	m_pLine->AddLine(Vector3(0.f, 0.4f, 0.f), Vector3(0.f, 0.5f, 0.f));
	m_pLine->AddLine(Vector3(0.f, 0.5f, 0.f), Vector3(0.f, 0.4f, 0.f));
}

void CLineRenderingTestScript::Update()
{
	CRenderManager::GetInstance()->RenderDebugSphere(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f), 400.f, 2.f);
}

void CLineRenderingTestScript::LateUpdate()
{
	m_pLine->UpdateData();
}
