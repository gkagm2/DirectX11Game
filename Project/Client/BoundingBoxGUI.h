#pragma once
#include "ComponentGUI.h"
class BoundingBoxGUI : public ComponentGUI
{
public:
	virtual void Update() override;
public:
	BoundingBoxGUI();
	virtual ~BoundingBoxGUI() override;
};