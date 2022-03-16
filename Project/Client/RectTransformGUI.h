#pragma once
#include "ComponentGUI.h"
class RectTransformGUI : public ComponentGUI
{
public:
	virtual void Update() override;

public:
	RectTransformGUI();
	virtual ~RectTransformGUI() override;
};