#include "pch.h"
#include "CameraGUI.h"
#include <Engine\CCamera.h>
#include <Engine\CGameObject.h>
#include "ParamGUI.h"
#include "LayerCheckGUI.h"
#include "CImGuiManager.h"

CameraGUI::CameraGUI() :
	ComponentGUI(E_ComponentType::Camera)
{
	_InitComboBox();
}

CameraGUI::~CameraGUI()
{
}

void CameraGUI::Update()
{
	if (false == Start())
		return;

	CCamera* pCam = GetTargetObject()->Camera();

	E_ProjectionType eCamType = pCam->GetProjectionType();

	pCam->SetProjectionType(eCamType);

	// 카메라 타입
	string strCameraType;
	TStringToString(CameraProjectionTypeToStr(eCamType), strCameraType);
	ImGui::Text("State : %s", strCameraType.c_str());

	// 콤보로 표현하기
	static int iCurItem = (int)eCamType;
	ParamGUI::Render_ComboBox("Camera Type", &iCurItem, m_strList);
	eCamType = (E_ProjectionType)iCurItem;

	pCam->SetProjectionType(eCamType);

	if(ImGui::Button("Layer Setting")) {
		LayerCheckGUI* pLayerCheck = (LayerCheckGUI*)CImGuiManager::GetInstance()->FindGUI(STR_GUI_LayerCheck);
		if (pLayerCheck) {
			pLayerCheck->SetCamera(pCam, this);
			pLayerCheck->SetActive(true);
		}
	}

	switch(eCamType) {
	case E_ProjectionType::Orthographic: {
		_ShowOrthographicUI();
	}
		break;
	case E_ProjectionType::Perspective: {
		_ShowPerspectiveUI();
	}
		break;
	default:
		assert(nullptr);
		break;
	}

	End();
}

void CameraGUI::_ShowPerspectiveUI()
{
	static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
	CCamera* pCam = GetTargetObject()->Camera();

	TFOVAxis tFovAxis = pCam->GetFOVAxis();

	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	if (ImGui::DragFloat("Horizontal##camFovAxis", &tFovAxis.fHorizontal, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags))
		pCam->SetFOVAxis(tFovAxis);

	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	if(ImGui::DragFloat("Vertical##camFovAxis", &tFovAxis.fVertical, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags))
		pCam->SetFOVAxis(tFovAxis);
}

void CameraGUI::_ShowOrthographicUI()
{
	static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
	CCamera* pCam = GetTargetObject()->Camera();

	float fOrthographicSize = pCam->GetSize();
	TClippingPlanes tClippingPlanes = pCam->GetClippingPlanes();
	TViewportRect tViewportRect = pCam->GetViewportRect();

	// Size
	ImGui::Text("Orthographic Size");
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	if (ImGui::DragFloat("Size##camOrthographicSize", &fOrthographicSize, 0.001f, -FLT_MAX, +FLT_MAX, "%.4f", flags)) {
		if (0.f == fOrthographicSize)
			fOrthographicSize = 0.001f;
		pCam->SetSize(fOrthographicSize);
	}
		
	// Clipping Planes
	ImGui::Text("Clipping Planes");
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	if (ImGui::DragFloat("Near##camClippingPlanes", &tClippingPlanes.fNear, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags))
		pCam->SetClippingPlanes(tClippingPlanes);

	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	if (ImGui::DragFloat("Far##camClippingPlanes", &tClippingPlanes.fFar, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags))
		pCam->SetClippingPlanes(tClippingPlanes);

	// Viewport Rect
	ImGui::Text("Viewport Rect");
	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	if (ImGui::DragFloat("X     ##camViewportRect", &tViewportRect.fX, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags))
		pCam->SetViewportRect(tViewportRect);

	ImGui::SameLine();

	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	if (ImGui::DragFloat("Y     ##camViewportRect", &tViewportRect.fY, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags))
		pCam->SetViewportRect(tViewportRect);


	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	if (ImGui::DragFloat("Width ##camClippingPlanes", &tViewportRect.fWidth, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags))
		pCam->SetViewportRect(tViewportRect);

	ImGui::SameLine();

	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5);
	if (ImGui::DragFloat("Height##camClippingPlanes", &tViewportRect.fHeight, 1.f, -FLT_MAX, +FLT_MAX, "%.2f", flags))
		pCam->SetViewportRect(tViewportRect);
}

void CameraGUI::_InitComboBox()
{
	// Combo Box의 리스트 생성
	string strState;
	for (UINT i = 0; i < PROJECTION_TYPE_COUNT; ++i) {
		TStringToString(CameraProjectionTypeToStr((E_ProjectionType)i), strState);
		m_strList.push_back(strState);
	}
}