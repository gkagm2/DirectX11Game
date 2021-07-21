#include "pch.h"
#include "ComponentGUI.h"
#include <Engine\CGameObject.h>

ComponentGUI::ComponentGUI() :
	m_pTargetObj(nullptr)
{
	SetName("Component");
}

ComponentGUI::~ComponentGUI()
{
}

void ComponentGUI::Update()
{
	if (m_pTargetObj && m_pTargetObj->IsDead())
		m_pTargetObj = nullptr;
}