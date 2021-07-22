#include "pch.h"
#include "ComponentGUI.h"
#include <Engine\CGameObject.h>

ComponentGUI::ComponentGUI(E_ComponentType _eType) :
	m_eComponentType(_eType),
	m_pTargetObj(nullptr)
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

	ImGui::Begin(GetName().c_str(), &m_bGUIOpen);
	return true;
}