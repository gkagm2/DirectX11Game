#pragma once
#include "GUI.h"

class CObject;
class InspectorViewGUI : public GUI
{
private:
	ImVec2 m_vSize; // UI Size

public:
	virtual void Update() = 0;
protected:
	// Update�� ���۰� ���� Start�� End() �Լ��� �־�� ��.
	virtual bool Start() = 0;
	void End() {
		ImGui::Separator();
		ImGui::EndChild();
	}

public:
	void SetUISize(const ImVec2& _vSize) { m_vSize = _vSize; }
	const ImVec2& GetUISize() { return m_vSize; }

protected:
	void SetTitileButton(int _iPushId, const string& _strBtnName);

public:
	InspectorViewGUI();
	virtual ~InspectorViewGUI() override;
};