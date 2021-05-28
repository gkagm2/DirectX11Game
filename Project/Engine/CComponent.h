#pragma once
#include "CObject.h"
#include "CLifeCycleInterface.h"
class CComponent : public CObject, CLifeCycleInterface
{
public:
	virtual void Awake() override {}
	virtual void Start() override {}
	virtual void PrevUpdate() override {}
	virtual void Update() override {}
	virtual void LateUpdate() override {}

public:
	CComponent();
	virtual ~CComponent() override;
};