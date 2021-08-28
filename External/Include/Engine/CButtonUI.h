#pragma once
#include "CUI.h"
class CButtonUI : public CUI
{
public:
	CLONE(CButtonUI);
	CButtonUI();
	virtual ~CButtonUI() override;
};