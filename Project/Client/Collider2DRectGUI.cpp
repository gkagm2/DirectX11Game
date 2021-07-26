#include "pch.h"
#include "Collider2DRectGUI.h"
#include <Engine\CCollider2DRect.h>

Collider2DRectGUI::Collider2DRectGUI() :
	ComponentGUI(E_ComponentType::Collider2D)
{
	string strName = "Rect";
	string strType;
	TStringToString(ComponentTypeToStr(E_ComponentType::Collider2D), strType);
	strName = strName + strType;
	SetName(strName);
}

Collider2DRectGUI::~Collider2DRectGUI()
{
}

void Collider2DRectGUI::Update()
{
	if (false == Start())
		return;

	CCollider2DRect* pRectCol = (CCollider2DRect*)GetTargetObject()->Collider2D();
	assert(pRectCol);

	Vector2 vOffPosition = pRectCol->GetOffsetPosition();
	Vector2 vOffScale = pRectCol->GetOffsetScale();

	static ImGuiSliderFlags flags = ImGuiSliderFlags_None;

	// Offset Position
	ImGui::Text("Offset Position");
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##RectColPositionX", &vOffPosition.x, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##RectColPositionY", &vOffPosition.y, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags);

	// Offset Scale UI
	ImGui::Text("Offset Scale");
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##RectColScaleX", &vOffScale.x, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	ImGui::DragFloat("##RectColScaleY", &vOffScale.y, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();

	pRectCol->SetOffsetPosition(vOffPosition);
	pRectCol->SetOffsetScale(vOffScale);

	End();
}