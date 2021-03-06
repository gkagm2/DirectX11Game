#pragma once
#include "GUI.h"
#include "TreeViewGUI.h"
class ResourceViewGUI : public GUI
{
private:
	TreeViewGUI m_treeView;

public:
	virtual void Init() override;
	virtual void Update() override;

private:
	void _RenewResource();
	void _RenewTreeView();

public:
	void SelectResource(TreeViewNode* _pNode);
	void ReleaseSelectNode() { m_treeView.ReleaseSelectedNode(); }


public:
	ResourceViewGUI();
	virtual ~ResourceViewGUI();
};