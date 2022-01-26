#pragma once

enum class E_EventType {
	Create_GameObject,
	Destroy_GameObject,
	Create_Object,
	Destroy_Object,
	Destroy_Script,
	Add_Child,
	Unlink_Parent,
	Change_Scene,
	Change_Scene_InScript,
	Chagne_ResourceKey,
	Remove_Material,
	Remove_Resource,
	Change_State, // AI ���� ��ȯ
	Change_GameObject_And_Childs_Layer,
	Link_GameObjectWhenSceneLoad, // Scene�� load�� �� CScript�ʿ��� target ���ӿ�����Ʈ�� ����
	Link_GameObjectWhensceneLoadv2,
	Link_GameObjectWhenClone,
	Change_SceneMode, // ���� �� ��� ����

	Change_ToolState, // (EngineTool���� ���°� ����� ���)
};

struct TEvent {
	E_EventType eType;
	DWORD_PTR lparam;
	DWORD_PTR wparam;
	DWORD_PTR mparam;
};




class CGameObject;
class CEventManager : public CSingleton<CEventManager>
{
	SINGLETON(CEventManager)
public:
	vector<TEvent> m_vecEvent;
	vector<CGameObject*> m_vecDeadObj; // ���� ���� ������Ʈ ������Ʈ
	vector<CGameObject*> m_vecCreateObj; // ���� ���� ������Ʈ
	vector<std::pair<CGameObject**,uuid*>> m_vecTargetLinkObj; // Scene Load �� Script���� ��ũ ������ ������Ʈ
	vector<std::tuple<CGameObject**, tstring*, CComponent*>> m_vecTargetLinkObj2;
	vector<std::tuple<CComponent*, CGameObject**, CGameObject*>> m_vecTargetLinkComponent;
	bool m_bEventHappened;				// ���ο� ������ �߰��ǰų� ������ ��� true�� ��

	std::function<void(const tstring&,bool)> m_pLoadSceneFunc;

public:
	void Update();
	void AddEvent(const TEvent& _event) { m_vecEvent.push_back(_event); }
	bool DidEventHappended() { return m_bEventHappened; }

private:
	void _Excute(const TEvent& _event);
};