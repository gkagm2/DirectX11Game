#include "pch.h"
#include "CResource.h"

CResource::CResource(E_ResourceType _eType) :
	m_iRefCount(0),
	m_eResourceType(_eType)
{
}

CResource::~CResource()
{
}