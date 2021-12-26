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
	Vector3 vRotation =Transform()->GetLocalRotation();
	vRotation.z = m_fAngle;
	Transform()->SetLocalRotation(vRotation);
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
