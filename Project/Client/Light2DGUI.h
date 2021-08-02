#pragma once
#include "LightGUI.h"
class Light2DGUI : public LightGUI
{
public:
	virtual void Update() override;
	
protected:
	virtual void _InitComboBoxList() override;

public:
	Light2DGUI();
	virtual ~Light2DGUI() override;
};