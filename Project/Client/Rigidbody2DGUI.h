#pragma once
#include "RigidbodyGUI.h"
class Rigidbody2DGUI : public RigidbodyGUI
{
public:
	virtual void Update() override;

public:
	Rigidbody2DGUI();
	virtual ~Rigidbody2DGUI() override;
};