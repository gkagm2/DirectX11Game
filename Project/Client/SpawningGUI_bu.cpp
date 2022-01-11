#include "pch.h"
#include "SpawningGUI_bu.h"
#include <Engine\CKeyManager.h>
#include <Engine\CCamera.h>
#include <Engine\CRenderManager.h>
#include <Engine\CCamera.h>
#include "ParamGUI.h"

SpawningGUI_bu::SpawningGUI_bu()
{
}

SpawningGUI_bu::~SpawningGUI_bu()
{
}


void SpawningGUI_bu::Init()
{
	SetName(STR_GUI_Spawning_bu);

	m_CategoryList.push_back("objects");
	m_CategoryList.push_back("actors");
	m_CategoryList.push_back("pickups");
	m_CategoryList.push_back("doorAnd");

	// TODO(Jang ) : ÇØ¾ßµÊ
	m_CategoryList.push_back("corpses");
	m_CategoryList.push_back("boss");

	SetActive(false);
}

void SpawningGUI_bu::Update()
{
	if (!m_bGUIOpen) {
		return;
	}
	if (ImGui::Begin(STR_GUI_Spawning_bu, &m_bGUIOpen)) {
		if (!m_bGUIOpen) {
			_Clear();
			ImGui::End();
			return;
		}

		CCamera* pMainCam = CRenderManager::GetInstance()->GetMainCamera();
		if (E_ProjectionType::Perspective == pMainCam->GetProjectionType()) {
			pMainCam->SetProjectionType(E_ProjectionType::Orthographic);
		}
		Vector2 vMousePos = MousePosition;
		Vector3 vWorldPos = pMainCam->GetScreenToWorld2DPosition(vMousePos);
		vWorldPos.z = 0.f;

		ImGui::Text("Ctrl : Create/Clone Spawn points");
		ImGui::Text("Alt : Delete Spawn point");
		bool bCreate = false;
		bool bDelete = false;

		
		ParamGUI::Render_ComboBox("Category", &m_iCurCategoryItemIdx, m_CategoryList);

		// Create
		if (InputKeyHold(E_Key::LCtrl) && InputKeyPress(E_Key::LBUTTON)) {
			bCreate = true;
		}

		// Delete
		else if (InputKeyHold(E_Key::LAlt) && InputKeyPress(E_Key::LBUTTON)) {
			bDelete = true;
		}

		if (bCreate) {
		}

		if (bDelete) {

		}

		ImGui::End();
	}
}

void SpawningGUI_bu::_Clear()
{
}