#pragma once
#include "ComponentGUI.h"
class CScript;
class ScriptGUI : public ComponentGUI
{
public:
	CScript* m_pScript;
	float m_fHeight; // �Ķ���� heigth
	
public:
	virtual bool Start() override;
	virtual void Update() override;

public:
	void SetScript(CScript* _pScript) { m_pScript = _pScript; }

public:
	ScriptGUI();
	virtual ~ScriptGUI() override;
};