#include "pch.h"
#include "CCollider.h"

CCollider::CCollider(E_ComponentType _eComponentType) :
	CComponent(_eComponentType),
	m_pOwnerGameObj(nullptr),
	m_iCollisionCount(0)
{
}

CCollider::~CCollider()
{
}