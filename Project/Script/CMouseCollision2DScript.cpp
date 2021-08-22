#include "pch.h"
#include "CMouseCollision2DScript.h"
#include <Engine\CRenderManager.h>

CMouseCollision2DScript::CMouseCollision2DScript() :
	CScript((UINT)SCRIPT_TYPE::MOUSECOLLISION2DSCRIPT),
	m_pTargetObj(nullptr),
	m_vStartMousePos{},
	m_vStartTargetPos{}
{
}

CMouseCollision2DScript::~CMouseCollision2DScript()
{
}

void CMouseCollision2DScript::Awake()
{
	GetGameObject()->AddComponent<CCollider2D>();
	Collider2D()->SetOffsetScale(Vector2(1.f, 1.f));
	Collider2D()->SetActive(false);
}

void CMouseCollision2DScript::Start()
{
}

void CMouseCollision2DScript::Update()
{
	
}

void CMouseCollision2DScript::OnCollisionEnter2D(CCollider2D* _pOther)
{
	
}

void CMouseCollision2DScript::OnCollisionStay2D(CCollider2D* _pOther)
{
}

void CMouseCollision2DScript::OnCollisionExit2D(CCollider2D* _pOther)
{
}