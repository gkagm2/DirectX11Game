#pragma once
#include "ComponentGUI.h"
class ButtonUIGUI : public ComponentGUI
{
public:
	virtual void Update() override;

public:
	void _SelectHeightMapTexture(DWORD_PTR _pStr, DWORD_PTR _NONE);

public:
	ButtonUIGUI();
	virtual ~ButtonUIGUI() override;
};