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
	// ���� ���� ������Ʈ ����
	for (UINT i = 0; i < m_vecDeadObj.size(); ++i) {
		m_vecDeadObj[i]->_UnlinkParentGameObject();
		delete m_vecDeadObj[i];
	}
	m_vecDeadObj.clear();

	// �̺�Ʈ ó��
	for (UINT i = 0; i < m_vecEvent.size(); ++i)
		_Excute(m_vecEvent[i]);
	m_vecEvent.clear();

	// ���� ���� ������Ʈ ����
	for (UINT i = 0; i < m_vecCreateObj.size(); ++i) {
			m_vecCreateObj[i]->Awake();
			m_vecCreateObj[i]->Start();
	}
	m_vecCreateObj.clear();
}

void CEventManager::_Excute(const TEvent& _event)
{
	switch (_event.eType)
	{
	case E_EventType::Create_Object: {
		// lparam : Object Address
		// wparam : Layer Index
		CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
		CGameObject* pNewGameObject = (CGameObject*)_event.lparam;
		int iLayer = (int)_event.wparam;
		pCurScene->AddGameObject(pNewGameObject, (E_Layer)iLayer);
		m_vecCreateObj.push_back(pNewGameObject);
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
	case E_EventType::Unlink_Parent: { // �θ� ������Ʈ�� ������ �����Ѵ�.
		// lparam : child object		
		CGameObject* pChild = (CGameObject*)_event.lparam;
		pChild->_UnlinkParentGameObject();

		// �ֻ��� �θ�� ���̾ ���.
		CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
		CLayer* pCurLayer = pCurScene->GetLayer(pChild->m_eLayer);

		// �ڽ� ������Ʈ�� �ֻ��� �θ� ������Ʈ���� ���� ���Ϳ� �ٷ� ���.
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