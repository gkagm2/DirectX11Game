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
	vCurRotate.z += DeltaTime * m_fSpeed;
	Transform()->SetLocalRotation(vCurRotate);
}