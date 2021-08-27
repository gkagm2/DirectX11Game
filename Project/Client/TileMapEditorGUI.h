#pragma once
#include "GUI.h"
class TileMapEditorGUI : public GUI
{
public:
	virtual void Init();
	virtual void Update() override;

public:
	TileMapEditorGUI();
	virtual ~TileMapEditorGUI() override;
};