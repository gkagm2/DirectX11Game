#pragma once

#include "ListViewGUI.h"

typedef void(GUI::* GUI_CT_CALLBACK)(DWORD_PTR);  // CT : Context

struct TContextInfo {
	GUI* pInst; // 최상위 부모
	GUI_CT_CALLBACK callback_func; // GUI를 상속받고 있는 클래스의 멤버 함수.
	string strName; // 이름
TContextInfo() :
	pInst(nullptr),
		callback_func(nullptr)
	{}
};

struct TTexture_ImGUIInfo {
	ImVec2 uv_min; // top left
	ImVec2 uv_max; // bottom right
	ImVec4 tint_col; // no tint
	ImVec2 vImageSize;
	TTexture_ImGUIInfo() :
		uv_min{ ImVec2(0.f, 0.f) },
		uv_max{ ImVec2(1.f, 1.f) },
		tint_col{ ImVec4(1.f, 1.f, 1.f, 1.f) },
		vImageSize{ ImVec2(150.f,150.f) }
	{}
};

struct TTextureInfo : public TTexture_ImGUIInfo {
	ImVec4 border_col; // 50% opaque white
	TTextureInfo() :
		border_col{ ImVec4(1.f, 1.f, 1.f, 0.5f) }
	{}
};

struct TTextureBtnInfo : public TTexture_ImGUIInfo {
	int iFramePadding;
	ImVec4 bg_col;
	TTextureBtnInfo() :
		iFramePadding(-1),
		bg_col(0.f, 0.f, 0.f, 0.f)
	{}
};

class CTexture;
class ParamGUI 
{
public:
	static bool Render_Int(const string& _strName, int* _pOut, float* _pHeightOut = nullptr);
	static bool Render_Float(const string& _strName, float* _pOut, float* _pHeightOut = nullptr);
	static bool Render_Vector2(const string& _strName, Vector2* _pOut, float* _pHeightOut = nullptr);
	static bool Render_Vector3(const string& _strName, Vector3* _pOut, float* _pHeightOut = nullptr);
	static bool Render_Vector4(const string& _strName, Vector4* _pOut, float* _pHeightOut = nullptr);
	static bool Render_Texture(const string& _strName, CTexture* _pTex, GUI* pInst, GUI_CALLBACK _pFunc, bool _bIsButtonOn = true, TTextureInfo _tTextureInfo = TTextureInfo{}, float _fHeight = 0.f);
	static bool Render_TextureBtn(const string& _strName, CTexture* _pTex, TTextureBtnInfo _tTexBtnInfo = TTextureBtnInfo{});

	static bool Render_Matrix(const string	& _strName, Matrix* _pOut);

	static bool Render_Color(const string& _strName, UINT* _iColorInOut);

	static void Make_ComboBoxList(const vector<string>& _inStrList, vector<char>& _outStrList);
	static bool Render_ComboBox(const string& _strName, int* _piCurItem, const vector<char>& m_strList);

	static bool Render_ContextMenu(vector<TContextInfo>& _vecInfo, int* _iSelectNum_out);

	static bool Render_GameObjectLink(const string& _strName, CGameObject* _pObj, float* _pHeightOut = nullptr);

public:
	ParamGUI();
	virtual ~ParamGUI();
};