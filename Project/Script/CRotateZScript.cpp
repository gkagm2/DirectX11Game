#include "pch.h"
#include "CRotateZScript.h"

CRotateZScript::CRotateZScript() :
	CScript((UINT)SCRIPT_TYPE::ROTATEZSCRIPT),
	m_fSpeed(2.f)
{
}

CRotateZScript::~CRotateZScript()
{
}

void CRotateZScript::Update()
{
	Vector3 vCurRotate = Transform()->GetLocalRotation();
	vCurRotate.z += DT * m_fSpeed;
	Transform()->SetLocalRotation(vCurRotate);
}

bool CRotateZScript::SaveToScene(FILE* _pFile)
{
	FWrite(m_fSpeed, _pFile);
	return true;
}

bool CRotateZScript::LoadFromScene(FILE* _pFile)
{
	FRead(m_fSpeed, _pFile);
	return true;
}