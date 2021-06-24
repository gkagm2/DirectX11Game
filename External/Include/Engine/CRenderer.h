#pragma once
#include "CComponent.h"
class CRenderer : public CComponent
{
public:
	CLONE(CRenderer);
	CRenderer() = delete;
	CRenderer(E_ComponentType _eType);
	virtual ~CRenderer() override;
};