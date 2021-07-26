#pragma once
#include "GUI.h"
#include "TreeViewGUI.h"
class ResourceGUI : public GUI
{
private:
	TreeViewGUI m_treeView;
	bool m_bUseTreeFrame;

public:
	virtual void Init() override;
	virtual void Update() override;

private:
	void _RenewResource();
	void _RenewTreeView();
	  
public:
	ResourceGUI();
	virtual ~ResourceGUI() override;
};