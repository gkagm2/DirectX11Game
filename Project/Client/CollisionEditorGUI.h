#pragma once
#include "GUI.h"
class CollisionEditorGUI : public GUI
{
public:
	virtual void Init() override;
	virtual void Update() override;

public:
	CollisionEditorGUI();
	virtual ~CollisionEditorGUI() override;
};