#include "pch.h"
#include "ToolCameraGUI.h"
#include <Engine\CSceneManager.h>
#include <Engine\CRenderManager.h>
#include <Engine\CTransform.h>
#include "ParamGUI.h"

ToolCameraGUI::ToolCameraGUI() :
	m_pTargetObject(nullptr),
	m_flags(ImGuiSliderFlags_None)
{
}

ToolCameraGUI::~ToolCameraGUI()
{
}

void ToolCameraGUI::Init()
{
	SetName(STR_GUI_ToolCamera);
}

void ToolCameraGUI::Update()
{	
    // 창을 하나 연다
    if (!m_bGUIOpen)
        return;

    // GUI
    ImGui::SetNextWindowSize(ImVec2(400, 50), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(STR_GUI_ToolCamera, &m_bGUIOpen, ImGuiWindowFlags_MenuBar))
    {
        CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();

		// Position UI
		string strObjName;
		TStringToString(pToolCam->GetGameObject()->GetName(), strObjName);
		ImGui::Text("%s", strObjName.c_str());

		CTransform* pTransform = pToolCam->Transform();
		PrintPosition(pTransform);


		CCamera* pToolUICam = CRenderManager::GetInstance()->GetToolUICamera();
		TStringToString(pToolCam->GetGameObject()->GetName(), strObjName);
		ImGui::Text("%s", strObjName.c_str());

		pTransform = pToolUICam->Transform();
		PrintPosition(pTransform);


		/*Vector3 vSale = pTransform->GetLocalScale();
		Vector3 vRotation = pTransform->GetLocalRotation();*/

		//// Scale UI
		//ImGui::Text("Scale    "); ImGui::SameLine();
		//ImGui::Text("x"); ImGui::SameLine();
		//ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
		//ImGui::DragFloat("##ScaleX", &vScale.x, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
		//ImGui::Text("y"); ImGui::SameLine();
		//ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
		//ImGui::DragFloat("##ScaleY", &vScale.y, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
		//ImGui::Text("z"); ImGui::SameLine();
		//ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
		//ImGui::DragFloat("##ScaleZ", &vScale.z, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", flags);

		//// Rotation UI
		//vRotation = vRotation * CMyMath::Rad2Deg();
		//ImGui::Text("Rotation "); ImGui::SameLine();
		//ImGui::Text("x"); ImGui::SameLine();
		//ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
		//ImGui::DragFloat("##RotationX", &vRotation.x, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
		//ImGui::Text("y"); ImGui::SameLine();
		//ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
		//ImGui::DragFloat("##RotationY", &vRotation.y, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags); ImGui::SameLine();
		//ImGui::Text("z"); ImGui::SameLine();
		//ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
		//ImGui::DragFloat("##RotationZ", &vRotation.z, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags);
		//vRotation = vRotation * CMyMath::Deg2Rad();

	/*	pTransform->SetLocalScale(vScale);
		pTransform->SetLocalRotation(vRotation);*/

    }
    ImGui::End();
}

void ToolCameraGUI::PrintPosition(CTransform* pTransform)
{
	Vector3 vTrans = pTransform->GetLocalPosition();
	string strLabel;
	string strObjName;
	TStringToString(pTransform->GetGameObject()->GetName(), strObjName);

	ImGui::Text("Position "); ImGui::SameLine();
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	
	strLabel = "##PositionX" + strObjName;
	ImGui::DragFloat(strLabel.c_str(), &vTrans.x, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", m_flags); ImGui::SameLine();
	ImGui::Text("y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	strLabel = "##PositionY" + strObjName;
	ImGui::DragFloat(strLabel.c_str(), &vTrans.y, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", m_flags); ImGui::SameLine();
	ImGui::Text("z"); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	strLabel = "##PositionZ" + strObjName;
	ImGui::DragFloat(strLabel.c_str(), &vTrans.z, 0.1f, -FLT_MAX, +FLT_MAX, "%.2f", m_flags);

	pTransform->SetLocalPosition(vTrans);
}
