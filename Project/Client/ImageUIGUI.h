#pragma once
#include "ComponentGUI.h"
class ImageUIGUI : public ComponentGUI
{
public:
	virtual void Update() override;

public:
	void SelectTexture(DWORD_PTR _pStr, DWORD_PTR _NONE);

public:
	ImageUIGUI();
	virtual ~ImageUIGUI() override;
};