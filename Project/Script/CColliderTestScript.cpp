#include "pch.h"
#include "CColliderTestScript.h"

CColliderTestScript::CColliderTestScript() :
	CScript((UINT)SCRIPT_TYPE::COLLIDERTESTSCRIPT)
{
}

CColliderTestScript::~CColliderTestScript()
{
}



void CColliderTestScript::Update()
{
}

void CColliderTestScript::OnCollisionEnter2D(CCollider2D* _pOther)
{
	static int i = 0;
	_tcprintf(_T("Enter Test%d \n"), ++i);

	DestroyGameObjectEvn(_pOther->GetGameObject());
}

void CColliderTestScript::OnCollisionStay2D(CCollider2D* _pOther)
{
	static int i = 0;
	_tcprintf(_T("Stay Test%d \n"), ++i);
}

void CColliderTestScript::OnCollisionExit2D(CCollider2D* _pOther)
{
	static int i = 0;
	_tcprintf(_T("Exit Test %d\n"), ++i);
}