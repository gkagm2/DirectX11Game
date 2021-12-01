#include "pch.h"
#include "CollisionEditorGUI.h"
#include <Engine\CCollisionManager.h>
#include "CImGuiManager.h"

CollisionEditorGUI::CollisionEditorGUI()
{
}

CollisionEditorGUI::~CollisionEditorGUI()
{
}

void CollisionEditorGUI::Init()
{
	SetName(STR_GUI_CollisionEditor);
	SetActive(false);
}

void CollisionEditorGUI::Update()
{
	if (!m_bGUIOpen)
		return;

	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
	if (ImGui::Begin(STR_GUI_CollisionEditor, &m_bGUIOpen)) {
		for (UINT iRow = 0; iRow < MAX_SIZE_LAYER; ++iRow) {
			for (UINT iCol = iRow; iCol < MAX_SIZE_LAYER; ++iCol) {
				bool b = CCollisionManager::GetInstance()->CheckCollisionLayer(iRow, iCol);
				ImGui::PushID(CImGuiManager::GetInstance()->GetWidgetID());
				if (ImGui::Checkbox("", &b))
					CCollisionManager::GetInstance()->SetOnOffCollision(iRow, iCol, b);
				ImGui::PopID();
				ImGui::SameLine();
			}
			ImGui::NewLine();
		}
	}
	ImGui::End();
}