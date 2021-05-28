#pragma once

// Game Object Life Cycle Interface
class ILifeCycleInterface
{
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void PrevUpdate() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
};