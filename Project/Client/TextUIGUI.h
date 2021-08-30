#pragma once
#include "ComponentGUI.h"
class TextUIGUI : public ComponentGUI
{
private:
	static const size_t m_gTextBuffSize;

private:
	float m_fFontScrollSpeed;
	float m_fFontSizeMin;
	float m_fFontSizeMax;

public:
	virtual void Update() override;
public:
	TextUIGUI();
	virtual ~TextUIGUI() override;
};