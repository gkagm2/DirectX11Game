#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(E_ComponentType _eType) :
	m_pGameObj(nullptr),
	m_eType{ _eType }
{
}

CComponent::~CComponent()
{
}
