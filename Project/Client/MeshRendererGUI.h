#pragma once
#include "ComponentGUI.h"
class MeshRendererGUI : public ComponentGUI
{
public:
	virtual void Update() override;

private:
	// callback¿ë ÇÔ¼ö
	void _SetMesh(DWORD_PTR _dw1, DWORD_PTR _dw);
	void _SetMatrial(DWORD_PTR _dw1, DWORD_PTR _dw);

public:
	MeshRendererGUI();
	virtual ~MeshRendererGUI() override;
};