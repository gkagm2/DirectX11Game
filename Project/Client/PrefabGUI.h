#pragma once
#include "ResourceGUI.h"
class PrefabGUI : public ResourceGUI
{
private:
	int m_iLayer;
	Vector3 m_iRespawnPos;
public:
	virtual void Update() override;

public:
	PrefabGUI();
	virtual ~PrefabGUI() override;
};