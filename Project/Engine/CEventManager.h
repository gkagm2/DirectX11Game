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
	Reupdate_All_Scene,
	Remove_Material,
	Remove_Resource,
	Change_State, // AI 상태 변환
	Change_GameObject_And_Childs_Layer,
	Link_GameObjectWhenSceneLoad, // Scene이 load된 후 CScript쪽에서 target 게임오브젝트를 연결
	Link_GameObjectWhensceneLoadv2,
	Link_GameObjectWhenClone,
	Change_SceneMode, // 현재 씬 모드 변경

	Change_ToolState, // (EngineTool에서 상태가 변경된 경우)
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
	vector<CGameObject*> m_vecDeadObj; // 삭제 예정 오브젝트 오브젝트
	vector<CGameObject*> m_vecCreateObj; // 생성 예정 오브젝트
	vector<std::pair<CGameObject**,uuid*>> m_vecTargetLinkObj; // Scene Load 시 Script에서 링크 예정인 오브젝트
	vector<std::tuple<CGameObject**, tstring*, CComponent*>> m_vecTargetLinkObj2;
	vector<std::tuple<CComponent*, CGameObject**, CGameObject*>> m_vecTargetLinkComponent;
	bool m_bEventHappened;				// 새로운 정보가 추가되거나 수정될 경우 true가 됨

	std::function<void(const tstring&,bool)> m_pLoadSceneFunc;

public:
	void Update();
	void AddEvent(const TEvent& _event) { m_vecEvent.push_back(_event); }
	bool DidEventHappended() { return m_bEventHappened; }

private:
	void _Excute(const TEvent& _event);
};