#include "pch.h"
#include "CEventManager.h"
#include "CSceneManager.h"

#include "CGameObject.h"
#include "CComponent.h"
#include "CScene.h"
#include "CLayer.h"


CEventManager::CEventManager() :
	m_bEventHappened(false)
{
}

CEventManager::~CEventManager()
{
}

void CEventManager::Update()
{
	m_bEventHappened = false;
	// 삭제 예정 오브젝트 정리
	for (UINT i = 0; i < m_vecDeadObj.size(); ++i) {
		m_vecDeadObj[i]->_UnlinkParentGameObject();
		delete m_vecDeadObj[i];

		m_bEventHappened = true;
	}
	m_vecDeadObj.clear();

	// 이벤트 처리
	for (UINT i = 0; i < m_vecEvent.size(); ++i)
		_Excute(m_vecEvent[i]);
	m_vecEvent.clear();

	// 생성 예정 오브젝트 정리
	for (UINT i = 0; i < m_vecCreateObj.size(); ++i)
			m_vecCreateObj[i]->Awake();
	for (UINT i = 0; i < m_vecCreateObj.size(); ++i)
		m_vecCreateObj[i]->Start();

	m_vecCreateObj.clear();
}

void CEventManager::_Excute(const TEvent& _event)
{
	switch (_event.eType)
	{
	case E_EventType::Create_GameObject: {
		// lparam : Object Address
		// wparam : Layer Index
		CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
		assert(pCurScene);
		CGameObject* pNewGameObject = (CGameObject*)_event.lparam;
		assert(pNewGameObject);
		int iLayer = (int)_event.wparam;
		pCurScene->_AddGameObject(pNewGameObject, (E_Layer)iLayer);
		m_vecCreateObj.push_back(pNewGameObject);
	}
		break;
	case E_EventType::Destroy_GameObject: {
		// lparam  : Object Address
		CGameObject* pDeleteObj = (CGameObject*)_event.lparam;
		if (pDeleteObj->IsDead())
			break;
		pDeleteObj->_SetDead();
		m_vecDeadObj.push_back(pDeleteObj);
	}
		break;
	case E_EventType::Create_Object: {
		// lparam : Object Address
		CObject* pObj = (CObject*)_event.lparam;
		if (dynamic_cast<CGameObject*>(pObj)) {
			// wparam : Layer Index
			CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
			assert(pCurScene);
			CGameObject* pNewGameObject = (CGameObject*)_event.lparam;
			assert(pNewGameObject);
			int iLayer = (int)_event.wparam;
			pCurScene->_AddGameObject(pNewGameObject, (E_Layer)iLayer);
			m_vecCreateObj.push_back(pNewGameObject);
		}
	}
		break;
	case E_EventType::Destroy_Object: {
		// lparam : Object Address
		CObject* pObj = (CObject*)_event.lparam;
		if (dynamic_cast<CGameObject*>(pObj)) {
			CGameObject* pDeleteObj = (CGameObject*)pObj;
			if (pDeleteObj->IsDead())
				break;
			pDeleteObj->_SetDead();
			m_vecDeadObj.push_back(pDeleteObj);
		}
		else if (dynamic_cast<CComponent*>(pObj)) {
			CComponent* pComponent = (CComponent*)pObj;
			E_ComponentType pComponentType = pComponent->GetComponentType();
			pComponent->GetGameObject()->_DestroyComponent(pComponentType);
		}
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
		// lparam : next Scene
		CScene* pNextScene = (CScene*)_event.lparam;
		CSceneManager::GetInstance()->ChangeScene(pNextScene);
	}
		break;
	case E_EventType::Change_State: {

	}
		break;
	case E_EventType::Change_SceneMode: {
		// lparam : enum SceneMode
		E_SceneMode eSceneMode = (E_SceneMode)_event.lparam;
		CSceneManager::GetInstance()->_SetSceneMode(eSceneMode);
	}
		break;
	default:
		assert(nullptr);
		break;
	}

	switch (_event.eType) {
	case E_EventType::Create_GameObject:
	case E_EventType::Destroy_GameObject:
	case E_EventType::Add_Child:
	case E_EventType::Unlink_Parent:
	case E_EventType::Change_Scene:
	case E_EventType::Create_Object:
	case E_EventType::Destroy_Object:
	//case E_EventType::Change_SceneMode:
		m_bEventHappened = true;
		break;
	}
}