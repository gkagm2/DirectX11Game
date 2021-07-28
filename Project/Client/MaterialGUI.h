#pragma once
#include "ResourceGUI.h"
class MaterialGUI : public ResourceGUI
{
public:
	virtual void Init() override;
	virtual void Update() override;
public:
	MaterialGUI();
	virtual ~MaterialGUI() override;
};