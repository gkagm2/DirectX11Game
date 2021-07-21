#pragma once
#include "GUI.h"
class CGameObject;
class ComponentGUI : public GUI
{
private:
	CGameObject* m_pTargetObj;

public:
	virtual void Update() override;

public:
	void SetTargetGameObject(CGameObject* _pTargetObj) { m_pTargetObj = _pTargetObj; }
	CGameObject* GetTargetGameObject() { return m_pTargetObj; }

public:
	ComponentGUI();
	virtual ~ComponentGUI() override;
};