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

void CRotateZScript::OnCollisionEnter2D(CCollider2D* _pCol)
{
}

void CRotateZScript::OnCollisionStay2D(CCollider2D* _pCol)
{
}

void CRotateZScript::OnCollisionExit2D(CCollider2D* _pCol)
{
}