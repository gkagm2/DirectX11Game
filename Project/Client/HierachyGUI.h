#pragma once
#include "GUI.h"
#include "TreeViewGUI.h"

class CGameObject;
class HierachyGUI : public GUI
{
private:
	TreeViewGUI m_treeView;

public:
	virtual void Init() override;
	virtual void Update() override;

private:
	void _RenewTreeView();

public:
	HierachyGUI();
	virtual ~HierachyGUI() override;
};