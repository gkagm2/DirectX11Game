#pragma once
#include "ComponentGUI.h"
class SkyboxGUI : public ComponentGUI
{
	vector<string> m_vecSkyboxType;
public:
	virtual void Update() override;

private:
	// callback¿ë ÇÔ¼ö
	void _SetMesh(DWORD_PTR _dw1, DWORD_PTR _dw);
	void _SetMatrial(DWORD_PTR _dw1, DWORD_PTR _dw);
	void _SelectTexture(DWORD_PTR _strKey, DWORD_PTR _NONE);

public:
	SkyboxGUI();
	virtual ~SkyboxGUI() override;
};