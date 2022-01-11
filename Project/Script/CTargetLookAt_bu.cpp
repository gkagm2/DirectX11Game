#include "pch.h"
#include "CTargetLookAt_bu.h"

CTargetLookAt_bu::CTargetLookAt_bu() :
	CScript((UINT)SCRIPT_TYPE::TARGETLOOKAT_BU),
	m_pTargetObj(nullptr)
{
}

void CTargetLookAt_bu::Update()
{
	if (!m_pTargetObj)
		return;
	Transform()->LookAt2D(m_pTargetObj->Transform()->GetPosition().XY());
}