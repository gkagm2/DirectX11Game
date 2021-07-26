#pragma once
#include "ComponentGUI.h"
class Collider2DRectGUI : public ComponentGUI
{
public:
	virtual void Update() override;

public:
	Collider2DRectGUI();
	virtual ~Collider2DRectGUI() override;
};