#pragma once
#include "InspectorViewGUI.h"
#include <Engine\CGameObject.h>
class ComponentGUI : public InspectorViewGUI
{
private:
	CGameObject* m_pTargetObj;
	E_ComponentType m_eComponentType;
	bool m_bDeleteEnable;

public:
	virtual bool Start() override;

public:
	void SetTargetObject(CGameObject* _pTargetObj) { m_pTargetObj = _pTargetObj; }
	CGameObject* GetTargetObject() { return m_pTargetObj; }

	E_ComponentType GetComponentType() { return m_eComponentType; }
	void SetDelBtn_Component(bool _bActive) { m_bDeleteEnable = _bActive; }
	
public:
	ComponentGUI(E_ComponentType _eType);
	virtual ~ComponentGUI() override;
};