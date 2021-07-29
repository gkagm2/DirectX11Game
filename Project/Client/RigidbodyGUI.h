#pragma once
#include "ComponentGUI.h"
class CGameObject;
class RigidbodyGUI : public ComponentGUI
{
public:
	RigidbodyGUI(E_ComponentType _eType);
	virtual ~RigidbodyGUI() override;
};