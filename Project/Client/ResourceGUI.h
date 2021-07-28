#pragma once
#include "GUI.h"
class CResource;
class ResourceGUI : public GUI
{
private:
	CResource* m_pTargetResource;

public:
	void SetTargetResource(CResource* _pTargetResource) { m_pTargetResource = _pTargetResource; }
public:
	ResourceGUI();
	virtual ~ResourceGUI() override;
};