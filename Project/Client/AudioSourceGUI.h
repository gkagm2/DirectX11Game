#pragma once
#include "ComponentGUI.h"
class AudioSourceGUI : public ComponentGUI
{
public:
	virtual void Update() override;

public:
	AudioSourceGUI();
	virtual ~AudioSourceGUI() override;
};