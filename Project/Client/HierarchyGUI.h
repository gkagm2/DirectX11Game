#pragma once
#include "GUI.h"
#include "TreeViewGUI.h"

class CGameObject;
class HierarchyGUI : public GUI
{
private:
	TreeViewGUI m_treeView;

public:
	virtual void Init() override;
	virtual void Update() override;

private:
	void _RenewTreeView();

public:
	void SelectGameObject(TreeViewNode* _pNode);
	void DragDrop(TreeViewNode* _pDragStartNode, TreeViewNode* _pDropTargetNode);

public:
	HierarchyGUI();
	virtual ~HierarchyGUI() override;
};