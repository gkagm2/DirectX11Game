#pragma once
#include "ComponentGUI.h"
class Collider3DGUI : public ComponentGUI
{
public:
	virtual void Update() override;

public:
	Collider3DGUI();
	virtual ~Collider3DGUI() override;
};