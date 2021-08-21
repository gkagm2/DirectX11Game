#include "pch.h"
#include "CMouseCollision2DScript.h"

CMouseCollision2DScript::CMouseCollision2DScript() :
	CScript((UINT)SCRIPT_TYPE::MOUSECOLLISION2DSCRIPT),
	m_pTargetObj(nullptr)
{
}

CMouseCollision2DScript::~CMouseCollision2DScript()
{
}

void CMouseCollision2DScript::Awake()
{
	GetGameObject()->AddComponent<CCollider2D>();
}

void CMouseCollision2DScript::Start()
{
}

void CMouseCollision2DScript::Update()
{
	if (InputKeyPress(E_Key::LBUTTON)) {
	}
	if (InputKeyHold(E_Key::LBUTTON)) {

	}
	if (InputKeyPress(E_Key::LBUTTON)) {

	}
}

void CMouseCollision2DScript::OnCollisionEnter2D(CCollider2D* _pOther)
{
	// 클릭한것이라면
	if (1 == _pOther->GetGameObject()->GetLayer()) {
	}
}

void CMouseCollision2DScript::OnCollisionStay2D(CCollider2D* _pOther)
{
}

void CMouseCollision2DScript::OnCollisionExit2D(CCollider2D* _pOther)
{
}