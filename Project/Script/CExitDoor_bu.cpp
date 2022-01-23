#include "pch.h"
#include "CExitDoor_bu.h"

CExitDoor_bu::CExitDoor_bu() :
	CScript((UINT)SCRIPT_TYPE::EXITDOOR_BU),
	m_bIsPlayerIn(false)
{
}


void CExitDoor_bu::Awake()
{
	m_vDoorClosePos = Transform()->GetLocalPosition();
	m_vDoorOpenPos = m_vDoorClosePos;
	m_vDoorOpenPos.y = Transform()->GetLocalScale().y;
}

void CExitDoor_bu::Update()
{
	if (m_bIsPlayerIn) {
		m_fTime += DT;
	}
	else {
		m_fTime -= DT;
	}
	Vector3 vDoorPos = CMyMath::Lerp(m_vDoorClosePos, m_vDoorOpenPos, m_fTime);
	m_fTime = CMyMath::Clamp(m_fTime, 0.f, 1.f);
	
	Transform()->SetLocalPosition(vDoorPos);
}

void CExitDoor_bu::OnCollisionEnter2D(CCollider2D* _pCol)
{
	UINT iPlayerTag = (UINT)E_Tag::Player;
	CGameObject* pObj = _pCol->GetGameObject();
	if (iPlayerTag == pObj->GetTag()) {
		m_bIsPlayerIn = true;
		m_fTime = 0.f;
	}
}

void CExitDoor_bu::OnCollisionExit2D(CCollider2D* _pCol)
{
	UINT iPlayerTag = (UINT)E_Tag::Player;
	CGameObject* pObj = _pCol->GetGameObject();
	if (iPlayerTag == pObj->GetTag()) {
		m_bIsPlayerIn = false;
		m_fTime = 1.f;
	}
}