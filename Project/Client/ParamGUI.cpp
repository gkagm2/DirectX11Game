#include "pch.h"
#include "ParamGUI.h"

#include "imgui.h"

ParamGUI::ParamGUI()
{
}

ParamGUI::~ParamGUI()
{
}

void ParamGUI::Render_Int(const string& _strName, int* _pOut)
{
	// ##기준으로 나눔
	string strText = _strName.substr(0, _strName.find("##"));
	ImGui::Text(strText.c_str());

	string strLabel = "##";
	strLabel += strText;
	ImGui::InputInt(strLabel.c_str(), _pOut, 1);
}

void ParamGUI::Render_Float(const string& _strName, float* _pOut)
{
	// ##기준으로 나눔
	string strText = _strName.substr(0, _strName.find("##"));
	ImGui::Text(strText.c_str());

	string strLabel = "##";
	strLabel += strText;
	ImGui::InputFloat(strLabel.c_str(), _pOut, 1);
}