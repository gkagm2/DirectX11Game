#pragma once
#include "ComponentGUI.h"
class TileMapGUI : public ComponentGUI
{
public:
	virtual void Update() override;

public:
	TileMapGUI();
	virtual ~TileMapGUI() override;
};