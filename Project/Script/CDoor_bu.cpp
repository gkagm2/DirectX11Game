#include "pch.h"
#include "CDoor_bu.h"

CDoor_bu::CDoor_bu() :
	CInteractiveObj_bu((UINT)SCRIPT_TYPE::DOOR_BU),
	m_bIsDoorOpen(false),
	m_fDoorMoveTime{ 0.0f },
	m_fOpenHeight(2.f)
{
}

CDoor_bu::~CDoor_bu()
{ 
}

void CDoor_bu::Start()
{
	m_vClosePos = Transform()->GetLocalPosition();
	m_vOpenPos = m_vClosePos;
	m_vOpenPos.y += Transform()->GetLocalScale().y;

	SetActivateCallBack(this, &CDoor_bu::OpenDoor);
	SetDeActivateCallBack(this, &CDoor_bu::CloseDoor);
}

void CDoor_bu::Update()
{
	if (m_bIsDoorOpen) {
		m_fDoorMoveTime += DT;
		Vector3 vCurPos = CMyMath::Lerp(m_vClosePos, m_vOpenPos, m_fDoorMoveTime);
		Transform()->SetLocalPosition(vCurPos);
		m_fDoorMoveTime = min(m_fDoorMoveTime, 1.f);
	}
	else {
		m_fDoorMoveTime -= DT;
		Vector3 vCurPos = CMyMath::Lerp(m_vClosePos, m_vOpenPos, m_fDoorMoveTime);
		Transform()->SetLocalPosition(vCurPos);
		m_fDoorMoveTime = max(m_fDoorMoveTime, 0.f);
	}
}

void CDoor_bu::OpenDoor()
{
	m_bIsDoorOpen = true;
}

void CDoor_bu::CloseDoor()
{
	m_bIsDoorOpen = false;
}

void CDoor_bu::Interaction(bool _bActive)
{
	if (_bActive) {
		if (m_ActivateCallBack)
			m_ActivateCallBack();
	}
	else {
		if (m_DeActivateCallBack)
			m_DeActivateCallBack();
	}
	m_bIsActivate = _bActive;
}