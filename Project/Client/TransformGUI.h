#pragma once
#include "ComponentGUI.h"
class TransformGUI : public ComponentGUI
{
public:
	virtual void Update() override;
public:
	TransformGUI();
	virtual ~TransformGUI() override;
};