#pragma once
#include "ComponentGUI.h"
class ScriptGUI : public ComponentGUI
{
public:
	virtual bool Start() override;
	virtual void Update() override;
public:
	ScriptGUI();
	virtual ~ScriptGUI() override;
};