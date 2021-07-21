#include "pch.h"
#include "TransformGUI.h"

TransformGUI::TransformGUI()
{
	SetName("Transform"); 
}

TransformGUI::~TransformGUI()
{
}

void TransformGUI::Update()
{
	ComponentGUI::Update();
	if (nullptr == GetTargetGameObject())
		return;

	ImGui::Begin(GetName().c_str(), &m_bGUIOpen);

	static float vec4[4] = { 0.1f, 0.2f, 0.3f, 0.4f };
	ImGui::InputFloat3(u8"Postion", vec4);

	ImGui::End();
}