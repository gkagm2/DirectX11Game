#pragma once
class ParamGUI 
{
public:
	static void Render_Int(const string& _strName, int* _pOut);
	static void Render_Float(const string& _strName, float* _pOut);

public:
	ParamGUI();
	virtual ~ParamGUI();
};