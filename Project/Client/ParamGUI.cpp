#include "pch.h"
#include "ParamGUI.h"

#include "imgui.h"
#include "CImGuiManager.h"

#include <Engine\CFontManager.h>

#include <Engine\CTexture.h>
#include <Engine\CResourceManager.h>
ParamGUI::ParamGUI()
{
}

ParamGUI::~ParamGUI()
{
}

bool ParamGUI::Render_Int(const string& _strName, int* _pOut)
{
	// ##기준으로 나눔
	string strText = _strName.substr(0, _strName.find("##"));
	ImGui::Text(strText.c_str());

	string strLabel = "##";
	strLabel += strText;
	if (ImGui::InputInt(strLabel.c_str(), _pOut, 1))
		return true;
	return false;
}

bool ParamGUI::Render_Float(const string& _strName, float* _pOut)
{
	// ##기준으로 나눔
	string strText = _strName.substr(0, _strName.find("##"));
	ImGui::Text(strText.c_str());

	string strLabel = "##";
	strLabel += strText;
	if (ImGui::InputFloat(strLabel.c_str(), _pOut, 1))
		return true;
	return false;
}

bool ParamGUI::Render_Vector2(const string& _strName, Vector2* _pOut)
{
	string strText = _strName.substr(0, _strName.find("##"));
	ImGui::Text(strText.c_str());

	string strLabel = "##";
	strLabel += strText;
	if (ImGui::InputFloat2(strLabel.c_str(), (float*)_pOut, "%.2f"))
		return true;
	return false;
}

bool ParamGUI::Render_Vector4(const string& _strName, Vector4* _pOut)
{
	string strText = _strName.substr(0, _strName.find("##"));
	ImGui::Text(strText.c_str());

	string strLabel = "##";
	strLabel += strText;
	if (ImGui::InputFloat4(strLabel.c_str(), (float*)_pOut, "%.2f"))
		return true;
	return false;
}

// ButtonOn을 false로 할 경우 pInst와 pFunc는 nullptr로 한다.
// texture에 대해 상세한 설정 필요 시 _tTextureInfo를 이용하여 설정 가능
bool ParamGUI::Render_Texture(const string& _strName, CTexture* _pTex, GUI* pInst, GUI_CALLBACK _pFunc, bool _bIsButtonOn, TTextureInfo _tTextureInfo)
{
	string strText = _strName.substr(0, _strName.find("##"));
	if (0 != strText.size())
		ImGui::Text(strText.c_str());

	ListViewGUI* pListGUI = (ListViewGUI*)CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView);

	ImTextureID tex_id = 0;
	if (nullptr != _pTex)
		tex_id = (ImTextureID)(_pTex->GetSRV().Get());

	ImGui::Image(tex_id, _tTextureInfo.vImageSize, _tTextureInfo.uv_min, _tTextureInfo.uv_max, _tTextureInfo.tint_col, _tTextureInfo.border_col);
	ImGui::SameLine();

	string strLabel = "Select##";
	strLabel += _strName;

	tstring titleName = _T("Texture");

	ImGui::PushID(CImGuiManager::GetInstance()->GetWidgetID());
	if (_bIsButtonOn) {
		if (ImGui::Button(strLabel.c_str())) {
			// 목록 전달
			vector<tstring> vecName;
			CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Texture, vecName);
			pListGUI->SetList(vecName, titleName);
			pListGUI->SetDoubleClickCallBack(pInst, (GUI_CALLBACK)_pFunc);
			pListGUI->SetActive(true);
			ImGui::PopID();
			return true;
		}
	}
	ImGui::PopID();

	if (!_bIsButtonOn)
		return true;
	
	return false;
}

bool ParamGUI::Render_TextureBtn(const string& _strName, CTexture* _pTex, TTextureBtnInfo _tTexBtnInfo) {
	string strText = _strName.substr(0, _strName.find("##"));
	if (0 != strText.size())
		ImGui::Text(strText.c_str());

	ImTextureID tex_id = 0;
	if (nullptr != _pTex)
		tex_id = (ImTextureID)(_pTex->GetSRV().Get());

	ImGui::PushID(CImGuiManager::GetInstance()->GetWidgetID());
	if (
		ImGui::ImageButton(tex_id, _tTexBtnInfo.vImageSize, _tTexBtnInfo.uv_min, _tTexBtnInfo.uv_max, _tTexBtnInfo.iFramePadding, _tTexBtnInfo.bg_col, _tTexBtnInfo.tint_col)) {
		ImGui::SameLine();
		ImGui::PopID();
		return true;
	}
	ImGui::PopID();
	return false;
}


bool ParamGUI::Render_Matrix(const string& _strName, Matrix* _pOut)
{
	string strText = _strName.substr(0, _strName.find("##"));
	ImGui::Text(strText.c_str());

	string strLabel = "##";
	strLabel += strText;
	
	float _11to14[] = { _pOut->_11, _pOut->_12, _pOut->_13, _pOut->_14 };
	float _21to24[] = { _pOut->_21, _pOut->_22, _pOut->_23, _pOut->_24 };
	float _31to34[] = { _pOut->_31, _pOut->_32, _pOut->_33, _pOut->_34 };
	float _41to44[] = { _pOut->_41, _pOut->_42, _pOut->_43, _pOut->_44 };

	bool bFixed = false;
	string str11to14 = strLabel + "11to14";
	if (ImGui::InputFloat4(str11to14.c_str(), (float*)_pOut, "%.2f")) {
		_pOut->_11 = _11to14[0];
		_pOut->_12 = _11to14[1];
		_pOut->_13 = _11to14[2];
		_pOut->_14 = _11to14[3];
		bFixed = true;
	}

	string str21to24 = strLabel + "21to24";
	if (ImGui::InputFloat4(str21to24.c_str(), (float*)_pOut, "%.2f")) {
		_pOut->_21 = _21to24[0];
		_pOut->_22 = _21to24[1];
		_pOut->_23 = _21to24[2];
		_pOut->_24 = _21to24[3];
		bFixed = true;
	}

	string str31to34 = strLabel + "31to34";
	if (ImGui::InputFloat4(str31to34.c_str(), (float*)_pOut, "%.2f")) {
		_pOut->_31 = _31to34[0];
		_pOut->_32 = _31to34[1];
		_pOut->_33 = _31to34[2];
		_pOut->_34 = _31to34[3];
		bFixed = true;
	}

	string str41to44 = strLabel + "41to44";
	if (ImGui::InputFloat4(str41to44.c_str(), (float*)_pOut, "%.2f")) {
		_pOut->_41 = _41to44[0];
		_pOut->_42 = _41to44[1];
		_pOut->_43 = _41to44[2];
		_pOut->_44 = _41to44[3];
		bFixed = true;
	}
	return bFixed;
}

bool ParamGUI::Render_Color(const string& _strName, UINT* _iColorInOut)
{
	string strText = _strName.substr(0, _strName.find("##"));
	ImGui::Text(strText.c_str());

	string strLabel = "##";
	strLabel += strText;

	UINT iColor = *_iColorInOut;
	Vector4 colf = ChangeColorUintToVector4(iColor);

	bool bIsFixed = false;
	ImGui::PushID(CImGuiManager::GetInstance()->GetWidgetID());
	if (ImGui::ColorEdit4(_strName.c_str(), &colf.x, ImGuiColorEditFlags_InputRGB)) {
		iColor = ChangeColorVector4ToUint(&colf.x);
		bIsFixed = true;
		*_iColorInOut = iColor;
	}
	ImGui::PopID();
	
	return bIsFixed;
}

void ParamGUI::Make_ComboBoxList(const vector<string>& _inStrList, vector<char>& _outStrList)
{
	for (UINT i = 0; i < _inStrList.size(); ++i) {
		for (UINT j = 0; j < _inStrList[i].size(); ++j)
			_outStrList.push_back(_inStrList[i][j]);
		_outStrList.push_back('\0');
	}
	_outStrList.push_back('\0');
}

bool ParamGUI::Render_ComboBox(const string& _strName, int* _piCurItem, const vector<char>& _strList)
{
	if (ImGui::Combo(_strName.c_str(), (&(*_piCurItem)), _strList.data(), _strList.size()))
		return true;
	return false;
}