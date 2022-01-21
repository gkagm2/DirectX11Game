#include "pch.h"
#include "CGroundCheck_bu.h"

CGroundCheck_bu::CGroundCheck_bu() :
	CScript((UINT)SCRIPT_TYPE::GROUNDCHECK_BU),
	m_bTouchGround{false}
{
}

void CGroundCheck_bu::OnCollisionEnter2D(CCollider2D* _pCol)
{
	UINT iLayer = (UINT)E_Layer::Wall;
	if (iLayer == _pCol->GetGameObject()->GetLayer()) {
		m_bTouchGround = true;
	}
}

void CGroundCheck_bu::OnCollisionExit2D(CCollider2D* _pCol)
{
	UINT iLayer = (UINT)E_Layer::Wall;
	if (iLayer == _pCol->GetGameObject()->GetLayer()) {
		m_bTouchGround = false;
	}
}