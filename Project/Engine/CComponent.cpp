#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(E_ComponentType _eType) :
	m_pGameObj(nullptr),
	m_eType{ _eType },
	m_bActive(true)
{
}

CComponent::CComponent() :
	m_bActive(true),
	m_eType(E_ComponentType::End),
	m_pGameObj(nullptr)
{
	assert(nullptr);
}

CComponent::CComponent(const CComponent& _origin) :
	CObject(_origin),
	m_pGameObj(nullptr),
	m_eType(_origin.m_eType),
	m_bActive(_origin.m_bActive)
{
}

CComponent::~CComponent()
{
}

// TODO (Jang) : Component
bool CComponent::SaveToScene(FILE* _pFile)
{
	FWrite(m_bActive, _pFile);
	return CObject::SaveToScene(_pFile);
}

bool CComponent::LoadFromScene(FILE* _pFile)
{
	FRead(m_bActive, _pFile);
	return CObject::LoadFromScene(_pFile);
}