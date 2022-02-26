#include "pch.h"
#include "Light3DGUI.h"
#include <Engine\CLight3D.h>
#include "ParamGUI.h"
#include "CImGuiManager.h"
Light3DGUI::Light3DGUI() :
	LightGUI(E_ComponentType::Light3D),
	m_pLight(nullptr)
{
}

Light3DGUI::~Light3DGUI()
{
}

void Light3DGUI::Update()
{
	if (false == Start())
		return;

	CGameObject* pTargetObj = GetTargetObject();
	m_pLight = pTargetObj->Light3D();

	TLightInfo& tLightInfoRef = m_pLight->GetLightInfoRef();


	// lightType 
	string strLightType;
	TStringToString(LightTypeToStr(tLightInfoRef.eLightType), strLightType);
	// Type을 String으로 표시
	int iCurLightType = (int)tLightInfoRef.eLightType;
	ImGui::PushID(CImGuiManager::GetInstance()->GetWidgetID());
	if (ImGui::Combo("Light Type", &iCurLightType, m_vecStrLigthTypeList.data(), (int)m_vecStrLigthTypeList.size())) {
		m_pLight->SetLightType((E_LightType)iCurLightType);
	}
	ImGui::PopID();
	

	switch (tLightInfoRef.eLightType) {
	case E_LightType::Direction:
		_RenderDirection(tLightInfoRef);
		break;
	case E_LightType::Point:
		_RenderPoint(tLightInfoRef);
		break;
	case E_LightType::Spot:
		_RenderSpot(tLightInfoRef);
		break;
	default:
		assert(nullptr && _T("light 타입 부족"));
		break;
	}

	ParamGUI::Render_Color("Diffuse##Light3D", &tLightInfoRef.tColor.vDiffuse);
	ParamGUI::Render_Color("Embient##Light3D", &tLightInfoRef.tColor.vEmbient);
	ParamGUI::Render_Color("Specular##Light3D", &tLightInfoRef.tColor.vSpecular);

	if (ImGui::Button("Material Change##Light3D")) {
		//// 리스트 뷰를 보여준다.
		//ListViewGUI* pListViewGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
		//assert(pListViewGUI);

		//vector<tstring> vecNames;
		//CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Material, vecNames);
		//pListViewGUI->SetList(vecNames, ResourceTypeToStr(E_ResourceType::Material));
		//pListViewGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&MeshRendererGUI::_SetMatrial);
		//pListViewGUI->SetActive(true);
	}

	End();
}

void Light3DGUI::_InitComboBoxList()
{
	string strState;
	for (UINT i = 0; i < (UINT)E_LightType::End; ++i) {
		TStringToString(LightTypeToStr((E_LightType)i), strState);
		for (UINT j = 0; j < strState.size(); ++j)
			m_vecStrLigthTypeList.push_back(strState[j]);
		m_vecStrLigthTypeList.push_back('\0');
	}
	m_vecStrLigthTypeList.push_back('\0');
}

void Light3DGUI::_RenderDirection(TLightInfo& _tLightInfoRef)
{
}

void Light3DGUI::_RenderSpot(TLightInfo& _tLightInfoRef)
{
	float fDegree = m_pLight->GetAngle();
	ImGui::DragFloat("Degree##Light3D ", &fDegree, 0.1f, FLOAT_MIN, FLOAT_MAX, "%.2f");
	m_pLight->SetAngle(fDegree);

	_RenderParam_Range(_tLightInfoRef);
}

void Light3DGUI::_RenderPoint(TLightInfo& _tLightInfoRef)
{
	_RenderParam_Range(_tLightInfoRef);
}

void Light3DGUI::_RenderParam_Range(TLightInfo& _tLightInfoRef)
{
	ImGui::DragFloat("Range##Light3D ", &_tLightInfoRef.fRange, 0.2f, 0.f, FLOAT_MAX, "%.2f");
}

void Light3DGUI::_RenderParam_LightColor(TLightInfo& _tLightInfoRef)
{
}
