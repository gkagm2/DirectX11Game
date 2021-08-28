#pragma once
#include "ComponentGUI.h"
class RectTransformGUI : public ComponentGUI
{
	virtual void Update() override;

public:
	RectTransformGUI();
	virtual ~RectTransformGUI() override;
};