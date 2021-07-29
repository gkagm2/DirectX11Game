#pragma once
#include "ResourceGUI.h"
class MaterialGUI : public ResourceGUI
{
public:
	virtual void Update() override;

public:
	void SetShader(DWORD_PTR _pShaderName, DWORD_PTR _NONE);

public:
	MaterialGUI();
	virtual ~MaterialGUI() override;
};