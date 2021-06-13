#pragma once

enum class E_EventType {
	Create_Object,
	Destroy_Object,
	Add_Child,
	Unlink_Parent,
	Change_Scene,
	Change_State, // AI 상태 변환
};

struct TEvent {
	E_EventType eType;
	DWORD_PTR lparam;
	DWORD_PTR wparam;
};

class CGameObject;
class CEventManager : public CSingleton<CEventManager>
{
	SINGLETON(CEventManager)
public:
	vector<TEvent> m_vecEvent;
	vector<CGameObject*> m_vecDeadObj;

public:
	void Update();
	void AddEvent(const TEvent& _event) { m_vecEvent.push_back(_event); }

private:
	void _Excute(const TEvent& _event);
};