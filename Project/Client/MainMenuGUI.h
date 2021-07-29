#pragma once
#include "MenuGUI.h"
class MainMenuGUI : public MenuGUI
{
public:
	virtual void Update() override;

	void ShowExampleMenuFile();

public:
	MainMenuGUI();
	virtual ~MainMenuGUI() override;
};