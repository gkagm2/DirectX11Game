#include "pch.h"
#include "CEventManager.h"
#include "CSceneManager.h"

#include "CGameObject.h"
#include "CScene.h"
#include "CLayer.h"


CEventManager::CEventManager()
{

}

CEventManager::~CEventManager()
{
}

void CEventManager::Update()
{
	// 삭제 예정 오브젝트 정리
	for (UINT i = 0; i < m_vecDeadObj.size(); ++i) {
		m_vecDeadObj[i]->_UnlinkParentGameObject();
		delete m_vecDeadObj[i];
	}
		
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
		int iLayer = (int)_event.wparam;
		pCurScene->AddGameObject((CGameObject*)_event.lparam, (E_Layer)iLayer);
	}
		break;
	case E_EventType::Destroy_Object: {
		// lparam  : Object Address
		CGameObject* pDeleteObj = (CGameObject*)_event.lparam;
		if (pDeleteObj->IsDead())
			break;
		pDeleteObj->_SetDead();
		m_vecDeadObj.push_back(pDeleteObj);
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
	case E_EventType::Unlink_Parent: { // 부모 오브젝트와 연결을 해제한다.
		// lparam : child object		
		CGameObject* pChild = (CGameObject*)_event.lparam;
		pChild->_UnlinkParentGameObject();

		// 최상위 부모로 레이어에 등록.
		CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
		CLayer* pCurLayer = pCurScene->GetLayer(pChild->m_eLayer);

		// 자식 오브젝트를 최상위 부모 오브젝트들을 가진 벡터에 바로 등록.
		pCurLayer->_RegisterInRootGameObject(pChild);
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