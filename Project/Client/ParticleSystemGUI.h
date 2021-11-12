#pragma once
#include "ComponentGUI.h"
class ParticleSystemGUI : public ComponentGUI
{
public:

	virtual void Update() override;

public:
	ParticleSystemGUI();
	virtual ~ParticleSystemGUI() override;
};