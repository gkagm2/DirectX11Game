#pragma once
#include "ComponentGUI.h"
class ImageUIGUI : public ComponentGUI
{
public:
	virtual void Update() override;

public:
	ImageUIGUI();
	virtual ~ImageUIGUI() override;
};