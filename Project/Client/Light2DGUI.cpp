#include "pch.h"
#include "Light2DGUI.h"
#include <Engine\CLight2D.h>

Light2DGUI::Light2DGUI() :
	LightGUI(E_ComponentType::Light2D)
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
	CLight2D* pLight = pTargetObj->Light2D();

	TLightInfo& tLightInfoRef = pLight->GetLightInfoRef();
	
	ImGui::InputFloat("Angle##Light2D ", &tLightInfoRef.fAngle, 0.f, 0.f, "%.2f");
	ImGui::InputFloat("Range##Light2D ", &tLightInfoRef.fRange, 0.f, 0.f, "%.2f");

	// lightType 
	string strLightType;
	TStringToString(LightTypeToStr(tLightInfoRef.eLightType), strLightType);
	// Type을 String으로 표시
	static int iCurLightType = (int)tLightInfoRef.eLightType;
	ImGui::Combo("Light Type", &iCurLightType, m_vecStrLigthTypeList.data(), m_vecStrLigthTypeList.size());
	tLightInfoRef.eLightType = (E_LightType)iCurLightType;

	/*Vector4* pDir = &tLightInfoRef.vLightDir;
	ImGui::InputFloat4("Light Direction ##Light2D", *pDir, "%.2f");*/

	ImGui::InputFloat4("Light Direction ##Light2D", tLightInfoRef.vLightDir, "%.2f");
	ImGui::InputFloat4("Light Position  ##Light2D", tLightInfoRef.vLightPos, "%.2f");

	ImGui::InputFloat4("Diffuse##Light2D  ", tLightInfoRef.tColor.vDiffuse, "%.2f");
	ImGui::InputFloat4("Embient##Light2D  ", tLightInfoRef.tColor.vEmbient, "%.2f");
	ImGui::InputFloat4("Specular##Light2D ", tLightInfoRef.tColor.vSpecular, "%.2f");

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