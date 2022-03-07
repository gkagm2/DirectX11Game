#pragma once
#include "ComponentGUI.h"
class DecalGUI : public ComponentGUI
{
public:
	virtual void Update() override;

public:
	DecalGUI();
	virtual ~DecalGUI() override;
};