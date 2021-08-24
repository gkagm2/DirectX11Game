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

	void SaveScene();
	void LoadScene();
	void ShowExampleMenuFile();
	void ShowSceneMode();
	void CreateEmptyMaterial();
	CGameObject* CreateEmptyGameObject();
	void CreateCamera2DGameObject();
	void Create2DRectGameObjet();

	// CaptainForever
	void OpenModuleCreatorToolWindows();

public:
	MainMenuGUI();
	virtual ~MainMenuGUI() override;
};