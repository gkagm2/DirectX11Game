#pragma once
#include "GUI.h"
class MenuGUI : public GUI
{
public:
	virtual void Update() = 0;
public:
	MenuGUI();
	virtual ~MenuGUI() override;
};