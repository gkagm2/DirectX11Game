#pragma once

#include "ListViewGUI.h"

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
	static bool Render_Int(const string& _strName, int* _pOut);
	static bool Render_Float(const string& _strName, float* _pOut);
	static bool Render_Vector2(const string& _strName, Vector2* _pOut);
	static bool Render_Vector4(const string& _strName, Vector4* _pOut);
	static bool Render_Texture(const string& _strName, CTexture* _pTex, GUI* pInst, GUI_CALLBACK _pFunc, bool _bIsButtonOn = true, TTextureInfo _tTextureInfo = TTextureInfo{});
	static bool Render_TextureBtn(const string& _strName, CTexture* _pTex, TTextureBtnInfo _tTexBtnInfo = TTextureBtnInfo{});

	static bool Render_Matrix(const string	& _strName, Matrix* _pOut);

	static bool Render_Color(const string& _strName, UINT* _iColorInOut);

	static void Make_ComboBoxList(const vector<string>& _inStrList, vector<char>& _outStrList);
	static bool Render_ComboBox(const string& _strName, int* _piCurItem, const vector<char>& m_strList);
public:
	ParamGUI();
	virtual ~ParamGUI();
};