#pragma once
#include "ComponentGUI.h"
class MeshRendererGUI : public ComponentGUI
{
public:
	virtual void Update() override;

public:
	MeshRendererGUI();
	virtual ~MeshRendererGUI() override;
};