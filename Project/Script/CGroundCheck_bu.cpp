#include "pch.h"
#include "CGroundCheck_bu.h"

CGroundCheck_bu::CGroundCheck_bu() :
	CScript((UINT)SCRIPT_TYPE::GROUNDCHECK_BU)
{
}

void CGroundCheck_bu::OnCollisionEnter2D(CCollider2D* _pCol)
{
	UINT iTag = (UINT)E_Tag::Wall;
	if (iTag == _pCol->GetGameObject()->GetTag()) {
		m_bTouchGround = true;
	}

}

void CGroundCheck_bu::OnCollisionExit2D(CCollider2D* _pCol)
{
	UINT iTag = (UINT)E_Tag::Wall;
	if (iTag == _pCol->GetGameObject()->GetTag()) {
		m_bTouchGround = false;
	}
}