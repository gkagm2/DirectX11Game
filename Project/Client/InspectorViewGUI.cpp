#include "pch.h"
#include "InspectorViewGUI.h"

bool InspectorViewGUI::Start()
{
	return false;
}

InspectorViewGUI::InspectorViewGUI() :
	m_vSize{0.f,0.f}
{
}

InspectorViewGUI::~InspectorViewGUI()
{
}

void InspectorViewGUI::SetTitileButton(int _pushId, const string& _strBtnName)
{
	ImGui::PushID(_pushId);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 7.6f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.4f, 7.6f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.8f, 7.6f, 0.8f));
	ImGui::Button(_strBtnName.c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();
}