#pragma once
#include "MenuGUI.h"
class MainMenuGUI : public MenuGUI
{
private:
	bool bPlay;
	bool bPause;
	bool bStop;

private:
	void _SetSceneModeMenu(E_SceneMode _eMode);

public:
	virtual void Update() override;


	void SaveScene();
	void LoadScene();
	void ShowExampleMenuFile();
	void ShowSceneMode();
	void CreateEmptyMaterial();
	CGameObject* CreateEmptyGameObject();
	void CreateCamera2DGameObject();
	void CreateParticleSystemGameObject();
	void CreateLight2D();

	void CreateNewScene();

	// 2D
	void Create2DRectGameObject();

	// 3D 
	void CreateCube();
	void CreateSphere();
	void CreateSkybox();
	void CreateLight3D();

	// UI
	void CreateTextUI();
	void CreateImageUI();
	void CreateButtonUI();

	void CreateCursorUI();

	// Engine
	void OpenToolCameraUI();
	void OpenHierarchyUI();
	void OpenInspectorUI();
	void OpenResourceViewUI();
	void OpenSceneViewUI();
	void OpenDebugUI();

	void OpenDeferredViewUI();
	// Editor
	void OpenTileMapEditor();
	void OpenCollisionEditor();
	void OpenAnimator2DEditor();

	// CaptainForever
	void OpenModuleCreatorToolWindows();

	// Butcher
	void OpenWayPointTool();
	void OpenSpawningTool_bu();

public:
	MainMenuGUI();
	virtual ~MainMenuGUI() override;

	friend class SceneViewGUI;
};