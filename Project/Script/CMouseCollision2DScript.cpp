#include "pch.h"
#include "CMouseCollision2DScript.h"

CMouseCollision2DScript::CMouseCollision2DScript() :
	CScript((UINT)SCRIPT_TYPE::MOUSECOLLISION2DSCRIPT)
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
}