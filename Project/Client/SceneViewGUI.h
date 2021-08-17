#pragma once
#include "GUI.h"
#include "TreeViewGUI.h"
class SceneViewGUI : public GUI
{
private:
	TreeViewGUI m_treeView;

public:
	virtual void Init() override;
	virtual void Update() override;

private:
	void _RenewScene();
	void _RenewTreeView();

public:
	void SelectScene(DWORD_PTR _strRelativePath, DWORD_PTR _non);

public:
	SceneViewGUI();
	virtual ~SceneViewGUI() override;
};