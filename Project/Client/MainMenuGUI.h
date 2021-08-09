#pragma once
#include "MenuGUI.h"
class MainMenuGUI : public MenuGUI
{
private:
	bool bPlay;
	bool bPause;
	bool bStop;
public:
	virtual void Update() override;

	void ShowExampleMenuFile();
	void ShowSceneMode();
	void CreateEmptyMaterial();

public:
	MainMenuGUI();
	virtual ~MainMenuGUI() override;
};