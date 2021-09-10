#pragma once

#include "ListViewGUI.h"
class CTexture;
class ParamGUI 
{
public:
	static bool Render_Int(const string& _strName, int* _pOut);
	static bool Render_Float(const string& _strName, float* _pOut);
	static bool Render_Vector2(const string& _strName, Vector2* _pOut);
	static bool Render_Vector4(const string& _strName, Vector4* _pOut);
	static bool Render_Texture(const string& _strName, CTexture* _pTex, GUI* pInst, GUI_CALLBACK _pFunc, bool _bIsButtonOn = true); 
	static bool Render_Matrix(const string& _strName, Matrix* _pOut);

	static bool Render_Color(const string& _strName, UINT* _iColorInOut);

	static void Make_ComboBoxList(const vector<string>& _inStrList, vector<char>& _outStrList);
	static bool Render_ComboBox(const string& _strName, int* _piCurItem, const vector<char>& m_strList);
public:
	ParamGUI();
	virtual ~ParamGUI();
};