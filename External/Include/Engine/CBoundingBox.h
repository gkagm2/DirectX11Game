#pragma once
#include "CComponent.h"
class CBoundingBox : public CComponent
{

public:
	virtual void Render() override;

public:
	CLONE(CBoundingBox);
	CBoundingBox();
	virtual ~CBoundingBox() override;
};