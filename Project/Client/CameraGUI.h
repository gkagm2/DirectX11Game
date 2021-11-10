#pragma once
#include "ComponentGUI.h"

class LayerCheckGUI;
class CameraGUI : public ComponentGUI
{
private:
	vector<char> m_strList;

public:
	virtual void Update() override;

private:
	void _ShowPerspectiveUI();
	void _ShowOrthographicUI();

	void _InitComboBox();

public:
	CameraGUI();
	virtual ~CameraGUI() override;
};