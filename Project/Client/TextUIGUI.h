#pragma once
#include "ComponentGUI.h"
class TextUIGUI : public ComponentGUI
{
public:
	union FONT_COLOR {
		UINT iColor;
		struct {
			BYTE a;
			BYTE b;
			BYTE g;
			BYTE r;
		};
	};
private:
	string m_strContent;
	Vector2 m_vPos; // Window Screen Position
	float m_fFontSize;
	FONT_COLOR m_color;

public:
	virtual void Update() override;
public:
	TextUIGUI();
	virtual ~TextUIGUI() override;
};