#include "pch.h"
#include "CTargetLookAt_bu.h"
#include "CCharacter_bu.h"

CTargetLookAt_bu::CTargetLookAt_bu() :
	CScript((UINT)SCRIPT_TYPE::TARGETLOOKAT_BU),
	m_pTargetObj(nullptr)
{
}

void CTargetLookAt_bu::Awake()
{
	m_pCharacter = GetGameObject()->FindComponentInParent<CCharacter_bu>();
	assert(m_pCharacter);
}

void CTargetLookAt_bu::Update()
{
	if (!m_pTargetObj) {
		Vector3 vPos = m_pCharacter->GetGameObject()->Transform()->GetPosition();
		if (m_pCharacter->IsLookRight())
			vPos.x += 10.f;
		else
			vPos.x -= 10.f;
		Transform()->LookAt2D(vPos.XY());
		return;
	}
		
	Transform()->LookAt2D(m_pTargetObj->Transform()->GetPosition().XY());
}