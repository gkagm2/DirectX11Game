#include "pch.h"
#include "ResourceGUI.h"

ResourceGUI::ResourceGUI(E_ResourceType _eType) :
	m_pTargetResource(nullptr),
	m_eResourceType(_eType)
{
}

ResourceGUI::~ResourceGUI()
{
}

void ResourceGUI::SetTargetResource(CResource* _pTargetResource)
{
	m_pTargetResource = _pTargetResource;
	assert(_pTargetResource->GetResourceType() == m_eResourceType);
}

bool ResourceGUI::Start()
{
	if (nullptr == m_pTargetResource)
		return false;

	if (!IsActive())
		return false;

	ImGui::BeginChild(GetName().c_str(), GetUISize());

	string strResourceName;
	WStringToString(ResourceTypeToStr(m_eResourceType), strResourceName);
	SetTitileButton((int)m_eResourceType, strResourceName);

	return true;
}