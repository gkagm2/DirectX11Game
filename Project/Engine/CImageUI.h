#pragma once
#include "CUI.h"
class CImageUI : public CUI
{
public:
	CLONE(CImageUI);
	CImageUI() = delete;
	CImageUI(E_ComponentType _eComponentType);
	virtual ~CImageUI() override;
};