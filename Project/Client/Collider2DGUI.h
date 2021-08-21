#pragma once
#include "ComponentGUI.h"
class Collider2DGUI : public ComponentGUI
{
public:
	virtual void Update() override;

public:
	Collider2DGUI();
	virtual ~Collider2DGUI() override;
};