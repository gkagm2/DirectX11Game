#include "pch.h"
#include "CCollider.h"
#include "CResourceManager.h"

CCollider::CCollider(E_ComponentType _eComponentType) :
	CComponent(_eComponentType),
	m_iCollisionCount(0)
{
	m_pCollisionMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_Collider2DCollisionMtrl);

	m_pNonColliedMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_Collider2DNoneColliedMtrl);

	assert(m_pCollisionMaterial);
	assert(m_pNonColliedMaterial);
}

CCollider::CCollider(const CCollider& _origin) :
	CComponent(_origin),
	m_iCollisionCount(0)
{
	m_pCollisionMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_Collider2DCollisionMtrl);

	m_pNonColliedMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_Collider2DNoneColliedMtrl);

	assert(m_pCollisionMaterial);
	assert(m_pNonColliedMaterial);
}

CCollider::~CCollider()
{
}