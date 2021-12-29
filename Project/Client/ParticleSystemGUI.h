#pragma once
#include "ComponentGUI.h"
class ParticleSystemGUI : public ComponentGUI
{
private:
	vector<char> m_vecStrShape; // combo box¿ë
	CTexture* m_pPreviewTex;

public:
	virtual void Update() override;
	
private:
	void _ResetShapeComboBoxStr(vector<char>& _vecStrShape_out);

	void _SelectParticleTexture(DWORD_PTR _strKey, DWORD_PTR _NONE);
	void _UpdateTexture(DWORD_PTR _strKey, DWORD_PTR _NONE);

public:
	ParticleSystemGUI();
	virtual ~ParticleSystemGUI() override;
};