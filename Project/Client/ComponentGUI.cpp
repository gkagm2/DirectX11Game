#include "pch.h"
#include "ComponentGUI.h"
#include <Engine\CGameObject.h>

ComponentGUI::ComponentGUI(E_ComponentType _eType) :
	m_eComponentType(_eType),
	m_pTargetObj(nullptr)
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

	ImGui::SameLine();

	// Delete Component Button
	if (ImGui::Button("Del")) {
		//CObject::DestroyGameObjectEvn()
	}

	return true;
}