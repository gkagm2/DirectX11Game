#include "pch.h"
#include "CDraggable_bu.h"

CDraggable_bu::CDraggable_bu() :
	CScript((UINT)SCRIPT_TYPE::DRAGGABLE_BU)
{
}

CDraggable_bu::~CDraggable_bu()
{
}

void CDraggable_bu::OnCollisionStay2D(CCollider2D* _pOther)
{

	m_bisMouseIn = true;
}