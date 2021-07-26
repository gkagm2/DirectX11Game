#pragma once
#include "GUI.h"
#include "TreeViewGUI.h"
class HierachyGUI : public GUI
{
private:
	TreeViewGUI m_tree;
	
public:
	HierachyGUI();
	virtual ~HierachyGUI() override;
};