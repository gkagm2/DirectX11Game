#include "pch.h"
#include "CTestLight2DScript.h"

CTestLight2DScript::CTestLight2DScript() :
	m_fAngle(0.f)
{
}

CTestLight2DScript::~CTestLight2DScript()
{
}

void CTestLight2DScript::Start()
{
}

void CTestLight2DScript::Update()
{
	m_fAngle += DeltaTime * 3.f;
	CLight2D* pLight2D = Light2D();
	Light2D()->SetLightDir(Vector3(cosf(m_fAngle), sinf(m_fAngle), 0.f));
}