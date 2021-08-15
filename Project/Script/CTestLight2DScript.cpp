#include "pch.h"
#include "CTestLight2DScript.h"

CTestLight2DScript::CTestLight2DScript() :
	CScript((UINT)SCRIPT_TYPE::TESTLIGHT2DSCRIPT),
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
	m_fAngle += DT * 3.f;
	CLight2D* pLight2D = Light2D();
	Light2D()->SetLightDir(Vector3(cosf(m_fAngle), sinf(m_fAngle), 0.f));
}

bool CTestLight2DScript::SaveToScene(FILE* _pFile)
{
	FWrite(m_fAngle, _pFile);
	return true;
}

bool CTestLight2DScript::LoadFromScene(FILE* _pFile)
{
	FRead(m_fAngle, _pFile);
	return true;
}
