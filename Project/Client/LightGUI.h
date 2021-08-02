#pragma once
#include "ComponentGUI.h"
class LightGUI: public ComponentGUI
{
protected:
	vector<char> m_vecStrLigthTypeList; // combobox¿ë string list

protected:
	virtual void _InitComboBoxList() = 0;

public:
	LightGUI(E_ComponentType _eType);
	virtual ~LightGUI() override;
};