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
	Chagne_ResourceKey,
	Remove_Material,
	Remove_Resource,
	Change_State, // AI ���� ��ȯ
	Change_GameObject_And_Childs_Layer,
	Link_GameObjectWhenSceneLoad, // Scene�� load�� �� CScript�ʿ��� target ���ӿ�����Ʈ�� ����

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
	bool m_bEventHappened;				// ���ο� ������ �߰��ǰų� ������ ��� true�� ��

public:
	void Update();
	void AddEvent(const TEvent& _event) { m_vecEvent.push_back(_event); }
	bool DidEventHappended() { return m_bEventHappened; }

private:
	void _Excute(const TEvent& _event);
};