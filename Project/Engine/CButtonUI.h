#pragma once
#include "CUI.h"
class CButtonUI : public CUI
{
public:
	CLONE(CButtonUI);
	CButtonUI() = delete;
	CButtonUI(E_ComponentType _eComponentType);
	virtual ~CButtonUI() override;
};