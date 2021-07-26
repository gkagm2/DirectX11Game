#pragma once
#include "GUI.h"
class TreeViewGUI : public GUI
{
public:
	virtual void Init() override;
	virtual void Update() override;

public:
	TreeViewGUI();
	virtual ~TreeViewGUI() override;
};