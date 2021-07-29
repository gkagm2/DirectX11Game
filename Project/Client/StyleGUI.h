#pragma once
#include "pch.h"
#include "imgui.h"
#include "GUI.h"

class StyleGUI : public GUI
{
private:
	ImVec2 m_vSize; // UI Size

public:
	void SetUISize(const ImVec2& _vSize) { m_vSize = _vSize; }
	const ImVec2& GetUISize() { return m_vSize; }

protected:
	void SetTitileButton(int _iPushId, const string& _strBtnName);

public:
	StyleGUI();
	virtual ~StyleGUI() override;
};