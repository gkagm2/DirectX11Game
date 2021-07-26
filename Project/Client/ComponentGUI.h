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
	// Warning
	// Update문 시작과 끝에 Start와 End() 함수가 있어야 함.
	bool Start();
	virtual void Update() = 0;
	void End() { ImGui::EndChild(); }

public:
	void SetTargetObject(CGameObject* _pTargetObj) { m_pTargetObj = _pTargetObj; }
	CGameObject* GetTargetObject() { return m_pTargetObj; }
	void SetSize(const ImVec2& _vSize) { m_vSize = _vSize; }
	const ImVec2& GetSize() { return m_vSize; }

public:
	ComponentGUI(E_ComponentType _eType);
	virtual ~ComponentGUI() override;
};