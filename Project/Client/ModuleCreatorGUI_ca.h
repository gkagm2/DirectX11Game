#pragma once
#include "GUI.h"
class CModuleCreator_ca;
class ModuleCreatorGUI_ca : public GUI
{
private:
	CModuleCreator_ca* m_pModuleCreator;
	vector<string> m_vecModuleTypeName;
	vector<string> m_vecModuleLevelName;

	vector<string> m_vecPrefabKey;

public:
	virtual void Init() override;
	virtual void Update() override;

public:
	ModuleCreatorGUI_ca();
	virtual ~ModuleCreatorGUI_ca() override;
};