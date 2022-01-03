#include "pch.h"
#include "CPlayerController_bu.h"
CPlayerController_bu::CPlayerController_bu() :
	CCharacter_bu((UINT)SCRIPT_TYPE::PLAYERCONTROLLER_BU),
	m_pRigid(nullptr),
	m_pAnim(nullptr)
{
}

CPlayerController_bu::~CPlayerController_bu()
{
}

void CPlayerController_bu::Start()
{
	m_pRigid = GetGameObject()->GetComponent<CRigidbody2D>();
	m_pAnim = GetGameObject()->GetComponent<CAnimator2D>();
	assert(m_pRigid); 
	assert(m_pAnim);
}

void CPlayerController_bu::Update()
{
}

void CPlayerController_bu::Attack()
{
}

void CPlayerController_bu::Jump()
{
}

void CPlayerController_bu::Move()
{
}