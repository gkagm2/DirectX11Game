#pragma once
#include "GUI.h"
#include <Engine\CGameObject.h>
class ComponentGUI : public GUI
{
private:
	CGameObject* m_pTargetObj;
	E_ComponentType m_eComponentType;

public:
	// Warning
	// Update�� ���۰� ���� Start�� End() �Լ��� �־�� ��.
	bool Start();
	virtual void Update() = 0;
	void End() { ImGui::End(); }

public:
	void SetTargetGameObject(CGameObject* _pTargetObj) { m_pTargetObj = _pTargetObj; }
	CGameObject* GetTargetObject() { return m_pTargetObj; }

public:
	ComponentGUI(E_ComponentType _eType);
	virtual ~ComponentGUI() override;
};