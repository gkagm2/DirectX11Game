#include "pch.h"
#include "ParamGUI.h"

#include "imgui.h"
#include "CImGuiManager.h"

#include <Engine\CFontManager.h>

#include <Engine\CTexture.h>
#include <Engine\CResourceManager.h>
#include "InspectorGUI.h"
ParamGUI::ParamGUI()
{
}

ParamGUI::~ParamGUI()
{
}

bool ParamGUI::Render_Int(const string& _strName, int* _pOut, float* _pHeightOut)
{
	float fHeight = 0.f;
	bool bChanged = false;
	// ##기준으로 나눔
	string strText = _strName.substr(0, _strName.find("##"));
	ImGui::Text(strText.c_str());
	fHeight += ImGui::GetItemRectSize().y;

	string strLabel = "##";
	strLabel += strText;
	if (ImGui::InputInt(strLabel.c_str(), _pOut, 1))
		bChanged = true;
	fHeight += ImGui::GetItemRectSize().y;
	if(_pHeightOut)
		*_pHeightOut = fHeight;
	return bChanged;
}

bool ParamGUI::Render_Float(const string& _strName, float* _pOut, float* _pHeightOut)
{
	float fHeight = 0.f;
	bool bChanged = false;
	// ##기준으로 나눔
	string strText = _strName.substr(0, _strName.find("##"));
	ImGui::Text(strText.c_str());
	fHeight += ImGui::GetItemRectSize().y;

	string strLabel = "##";
	strLabel += strText;
	if (ImGui::InputFloat(strLabel.c_str(), _pOut, 1))
		bChanged = true;
	fHeight += ImGui::GetItemRectSize().y;
	if (_pHeightOut)
		*_pHeightOut = fHeight;
	return bChanged;
}

bool ParamGUI::Render_Vector2(const string& _strName, Vector2* _pOut, float* _pHeightOut)
{
	float fHeight = 0.f;
	bool bChanged = false;
	string strText = _strName.substr(0, _strName.find("##"));
	ImGui::Text(strText.c_str());
	fHeight += ImGui::GetItemRectSize().y;

	string strLabel = "##";
	strLabel += strText;
	if (ImGui::InputFloat2(strLabel.c_str(), (float*)_pOut, "%.2f"))
		bChanged = true;
	fHeight += ImGui::GetItemRectSize().y;
	if (_pHeightOut)
		*_pHeightOut = fHeight;
	return bChanged;
}

bool ParamGUI::Render_Vector3(const string& _strName, Vector3* _pOut, float* _pHeightOut)
{
	float fHeight = 0.f;
	bool bChanged = false;
	string strText = _strName.substr(0, _strName.find("##"));
	ImGui::Text(strText.c_str());
	fHeight += ImGui::GetItemRectSize().y;

	string strLabel = "##";
	strLabel += strText;
	if (ImGui::InputFloat3(strLabel.c_str(), (float*)_pOut, "%.2f"))
		bChanged = true;
	fHeight += ImGui::GetItemRectSize().y;
	if (_pHeightOut)
		*_pHeightOut = fHeight;
	return bChanged;
}

bool ParamGUI::Render_Vector4(const string& _strName, Vector4* _pOut, float* _pHeightOut)
{
	float fHeight = 0.f;
	bool bChanged = false;
	string strText = _strName.substr(0, _strName.find("##"));
	ImGui::Text(strText.c_str());
	fHeight += ImGui::GetItemRectSize().y;

	string strLabel = "##";
	strLabel += strText;
	if (ImGui::InputFloat4(strLabel.c_str(), (float*)_pOut, "%.2f"))
		bChanged = true;
	fHeight += ImGui::GetItemRectSize().y;
	if (_pHeightOut)
		*_pHeightOut = fHeight;
	return bChanged;
}

// ButtonOn을 false로 할 경우 pInst와 pFunc는 nullptr로 한다.
// texture에 대해 상세한 설정 필요 시 _tTextureInfo를 이용하여 설정 가능
bool ParamGUI::Render_Texture(const string& _strName, CTexture* _pTex, GUI* pInst, GUI_CALLBACK _pFunc, bool _bIsButtonOn, TTextureInfo _tTextureInfo, float _fHeight)
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

	if (_bIsButtonOn) {
		bool bButtonClick = false;
		ImGui::PushID(CImGuiManager::GetInstance()->GetWidgetID());
		if (ImGui::Button(strLabel.c_str())) {
			// 목록 전달
			vector<tstring> vecName;
			CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Texture, vecName);
			pListGUI->SetList(vecName, titleName);
			pListGUI->SetDoubleClickCallBack(pInst, (GUI_CALLBACK)_pFunc);
			pListGUI->SetPreViewTexRender(true);
			pListGUI->SetActive(true);
			bButtonClick = true;
		}
		ImGui::PopID();

		if (bButtonClick)
			return true;
	}

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
	bool isClick = false;
	if (ImGui::ImageButton(tex_id, _tTexBtnInfo.vImageSize, _tTexBtnInfo.uv_min, _tTexBtnInfo.uv_max, _tTexBtnInfo.iFramePadding, _tTexBtnInfo.bg_col, _tTexBtnInfo.tint_col)) {
		ImGui::SameLine();
		isClick = true;
	}
	ImGui::PopID();
	return isClick;
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

bool ParamGUI::Render_Color(const string& _strName, Vector4* _pOut)
{
	string strText = _strName.substr(0, _strName.find("##"));
	ImGui::Text(strText.c_str());

	string strLabel = "##";
	strLabel += strText;

	bool bIsFixed = false;
	ImGui::PushID(CImGuiManager::GetInstance()->GetWidgetID());
	if (ImGui::ColorEdit4(_strName.c_str(), &(_pOut->x), ImGuiColorEditFlags_InputRGB)) {
		bIsFixed = true;
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
	if (ImGui::Combo(_strName.c_str(), (&(*_piCurItem)), _strList.data(), (int)_strList.size()))
		return true;
	return false;
}



// FIXED (Jang) : 팝업 형태로 안뜸. 
/*
    //사용 예시
	// context Menu setting
	vector<TContextInfo> _vecContextMenuList;
	TContextInfo tContextInfo;
	tContextInfo.callback_func = (GUI_CT_CALLBACK)&Animator2DEditorGUI::_DrawImageDeleteInResultQue;
	tContextInfo.pInst = this;
	tContextInfo.strName = "delete";
	_vecContextMenuList.push_back(tContextInfo);

	int iSelectNum = 0;
	ParamGUI::Render_ContextMenu(_vecContextMenuList, &iSelectNum);
*/
/// <param name="_vecInfo"></param>
/// <param name="iSelectNum">선택 할 경우의 해당 인덱스 번호를 가져옴</param>
/// <returns>선택 됐을 경우 true return</returns>
bool ParamGUI::Render_ContextMenu(vector<TContextInfo>& _vecInfo, int* _iSelectNum_out)
{
	*_iSelectNum_out = -1;
	bool bResult = false;
	for (size_t i = 0; i < _vecInfo.size(); ++i) {
		string strName = _vecInfo[i].strName + "##ContextMenuName";
		ImGui::Selectable(strName.c_str());
		if (ImGui::BeginPopupContextItem()) {

			if (ImGui::Button(strName.c_str())) {
				*_iSelectNum_out = (int)i;
				GUI* pInst = _vecInfo[i].pInst;
				GUI_CT_CALLBACK callbackFunc = _vecInfo[i].callback_func;
				if (callbackFunc && pInst)
					((*pInst).*callbackFunc)((DWORD_PTR)*_iSelectNum_out); // 콜백 함수 호출
				bResult = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	return bResult;
}

bool ParamGUI::Render_GameObjectLink(const string& _strName, CGameObject** _pObj, float* _pHeightOut)
{
	float fHeight = 0.f;

	ImGui::Text(_strName.c_str());
	fHeight += ImGui::GetItemRectSize().y;
	tstring tstrName = _T("");
	string strName = "NULL";
	if (*_pObj) {
		try {
			tstrName = (*_pObj)->GetName();
			TStringToString(tstrName, strName);
		}
		catch (std::exception e) {
			(*_pObj) = nullptr;
			strName = "NULL";
		}
	}

	char szName[255] = {};
	StringToArr(strName, szName, 255);
	
	ImGui::InputText("", szName, ImGuiInputTextFlags_ReadOnly);
	fHeight += ImGui::GetItemRectSize().y;
	fHeight += ImGui::GetItemRectSize().y;

	// 드랍 된 경우
	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload(STR_GUI_HierarchyTree))
		{
			DWORD_PTR dwData = *((DWORD_PTR*)ImGui::GetDragDropPayload()->Data);
			CGameObject* pObj = (CGameObject*)dwData;
			assert(pObj);
			*_pObj = pObj;
		}

		ImGui::EndDragDropTarget();
	}

	if (_pHeightOut)
		*_pHeightOut = fHeight; 

	return true;
}

bool ParamGUI::Render_PrefabLink(const string& _strName, CPrefab** _pPrefab, float* _pHeightOut)
{
	float fHeight = 0.f;
	ImGui::Text(_strName.c_str());
	tstring tstrName = _T("");
	string strName = "NULL";
	if (*_pPrefab) {
		if ((*_pPrefab)->GetProtoObj()) {
			tstrName = (*_pPrefab)->GetProtoObj()->GetName();
			TStringToString(tstrName, strName);
		}
	}


	char szName[255] = {};
	StringToArr(strName, szName, 255);

	ImGui::InputText("", szName, ImGuiInputTextFlags_ReadOnly);
	fHeight += ImGui::GetItemRectSize().y;
	fHeight += ImGui::GetItemRectSize().y;

	// 드랍 된 경우
	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload(STR_GUI_ResourceViewTree))
		{
			DWORD_PTR dwData = *((DWORD_PTR*)ImGui::GetDragDropPayload()->Data);
			CPrefab* pPrefab= (CPrefab*)dwData;
			assert(pPrefab);
			*_pPrefab = pPrefab;
		}

		ImGui::EndDragDropTarget();
	}

	if (_pHeightOut)
		*_pHeightOut = fHeight;

	return true;
}
