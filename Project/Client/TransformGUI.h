#pragma once
#include "ComponentGUI.h"
class TransformGUI : public ComponentGUI
{
public:
	virtual void Update() override;
	void TransformUpdate();
public:
	TransformGUI();
	virtual ~TransformGUI() override;
};