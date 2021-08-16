#pragma once
#include "ResourceGUI.h"
class PrefabGUI : public ResourceGUI
{
public:
	virtual void Update() override;

public:
	PrefabGUI();
	virtual ~PrefabGUI() override;
};