#pragma once
#include "CCursorScript.h"
class CCursor_bu : public CCursorScript
{
public:
	virtual void Start() override;

public:
	CLONE(CCursor_bu);
	CCursor_bu();
	virtual ~CCursor_bu() override;
};