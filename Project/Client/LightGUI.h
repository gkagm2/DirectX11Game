#pragma once
#include "ComponentGUI.h"
class LightGUI: public ComponentGUI
{
protected:
	vector<char> m_vecStrLigthTypeList; // combobox¿ë string list

protected:
	virtual void _InitComboBoxList();
	void _SetMesh(DWORD_PTR _dw1, DWORD_PTR _dw);
	void _SetMatrial(DWORD_PTR _dw1, DWORD_PTR _dw);
public:
	LightGUI(E_ComponentType _eType);
	virtual ~LightGUI() override;
};