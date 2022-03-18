#pragma once
#include "ComponentGUI.h"
class LandscapeGUI : public ComponentGUI
{
public:
	virtual void Update() override;

public:
	LandscapeGUI();
	virtual ~LandscapeGUI() override;
};