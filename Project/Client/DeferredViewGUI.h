#pragma once
#include "GUI.h"
class InspectorGUI;
class DeferredViewGUI : public GUI
{
private:
	InspectorGUI* m_pInspectorGUI;

public:
	virtual void Init();
	virtual void Update() override;

public:
	DeferredViewGUI();
	virtual ~DeferredViewGUI() override;
};