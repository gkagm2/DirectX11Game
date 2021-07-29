#pragma once
#include "InspectorViewGUI.h"
class CResource;
class ResourceGUI : public InspectorViewGUI
{
private:
	CResource* m_pTargetResource;
	E_ResourceType m_eResourceType;

public:
	virtual bool Start() override;

public:
	void SetTargetResource(CResource* _pTargetResource);
	CResource* GetTargetResource() { return m_pTargetResource; }

public:
	ResourceGUI(E_ResourceType _eType);
	virtual ~ResourceGUI() override;
};