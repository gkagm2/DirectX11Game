#pragma once
#include "GUI.h"
class CGameObject;
class ComponentGUI;
class InspectorGUI : public GUI
{
private:
	ComponentGUI* m_arrComGUI[(UINT)E_ComponentType::End];
	CGameObject* m_pTargetObject;

public:
	virtual void Init() override;
	virtual void Update() override;

public:
	InspectorGUI();
	virtual ~InspectorGUI() override;
};