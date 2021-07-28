#pragma once
#include "ResourceGUI.h"
class TextureGUI : public ResourceGUI
{
public:
	virtual void Init() override;
	virtual void Update() override;

public:
	TextureGUI();
	virtual ~TextureGUI() override;
};