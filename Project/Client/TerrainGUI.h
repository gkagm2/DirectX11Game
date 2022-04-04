#pragma once
#include "ComponentGUI.h"
class TerrainGUI : public ComponentGUI
{
private:
	bool m_bEditMode;


	vector<string> m_vecQuadList;
	vector<string> m_vecComponentList;
	vector<UINT> m_vecQuadNumList;
	vector<UINT> m_vecCompNumList;

public:
	virtual void Init() override;
	virtual void Update() override;

private:
	void _SelectHeightMapTexture(DWORD_PTR _pStr, DWORD_PTR _NONE);
	void _SelectWeightMapTexture(DWORD_PTR _pStr, DWORD_PTR _NONE);
	void _SelectTileArrTexture(DWORD_PTR _pStr, DWORD_PTR _NONE);

public:
	TerrainGUI();
	virtual ~TerrainGUI() override;
};