#pragma once
#include "GUI.h"
class CGameObject;
class CTransform;
class ToolCameraGUI : public GUI
{
private:
	CGameObject* m_pTargetObject;

	ImGuiSliderFlags m_flags;
public:
	virtual void Init();
	virtual void Update() override;

protected:
	void PrintPosition(CTransform* pTransform);

public:
	void SetTargetObject(CGameObject* _pTargetObj) { m_pTargetObject = _pTargetObj; }
	CGameObject* GetTargetObject() { return m_pTargetObject; }

public:
	ToolCameraGUI();
	virtual ~ToolCameraGUI() override;
};