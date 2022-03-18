#pragma once
#include "ComponentGUI.h"
class TileMapGUI : public ComponentGUI
{
public:
	virtual void Update() override;

	void ShowMeshRenderer();
public:
	void _SelectHeightMapTexture(DWORD_PTR _pStr, DWORD_PTR _NONE);

	// callback¿ë ÇÔ¼ö
	void _SetMesh(DWORD_PTR _dw1, DWORD_PTR _dw);
	void _SetMatrial(DWORD_PTR _dw1, DWORD_PTR _dw);

public:
	TileMapGUI();
	virtual ~TileMapGUI() override;
};