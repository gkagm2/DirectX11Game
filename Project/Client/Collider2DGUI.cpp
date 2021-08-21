#include "pch.h"
#include "Collider2DGUI.h"
#include <Engine\CCollider2D.h>

Collider2DGUI::Collider2DGUI() :
	ComponentGUI(E_ComponentType::Collider2D)
{
	string strName = "Rect";
	string strType;
	TStringToString(ComponentTypeToStr(E_ComponentType::Collider2D), strType);
	strName = strName + strType;
	SetName(strName);
}

Collider2DGUI::~Collider2DGUI()
{
}

void Collider2DGUI::Update()
{
	if (false == Start())
		return;

	CCollider2D* pCol = GetTargetObject()->Collider2D();
	assert(pCol);

	Vector2 vOffPosition = pCol->GetOffsetPosition();
	Vector2 vOffScale = pCol->GetOffsetScale();

	static ImGuiSliderFlags flags = ImGuiSliderFlags_None;

	// Offset Position
	ImGui::Text("Offset Position"); ImGui::SameLine();
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##RectColPositionX", &vOffPosition.x, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##RectColPositionY", &vOffPosition.y, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags);

	// Offset Scale UI
	ImGui::Text("Offset Scale   "); ImGui::SameLine();
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##RectColScaleX", &vOffScale.x, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##RectColScaleY", &vOffScale.y, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags);

	pCol->SetOffsetPosition(vOffPosition);
	pCol->SetOffsetScale(vOffScale);

	End();
}