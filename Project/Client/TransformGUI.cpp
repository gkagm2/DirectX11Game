#include "pch.h"
#include "TransformGUI.h"

#include <Engine\CTransform.h>

TransformGUI::TransformGUI() :
	ComponentGUI(E_ComponentType::Transform)
{
}

TransformGUI::~TransformGUI()
{
}

void TransformGUI::Update()
{
	if (false == Start())
		return;

	CTransform* pTransform = GetTargetObject()->Transform();

	Vector3 vTrans = pTransform->GetLocalPosition();
	Vector3 vScale = pTransform->GetLocalScale();
	Vector3 vRotation = pTransform->GetLocalRotation();

	static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
	// Position UI
	
	ImGui::Text("Position "); ImGui::SameLine();
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##PositionX", &vTrans.x, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##PositionY", &vTrans.y, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
	ImGui::Text("z"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##PositionZ", &vTrans.z, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags);

	// Scale UI
	ImGui::Text("Scale    "); ImGui::SameLine();
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##ScaleX", &vScale.x, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##ScaleY", &vScale.y, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
	ImGui::Text("z"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##ScaleZ", &vScale.z, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags);

	// Rotation UI
	vRotation = vRotation * CMyMath::Rad2Deg();
	ImGui::Text("Rotation "); ImGui::SameLine();
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##RotationX", &vRotation.x, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##RotationY", &vRotation.y, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
	ImGui::Text("z"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##RotationZ", &vRotation.z, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags);
	vRotation = vRotation * CMyMath::Deg2Rad();

	pTransform->SetLocalPosition(vTrans);
	pTransform->SetLocalScale(vScale);
	pTransform->SetLocalRotation(vRotation);

	End();
}