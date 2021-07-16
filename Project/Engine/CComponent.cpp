#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(E_ComponentType _eType) :
	m_pGameObj(nullptr),
	m_eType{ _eType }
{
}

CComponent::CComponent(const CComponent& _origin) :
	CObject(_origin),
	m_pGameObj(nullptr),
	m_eType(_origin.m_eType)
{
}

CComponent::~CComponent()
{
}

// TODO (Jang) : Component
bool CComponent::SaveToScene(FILE* _pFile)
{
	return CObject::SaveToScene(_pFile);
}

bool CComponent::LoadFromScene(FILE* _pFile)
{
	return CObject::LoadFromScene(_pFile);
}