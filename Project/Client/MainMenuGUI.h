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

	tstring _CreateObjectName( const tstring& _strObjDefaultName, UINT& id);


	void SaveScene();
	void LoadScene();
	void ShowExampleMenuFile();
	void ShowSceneMode();
	void CreateEmptyMaterial();
	CGameObject* CreateEmptyGameObject();
	void CreateCamera2DGameObject();

	// 2D
	void Create2DRectGameObjet();

	// 3D

	// UI
private:
	CGameObject* _CreateUIGameObject();
	CGameObject* _CreateCanvas();
	CGameObject* _CreateDefaultUICamera();
public:
	void CreateTextUI();
	void CreateImageUI();
	void CreateButtonUI();

	// Engine
	void OpenToolCameraUI();

	// Editor
	void OpenTileMapEditor();

	// CaptainForever
	void OpenModuleCreatorToolWindows();

public:
	MainMenuGUI();
	virtual ~MainMenuGUI() override;
};