#include "pch.h"
#include "CEventManager.h"
#include "CSceneManager.h"

#include "CGameObject.h"
#include "CScene.h"


CEventManager::CEventManager()
{

}

CEventManager::~CEventManager()
{
}

void CEventManager::Update()
{
	// 삭제 예정 오브젝트 정리
	for (UINT i = 0; i < m_vecDeadObj.size(); ++i)
		delete m_vecDeadObj[i];
	m_vecDeadObj.clear();

	// 이벤트 처리
	for (UINT i = 0; i < m_vecEvent.size(); ++i)
		_Excute(m_vecEvent[i]);
	m_vecEvent.clear();
}

void CEventManager::_Excute(const TEvent& _event)
{
	switch (_event.eType)
	{
	case E_EventType::Create_Object: {
		// lparam : Object Address
		// wparam : Layer Index
		CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
		pCurScene->AddGameObject((CGameObject*)_event.lparam, (int)_event.wparam);
	}
		break;
	case E_EventType::Destroy_Object: {
		// lparam  : Object Address
		CGameObject* pDeleteObj = (CGameObject*)_event.lparam;
		pDeleteObj->_SetDead();
	}
		break;
	case E_EventType::Add_Child: {
		// lparam : parent object
		// wparam : child object
		CGameObject* pParent = (CGameObject*)_event.lparam;
		CGameObject* pChild = (CGameObject*)_event.wparam;
		pParent->_AddChildGameObject(pChild);
	}
		break;
	case E_EventType::Disconnect_Object: {

	}
		break;
	case E_EventType::Change_Scene: {

	}
		break;
	case E_EventType::Change_State: {

	}
		break;
	default:
		break;
	}
}