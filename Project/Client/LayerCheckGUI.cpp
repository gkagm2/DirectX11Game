#include "LayerCheckGUI.h"
#include "pch.h"
#include "CameraGUI.h"
#include "ParamGUI.h"
// LayerCheck

LayerCheckGUI::LayerCheckGUI() :
	m_pCamGUI(nullptr),
	m_pCam(nullptr)
{
}

LayerCheckGUI::~LayerCheckGUI()
{
}

void LayerCheckGUI::Init()
{
	SetName(STR_GUI_LayerCheck);
}

void LayerCheckGUI::Update()
{
	if (!(m_pCamGUI && m_pCam))
		return;

	// â�� �ϳ� ����
	if (!m_bGUIOpen) {
		return;
	}

	// GUI
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Module Creator", &m_bGUIOpen, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu(""))
			{
				if (ImGui::MenuItem("Close")) {
					SetActive(false);
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		// TODO (Jang) : üũ�ڽ��� ����� �ȵ�.
		static int layer = 0;
		static bool checkLayer = false;
		bool bIsTouched = false;
		bool bLayerCheck[MAX_SIZE_LAYER] = {};
		for (int i = 0; i < MAX_SIZE_LAYER; ++i) {
			bLayerCheck[i]= m_pCam->IsLayerChecked(i);
			ImGui::Text("%d", i);
			ImGui::SameLine();
			if (ImGui::Checkbox("##Camera Layer Check", &bLayerCheck[i])) {
				bIsTouched = true;
				layer = i;
				checkLayer = bLayerCheck[i];
			}
		}
		if (bIsTouched) {
			m_pCam->SetLayerCheck(layer, checkLayer);
		}

	}
	ImGui::End();
}