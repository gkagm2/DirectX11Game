#include "pch.h"
#include "RectTransformGUI.h"
#include <Engine\CRectTransform.h>

RectTransformGUI::RectTransformGUI() :
	ComponentGUI(E_ComponentType::RectTransform)
{
}

RectTransformGUI::~RectTransformGUI()
{
}

void RectTransformGUI::Update()
{
	if (false == Start())
		return;

	CRectTransform* pRectTransform = GetTargetObject()->RectTransform();

	Vector3 vTrans = pRectTransform->GetLocalPosition();
	Vector3 vScale = pRectTransform->GetLocalScale();
	Vector3 vRotation = pRectTransform->GetLocalRotation();

	static ImGuiSliderFlags flags = ImGuiSliderFlags_None;

	// Width , Height UI
	float fWidth = pRectTransform->GetWidth();
	float fHeight = pRectTransform->GetHeight();

	ImGui::Text("Width    "); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##Width", &fWidth, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();

	ImGui::Text("Height   "); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##Height", &fHeight, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags); 

	pRectTransform->SetWidth(fWidth);
	pRectTransform->SetHeight(fHeight);

	ImGui::Spacing();

	// Position UI

	ImGui::Text("Position "); ImGui::SameLine();
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##PositionX", &vTrans.x, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##PositionY", &vTrans.y, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
	ImGui::Text("z"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##PositionZ", &vTrans.z, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags);

	// Scale UI
	ImGui::Text("Scale    "); ImGui::SameLine();
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##ScaleX", &vScale.x, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##ScaleY", &vScale.y, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
	ImGui::Text("z"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##ScaleZ", &vScale.z, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags);

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

	pRectTransform->SetLocalPosition(vTrans);
	pRectTransform->SetLocalScale(vScale);
	pRectTransform->SetLocalRotation(vRotation);

	End();
}