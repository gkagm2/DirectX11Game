#include "pch.h"
#include "Light2DGUI.h"
#include <Engine\CLight2D.h>
#include "ParamGUI.h"

Light2DGUI::Light2DGUI() :
	LightGUI(E_ComponentType::Light2D),
	m_pLight(nullptr)
{
	_InitComboBoxList();
}

Light2DGUI::~Light2DGUI()
{
}

void Light2DGUI::Update()
{
	if (false == Start())
		return;

	CGameObject* pTargetObj = GetTargetObject();
	m_pLight = pTargetObj->Light2D();

	TLightInfo& tLightInfoRef = m_pLight->GetLightInfoRef();
	

	// lightType 
	string strLightType;
	TStringToString(LightTypeToStr(tLightInfoRef.eLightType), strLightType);
	// Type을 String으로 표시
	static int iCurLightType = (int)tLightInfoRef.eLightType;
	ImGui::Combo("Light Type", &iCurLightType, m_vecStrLigthTypeList.data(), (int)m_vecStrLigthTypeList.size());
	tLightInfoRef.eLightType = (E_LightType)iCurLightType;

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

	ParamGUI::Render_Color("Diffuse##Light2D", &tLightInfoRef.tColor.vDiffuse);
	ParamGUI::Render_Color("Embient##Light2D", &tLightInfoRef.tColor.vEmbient);
	ParamGUI::Render_Color("Specular##Light2D", &tLightInfoRef.tColor.vSpecular);

	End();
}

void Light2DGUI::_InitComboBoxList()
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

void Light2DGUI::_RenderDirection(TLightInfo& tLightInfoRef)
{
}

void Light2DGUI::_RenderSpot(TLightInfo& _tLightInfoRef)
{
	float fDegree = m_pLight->GetAngle();
	ImGui::DragFloat("Degree##Light2D ", &fDegree, 0.1f, FLOAT_MIN, FLOAT_MAX, "%.2f");
		m_pLight->SetAngle(fDegree);

	_RenderParam_Range(_tLightInfoRef);

}

void Light2DGUI::_RenderPoint(TLightInfo& _tLightInfoRef)
{
	_RenderParam_Range(_tLightInfoRef);
}

void Light2DGUI::_RenderParam_Range(TLightInfo& _tLightInfoRef)
{
	ImGui::DragFloat("Range##Light2D ", &_tLightInfoRef.fRange, 0.2f, 0.f, FLOAT_MAX, "%.2f");
}