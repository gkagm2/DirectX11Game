#pragma once
#include "pch.h"
#include "ComponentGUI.h"
#include <Engine\CGameObject.h>
#include <Engine\CComponent.h>
#include "CImGuiManager.h"

ComponentGUI::ComponentGUI(E_ComponentType _eType) :
	m_eComponentType(_eType),
	m_pTargetObj(nullptr),
	m_bDeleteEnable(true)
{
	string strType;
	TStringToString(ComponentTypeToStr(_eType), strType);
	SetName(strType);
}

ComponentGUI::~ComponentGUI()
{
}

bool ComponentGUI::Start()
{
	if (nullptr == m_pTargetObj)
		return false;
		
	if (m_pTargetObj->IsDead()) {
		m_pTargetObj = nullptr;
		return false;
	}

	// 타겟의 어떤 컴포넌트가 없으면 active false로 하고 종료
	if (nullptr == m_pTargetObj->GetComponent(m_eComponentType))
		return false;
	
	if (!IsActive())
		return false;

	ImGui::BeginChild(GetName().c_str(), GetUISize());

	string strComponentName;
	WStringToString(ComponentTypeToStr(m_eComponentType), strComponentName);
	SetTitileButton((int)m_eComponentType, strComponentName);


	// Delete Component Button
	if (m_bDeleteEnable) {
		ImGui::SameLine();
		if (ImGui::Button("Del")) {
			CComponent* pComponent = m_pTargetObj->GetComponent(m_eComponentType);
			CObject::DestroyObjectEvn(pComponent);
		}
	}

	ImGui::SameLine();
	CComponent* pComponent = m_pTargetObj->GetComponent(m_eComponentType);
	bool bActive = pComponent->IsActive();
	ImGui::PushID(CImGuiManager::GetInstance()->GetWidgetID());
	if (ImGui::Checkbox("Active", &bActive))
		pComponent->SetActive(bActive);
	ImGui::PopID();
	


	return true;
}