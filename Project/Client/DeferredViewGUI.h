#pragma once
#include "GUI.h"
class DeferredViewGUI : public GUI
{
public:
	virtual void Init();
	virtual void Update() override;

public:
	DeferredViewGUI();
	virtual ~DeferredViewGUI() override;
};