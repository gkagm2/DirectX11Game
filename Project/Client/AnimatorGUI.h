#pragma once
#include "ComponentGUI.h"
class AnimatorGUI : public ComponentGUI
{
public:
	virtual void Update() override;

public:
	AnimatorGUI(E_ComponentType _eType);
	virtual ~AnimatorGUI() override;
};