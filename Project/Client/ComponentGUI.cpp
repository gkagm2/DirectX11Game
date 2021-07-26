#include "pch.h"
#include "ComponentGUI.h"
#include <Engine\CGameObject.h>

ComponentGUI::ComponentGUI(E_ComponentType _eType) :
	m_eComponentType(_eType),
	m_pTargetObj(nullptr),
	m_vSize{}
{
	string strType;
#ifdef UNICODE
	wstring str = ComponentTypeToStr(_eType);
	WStringToString(str, strType);
#elif
	strType = ComponentTypeToString(_eType);
#endif
	SetName(strType);
}

ComponentGUI::~ComponentGUI()
{
}

bool ComponentGUI::Start()
{
	if (m_pTargetObj && m_pTargetObj->IsDead()) {
		m_pTargetObj = nullptr;
		SetActive(false);
		return false;
	}

	if (nullptr == m_pTargetObj)
		return false;
		
	if (nullptr == m_pTargetObj->GetComponent(m_eComponentType)) {
		SetActive(false);
		return false;
	}
	if (!IsActive())
		return false;

	ImGui::BeginChild(GetName().c_str(), m_vSize);

	ImGui::PushID((int)m_eComponentType);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 7.6f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.4f, 7.6f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.8f, 7.6f, 0.8f));
	string strComponent;
	WStringToString(ComponentTypeToStr(m_eComponentType), strComponent);
	ImGui::Button(strComponent.c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	return true;
}