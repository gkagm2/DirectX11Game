#pragma once
#include "ComponentGUI.h"
class ButtonUIGUI : public ComponentGUI
{
public:
	virtual void Update() override;
public:
	ButtonUIGUI();
	virtual ~ButtonUIGUI() override;
};