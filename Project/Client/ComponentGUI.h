#pragma once
#include "GUI.h"
#include <Engine\CGameObject.h>
class ComponentGUI : public GUI
{
private:
	CGameObject* m_pTargetObj;
	E_ComponentType m_eComponentType;

	ImVec2 m_vSize;

public:
	// Update�� ���۰� ���� Start�� End() �Լ��� �־�� ��.
	virtual void Update() = 0;

protected:
	bool Start();
	
	void End() { 
		ImGui::Separator(); 
		ImGui::EndChild();
	}

public:
	void SetTargetObject(CGameObject* _pTargetObj) { m_pTargetObj = _pTargetObj; }
	CGameObject* GetTargetObject() { return m_pTargetObj; }
	void SetSize(const ImVec2& _vSize) { m_vSize = _vSize; }
	const ImVec2& GetSize() { return m_vSize; }

public:
	ComponentGUI(E_ComponentType _eType);
	virtual ~ComponentGUI() override;
};