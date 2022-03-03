#include "pch.h"
#include "CEventManager.h"
#include "CSceneManager.h"
#include "CResourceManager.h"

#include "CGameObject.h"
#include "CComponent.h"
#include "CScene.h"
#include "CLayer.h"
#include "CScript.h"

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
	// ���� ���� ������Ʈ ����
	for (UINT i = 0; i < m_vecDeadObj.size(); ++i) {
		m_vecDeadObj[i]->_UnlinkParentGameObject();
		m_vecDeadObj[i]->SetActive(false, true);
	}
	for (UINT i = 0; i < m_vecDeadObj.size(); ++i) {
		queue<CGameObject*> que;
		que.push(m_vecDeadObj[i]);
		while (!que.empty()) {
			CGameObject* pDeadObj = que.front();
			que.pop();

			for (size_t j = 0; j < (size_t)E_ComponentType::End; ++j) {
				if (pDeadObj->m_arrComponent[j])
					pDeadObj->m_arrComponent[j]->OnDestroy();
			}
			for (size_t j = 0; j < pDeadObj->m_vecScript.size(); ++j)
				pDeadObj->m_vecScript[j]->OnDestroy();
			
			for (size_t i = 0; i < pDeadObj->GetChildsObject().size(); ++i)
				que.push(pDeadObj->GetChildsObject()[i]);
		}
	}
	for (UINT i = 0; i < m_vecDeadObj.size(); ++i) {
		SAFE_DELETE(m_vecDeadObj[i]);
		m_bEventHappened = true;
	}
	
	m_vecDeadObj.clear();

	for (UINT i = 0; i < m_vecTargetLinkObj.size(); ++i) {
		CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
		if (pCurScene) {
			CGameObject** pObj = m_vecTargetLinkObj[i].first;
			uuid* id = m_vecTargetLinkObj[i].second;
			uuid ids = *id;
			string str = to_string(ids);
			CGameObject* pTargetGameObject = pCurScene->FindGameObject(ids);
			if (pObj)
				*pObj = pTargetGameObject;
			SAFE_DELETE(id);
		}
	}
	m_vecTargetLinkObj.clear();
	
	for (size_t i = 0; i < m_vecTargetLinkObj2.size(); ++i) {
		CGameObject** pTargetObj = std::get<0>(m_vecTargetLinkObj2[i]);
		tstring* pStrLocalAdr = std::get<1>(m_vecTargetLinkObj2[i]);
		CComponent* pComponent = std::get<2>(m_vecTargetLinkObj2[i]);
		CGameObject* pComObj = pComponent->GetGameObject();

		CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
		if (pCurScene) {
			vector<int> vecLocalAddress;
			pComObj->ChangeLocalAddressToIntTotal(vecLocalAddress, *pStrLocalAdr);

			vector<CGameObject*> vecRoots;
			pCurScene->GetRootGameObjects(vecRoots);
			if (vecRoots.empty()) { // prefab�̸鼭 scened load�Ǳ����� prefab���� load�� ����

			}
			else {
				CGameObject* pObj = nullptr;
				if (0 < vecLocalAddress.size())
					pObj = vecRoots[vecLocalAddress[0]];
				for (size_t j = 1; j < vecLocalAddress.size(); ++j) {
					int num = vecLocalAddress[j];
					pObj = pObj->GetChildsObject()[num];
				}
				*pTargetObj = pObj;
			}
		}
		SAFE_DELETE(pStrLocalAdr);
	}
	m_vecTargetLinkObj2.clear();



	for (size_t i = 0; i < m_vecTargetLinkComponent.size(); ++i) {
		CComponent* pClonedCom = std::get<0>(m_vecTargetLinkComponent[i]);
		CGameObject** pTargetObj = std::get<1>(m_vecTargetLinkComponent[i]); // ������ Ÿ�ٿ�����Ʈ
		CGameObject* pOriginTargetObj = std::get<2>(m_vecTargetLinkComponent[i]);
		CGameObject* pClonedObj = pClonedCom->GetGameObject();
		//������� ���� ������Ʈ���� Ÿ�� ������Ʈ�� �ڽ� ������Ʈ�� ������
		// ������� ������Ʈ���� ����Ű���ִ� Ÿ�� ������Ʈ�� �����ּҸ� �����´�.
		tstring strLocalAdr = pOriginTargetObj->GetLocalAddressTotal();
		// �� ���� �ּҸ� �̿��Ͽ� �ڱ� �����ּ��� ���� ������Ʈ�� ã�Ƽ� Ÿ�ٿ�����Ʈ�� �������ش�.
		CGameObject* pFindTargetObj = pClonedObj->FindGameObjectFromLocalAddress(strLocalAdr);
		(*pTargetObj) = pFindTargetObj;
	}
	m_vecTargetLinkComponent.clear();


	// �̺�Ʈ ó��
	for (UINT i = 0; i < m_vecEvent.size(); ++i)
		_Excute(m_vecEvent[i]);
	m_vecEvent.clear();

	// ���� ���� ������Ʈ ����
	if (E_SceneMode::Play == CSceneManager::GetInstance()->GetSceneMode()) {
		for (UINT i = 0; i < m_vecCreateObj.size(); ++i)
			m_vecCreateObj[i]->Awake();
		for (UINT i = 0; i < m_vecCreateObj.size(); ++i)
			m_vecCreateObj[i]->Start();
	}

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
		UINT iLayer = (UINT)_event.wparam;
		pCurScene->_AddGameObject(pNewGameObject, iLayer);
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
			UINT iLayer = (UINT)_event.wparam;
			pCurScene->_AddGameObject(pNewGameObject, iLayer);
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
	case E_EventType::Destroy_Script: {
		// lapram : Game Object Address
		// wparam : CScript Address
		CGameObject* pObj = (CGameObject*)_event.lparam;
		CScript* pScript = (CScript*)_event.wparam;
		pObj->_DestroyScript(pScript);
	}
									break;
	case E_EventType::Add_Child: {
		// lparam : parent object
		// wparam : child object
		CGameObject* pParent = (CGameObject*)_event.lparam;
		CGameObject* pChild = (CGameObject*)_event.wparam;
		assert(pParent);
		pParent->_AddChildGameObject(pChild);
	}
		break;
	case E_EventType::Unlink_Parent: { // �θ� ������Ʈ�� ������ �����Ѵ�.
		// lparam : child object
		CGameObject* pChild = (CGameObject*)_event.lparam;
		pChild->_UnlinkParentGameObject();

		// �ֻ��� �θ�� ���̾ ���.
		CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
		CLayer* pCurLayer = pCurScene->GetLayer(pChild->m_iLayer);
		pCurLayer->_RegisterInRootGameObject(pChild);
	}
		break;
	case E_EventType::Change_Scene: {
		// lparam : next Scene
		CScene* pNextScene = (CScene*)_event.lparam;
		CSceneManager::GetInstance()->ChangeScene(pNextScene);
	}
		break;
	case E_EventType::Change_Scene_InScript: {
		// lparam : path
		// wparam : _bRelativePath
		tstring* pRelativePath = (tstring*)_event.lparam;
		bool bRelativePath = (bool)_event.wparam;

		CScene* pScene = CSceneManager::GetInstance()->m_pLoadSceneFunc(*pRelativePath, bRelativePath);
		CSceneManager::GetInstance()->ChangeScene(pScene);
		SAFE_DELETE(pRelativePath);
	}
		break;
	case E_EventType::Chagne_ResourceKey: {
		// lparam : key
		// wparam : key to change
		// mparam : resource type
		tstring* pStrKey = (tstring*)_event.lparam;
		tstring* pStrKeyChange = (tstring*)_event.wparam;
		E_ResourceType eResourceType = (E_ResourceType)_event.mparam;

		CResourceManager::GetInstance()->_ChangeResourceKey(*pStrKey, *pStrKeyChange, eResourceType);
		SAFE_DELETE(pStrKey);
		SAFE_DELETE(pStrKeyChange);
	}
		break;
	case E_EventType::Remove_Material: {
		// lparam : tstring material Key
		tstring* pStrKey = (tstring*)_event.lparam;
		CResourceManager::GetInstance()->_DeleteCopiedMaterial(*pStrKey);
		SAFE_DELETE(pStrKey);
	}
		break;
	case E_EventType::Remove_Resource: {
		// lparam : tstring resource Key
		// wparam : E_ResourceType resource Type
		tstring* pStrResourceKey = (tstring*)_event.lparam;
		E_ResourceType eResourceType = (E_ResourceType)_event.wparam;
		CResourceManager::GetInstance()->_DeleteCustomResource(*pStrResourceKey, eResourceType);
		
		SAFE_DELETE(pStrResourceKey);
	}
		break;
	case E_EventType::Change_State: {
	}
		break;
	case E_EventType::Change_GameObject_And_Childs_Layer: {
		// lparam : Target GameObject
		// wparam : Layer num
		CGameObject* pObj = (CGameObject*)_event.lparam;
		UINT iLayer = (UINT)_event.wparam;

		CLayer* pLayer = CSceneManager::GetInstance()->GetCurScene()->GetLayer(iLayer);
		pLayer->AddGameObject(pObj, true);
	}
		break;
	case E_EventType::Change_SceneMode: {
		// lparam : enum SceneMode
		E_SceneMode eSceneMode = (E_SceneMode)_event.lparam;
		CSceneManager::GetInstance()->_SetSceneMode(eSceneMode);
	}
		break;
	case E_EventType::Link_GameObjectWhenSceneLoad: {
		// lparam : CObject
		// wparam :	uuid
		CGameObject** pObj = (CGameObject**)_event.lparam;
		uuid* id = (uuid*)_event.wparam;
		m_vecTargetLinkObj.push_back(std::make_pair(pObj, id));
	}
		break;
	case E_EventType::Link_GameObjectWhensceneLoadv2: {
		// lparam : Target object (out)
		// wparam : ���� �ּ� ���
		// mparam : Target object�� ��������� �ִ� Ŭ����
		CGameObject** pTargetObjOut = (CGameObject**)_event.lparam;
		tstring* pStrLocalAdr = (tstring*)_event.wparam;
		CComponent* pComp = (CComponent*)_event.mparam;
		m_vecTargetLinkObj2.push_back(std::make_tuple(pTargetObjOut, pStrLocalAdr, pComp));
	}
		break;
	case E_EventType::Link_GameObjectWhenClone: {
		// lparam : ���� ����� Root ������Ʈ
		// wparam : �־��� ��� ������Ʈ
		// mparam : ������ ������ Ÿ�� ������Ʈ
		CComponent* pComponent = (CComponent*)_event.lparam;
		CGameObject** pTargetObj = (CGameObject**)_event.wparam;
		CGameObject* pOriginTargetObj = (CGameObject*)_event.mparam;
		m_vecTargetLinkComponent.push_back(std::make_tuple(pComponent, pTargetObj, pOriginTargetObj));
	}
		break;
	case E_EventType::Change_ToolState: {
		// ����
	}
		break;
	case E_EventType::Reupdate_All_Scene: {
		// lparam : next Scene relativePath
		tstring* pRelativePath = (tstring*)_event.lparam;
		CScene* pScene = CSceneManager::GetInstance()->m_pLoadSceneFunc(*pRelativePath, true);
		CSceneManager::GetInstance()->ChangeScene(pScene);
		CSceneManager::GetInstance()->m_pSaveSceneFunc(pScene, *pRelativePath, true);
		_tcprintf(_T("[scene]%s [reupdated]\n"), (*pRelativePath).c_str()); // Using

		SAFE_DELETE(pRelativePath);
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
	case E_EventType::Change_State:
	case E_EventType::Chagne_ResourceKey:
	case E_EventType::Remove_Material:
	case E_EventType::Remove_Resource:
	case E_EventType::Destroy_Script:
	//case E_EventType::Change_SceneMode:
	case E_EventType::Change_GameObject_And_Childs_Layer:
	case E_EventType::Change_ToolState:
	case E_EventType::Reupdate_All_Scene:
		m_bEventHappened = true;
		break;
	}
}