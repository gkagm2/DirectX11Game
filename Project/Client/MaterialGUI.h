#pragma once
#include "ResourceGUI.h"
class MaterialGUI : public ResourceGUI
{
private:
	E_ShaderParam m_eSelectedTexParam;
	char strResourceName[255];

	CTexture* m_pPreViewTexture;

public:
	virtual void Update() override;

public:
	void SetShader(DWORD_PTR _pShaderName, DWORD_PTR _NONE);

public:	
	void _SelectHeightMapTexture(DWORD_PTR _pStr, DWORD_PTR _NONE);

private:
	void CopyMaterial(CMaterial* _pMtrl);

public:
	MaterialGUI();
	virtual ~MaterialGUI() override;
};