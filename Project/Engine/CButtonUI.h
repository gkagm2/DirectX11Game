#pragma once
#include "CImageUI.h"
class CButtonUI : public CImageUI
{

public:
	CLONE(CButtonUI);
	CButtonUI();
	virtual ~CButtonUI() override;
};