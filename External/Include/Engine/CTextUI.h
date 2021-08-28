#pragma once
#include "CUI.h"
class CTextUI : public CUI
{
public:
	CLONE(CTextUI);
	CTextUI();
	virtual ~CTextUI() override;
};