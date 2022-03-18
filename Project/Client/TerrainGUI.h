#pragma once
#include "ComponentGUI.h"
class TerrainGUI : public ComponentGUI
{
private:
	vector<string> m_vecQuadList;
	vector<string> m_vecComponentList;
	vector<UINT> m_vecQuadNumList;
	vector<UINT> m_vecCompNumList;

public:
	virtual void Init() override;
	virtual void Update() override;

public:
	TerrainGUI();
	virtual ~TerrainGUI() override;
};