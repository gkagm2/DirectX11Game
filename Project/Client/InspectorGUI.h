#pragma once
#include "GUI.h"
class CGameObject;
class ComponentGUI;
class ResourceGUI;
class ScriptGUI;
//class CResource;

enum class E_InspectorUIMode {
	GameObject,
	Resource,
	None,
};

class InspectorGUI : public GUI
{
private:
	ComponentGUI*	m_arrComGUI[(UINT)E_ComponentType::End]; // Com : ComponentSX
	vector<ScriptGUI*>   m_vecScriptGUI;
	CGameObject*	m_pTargetObject;

	ResourceGUI*	m_arrResGUI[(UINT)E_ResourceType::End]; // Res : Resource
	CResource*		m_pTargetResource;

	E_InspectorUIMode m_eMode;

public:
	virtual void Init() override;
	virtual void Update() override;
	      
public:
	void UpdateObjectGUI();
	void UpdateResourceGUI();

public:
	void SetTargetObject(CGameObject* _pTargetObj);
	void SetTargetResource(CResource* _pTargetResource);

	void SetInspectorUIMode(E_InspectorUIMode _eMode) { m_eMode = _eMode; }

private:
	void _AddNewComponent(DWORD_PTR _dw1, DWORD_PTR _dw2);
	void _AddNewScriptComponent(DWORD_PTR _dw1, DWORD_PTR _dw2);

private:
	void _ResizeScriptGUICnt();

public:
	InspectorGUI();
	virtual ~InspectorGUI() override;
};