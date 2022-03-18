#pragma once
#include "ComponentGUI.h"
class ImageUIGUI : public ComponentGUI
{
private:
	CTexture* m_pTexture;
public:
	virtual void Update() override;

public:
	void _SelectHeightMapTexture(DWORD_PTR _pStr, DWORD_PTR _NONE);
private:
	void _UpdateTexture(DWORD_PTR _pStr, DWORD_PTR _NONE);

public:
	ImageUIGUI();
	virtual ~ImageUIGUI() override;
};