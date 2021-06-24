#include "pch.h"
#include "CCollider.h"

CCollider::CCollider(E_ComponentType _eComponentType) :
	CComponent(_eComponentType),
	m_iCollisionCount(0)
{
}

CCollider::CCollider(const CCollider& _origin) :
	CComponent(_origin),
	m_iCollisionCount(0)
{
}

CCollider::~CCollider()
{
}