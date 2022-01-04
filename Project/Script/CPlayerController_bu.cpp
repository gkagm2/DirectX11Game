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
	assert(m_pRigid); 
	/*m_pAnim = GetGameObject()->GetComponent<CAnimator2D>();
	assert(m_pAnim);*/
}

void CPlayerController_bu::Update()
{
	if (InputKeyHold(E_Key::A)) { // left
		m_vCurMoveDir = Vector3(0.f, -1.f, 0.f);
		Move();
	}
	if (InputKeyHold(E_Key::D)) { // right
		m_vCurMoveDir = Vector3(0.f, 1.f, 0.f);
		Move();
	}
	
	Jump();

	if (InputKeyHold(E_Key::LBUTTON)) {
		Attack();

	}

}

void CPlayerController_bu::Attack()
{
}

void CPlayerController_bu::Jump()
{
	//Vector3 vJumpPower = Vector3(0.f, 1.f, 0.f);
	//vJumpPower *= m_fJumpPower;
	//m_pRigid->AddForce(vJumpPower);
}

void CPlayerController_bu::Move()
{
	Vector3 vMovePower = m_vCurMoveDir;
	vMovePower *= m_fMovePower;
	m_pRigid->AddForce(vMovePower);
}