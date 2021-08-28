#pragma once
#include "ComponentGUI.h"
class CanvasRendererGUI : public ComponentGUI
{
public:
	virtual void Update() override;

public:
	CanvasRendererGUI();
	virtual ~CanvasRendererGUI() override;
};