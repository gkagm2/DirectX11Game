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
	static bool Render_Texture(const string& _strName, CTexture* _pTex, GUI* pInst, GUI_CALLBACK _pFunc);
	static bool Render_Matrix(const string& _strName, Matrix* _pOut);

public:
	ParamGUI();
	virtual ~ParamGUI();
};