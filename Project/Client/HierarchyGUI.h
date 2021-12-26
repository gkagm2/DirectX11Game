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

private:
	void SelectGameObject(TreeViewNode* _pNode);
	void DragDrop(TreeViewNode* _pDragStartNode, TreeViewNode* _pDropTargetNode);
private:
	void _ClickedGameObject(DWORD_PTR _dwItem, DWORD_PTR _dwData);
public:
	void ReleaseSelectNode() { m_treeView.ReleaseSelectedNode(); }

public:
	HierarchyGUI();
	virtual ~HierarchyGUI() override;
};