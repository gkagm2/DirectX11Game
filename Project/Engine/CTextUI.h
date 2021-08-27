#pragma once
#include "CUI.h"
class CTextUI : public CUI
{
public:
	CLONE(CTextUI);
	CTextUI() = delete;
	CTextUI(E_ComponentType _eComponentType);
	virtual ~CTextUI() override;
};