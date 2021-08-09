#pragma once
#include "ResourceGUI.h"
class MaterialGUI : public ResourceGUI
{
private:
	E_ShaderParam m_eSelectedTexParam;

public:
	virtual void Update() override;

public:
	void SetShader(DWORD_PTR _pShaderName, DWORD_PTR _NONE);

public:	
	void SelectTexture(DWORD_PTR _pStr, DWORD_PTR _NONE);

public:
	MaterialGUI();
	virtual ~MaterialGUI() override;
};