#include "pch.h"
#include "CCollider.h"
#include "CResourceManager.h"


CCollider::CCollider(E_ComponentType _eComponentType) :
	CComponent(_eComponentType),
	m_iCollisionCount(0),
	m_bIsTrigger(false)
{
	m_pCollisionMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_Collider2DCollisionMtrl);

	m_pNonColliedMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_Collider2DNoneColliedMtrl);

	assert(m_pCollisionMaterial.Get());
	assert(m_pNonColliedMaterial.Get());
}

CCollider::CCollider(const CCollider& _origin) :
	CComponent(_origin),
	m_iCollisionCount(0),
	m_bIsTrigger(_origin.m_bIsTrigger)
{
	m_pCollisionMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_Collider2DCollisionMtrl);

	m_pNonColliedMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_Collider2DNoneColliedMtrl);

	assert(m_pCollisionMaterial.Get());
	assert(m_pNonColliedMaterial.Get());
}

CCollider::~CCollider()
{
}

bool CCollider::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
	FWrite(m_bIsTrigger, _pFile);

	return true;
}

bool CCollider::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);
	FRead(m_bIsTrigger, _pFile);

	return true;
}
