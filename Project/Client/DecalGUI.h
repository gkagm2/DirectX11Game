#pragma once
#include "ComponentGUI.h"
class DecalGUI : public ComponentGUI
{
public:
	virtual void Update() override;
	void _SelectHeightMapTexture(DWORD_PTR _pStr, DWORD_PTR _NONE);

public:
	DecalGUI();
	virtual ~DecalGUI() override;
};