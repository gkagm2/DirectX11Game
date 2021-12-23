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

	tstring _CreateObjectName( const tstring& _strObjDefaultName, UINT& id);


	void SaveScene();
	void LoadScene();
	void ShowExampleMenuFile();
	void ShowSceneMode();
	void CreateEmptyMaterial();
	CGameObject* CreateEmptyGameObject();
	void CreateCamera2DGameObject();
	void CreateParticleSystemGameObject();

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
	void OpenHierarchyUI();
	void OpenInspectorUI();
	void OpenResourceViewUI();
	void OpenSceneViewUI();
	void OpenDebugUI();

	// Editor
	void OpenTileMapEditor();
	void OpenCollisionEditor();
	void OpenAnimator2DEditor();

	// CaptainForever
	void OpenModuleCreatorToolWindows();

public:
	MainMenuGUI();
	virtual ~MainMenuGUI() override;

	friend class SceneViewGUI;
};